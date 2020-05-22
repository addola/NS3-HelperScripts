#ifndef NS3_UDP_ARQ_APPLICATION_H
#define NS3_UDP_ARQ_APPLICATION_H
#include "ns3/socket.h"
#include "ns3/application.h"

using namespace ns3;

namespace ns3
{
  class SimpleUdpApplication : public Application 
  {
    public:
      SimpleUdpApplication ();
      virtual ~SimpleUdpApplication ();

      static TypeId GetTypeId ();
      virtual TypeId GetInstanceTypeId () const;

      /** \brief handles incoming packets on port 7777
       */
      void HandleReadOne (Ptr<Socket> socket);

      /** \brief handles incoming packets on port 9999
       */
      void HandleReadTwo (Ptr<Socket> socket);

      /** \brief Send an outgoing packet. This creates a new socket every time (not the best solution)
      */
      void SendPacket (Ptr<Packet> packet, Ipv4Address destination, uint16_t port);

    private:
      
      
      void SetupReceiveSocket (Ptr<Socket> socket, uint16_t port);
      virtual void StartApplication ();


      Ptr<Socket> m_recv_socket1; /**< A socket to receive on a specific port */
      Ptr<Socket> m_recv_socket2; /**< A socket to receive on a specific port */
      uint16_t m_port1; 
      uint16_t m_port2;

      Ptr<Socket> m_send_socket; /**< A socket to listen on a specific port */
  };
}

#endif