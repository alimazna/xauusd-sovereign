#pragma once

#include "EntityId.h"
#include "ValidationMethod.h"
#include "Version.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace xauusd::sovereign {

struct ValidationProtocol {
    EntityId                      protocol_id;
    std::string                   name;
    std::vector<ValidationMethod> methods;
    std::uint64_t                 max_trials;
    double                        min_confidence;
    Version                       evaluator_version;

    ValidationProtocol() = default;

    ValidationProtocol(EntityId protocol_id, std::string name,
                       std::vector<ValidationMethod> methods,
                       std::uint64_t max_trials, double min_confidence,
                       Version evaluator_version)
        : protocol_id(protocol_id),
          name(std::move(name)),
          methods(std::move(methods)),
          max_trials(max_trials),
          min_confidence(min_confidence),
          evaluator_version(evaluator_version) {}
};

} // namespace xauusd::sovereign
