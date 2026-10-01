#pragma once
#include "AuditRecord.h"
#include "AuditAction.h"
#include <vector>
namespace xauusd::sovereign {class IAuditEngine{public:virtual~IAuditEngine()=default;virtual void record(const AuditRecord&)=0;virtual std::vector<AuditRecord> all() const=0;virtual std::size_t size() const=0;};}
