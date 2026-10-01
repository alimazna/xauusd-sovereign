#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "OutcomeStatus.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

class Outcome {
public:
    EntityId       outcome_id;
    EntityId       prediction_id;
    Timestamp      observed_at;
    OutcomeStatus  status;
    double         observed_price;
    std::string    notes;

    Outcome() = default;

    Outcome(
        EntityId outcome_id,
        EntityId prediction_id,
        Timestamp observed_at,
        OutcomeStatus status,
        double observed_price,
        std::string notes)
        : outcome_id(outcome_id),
          prediction_id(prediction_id),
          observed_at(observed_at),
          status(status),
          observed_price(observed_price),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
