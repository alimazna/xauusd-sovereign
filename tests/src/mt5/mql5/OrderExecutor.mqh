//+------------------------------------------------------------------+
//|                                             OrderExecutor.mqh    |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#include "Common.mqh"

class COrderExecutor
{
public:
   static ulong SendMarketOrder(const string command_id,const string symbol,const string side,
                                const double volume,const double sl,const double tp,const string comment)
   {
      Print("[MT5][ORDER] NEW_ORDER symbol=",symbol," side=",side," volume=",volume,
            " sl=",sl," tp=",tp," comment=",comment,
            " mode=",(InpEnableLiveOrders ? "LIVE" : "SHADOW"));
      if(!InpEnableLiveOrders) return 0;
      if(side!="BUY" && side!="SELL")
      {
         Print("[MT5][ORDER] Invalid side: ",side);
         return 0;
      }
      if(!SymbolSelect(symbol,true)) return 0;
      if(!MathIsValidNumber(volume) || volume<=0.0) return 0;

      MqlTradeRequest req;
      MqlTradeResult res;
      ZeroMemory(req);
      ZeroMemory(res);
      req.action = TRADE_ACTION_DEAL;
      req.symbol = symbol;
      req.volume = volume;
      req.type = (side=="BUY") ? ORDER_TYPE_BUY : ORDER_TYPE_SELL;
      req.price = (req.type==ORDER_TYPE_BUY) ? SymbolInfoDouble(symbol,SYMBOL_ASK) : SymbolInfoDouble(symbol,SYMBOL_BID);
      req.sl = sl;
      req.tp = tp;
      req.deviation = InpMaxDeviationPoints;
      req.magic = (ulong)XAUS_MAGIC;
      req.comment = "[XAUS_CMD:" + command_id + "]" + (comment=="" ? "" : " " + comment);
      if(!GetFillingMode(symbol,req.type_filling))
      {
         Print("[MT5][ORDER] No compatible filling mode for ",symbol);
         return 0;
      }

      ResetLastError();
      MqlTradeCheckResult check;
      ZeroMemory(check);
      if(!OrderCheck(req,check))
      {
         Print("[MT5][ORDER] OrderCheck failed: ",GetLastError()," retcode=",check.retcode," comment=",check.comment);
         return 0;
      }
      ResetLastError();
      if(!OrderSend(req,res))
      {
         Print("[MT5][ORDER] OrderSend failed: ",GetLastError()," retcode=",res.retcode);
         return 0;
      }
      if(!IsAcceptedRetcode(res.retcode))
      {
         Print("[MT5][ORDER] Rejected retcode=",res.retcode," comment=",res.comment);
         return 0;
      }
      Print("[MT5][ORDER] Accepted order=",res.order," deal=",res.deal);
      return res.order!=0 ? res.order : res.deal;
   }

   static bool ClosePosition(const string command_id,const ulong ticket)
   {
      Print("[MT5][ORDER] CLOSE_ORDER ticket=",ticket,
            " mode=",(InpEnableLiveOrders ? "LIVE" : "SHADOW"));
      if(!InpEnableLiveOrders) return true;
      if(!PositionSelectByTicket(ticket)) return false;

      string symbol = PositionGetString(POSITION_SYMBOL);
      double volume = PositionGetDouble(POSITION_VOLUME);
      long position_type = PositionGetInteger(POSITION_TYPE);
      MqlTradeRequest req;
      MqlTradeResult res;
      ZeroMemory(req);
      ZeroMemory(res);
      req.action = TRADE_ACTION_DEAL;
      req.position = ticket;
      req.symbol = symbol;
      req.volume = volume;
      req.type = (position_type==POSITION_TYPE_BUY) ? ORDER_TYPE_SELL : ORDER_TYPE_BUY;
      req.price = (req.type==ORDER_TYPE_BUY) ? SymbolInfoDouble(symbol,SYMBOL_ASK) : SymbolInfoDouble(symbol,SYMBOL_BID);
      req.deviation = InpMaxDeviationPoints;
      req.magic = (ulong)XAUS_MAGIC;
      req.comment = "[XAUS_CMD:" + command_id + "]";
      if(!GetFillingMode(symbol,req.type_filling)) return false;
      return SendChecked(req,res,"close");
   }

   static bool ModifyPosition(const string command_id,const ulong ticket,const double sl,const double tp)
   {
      Print("[MT5][ORDER] MODIFY_ORDER ticket=",ticket," sl=",sl," tp=",tp,
            " mode=",(InpEnableLiveOrders ? "LIVE" : "SHADOW"));
      if(!InpEnableLiveOrders) return true;
      if(!PositionSelectByTicket(ticket)) return false;
      MqlTradeRequest req;
      MqlTradeResult res;
      ZeroMemory(req);
      ZeroMemory(res);
      req.action = TRADE_ACTION_SLTP;
      req.position = ticket;
      req.symbol = PositionGetString(POSITION_SYMBOL);
      req.sl = sl;
      req.tp = tp;
      req.magic = (ulong)XAUS_MAGIC;
      req.comment = "[XAUS_CMD:" + command_id + "]";
      return SendChecked(req,res,"modify");
   }

   static bool CancelOrder(const string command_id,const ulong ticket)
   {
      Print("[MT5][ORDER] CANCEL_ORDER ticket=",ticket,
            " mode=",(InpEnableLiveOrders ? "LIVE" : "SHADOW"));
      if(!InpEnableLiveOrders) return true;
      MqlTradeRequest req;
      MqlTradeResult res;
      ZeroMemory(req);
      ZeroMemory(res);
      req.action = TRADE_ACTION_REMOVE;
      req.order = ticket;
      req.magic = (ulong)XAUS_MAGIC;
      req.comment = "[XAUS_CMD:" + command_id + "]";
      return SendChecked(req,res,"cancel");
   }

private:
   static bool IsAcceptedRetcode(const uint code)
   {
      return code==TRADE_RETCODE_DONE || code==TRADE_RETCODE_PLACED ||
             code==TRADE_RETCODE_DONE_PARTIAL;
   }

   static bool SendChecked(MqlTradeRequest &req,MqlTradeResult &res,const string context)
   {
      ResetLastError();
      MqlTradeCheckResult check;
      ZeroMemory(check);
      if(!OrderCheck(req,check))
      {
         Print("[MT5][ORDER] OrderCheck failed (",context,") error=",GetLastError(),
               " retcode=",check.retcode," comment=",check.comment);
         return false;
      }
      ResetLastError();
      if(!OrderSend(req,res))
      {
         Print("[MT5][ORDER] OrderSend failed (",context,") error=",GetLastError(),
               " retcode=",res.retcode);
         return false;
      }
      return IsAcceptedRetcode(res.retcode);
   }

   static bool GetFillingMode(const string symbol,ENUM_ORDER_TYPE_FILLING &mode)
   {
      const long execution = SymbolInfoInteger(symbol,SYMBOL_TRADE_EXEMODE);
      const long filling = SymbolInfoInteger(symbol,SYMBOL_FILLING_MODE);
      const bool market_execution = execution==SYMBOL_TRADE_EXECUTION_MARKET;
      if((filling & SYMBOL_FILLING_FOK)==SYMBOL_FILLING_FOK)
      {
         mode = ORDER_FILLING_FOK;
         return true;
      }
      if((filling & SYMBOL_FILLING_IOC)==SYMBOL_FILLING_IOC)
      {
         mode = ORDER_FILLING_IOC;
         return true;
      }
      if(market_execution)
      {
         Print("[MT5][ORDER] Market execution requires symbol-supported FOK/IOC filling");
         return false;
      }
      mode = ORDER_FILLING_RETURN;
      return true;
   }
};
