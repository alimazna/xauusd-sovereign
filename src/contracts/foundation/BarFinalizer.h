#pragma once

#include "IBarFinalizer.h"
#include "IDataValidator.h"

namespace xauusd::sovereign {

class BarFinalizer final : public IBarFinalizer {
public:
    explicit BarFinalizer(const IDataValidator* validator) noexcept;

    BarFinalizationState process_bar(
        Timeframe timeframe,
        const Bar& bar,
        DataValidationResult& out_validation) override;

private:
    const IDataValidator* validator_;
};

} // namespace xauusd::sovereign
