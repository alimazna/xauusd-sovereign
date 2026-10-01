#pragma once

#include "Version.h"

namespace xauusd::sovereign {

class EvaluatorVersion {
public:
    EvaluatorVersion() = default;
    explicit EvaluatorVersion(Version v) : version_(v) {}

    const Version& version() const noexcept { return version_; }

    friend bool operator==(const EvaluatorVersion&, const EvaluatorVersion&) = default;
    friend bool operator!=(const EvaluatorVersion&, const EvaluatorVersion&) = default;

private:
    Version version_{};
};

} // namespace xauusd::sovereign
