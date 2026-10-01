#include "AuditEngine.h"
namespace xauusd::sovereign {
void AuditEngine::record(const AuditRecord&r){records_.push_back(r);}
std::vector<AuditRecord> AuditEngine::all() const{return records_;}
std::vector<AuditRecord> AuditEngine::by_action(AuditAction a) const{std::vector<AuditRecord> out;for(const auto&r:records_)if(r.action==a)out.push_back(r);return out;}
std::size_t AuditEngine::size() const{return records_.size();}
void AuditEngine::clear(){records_.clear();}
}
