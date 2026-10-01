#pragma once

#include "Timeframe.h"
#include "Bar.h"
#include "BarFinalizationState.h"
#include "DataValidationResult.h"

namespace xauusd::sovereign {

class IBarFinalizer {
public:
    virtual ~IBarFinalizer() = default;

    virtual BarFinalizationState process_bar(
        Timeframe timeframe,
        const Bar& bar,
        DataValidationResult& out_validation) = 0;
};

} // namespace xauusd::sovereign
