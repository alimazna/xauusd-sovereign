#pragma once

#include "ILiveController.h"

namespace xauusd::sovereign {

class LiveController final : public ILiveController {
public:
    LiveController() = default;

    bool enable(const LiveConfig& config) override;
    bool disable() override;
    bool is_enabled() const override;
    LiveModeStatus current_status() const override;

    bool record_decision(const LiveDecision& decision) override;
    std::vector<LiveDecision> all_decisions() const override;

    LiveSession current_session() const override;
    std::size_t decision_count() const override;

    void clear();

private:
    bool                       enabled_{false};
    LiveModeStatus             status_{LiveModeStatus::DISABLED};
    LiveConfig                 config_{};
    LiveSession                session_{};
    std::vector<LiveDecision>  decisions_;
};

} // namespace xauusd::sovereign
