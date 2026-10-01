#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <cstdint>

namespace xauusd::sovereign {

struct OOSWindow {
    EntityId      window_id;
    EntityId      dataset_id;
    Timestamp     start;
    Timestamp     end;
    std::uint64_t sample_count;
    bool          locked;

    OOSWindow() = default;

    OOSWindow(EntityId window_id, EntityId dataset_id,
              Timestamp start, Timestamp end,
              std::uint64_t sample_count, bool locked)
        : window_id(window_id),
          dataset_id(dataset_id),
          start(start),
          end(end),
          sample_count(sample_count),
          locked(locked) {}
};

} // namespace xauusd::sovereign
