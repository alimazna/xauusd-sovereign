#pragma once
#include "MacroState.h"
#include "Timestamp.h"
namespace xauusd::sovereign {class IMacroContextEngine{public:virtual~IMacroContextEngine()=default;virtual MacroState evaluate(Timestamp now) const=0;virtual bool blocks_signals(Timestamp now) const=0;};}
