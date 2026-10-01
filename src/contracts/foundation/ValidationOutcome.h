#pragma once

// Prevent third-party headers (Windows/MT5 SDK) from redefining enum token names.
#ifdef UNKNOWN_VALUE
#undef UNKNOWN_VALUE
#endif

namespace xauusd::sovereign {

enum class ValidationOutcome
{
    UNKNOWN_VALUE,
    ACCEPTED,
    REJECTED,
    PARTIAL,
    QUARANTINED
};

}