//+------------------------------------------------------------------+
//|                                              SocketClient.mqh    |
//+------------------------------------------------------------------+
#property copyright "XAUUSD Sovereign"
#include "Common.mqh"

class CSocketClient
{
private:
   int    m_socket;
   bool   m_connected;
   string m_host;
   uint   m_port;
   uint   m_sequence;

public:
   CSocketClient() : m_socket(INVALID_HANDLE),m_connected(false),m_sequence(0) {}
   ~CSocketClient() { Disconnect(); }

   bool Connect(const string host,const uint port)
   {
      Disconnect();
      m_host = host;
      m_port = port;
      m_socket = SocketCreate(SOCKET_DEFAULT);
      if(m_socket==INVALID_HANDLE)
      {
         Print("[MT5] SocketCreate failed: ",GetLastError());
         return false;
      }
      if(!SocketConnect(m_socket,m_host,m_port,(uint)InpTimeoutMs))
      {
         Print("[MT5] SocketConnect failed: ",GetLastError());
         SocketClose(m_socket);
         m_socket = INVALID_HANDLE;
         return false;
      }
      SocketTimeouts(m_socket,(uint)InpTimeoutMs,(uint)InpTimeoutMs);
      m_connected = true;
      Print("[MT5] Connected to ",m_host,":",m_port);
      return true;
   }

   void Disconnect()
   {
      if(m_socket!=INVALID_HANDLE)
      {
         SocketClose(m_socket);
         m_socket = INVALID_HANDLE;
      }
      m_connected = false;
   }

   bool IsConnected() const
   {
      return m_connected && m_socket!=INVALID_HANDLE && SocketIsConnected(m_socket);
   }

   bool SendMessage(const ushort msg_type,const string json_payload)
   {
      if(!IsConnected() || StringLen(json_payload) > XAUS_MAX_PAYLOAD) return false;

      uchar payload[];
      int payload_len = StringToUtf8Bytes(json_payload,payload);
      if(payload_len > XAUS_MAX_PAYLOAD) return false;

      uchar message[];
      ArrayResize(message,24 + payload_len);
      uint seq = ++m_sequence;
      uint ts = (uint)TimeCurrent();
      uint crc = ComputeCRC32(payload,payload_len);
      PutU32(message,0,(uint)XAUS_MAGIC);
      PutU16(message,4,(ushort)XAUS_VERSION);
      PutU16(message,6,msg_type);
      PutU32(message,8,(uint)payload_len);
      PutU32(message,12,seq);
      PutU32(message,16,ts);
      PutU32(message,20,crc);
      if(payload_len>0) ArrayCopy(message,payload,24,0,payload_len);
      return SendRaw(message,ArraySize(message));
   }

   ushort ReceiveMessage(string &out_payload,int timeout_ms)
   {
      out_payload = "";
      if(!IsConnected()) return 0;
      uchar header[];
      ArrayResize(header,24);
      int received = ReceiveExact(header,24,timeout_ms);
      if(received!=24) return 0;

      uint magic = ReadU32(header,0);
      ushort version = ReadU16(header,4);
      ushort msg_type = ReadU16(header,6);
      uint payload_size = ReadU32(header,8);
      uint checksum = ReadU32(header,20);
      if(magic!=(uint)XAUS_MAGIC || version!=(ushort)XAUS_VERSION)
      {
         Print("[MT5] Protocol header mismatch; disconnecting");
         Disconnect();
         return 0;
      }
      if(payload_size>XAUS_MAX_PAYLOAD)
      {
         Print("[MT5] Payload exceeds 1MB; disconnecting");
         Disconnect();
         return 0;
      }
      if(payload_size==0) return msg_type;

      uchar payload[];
      ArrayResize(payload,(int)payload_size);
      if(ReceiveExact(payload,(int)payload_size,timeout_ms)!=(int)payload_size) return 0;
      if(ComputeCRC32(payload,(int)payload_size)!=checksum)
      {
         Print("[MT5] CRC mismatch; dropping message");
         return 0;
      }
      out_payload = CharArrayToString(payload,0,(int)payload_size,CP_UTF8);
      return msg_type;
   }

   uint NextSequence() { return ++m_sequence; }

private:
   bool SendRaw(const uchar &buffer[],const int size)
   {
      if(!IsConnected()) return false;
      int offset = 0;
      while(offset<size)
      {
         int sent = SocketSend(m_socket,buffer,size-offset);
         if(sent<=0)
         {
            Print("[MT5] SocketSend failed: ",GetLastError());
            Disconnect();
            return false;
         }
         offset += sent;
      }
      return true;
   }

   int ReceiveExact(uchar &buffer[],const int size,const int timeout_ms)
   {
      if(!IsConnected()) return 0;
      int received = 0;
      ulong deadline = GetTickCount64() + (ulong)MathMax(timeout_ms,1);
      while(received<size && IsConnected())
      {
         if(GetTickCount64() >= deadline) return 0;
         uint readable = SocketIsReadable(m_socket);
         if(readable==0)
         {
            Sleep(1);
            continue;
         }
         int want = MathMin((int)readable,size-received);
         uchar chunk[];
         int got = SocketRead(m_socket,chunk,(uint)want,(uint)MathMax(timeout_ms,1));
         if(got<=0)
         {
            Disconnect();
            return 0;
         }
         ArrayCopy(buffer,chunk,received,0,got);
         received += got;
         deadline = GetTickCount64() + (ulong)MathMax(timeout_ms,1);
      }
      return received;
   }

   uint ComputeCRC32(const uchar &data[],const int size)
   {
      uint crc = 0xFFFFFFFF;
      for(int i=0;i<size;++i)
      {
         crc ^= data[i];
         for(int j=0;j<8;++j)
            crc = (crc & 1) ? ((crc >> 1) ^ 0xEDB88320) : (crc >> 1);
      }
      return crc ^ 0xFFFFFFFF;
   }

   void PutU16(uchar &buffer[],const int pos,const ushort value)
   {
      buffer[pos] = (uchar)(value & 0xFF);
      buffer[pos+1] = (uchar)((value >> 8) & 0xFF);
   }

   void PutU32(uchar &buffer[],const int pos,const uint value)
   {
      buffer[pos] = (uchar)(value & 0xFF);
      buffer[pos+1] = (uchar)((value >> 8) & 0xFF);
      buffer[pos+2] = (uchar)((value >> 16) & 0xFF);
      buffer[pos+3] = (uchar)((value >> 24) & 0xFF);
   }

   ushort ReadU16(const uchar &buffer[],const int pos)
   {
      return (ushort)((uint)buffer[pos] | ((uint)buffer[pos+1] << 8));
   }

   uint ReadU32(const uchar &buffer[],const int pos)
   {
      return (uint)buffer[pos] |
             ((uint)buffer[pos+1] << 8) |
             ((uint)buffer[pos+2] << 16) |
             ((uint)buffer[pos+3] << 24);
   }
};
