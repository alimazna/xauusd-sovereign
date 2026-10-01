#pragma once

#include "IValidationFirewall.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace xauusd::sovereign {

class ValidationFirewall final : public IValidationFirewall {
public:
    ValidationFirewall() = default;

    bool register_protocol(const ValidationProtocol& protocol) override;
    bool get_protocol(const EntityId& protocol_id, ValidationProtocol& out) const override;

    bool record_run(const ValidationRun& run) override;
    bool record_result(const ValidationResult& result) override;

    bool all_passed(const EntityId& run_id) const override;
    std::vector<ValidationResult> results_for(const EntityId& run_id) const override;
    std::size_t protocol_count() const override;
    std::size_t run_count() const override;

    void clear();

private:
    using Key = std::array<std::uint8_t, 16>;

    static Key key_of(const EntityId& id);

    std::vector<ValidationProtocol> protocols_;
    std::map<Key, std::size_t> protocol_index_;
    std::vector<ValidationRun> runs_;
    std::map<Key, std::size_t> run_index_;
    std::vector<ValidationResult> results_;
};

} // namespace xauusd::sovereign
