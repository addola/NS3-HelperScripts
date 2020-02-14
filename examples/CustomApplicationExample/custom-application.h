#ifndef WAVETEST_CUSTOM_APPLICATION_H
#define WAVETEST_CUSTOM_APPLICATION_H
#include "ns3/application.h"
#include "ns3/wave-net-device.h"
#include "ns3/wifi-phy.h"

namespace ns3
{
    class CustomApplication : public ns3::Application
    {
        public: 
            
            static TypeId GetTypeId (void);
            virtual TypeId GetInstanceTypeId (void) const;

            CustomApplication();
            ~CustomApplication();

            /** \brief This is an inherited function. Code that executes once the application starts
             */
            void StartApplication();

            /** \brief Broadcast some information 
             */ 
            void BroadcastInformation();

            /** \brief This function is called when a net device receives a packet. 
             * I connect to the callback in StartApplication. This matches the signiture of NetDevice receive.
             */
            bool ReceivePacket (Ptr<NetDevice> device,Ptr<const Packet> packet,uint16_t protocol, const Address &sender);

            void PromiscRx (Ptr<const Packet> packet, uint16_t channelFreq, WifiTxVector tx, MpduInfo mpdu, SignalNoiseDbm sn);

            void SetBroadcastInterval (Time interval);
            //You can create more functions like getters, setters, and others
        
        private:
            Time m_broadcast_time; /**< How often do you broadcast messages */ 
            uint32_t m_packetSize; /**< Packet size in bytes */
            Ptr<WaveNetDevice> m_waveDevice; /**< A WaveNetDevice that is attached to this device */  

            //You can define more stuff to record statistics, etc.
    };
}

#endif