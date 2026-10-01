#pragma once
#include "IAuditEngine.h"
namespace xauusd::sovereign {class AuditEngine final:public IAuditEngine{public:void record(const AuditRecord&) override;std::vector<AuditRecord> all() const override;std::vector<AuditRecord> by_action(AuditAction) const;std::size_t size() const override;void clear();private:std::vector<AuditRecord> records_;};}
