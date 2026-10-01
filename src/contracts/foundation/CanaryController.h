#pragma once

#include "ICanaryController.h"

namespace xauusd::sovereign {

class CanaryController final : public ICanaryController {
public:
    CanaryController() = default;

    bool configure(const CanaryConfig& config) override;
    CanaryStage current_stage() const override;
    bool submit_scale_up(const ScaleUpRequest& request) override;
    bool decide_scale_up(const ScaleUpDecision& decision) override;
    std::vector<ScaleUpDecision> all_decisions() const override;

    void clear();

private:
    CanaryConfig                 config_{};
    std::vector<ScaleUpRequest>  pending_;
    std::vector<ScaleUpDecision> decisions_;
};

} // namespace xauusd::sovereign
