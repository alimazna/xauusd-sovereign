# Telegram Bot Setup

The project includes two Telegram bots:

- Operations / Monitoring
- Governance / Approval

Their Bot API credentials are stored locally in `config/telegram.local.env`. That file is intentionally git-ignored.

## Configuration

Set these values in `config/telegram.local.env` or as environment variables:

```text
XAUS_TELEGRAM_OPERATIONS_TOKEN=...
XAUS_TELEGRAM_GOVERNANCE_TOKEN=...
XAUS_TELEGRAM_PRIMARY_CHAT_ID=...
XAUS_TELEGRAM_GOVERNANCE_CHAT_ID=...
XAUS_TELEGRAM_ENABLE_OPERATIONS=true
XAUS_TELEGRAM_ENABLE_GOVERNANCE=true
XAUS_TELEGRAM_REQUIRE_AUTH=true
XAUS_TELEGRAM_MAX_MESSAGES_PER_MINUTE=30
XAUS_TELEGRAM_SEND_TIMEOUT_MS=5000
```

Environment variables take precedence over the local file.

## Chat IDs

The bot token alone is not enough to call `sendMessage`; a destination chat/channel ID is required. Start a chat with each bot (or add it to the required chat/channel) and fill the corresponding chat ID. The operations bot is the default sender. A message addressed to the configured governance chat is routed through the governance bot.

## Runtime behavior

`TelegramGateway` uses Telegram's HTTPS Bot API for real delivery when live delivery is enabled. Unit/integration tests explicitly disable live delivery, so running the test suite never sends messages to Telegram.

The project defaults to MT5 shadow mode; Telegram integration does not enable live broker orders.

## Secret handling

Never commit or publish `config/telegram.local.env`. The provided tokens should be treated as credentials. If a token has been exposed outside a trusted environment, rotate it with BotFather and replace the local value.
