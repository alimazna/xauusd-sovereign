from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
MQL = ROOT / "src" / "mt5" / "mql5"

protocol = (MQL / "Protocol.mqh").read_text()
adapter = (MQL / "AdapterBase.mqh").read_text()
executor = (MQL / "OrderExecutor.mqh").read_text()
common = (MQL / "Common.mqh").read_text()

checks = {
    "auth token input": 'InpAuthToken' in common,
    "handshake token": 'token' in protocol and 'BuildHandshake' in protocol,
    "tick millisecond timestamp": 'm_tick.time_msc' in adapter and 'event_time_msc' in protocol,
    "timeframe command routing": r'\"tf\"' in protocol and 'tf!=m_timeframe' in adapter,
    "strict side validation": 'side!="BUY" && side!="SELL"' in executor,
    "execution mode inspection": 'SYMBOL_TRADE_EXEMODE' in executor,
    "market return blocked": 'market_execution' in executor and 'ORDER_FILLING_RETURN' in executor,
    "OrderCheck": 'OrderCheck(req,check)' in executor,
    "trade transaction": ('OnTradeTransaction' in adapter and 'BuildTradeTransaction' in protocol and
                           'TrackCommand' in adapter and 'FindCommand' in adapter),
    "all EAs forward trade transaction": all('OnTradeTransaction(const MqlTradeTransaction' in (MQL/f'{tf}Adapter.mq5').read_text() for tf in ['M1','M5','M15','M30','H1','H4','D1','W1','MN1']),
}

failed = [name for name, ok in checks.items() if not ok]
if failed:
    raise SystemExit("MQL5 static checks failed: " + ", ".join(failed))
print(f"MQL5 static checks: PASS ({len(checks)} checks)")
