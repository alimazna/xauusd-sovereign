#pragma once

// Prevent third-party headers (Windows/MT5 SDK) from redefining enum token names.
// winsock2.h -> windows.h -> nb30.h defines DUPLICATE as a macro on MinGW, and the
// Windows SDK can leak UNKNOWN_VALUE; undefine them right before the enum so the
// header is safe regardless of include order.
#ifdef UNKNOWN_VALUE
#undef UNKNOWN_VALUE
#endif
#ifdef DUPLICATE
#undef DUPLICATE
#endif

namespace xauusd::sovereign
{

enum class DataQualityState
{
    UNKNOWN_VALUE,
    VALID,
    INVALID,
    DUPLICATE,
    OUT_OF_ORDER
};

}