//+------------------------------------------------------------------+
//| D1Adapter.mq5                                                  |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#property version   "1.00"
#property strict
#property description "XAUUSD Sovereign MT5 D1 adapter. Default mode is SHADOW."

#include "AdapterBase.mqh"

CAdapterBase *g_adapter = NULL;
datetime g_last_bar_time = 0;

int OnInit()
{
   g_adapter = new CAdapterBase("D1",PERIOD_D1);
   if(g_adapter==NULL)
   {
      Print("[MT5] D1 Adapter allocation failed");
      return INIT_FAILED;
   }
   if(!g_adapter.Init())
   {
      Print("[MT5] D1 Adapter init failed; reconnect will be retried");
   }
   EventSetTimer(1);
   g_last_bar_time = iTime(_Symbol,PERIOD_D1,0);
   return INIT_SUCCEEDED;
}

void OnDeinit(const int reason)
{
   EventKillTimer();
   if(g_adapter!=NULL)
   {
      g_adapter.Deinit();
      delete g_adapter;
      g_adapter=NULL;
   }
}

void OnTick()
{
   if(g_adapter==NULL) return;
   g_adapter.OnTickHandler();

   datetime current_bar = iTime(_Symbol,PERIOD_D1,0);
   if(current_bar!=0 && current_bar!=g_last_bar_time)
   {
      if(g_last_bar_time!=0) g_adapter.OnBarCloseHandler();
      g_last_bar_time=current_bar;
   }
}

void OnTimer()
{
   if(g_adapter!=NULL) g_adapter.OnTimerHandler();
}

void OnTradeTransaction(const MqlTradeTransaction &trans,
                       const MqlTradeRequest &request,
                       const MqlTradeResult &result)
{
   if(g_adapter!=NULL) g_adapter.OnTradeTransactionHandler(trans,request,result);
}
