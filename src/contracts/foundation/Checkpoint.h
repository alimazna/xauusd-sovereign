#pragma once
#include "CheckpointMetadata.h"
#include "CheckpointStatus.h"
#include <cstdint>
#include <utility>
#include <vector>
namespace xauusd::sovereign {
struct Checkpoint {
    CheckpointMetadata        metadata;
    CheckpointStatus          status;
    std::vector<std::uint8_t> payload;
    std::uint64_t             progress_percent;
    Checkpoint() = default;
    Checkpoint(CheckpointMetadata metadata, CheckpointStatus status,
               std::vector<std::uint8_t> payload, std::uint64_t progress_percent)
        : metadata(std::move(metadata)), status(status), payload(std::move(payload)), progress_percent(progress_percent) {}
};
}
