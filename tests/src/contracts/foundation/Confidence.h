#pragma once
#include "ConfidenceLevel.h"
namespace xauusd::sovereign {struct Confidence{ConfidenceLevel level{ConfidenceLevel::NONE};double numeric{0.0};Confidence()=default;Confidence(ConfidenceLevel l,double n):level(l),numeric(n){}};}
