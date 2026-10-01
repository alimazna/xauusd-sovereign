#pragma once

// Prevent third-party headers (Windows/MT5 SDK) from redefining enum token names.
#ifdef UNKNOWN_DATA_QUALITY
#undef UNKNOWN_DATA_QUALITY
#endif

namespace xauusd::sovereign
{

enum class DataQualityState
{
    UNKNOWN_DATA_QUALITY,
    VALID_QUALITY,
    INVALID_QUALITY,
    DUPLICATE_QUALITY,
    OUT_OF_ORDER_QUALITY
};

}