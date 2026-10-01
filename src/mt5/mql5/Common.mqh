//+------------------------------------------------------------------+
//|                                                  Common.mqh      |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#property version   "1.00"

#define XAUS_MAGIC          0x58415553
#define XAUS_VERSION        0x0001
#define XAUS_MAX_PAYLOAD    1048576

#define MT5_MSG_HANDSHAKE       0x0001
#define MT5_MSG_TICK            0x0002
#define MT5_MSG_BAR_CLOSED      0x0003
#define MT5_MSG_SYMBOL_SPEC     0x0004
#define MT5_MSG_HEARTBEAT       0x0005
#define MT5_MSG_COMMAND_RESULT  0x0006
#define MT5_MSG_DISCONNECT      0x0007

#define MT5_MSG_HANDSHAKE_ACK   0x0101
#define MT5_MSG_NEW_ORDER       0x0102
#define MT5_MSG_CLOSE_ORDER     0x0103
#define MT5_MSG_MODIFY_ORDER    0x0104
#define MT5_MSG_CANCEL_ORDER    0x0105
#define MT5_MSG_SYMBOL_REQUEST  0x0106
#define MT5_MSG_HEARTBEAT_ACK   0x0107
#define MT5_MSG_SHUTDOWN        0x0108

input string InpServerHost = "127.0.0.1";
input uint   InpServerPort = 5555;
input string InpAuthToken = ""; // Must match XAUS_MT5_AUTH_TOKEN on the C++ host.
input int    InpTimeoutMs = 5000;
input bool   InpEnableHeartbeat = true;
input int    InpHeartbeatSec = 5;
input bool   InpEnableLiveOrders = false; // Safety default: SHADOW only.
input int    InpMaxDeviationPoints = 10;

int StringToUtf8Bytes(const string text, uchar &buffer[])
{
   int copied = StringToCharArray(text, buffer, 0, WHOLE_ARRAY, CP_UTF8);
   return copied > 0 ? copied - 1 : 0;
}

string JsonEscape(const string text)
{
   string out = text;
   StringReplace(out, "\\", "\\\\");
   StringReplace(out, "\"", "\\\"");
   StringReplace(out, "\n", "\\n");
   StringReplace(out, "\r", "\\r");
   StringReplace(out, "\t", "\\t");
   return out;
}

string JsonNumber(const double value, const int digits = 8)
{
   return DoubleToString(value, digits);
}

string UintToStr(const uint value)
{
   return IntegerToString((long)value);
}

string UlongToStr(const ulong value)
{
   return IntegerToString((long)value);
}
