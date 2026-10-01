#pragma once

namespace xauusd::sovereign
{

enum class FreshnessState
{
    Unknown,
    Fresh,
    Stale,
    Expired,
    MISSING,
    FRESH,
    AGING,
    STALE
};

}
