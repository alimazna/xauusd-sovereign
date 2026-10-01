#pragma once

#include "EntityId.h"
#include "TelegramMessageType.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct TelegramMessage {
    EntityId            message_id;
    TelegramMessageType type;
    std::string         chat_id;
    std::string         text;
    Timestamp           created_at;
    bool                sent;
    std::string         failure_reason;

    TelegramMessage() = default;

    TelegramMessage(EntityId message_id, TelegramMessageType type,
                    std::string chat_id, std::string text,
                    Timestamp created_at, bool sent,
                    std::string failure_reason)
        : message_id(message_id), type(type),
          chat_id(std::move(chat_id)), text(std::move(text)),
          created_at(created_at), sent(sent),
          failure_reason(std::move(failure_reason)) {}
};

} // namespace xauusd::sovereign
