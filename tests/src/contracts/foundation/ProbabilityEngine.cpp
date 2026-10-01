#include "ProbabilityEngine.h"
namespace xauusd::sovereign {
ProbabilityEstimate ProbabilityEngine::estimate(const Score&) const{
 return ProbabilityEstimate{false,-1.0,"probability calibration not established"};
}
}
