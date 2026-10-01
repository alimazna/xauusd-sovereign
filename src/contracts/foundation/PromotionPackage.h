#pragma once
#include "EntityId.h"
#include "PromotionStatus.h"
#include "Timestamp.h"
#include "Version.h"
#include <string>
#include <utility>
namespace xauusd::sovereign {
struct PromotionPackage {
    EntityId          package_id;
    EntityId          candidate_id;
    EntityId          approval_record_id;
    PromotionStatus   status;
    Version           current_version;
    Version           candidate_version;
    Version           rollback_target_version;
    std::string       notes;
    Timestamp         created_at;
    PromotionPackage() = default;
    PromotionPackage(EntityId package_id, EntityId candidate_id,
                     EntityId approval_record_id, PromotionStatus status,
                     Version current_version, Version candidate_version,
                     Version rollback_target_version, std::string notes,
                     Timestamp created_at)
        : package_id(package_id), candidate_id(candidate_id),
          approval_record_id(approval_record_id), status(status),
          current_version(current_version), candidate_version(candidate_version),
          rollback_target_version(rollback_target_version), notes(std::move(notes)),
          created_at(created_at) {}
};
}
