#include "CapabilityRegistry.h"
#include "DependencyGraph.h"
#include "SubsystemIsolationManager.h"
#include "RecoveryManager.h"
#include "PauseResumeManager.h"
#include "StaleCandleDetector.h"
#include "ResilienceOrchestrator.h"

#include <iostream>
#include <cstdint>

static int g_failures = 0;

#define CHECK(expr) do {                                              \
    if (!(expr)) {                                                    \
        std::cerr << "FAIL: " #expr                                   \
                  << " at line " << __LINE__ << "\n";                 \
        ++g_failures;                                                 \
    }                                                                 \
} while (0)

using namespace xauusd::sovereign;

int main() {
    {
        CapabilityRegistry reg;
        CHECK(reg.size() == 0);
        CHECK(!reg.contains(CapabilityId::M15_FEED));

        CapabilityDescriptor desc{
            CapabilityId::M15_FEED,
            "M15_FEED",
            CriticalityLevel::IMPORTANT,
            true,
            true
        };
        CHECK(reg.register_capability(desc));
        CHECK(reg.contains(CapabilityId::M15_FEED));
        CHECK(reg.size() == 1);

        CapabilityDescriptor out{};
        CHECK(reg.get_descriptor(CapabilityId::M15_FEED, out));
        CHECK(out.capability_id == CapabilityId::M15_FEED);
    }

    {
        DependencyGraph g;
        CHECK(g.edge_count() == 0);

        DependencyDescriptor d1{
            CapabilityId::SIGNAL_ENGINE,
            CapabilityId::M15_FEED,
            true
        };
        CHECK(g.add_dependency(d1));

        auto deps = g.direct_dependencies_of(CapabilityId::SIGNAL_ENGINE);
        CHECK(deps.size() == 1);
        CHECK(deps[0] == CapabilityId::M15_FEED);

        auto dependents = g.direct_dependents_of(CapabilityId::M15_FEED);
        CHECK(dependents.size() == 1);
        CHECK(dependents[0] == CapabilityId::SIGNAL_ENGINE);

        CHECK(g.has_path(CapabilityId::SIGNAL_ENGINE, CapabilityId::M15_FEED));
        CHECK(!g.has_path(CapabilityId::M15_FEED, CapabilityId::SIGNAL_ENGINE));
    }

    {
        DependencyGraph g;
        g.add_dependency({CapabilityId::SIGNAL_ENGINE, CapabilityId::M15_FEED, true});
        g.add_dependency({CapabilityId::M15_FEED, CapabilityId::SIGNAL_ENGINE, true});
        CHECK(g.has_path(CapabilityId::SIGNAL_ENGINE, CapabilityId::SIGNAL_ENGINE));
    }

    {
        DependencyGraph g;
        g.add_dependency({CapabilityId::SIGNAL_ENGINE, CapabilityId::M15_FEED, true});
        g.add_dependency({CapabilityId::SHADOW_EXECUTION, CapabilityId::SIGNAL_ENGINE, true});

        SubsystemIsolationManager iso(&g);
        auto isolated = iso.isolate(CapabilityId::M15_FEED);
        CHECK(isolated.size() == 2);
        CHECK(iso.is_isolated(CapabilityId::SIGNAL_ENGINE));
        CHECK(iso.is_isolated(CapabilityId::SHADOW_EXECUTION));
        CHECK(!iso.is_isolated(CapabilityId::M15_FEED));
    }

    {
        RecoveryManager rm;
        CriticalityPolicy cp{};

        CHECK(rm.recommend(GuardianStatus::NORMAL, cp) == RecoveryAction::NONE);
        CHECK(rm.recommend(GuardianStatus::WARN, cp) == RecoveryAction::NONE);
        CHECK(rm.recommend(GuardianStatus::DEGRADE, cp) == RecoveryAction::ENTER_DEGRADED);
        CHECK(rm.recommend(GuardianStatus::SAFE_MODE, cp) == RecoveryAction::PAUSE);
        CHECK(rm.recommend(GuardianStatus::HALT, cp) == RecoveryAction::HALT);

        cp.requires_human_intervention = true;
        CHECK(rm.recommend(GuardianStatus::DEGRADE, cp)
              == RecoveryAction::MANUAL_INTERVENTION_REQUIRED);
        CHECK(rm.requires_human(GuardianStatus::SAFE_MODE, cp));
        CHECK(rm.requires_human(GuardianStatus::HALT, cp));
    }

    {
        PauseResumeManager pr;
        Timestamp t0{};
        CHECK(!pr.is_paused(CapabilityId::M15_FEED));
        CHECK(pr.pause(CapabilityId::M15_FEED, t0));
        CHECK(!pr.pause(CapabilityId::M15_FEED, t0));
        CHECK(pr.is_paused(CapabilityId::M15_FEED));

        CHECK(pr.resume(CapabilityId::M15_FEED, t0));
        CHECK(!pr.resume(CapabilityId::M15_FEED, t0));
        CHECK(!pr.is_paused(CapabilityId::M15_FEED));
    }

    {
        StaleCandleDetector sd{1000};
        Timestamp old_candle{0};
        Timestamp now{2000};
        CHECK(sd.is_stale(old_candle, now));

        Timestamp fresh_candle{1500};
        CHECK(!sd.is_stale(fresh_candle, now));
    }

    {
        ResilienceOrchestrator orch(nullptr);

        CapabilityDescriptor m15_desc{
            CapabilityId::M15_FEED, "M15_FEED",
            CriticalityLevel::CRITICAL, true, true
        };
        CHECK(orch.register_capability(m15_desc));

        CapabilityDescriptor sig_desc{
            CapabilityId::SIGNAL_ENGINE, "SIGNAL_ENGINE",
            CriticalityLevel::IMPORTANT, true, true
        };
        CHECK(orch.register_capability(sig_desc));

        CHECK(orch.register_dependency({
            CapabilityId::SIGNAL_ENGINE,
            CapabilityId::M15_FEED,
            true
        }));

        GuardianPolicy policy{};
        policy.requires_human_intervention = false;
        auto eval = orch.evaluate_failure(CapabilityId::M15_FEED, policy);
        CHECK(eval.capabilities_to_isolate.size() == 1);
        CHECK(eval.capabilities_to_isolate[0] == CapabilityId::SIGNAL_ENGINE);
        CHECK(!eval.requires_human);
    }

    {
        ResilienceOrchestrator orch(nullptr);

        Timestamp t0{1000};
        orch.observe_data(CapabilityId::M15_FEED, t0);

        Timestamp now{1500};
        CHECK(orch.evaluate_freshness(
            CapabilityId::M15_FEED, now, 600, 1200) == FreshnessState::FRESH);

        CHECK(orch.evaluate_freshness(
            CapabilityId::M15_FEED, now, 400, 1200) == FreshnessState::AGING);

        CHECK(orch.evaluate_freshness(
            CapabilityId::M15_FEED, now, 400, 499) == FreshnessState::STALE);

        CHECK(orch.evaluate_freshness(
            CapabilityId::W1_FEED, now, 600, 1200) == FreshnessState::MISSING);
    }

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }

    std::cerr << g_failures << " TEST(S) FAILED\n";
    return 1;
}
