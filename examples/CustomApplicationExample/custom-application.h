#ifndef WAVETEST_CUSTOM_APPLICATION_H
#define WAVETEST_CUSTOM_APPLICATION_H
#include "ns3/application.h"
#include "ns3/wave-net-device.h"
#include "ns3/wifi-phy.h"
#include <vector>

namespace ns3
{
    /** \brief A struct to represent information about this node's neighbors. I chose MAC address and the time last message was received form that node
     * The time 'last_beacon' is used to determine whether we should remove the neighbor from the list.
     */
    typedef struct 
    {
        Mac48Address neighbor_mac;
        Time last_beacon;
    } NeighborInformation;

    class CustomApplication : public ns3::Application
    {
        public: 
            
            static TypeId GetTypeId (void);
            virtual TypeId GetInstanceTypeId (void) const;

            CustomApplication();
            ~CustomApplication();

            /** \brief Broadcast some information 
             */ 
            void BroadcastInformation();

            /** \brief This function is called when a net device receives a packet. 
             * I connect to the callback in StartApplication. This matches the signiture of NetDevice receive.
             */
            bool ReceivePacket (Ptr<NetDevice> device,Ptr<const Packet> packet,uint16_t protocol, const Address &sender);

            void PromiscRx (Ptr<const Packet> packet, uint16_t channelFreq, WifiTxVector tx, MpduInfo mpdu, SignalNoiseDbm sn);

            void SetBroadcastInterval (Time interval);

            /** \brief Update a neighbor's last contact time, or add a new neighbor
             */
            void UpdateNeighbor (Mac48Address addr);
            /** \brief Print a list of neighbors
             */
            void PrintNeighbors ();
            
            /** \brief Change the data rate used for broadcasts.
             */ 
            void SetWifiMode (WifiMode mode);

            /** \brief Remove neighbors you haven't heard from after some time.
             */
            void RemoveOldNeighbors ();

            //You can create more functions like getters, setters, and others

        private:
            /** \brief This is an inherited function. Code that executes once the application starts
             */
            void StartApplication();
            Time m_broadcast_time; /**< How often do you broadcast messages */ 
            uint32_t m_packetSize; /**< Packet size in bytes */
            Ptr<WaveNetDevice> m_waveDevice; /**< A WaveNetDevice that is attached to this device */  
            
            std::vector <NeighborInformation> m_neighbors; /**< A table representing neighbors of this node */

            Time m_time_limit; /**< Time limit to keep neighbors in a list */
            
            WifiMode m_mode; /**< data rate used for broadcasts */
            //You can define more stuff to record statistics, etc.
    };
}

#endif