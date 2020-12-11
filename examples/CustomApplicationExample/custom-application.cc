#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "custom-application.h"
#include "custom-data-tag.h"
#include "ns3/random-variable-stream.h"

#define RED_CODE "\033[91m"
#define GREEN_CODE "\033[32m"
#define END_CODE "\033[0m"


namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("CustomApplication");
  NS_OBJECT_ENSURE_REGISTERED(CustomApplication);

TypeId CustomApplication::GetTypeId()
{
    static TypeId tid = TypeId("ns3::CustomApplication")
                .SetParent <Application> ()
                .AddConstructor<CustomApplication> ()
                .AddAttribute ("Interval", "Broadcast Interval",
                      TimeValue (MilliSeconds(100)),
                      MakeTimeAccessor (&CustomApplication::m_broadcast_time),
                      MakeTimeChecker()
                      )
                ;
    return tid;
}

TypeId CustomApplication::GetInstanceTypeId() const
{
    return CustomApplication::GetTypeId();
}

CustomApplication::CustomApplication()
{
    m_broadcast_time = MilliSeconds (100); //every 100ms
    m_packetSize = 1000; //1000 bytes
    m_time_limit = Seconds (5);
    m_mode = WifiMode("OfdmRate6MbpsBW10MHz");
}
CustomApplication::~CustomApplication()
{

}
void
CustomApplication::StartApplication()
{
    NS_LOG_FUNCTION (this);
    //Set A Receive callback
    Ptr<Node> n = GetNode ();
    for (uint32_t i = 0; i < n->GetNDevices (); i++)
    {
        Ptr<NetDevice> dev = n->GetDevice (i);
        if (dev->GetInstanceTypeId () == WaveNetDevice::GetTypeId())
        {
            m_waveDevice = DynamicCast <WaveNetDevice> (dev);
            //ReceivePacket will be called when a packet is received
            dev->SetReceiveCallback (MakeCallback (&CustomApplication::ReceivePacket, this));

            /*
            If you want promiscous receive callback, connect to this trace. 
            For every packet received, both functions ReceivePacket & PromiscRx will be called. with PromicRx being called first!
            */
            Ptr<WifiPhy> phy = m_waveDevice->GetPhys()[0]; //default, there's only one PHY in a WaveNetDevice
            phy->TraceConnectWithoutContext ("MonitorSnifferRx", MakeCallback(&CustomApplication::PromiscRx, this));
            break;
        } 
    }
    if (m_waveDevice)
    {
        //Let's create a bit of randomness with the first broadcast packet time to avoid collision
        Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable> ();
        Time random_offset = MicroSeconds (rand->GetValue(50,200));

        Simulator::Schedule (m_broadcast_time+random_offset, &CustomApplication::BroadcastInformation, this);
    }
    else
    {
        NS_FATAL_ERROR ("There's no WaveNetDevice in your node");
    }
    //We will periodically (every 1 second) check the list of neighbors, and remove old ones (older than 5 seconds)
    Simulator::Schedule (Seconds (1), &CustomApplication::RemoveOldNeighbors, this);
      
}
void 
CustomApplication::SetBroadcastInterval (Time interval)
{
    NS_LOG_FUNCTION (this << interval);
    m_broadcast_time = interval;
}
void
CustomApplication::SetWifiMode (WifiMode mode)
{
    m_mode = mode;
}

void
CustomApplication::BroadcastInformation()
{
    NS_LOG_FUNCTION (this);
    //Setup transmission parameters
    TxInfo tx;
    tx.channelNumber = CCH; 
    tx.preamble = WIFI_PREAMBLE_LONG;
    tx.priority = 7; //highest priority.
    tx.txPowerLevel = 7;
    tx.dataRate = m_mode;
    
    Ptr<Packet> packet = Create <Packet> (m_packetSize);
    
    //let's attach our custom data tag to it
    CustomDataTag tag;
    tag.SetNodeId ( GetNode()->GetId() );
    tag.SetPosition ( GetNode()->GetObject<MobilityModel>()->GetPosition());
    //timestamp is set in the default constructor of the CustomDataTag class as Simulator::Now()

    //attach the tag to the packet
    packet->AddPacketTag (tag);

    //Broadcast the packet as WSMP (0x88dc)
    m_waveDevice->SendX (packet, Mac48Address::GetBroadcast(), 0x88dc, tx);

    //Schedule next broadcast 
    Simulator::Schedule (m_broadcast_time, &CustomApplication::BroadcastInformation, this);
}

bool
CustomApplication::ReceivePacket (Ptr<NetDevice> device, Ptr<const Packet> packet,uint16_t protocol, const Address &sender)
{
    NS_LOG_FUNCTION (device << packet << protocol << sender);
    /*
        Packets received here only have Application data, no WifiMacHeader. 
        We created packets with 1000 bytes payload, so we'll get 1000 bytes of payload.
    */
    NS_LOG_INFO ("ReceivePacket() : Node " << GetNode()->GetId() << " : Received a packet from " << sender << " Size:" << packet->GetSize());
    
    //Let's check if packet has a tag attached!
    CustomDataTag tag;
    if (packet->PeekPacketTag (tag))
    {
        NS_LOG_INFO ("\tFrom Node Id: " << tag.GetNodeId() << " at " << tag.GetPosition() 
                        << "\tPacket Timestamp: " << tag.GetTimestamp() << " delay="<< Now()-tag.GetTimestamp());
    }

    return true;
}
void 
CustomApplication::PromiscRx (Ptr<const Packet> packet, uint16_t channelFreq, WifiTxVector tx, MpduInfo mpdu, SignalNoiseDbm sn)
{
    //This is a promiscous trace. It will pick broadcast packets, and packets not directed to this node's MAC address.
    /*
        Packets received here have MAC headers and payload.
        If packets are created with 1000 bytes payload, the size here is about 38 bytes larger. 
    */
    NS_LOG_DEBUG (Now () << " PromiscRx() : Node " << GetNode()->GetId() << " : ChannelFreq: " << channelFreq << " Mode: " << tx.GetMode()
                 << " Signal: " << sn.signal << " Noise: " << sn.noise << " Size: " << packet->GetSize()
                 << " Mode " << tx.GetMode ()
                 );    
    WifiMacHeader hdr;
    if (packet->PeekHeader (hdr))
    {
        //Let's see if this packet is intended to this node
        Mac48Address destination = hdr.GetAddr1();
        Mac48Address source = hdr.GetAddr2();

        UpdateNeighbor (source);

        Mac48Address myMacAddress = m_waveDevice->GetMac(CCH)->GetAddress();
        //A packet is intened to me if it targets my MAC address, or it's a broadcast message
        if ( destination==Mac48Address::GetBroadcast() || destination==myMacAddress)
        {
            NS_LOG_DEBUG ("\tFrom: " << source << "\n\tSeq. No. " << hdr.GetSequenceNumber() );
            //Do something for this type of packets
        }
        else //Well, this packet is not intended for me
        {
            //Maybe record some information about neighbors
        }    
    }
}

void CustomApplication::UpdateNeighbor (Mac48Address addr)
{
    bool found = false;
    //Go over all neighbors, find one matching the address, and updates its 'last_beacon' time.
    for (std::vector<NeighborInformation>::iterator it = m_neighbors.begin(); it != m_neighbors.end(); it++ )
    {
        if (it->neighbor_mac == addr)
        {
            it->last_beacon = Now();
            found = true;
            break;
        }
    }
    if (!found) //If no node with this address exist, add a new table entry
    {
        NS_LOG_INFO ( GREEN_CODE << Now() << " : Node " << GetNode()->GetId() << " is adding a neighbor with MAC="<<addr << END_CODE);
        NeighborInformation new_n;
        new_n.neighbor_mac = addr;
        new_n.last_beacon = Now ();
        m_neighbors.push_back (new_n);
    }
}

void CustomApplication::PrintNeighbors ()
{
    std::cout << "Neighbor Info for Node: " << GetNode()->GetId() << std::endl;
    for (std::vector<NeighborInformation>::iterator it = m_neighbors.begin(); it != m_neighbors.end(); it++ )
    {
        std::cout << "\tMAC: " << it->neighbor_mac << "\tLast Contact: " << it->last_beacon << std::endl;
    }
}

void CustomApplication::RemoveOldNeighbors ()
{
    //Go over the list of neighbors
    for (std::vector<NeighborInformation>::iterator it = m_neighbors.begin(); it != m_neighbors.end(); it++ )
    {
        //Get the time passed since the last time we heard from a node
        Time last_contact = Now () - it->last_beacon;

        if (last_contact >= Seconds (5)) //if it has been more than 5 seconds, we will remove it. You can change this to whatever value you want.
        {
            NS_LOG_INFO (RED_CODE << Now () << " Node " << GetNode()->GetId()<<" is removing old neighbor " << it->neighbor_mac <<END_CODE);
            //Remove an old entry from the table
            m_neighbors.erase (it);
            break;
        }    
    }
    //Check the list again after 1 second.
    Simulator::Schedule (Seconds (1), &CustomApplication::RemoveOldNeighbors, this);

}

}//end of ns3

