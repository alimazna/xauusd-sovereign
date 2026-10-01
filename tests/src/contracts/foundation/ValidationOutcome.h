#pragma once

// Prevent third-party headers (Windows/MT5 SDK) from redefining enum token names.
#ifdef UNKNOWN_VALIDATION_VALUE
#undef UNKNOWN_VALIDATION_VALUE
#endif

namespace xauusd::sovereign {

enum class ValidationOutcome
{
    UNKNOWN_VALIDATION_VALUE,
    ACCEPTED_VALIDATION,
    REJECTED_VALIDATION,
    PARTIAL_VALIDATION,
    QUARANTINED_VALIDATION
};

}