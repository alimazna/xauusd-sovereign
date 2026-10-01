# MT5 Protocol Specification

## Overview

The XAUUSD Sovereign MT5 bridge uses a bidirectional TCP connection on localhost:

```text
127.0.0.1:5555
```

Each frame is:

```text
24-byte binary header + UTF-8 JSON payload
```

The header is little-endian on the wire. The C++ implementation serializes fields explicitly rather than depending on host endianness.

## Header (24 bytes)

| Offset | Size | Field | Description |
|---:|---:|---|---|
| 0 | 4 | magic | `0x58415553` (`XAUS`) |
| 4 | 2 | version | `0x0001` |
| 6 | 2 | type | `MessageType` |
| 8 | 4 | payload_size | Payload bytes; maximum 1 MiB |
| 12 | 4 | sequence | Monotonic message counter per sender |
| 16 | 4 | timestamp | Epoch seconds |
| 20 | 4 | checksum | CRC32 of payload |

## Message Types

### MT5 → C++

| Type | Name | Payload |
|---:|---|---|
| `0x0001` | HANDSHAKE | `{"tf":"M15","version":1,"token":"..."}` |
| `0x0002` | TICK | `{"tf":"M15","sym":"XAUUSD",...}` |
| `0x0003` | BAR_CLOSED | `{"tf":"M15","ot":...,"o":...}` |
| `0x0004` | SYMBOL_SPEC | `{"broker":"...","digits":2,...}` |
| `0x0005` | HEARTBEAT | `{"tf":"M15","ts":1234567890}` |
| `0x0006` | COMMAND_RESULT | `{"cmd":"...","ticket":123,...}` |
| `0x0007` | DISCONNECT | `{"tf":"M15"}` |

### C++ → MT5

| Type | Name | Payload |
|---:|---|---|
| `0x0101` | HANDSHAKE_ACK | `{"tf":"M15","authenticated":true}` |
| `0x0102` | NEW_ORDER | `{"cmd":"...","tf":"M15","sym":"...",...}` |
| `0x0103` | CLOSE_ORDER | `{"cmd":"...","tf":"M15","ticket":123}` |
| `0x0104` | MODIFY_ORDER | `{"cmd":"...","tf":"M15","ticket":123,"sl":...,"tp":...}` |
| `0x0105` | CANCEL_ORDER | `{"cmd":"...","tf":"M15","ticket":123}` |
| `0x0106` | SYMBOL_REQUEST | Reserved for future use |
| `0x0107` | HEARTBEAT_ACK | `{"tf":"M15"}` |
| `0x0108` | SHUTDOWN | `{}` |

## Payloads

### TICK

```json
{
  "tf": "M15",
  "sym": "XAUUSD",
  "et": 1727442000000,
  "bid": 2645.50,
  "ask": 2645.60,
  "last": 2645.55,
  "vol": 1.0,
  "flg": 0
}
```

`et` is epoch milliseconds.

### BAR_CLOSED

```json
{
  "tf": "M15",
  "sym": "XAUUSD",
  "ot": 1727442000000,
  "ct": 1727442900000,
  "o": 2645.00,
  "h": 2648.50,
  "l": 2643.20,
  "c": 2647.80,
  "tv": 1234,
  "rv": 0
}
```

### SYMBOL_SPEC

```json
{
  "broker": "Example Broker",
  "server": "Example-Demo",
  "sym": "XAUUSDm",
  "digits": 2,
  "point": 0.01,
  "tick_size": 0.01,
  "tick_value": 1.0,
  "contract_size": 100.0,
  "volume_min": 0.01,
  "volume_max": 100.0,
  "volume_step": 0.01,
  "stops_level": 0,
  "freeze_level": 0
}
```

### NEW_ORDER

```json
{
  "cmd": "a1b2c3d4e5f6...",
  "tf": "M15",
  "sym": "XAUUSD",
  "side": "BUY",
  "vol": 0.01,
  "price": 2645.55,
  "sl": 2640.00,
  "tp": 2660.00,
  "cmt": "M15_Trend"
}
```

### COMMAND_RESULT

```json
{
  "tf": "M15",
  "cmd": "a1b2c3d4e5f6...",
  "ticket": 123456789,
  "ok": true,
  "stage": "request",
  "trans_type": -1,
  "order": 123456789,
  "deal": 0,
  "position": 0,
  "retcode": 10009,
  "comment": "REQUEST_ACCEPTED"
}
```

The initial `COMMAND_RESULT` with `stage=request` confirms request acceptance/rejection only; it is not a final fill/close/modify/cancel confirmation. Subsequent `stage=transaction` events carry the order/deal/position transaction identity for reconciliation.

## Error Handling

- CRC failure: drop the message and log the error.
- Magic mismatch: reject and close the connection.
- Version mismatch: reject and close the connection.
- `payload_size > 1 MiB`: reject and close the connection.
- Clients must authenticate with `HANDSHAKE` before sending market data or receiving commands.
- Incoming sequence numbers must be strictly newer per connection; duplicate/replayed/out-of-order frames are dropped.
- No protocol traffic for 30 seconds: consider the client dead and close the connection. The five-second heartbeat schedule is the normal mechanism that prevents this timeout.

The server validates the header before allocation, then validates the payload CRC before dispatching JSON to the C++ integration layer.

## Timing

- Each adapter sends a heartbeat every 5 seconds by default.
- The server responds with `HEARTBEAT_ACK` immediately.
- Adapter reconnect attempts are driven by the one-second EA timer after transport failure.

## JSON rules

The protocol uses flat JSON objects with UTF-8 strings and numeric values. The C++ bridge intentionally uses a small in-project parser instead of an external JSON dependency. Order commands carry an explicit timeframe so the server can route each command to exactly one authenticated adapter.

## Authentication and routing

The C++ integration requires a non-empty `XAUS_MT5_AUTH_TOKEN` (or a token passed to the `MT5Integration` constructor). The EA sends the same value through `InpAuthToken`. Each authenticated socket is bound to exactly one timeframe, duplicate timeframe sessions are refused, and trading commands are sent only through the targeted authenticated connection.

The C++ library does not call MT5 trading APIs. Broker trading calls exist only inside the MQL5 `OrderExecutor.mqh` layer and are gated by:

```text
InpEnableLiveOrders = false
```

All order commands are logged. In SHADOW mode the adapter returns a successful `COMMAND_RESULT` with ticket `0` and a `SHADOW_ACCEPTED` comment to make the distinction explicit without sending a broker order.
