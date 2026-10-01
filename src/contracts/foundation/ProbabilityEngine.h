#pragma once
#include "IProbabilityEngine.h"
namespace xauusd::sovereign {class ProbabilityEngine final:public IProbabilityEngine{public:ProbabilityEstimate estimate(const Score&) const override;};}
