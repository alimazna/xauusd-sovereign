#include "TestHelpers.h"

#include "AuditEngine.h"
int main(){AuditEngine a;AuditRecord r{};r.action=AuditAction::DECISION_RECORDED;r.outcome=AuditOutcome::SUCCESS;a.record(r);assert(a.size()==1);assert(a.by_action(AuditAction::DECISION_RECORDED).size()==1);return 0;}
