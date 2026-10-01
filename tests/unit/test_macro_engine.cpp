#include "TestHelpers.h"

#include "MacroContextEngine.h"
int main(){MacroContextEngine e;assert(e.evaluate(Timestamp{1})==MacroState::NORMAL);assert(!e.blocks_signals(Timestamp{1}));e.set_state(MacroState::SHOCK);assert(e.blocks_signals(Timestamp{2}));return 0;}
