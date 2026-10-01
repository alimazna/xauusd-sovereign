#pragma once
#include "CanaryStage.h"
#include <cstdint>
namespace xauusd::sovereign {
struct CanaryConfig {
    CanaryStage current_stage{CanaryStage::STAGE_0_OFF}; std::uint64_t min_trades_before_promotion{0}; std::uint64_t max_drawdown_bps_before_rollback{0}; bool require_human_promotion{true};
    CanaryConfig() = default;
    CanaryConfig(CanaryStage current_stage,std::uint64_t min_trades_before_promotion,std::uint64_t max_drawdown_bps_before_rollback,bool require_human_promotion)
        : current_stage(current_stage),min_trades_before_promotion(min_trades_before_promotion),max_drawdown_bps_before_rollback(max_drawdown_bps_before_rollback),require_human_promotion(require_human_promotion) {}
};
}
