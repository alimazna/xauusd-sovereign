#pragma once

#include "EntityId.h"
#include "PersistenceStatus.h"
#include "Timestamp.h"

namespace xauusd::sovereign {

class PersistenceTransaction {
public:
    EntityId          transaction_id;
    Timestamp         started_at;
    PersistenceStatus status;

    PersistenceTransaction() = default;

    PersistenceTransaction(
        const EntityId& transaction_id,
        const Timestamp& started_at,
        PersistenceStatus status)
        : transaction_id(transaction_id),
          started_at(started_at),
          status(status) {}
};

} // namespace xauusd::sovereign
