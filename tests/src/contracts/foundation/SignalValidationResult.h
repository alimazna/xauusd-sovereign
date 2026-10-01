#pragma once
#include "Signal.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {struct SignalValidationResult{bool passed{false};std::string reason;SignalValidationResult()=default;SignalValidationResult(bool p,std::string r):passed(p),reason(std::move(r)){};};}
