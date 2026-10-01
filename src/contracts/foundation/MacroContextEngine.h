#pragma once
#include "IMacroContextEngine.h"
namespace xauusd::sovereign {class MacroContextEngine final:public IMacroContextEngine{public:MacroState evaluate(Timestamp) const override;bool blocks_signals(Timestamp) const override;void set_state(MacroState state) noexcept{state_=state;}private:MacroState state_{MacroState::NORMAL};};}
