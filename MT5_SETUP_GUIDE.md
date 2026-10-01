# MT5 Setup Guide

## Prerequisites

- MetaTrader 5 installed on Windows.
- A demo account is recommended for validation.
- The broker's gold symbol is visible in Market Watch (usually `XAUUSD`; broker suffixes such as `XAUUSDm` are supported because the EA uses `_Symbol`).
- Build the XAUUSD application with the C++ MT5 library enabled.

## 1. Build the C++ application

From the project root:

```bash
cmake -S . -B build -DXAUUSD_BUILD_UI=OFF
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

The MT5 bridge listens on `127.0.0.1:5555` by default. The `MT5Integration` object is the application boundary: the host application should construct it with its `RuntimeEngine`, `ShadowLedger`, and an authentication token, then call `start()` during application startup and `stop()` during shutdown. The bridge refuses to start without a non-empty authentication token.

Set the token in the host environment, for example:

```bash
export XAUS_MT5_AUTH_TOKEN="replace-with-a-long-random-token"
```

On Windows PowerShell:

```powershell
$env:XAUS_MT5_AUTH_TOKEN = "replace-with-a-long-random-token"
```

## 2. Copy the MQL5 files

Copy every file in:

```text
src/mt5/mql5/
```

to a folder beneath the MT5 data directory, for example:

```text
<MT5_DATA_FOLDER>/MQL5/Experts/XAUUSD/
```

To find the data folder in MT5:

1. Open MetaTrader 5.
2. Choose **File → Open Data Folder**.
3. Open `MQL5/Experts/`.
4. Create `XAUUSD` and copy the `.mqh` and `.mq5` files there.

## 3. Allow the local socket address

MetaTrader 5 requires the destination address to be explicitly allowed for its Socket* network functions. In **Tools → Options → Expert Advisors**, add:

```text
127.0.0.1
```

The MQL5 Socket* API is native to MetaTrader 5; no DLL or third-party socket library is used by these adapters.

## 4. Compile the Expert Advisors

1. Press **F4** to open MetaEditor.
2. Open the nine Expert Advisors under `Experts/XAUUSD/`.
3. Compile each with **F7**.
4. Confirm there are no compiler errors.

The nine adapters are:

```text
M1Adapter.mq5
M5Adapter.mq5
M15Adapter.mq5
M30Adapter.mq5
H1Adapter.mq5
H4Adapter.mq5
D1Adapter.mq5
W1Adapter.mq5
MN1Adapter.mq5
```

## 5. Configure the same authentication token in MT5

For every EA set:

```text
InpAuthToken = <the same token used by XAUS_MT5_AUTH_TOKEN>
```

An empty token is intentionally rejected by the C++ bridge.

## 6. Start the C++ server

Start the XAUUSD application before attaching the EAs. The bridge will open:

```text
127.0.0.1:5555
```

Each EA connects, sends a handshake and symbol specification, and then streams ticks and closed bars. The EA timer sends a heartbeat every five seconds and polls for commands.

## 7. Attach the nine adapters

Attach one adapter to an appropriate chart for each timeframe. The EA uses its own fixed timeframe rather than relying on the chart period.

For example:

```text
XAUUSD M1  -> M1Adapter
XAUUSD M5  -> M5Adapter
XAUUSD M15 -> M15Adapter
...
XAUUSD MN1 -> MN1Adapter
```

Enable **Algo Trading** for the terminal/EAs.

## 8. Safety defaults

The default input is:

```text
InpEnableLiveOrders = false
```

With this setting, NEW_ORDER, CLOSE_ORDER, MODIFY_ORDER, and CANCEL_ORDER commands are logged and treated as SHADOW acknowledgements; no broker order is submitted.

Live broker execution must be enabled explicitly in the EA inputs, and all order commands continue to be logged before execution.

## 9. Connection checks

C++ side:

- The MT5 server reports connected client count.
- Adapter status tracks timeframe, heartbeat time, last activity, tick count, and bar count.
- Each timeframe has one authenticated socket route; ambiguous duplicate timeframe routes are refused.
- The server closes a client after 30 seconds with no protocol traffic.

MT5 side:

- Use **Toolbox → Experts** to inspect connection, reconnect, heartbeat, and order-command logs.
- A transport failure places the adapter into a reconnecting state; the one-second timer retries the connection.

## Troubleshooting

### `SocketConnect failed`

Check that the C++ application is running and that `127.0.0.1` is allowed in the MT5 Expert Advisors network list. On Windows, check whether port 5555 is occupied:

```powershell
netstat -ano | findstr 5555
```

### Adapter compiles but does not connect

Inspect the Experts log. The first connection sequence should be:

```text
Connected to 127.0.0.1:5555
Adapter connected
Handshake ACK received
```

### No live orders appear

That is the expected default. Confirm `InpEnableLiveOrders=false` unless live execution has been deliberately enabled for a controlled test.

### CRC or protocol errors

The bridge uses the fixed 24-byte header, little-endian integer fields, UTF-8 JSON payloads, and CRC32. A malformed header, protocol-version mismatch, payload over 1 MiB, or CRC failure is rejected.
