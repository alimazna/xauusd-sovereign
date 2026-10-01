#pragma once

#include "Bar.h"
#include "Tick.h"
#include "SymbolSpec.h"
#include "DataValidationResult.h"

namespace xauusd::sovereign {

class IDataValidator {
public:
    virtual ~IDataValidator() = default;

    virtual DataValidationResult validate_bar(const Bar& bar) const = 0;

    virtual DataValidationResult validate_tick(const Tick& tick) const = 0;

    virtual DataValidationResult validate_symbol_spec(
        const SymbolSpec& spec) const = 0;
};

} // namespace xauusd::sovereign
