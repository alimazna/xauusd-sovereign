#pragma once

#include "EntityId.h"
#include "Timestamp.h"

#include <string>
#include <utility>

namespace xauusd::sovereign {

struct TelegramCommand {
    EntityId    command_id;
    std::string chat_id;
    std::string command;
    std::string arguments;
    Timestamp   received_at;
    std::string user_id;
    bool        authenticated;

    TelegramCommand() = default;

    TelegramCommand(EntityId command_id, std::string chat_id,
                    std::string command, std::string arguments,
                    Timestamp received_at, std::string user_id,
                    bool authenticated)
        : command_id(command_id), chat_id(std::move(chat_id)),
          command(std::move(command)), arguments(std::move(arguments)),
          received_at(received_at), user_id(std::move(user_id)),
          authenticated(authenticated) {}
};

} // namespace xauusd::sovereign
