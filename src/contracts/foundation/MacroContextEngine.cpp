#include "MacroContextEngine.h"
namespace xauusd::sovereign {MacroState MacroContextEngine::evaluate(Timestamp) const{return state_;}bool MacroContextEngine::blocks_signals(Timestamp) const{return state_==MacroState::SHOCK||state_==MacroState::EVENT_ACTIVE;}}
