#pragma once

#include "EntityId.h"
#include "Timestamp.h"
#include "Prediction.h"
#include "ValidationResult.h"

#include <string>

namespace xauusd::sovereign {

class PredictionRecord {
public:
    EntityId            record_id;
    Timestamp           recorded_at;
    Prediction          prediction;
    ValidationResult    validation;
    std::string         notes;

    PredictionRecord() = default;

    PredictionRecord(
        EntityId record_id,
        Timestamp recorded_at,
        Prediction prediction,
        ValidationResult validation,
        std::string notes)
        : record_id(record_id),
          recorded_at(recorded_at),
          prediction(std::move(prediction)),
          validation(validation),
          notes(std::move(notes)) {}
};

} // namespace xauusd::sovereign
