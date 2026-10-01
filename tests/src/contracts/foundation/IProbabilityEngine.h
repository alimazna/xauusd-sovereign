#pragma once
#include "ProbabilityEstimate.h"
#include "Score.h"
namespace xauusd::sovereign {class IProbabilityEngine{public:virtual~IProbabilityEngine()=default;virtual ProbabilityEstimate estimate(const Score&) const=0;};}
