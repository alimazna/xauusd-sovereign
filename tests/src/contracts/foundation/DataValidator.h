#pragma once

#include "IDataValidator.h"

namespace xauusd::sovereign {

class DataValidator final : public IDataValidator {
public:
    DataValidator() = default;

    DataValidationResult validate_bar(const Bar& bar) const override;

    DataValidationResult validate_tick(const Tick& tick) const override;

    DataValidationResult validate_symbol_spec(
        const SymbolSpec& spec) const override;
};

} // namespace xauusd::sovereign
