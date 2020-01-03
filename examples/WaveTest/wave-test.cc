#include "ns3/wave-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"


//For colorful console printing
/*
 * Usage example :
 *    std::cout << BOLD_CODE << "some bold text << END_CODE << std::endl;
 *
 *    std::cout << YELLOW_CODE << BOLD_CODE << "some bold yellow text << END_CODE << std::endl;
 *
 */
#define YELLOW_CODE "\033[33m"
#define TEAL_CODE "\033[36m"
#define BOLD_CODE "\033[1m"
#define END_CODE "\033[0m"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WaveExample1");

//Note: this is a promiscuous trace for all packet reception. This is also on physical layer, so packets still have WifiMacHeader
void Rx (std::string context, Ptr <const Packet> packet, uint16_t channelFreqMhz,  WifiTxVector txVector,MpduInfo aMpdu, SignalNoiseDbm signalNoise)
{
	//context will include info about the source of this event. Use string manipulation if you want to extract info.
	std::cout << BOLD_CODE <<  context << END_CODE << std::endl;
	//Print the info.
	std::cout << "\tSize=" << packet->GetSize()
			  << " Freq="<<channelFreqMhz
			  << " Mode=" << txVector.GetMode()
			  << " Signal=" << signalNoise.signal
			  << " Noise=" << signalNoise.noise << std::endl;

	//We can also examine the WifiMacHeader
	WifiMacHeader hdr;
	if (packet->PeekHeader(hdr))
	{
		std::cout << "\tDestination MAC : " << hdr.GetAddr1() << "\tSource MAC : " << hdr.GetAddr2() << std::endl;
	}
}

/*
 * This function works for ns-3.30 onwards. For previous version, remove the last parameter (the "WifiPhyRxfailureReason")
 */
void RxDrop (std::string context, Ptr<const Packet> packet, ns3::WifiPhyRxfailureReason reason)
{
	std::cout << BOLD_CODE << YELLOW_CODE << "Packet Rx Dropped!" << END_CODE << std::endl;
	//From ns-3.30, the reasons are defined in an enum type in ns3::WifiPhy class.
	std::cout << " Reason : " << reason << std::endl;
	std::cout << context << std::endl;

	WifiMacHeader hdr;
	if (packet->PeekHeader(hdr))
	{

		std::cout << " Destination MAC : " << hdr.GetAddr1() << "\tSource MAC : " << hdr.GetAddr2() << "\tSeq No. " << hdr.GetSequenceNumber() << std::endl;
	}
}

//Fired when a packet is Enqueued in MAC
void EnqueueTrace(std::string context, Ptr<const WifiMacQueueItem> item)
{
	std::cout << TEAL_CODE << "A Packet was enqueued : " << context << END_CODE << std::endl;

	Ptr <const Packet> p = item->GetPacket();
	/*
	 * Do something with the packet, like attach a tag. ns3 automatically attaches a timestamp for enqueued packets;
	 */

}
//Fired when a packet is Dequeued from MAC layer. A packet is dequeued before it is transmitted.
void DequeueTrace(std::string context, Ptr<const WifiMacQueueItem> item)
{
	std::cout << TEAL_CODE << "A Packet was dequeued : " << context << END_CODE << std::endl;

	Ptr <const Packet> p = item->GetPacket();
	Time queue_delay = Simulator::Now() - item->GetTimeStamp();

	//Keep in mind that a packet might get dequeued (dropped_ if it exceeded MaxDelay (default is 500ms)
	std::cout << "\tQueuing delay=" << queue_delay << std::endl;


}

int main (int argc, char *argv[])
{
  CommandLine cmd;

  //Number of nodes
  uint32_t nNodes = 3;

  cmd.AddValue ("n","Number of nodes", nNodes);

  cmd.Parse (argc, argv);


  double simTime = 10;
  NodeContainer nodes;
  nodes.Create(nNodes);

  //Nodes MUST have some sort of mobility because that's needed to compute the received signal strength
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 10.0, 0.0));

  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  //I prefer using WaveHelper to create WaveNetDevice
  YansWifiChannelHelper waveChannel = YansWifiChannelHelper::Default ();
  YansWavePhyHelper wavePhy =  YansWavePhyHelper::Default ();
  wavePhy.SetChannel (waveChannel.Create ());
  wavePhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);

  /*
   * If you create applications that control TxPower, define the low & high end of TxPower.
   * This is done by using 'TxInfo' as shown below.
   * 33 dBm is the highest allowed for non-government use (as per 802.11-2016 standard, page 3271
   * 44.8 dBm is for government use.
   *
   * Setting them to the same value is the easy way to go.
   * I can instead set TxPowerStart to a value lower than 33, but then I need to set the number of levels for each PHY
   */
  wavePhy.Set ("TxPowerStart", DoubleValue (8) );
  wavePhy.Set ("TxPowerEnd", DoubleValue (33) );



  QosWaveMacHelper waveMac = QosWaveMacHelper::Default ();
  WaveHelper waveHelper = WaveHelper::Default ();

  waveHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
  						"DataMode", StringValue ("OfdmRate6MbpsBW10MHz"	),
  						"ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),
  						"NonUnicastMode", StringValue ("OfdmRate6MbpsBW10MHz"));


  NetDeviceContainer devices = waveHelper.Install (wavePhy, waveMac, nodes);
  wavePhy.EnablePcap ("WaveTest", devices);

  //prepare a packet with a payload of 500 Bytes. Basically it has zeros in the payload
  Ptr <Packet> packet 	= Create <Packet> (1000);

  //destination MAC
  Mac48Address dest	= Mac48Address::GetBroadcast();

  /*
   * 0x88dc is the ethertype corresponding to WSMP. IPv4's etherType is 0x0800, and IPv6 is 0x86DD
   * The standard doesn't allow sending IP packets over CCH channel
   */
  uint16_t protocol = 0x88dc;

  //We can also set the transmission parameters at the higher layeres
  TxInfo tx;

  //We set the channel on which the packet is sent. The WaveNetDevice must have access to the channel
  //CCH is enabled by default.
  tx.channelNumber = CCH;

  //We can set per-packet data rate. This packet will have a rate of 12Mbps.
  tx.dataRate = WifiMode ("OfdmRate12MbpsBW10MHz");

  /*
   * Set the Access Catogory (AC) of the packet.
   * The 802.11e EDCA standard defines 4 AC's named Voice, Video, Best Effort & Background in order of priority.
   * The value determines the EdcaQueue in which the packet will be enqueued.
   *
   * The 'tid' is a value from 0-7 that maps to ACs as follows
   * 1 or 2 : Background (Lowest priority)
   * 0 or 3 : Best effort
   * 4 or 5 : Video
   * 6 or 7 : Voice (Highest priority)
   */
  tx.priority = 7;	//We set the AC to highest priority. We can set this per packet.

  /*
   * We can also set TxPower. This maps to the user define TxPowerStart & TxPowerEnd.
   * 7 : corresponds to the highest user-defined power (TxPowerEnd). In this code, it's 33 dBm
   * 1 : lowest (TxPowerStart). In this code, it's 8.3 dBm
   *
   * We'll have N equally spaced power levels.
   * A value of 8 indicates application don't want to set power or data rate. Values >8 are invalid.
   */
  tx.txPowerLevel = 3; //When we define TxPowerStart & TxPowerEnd for a WifiPhy, 7 is correspond to TxPowerEnd, and 1 TxPowerStart, and numbers in between are levels.
  //tx.preamble = WIFI_PREAMBLE_LONG;

  /*************** Sending a packet ***************/

  /*
   * In order to send a packet, we will call SendX function of WaveNetDevice.
   */

  //Get the WaveNetDevice for the first devices, using node 0.
  Ptr <NetDevice> d0 = devices.Get (0);
  Ptr <WaveNetDevice> wd0 = DynamicCast <WaveNetDevice> (d0);

  /*
   * We want to call
   *     wd0->SendX (packet, destination, protocol, tx);
   * By scheduling a simulator event as follows:
   */
   Simulator::Schedule ( Seconds (1) , &WaveNetDevice::SendX, wd0, packet, dest, protocol, tx);

  //Let us schedule try to have all three nodes schedule messages for broadcast
  for (uint32_t t=0 ; t<simTime-2; t++)
  {
	  //Go over all the nodes.
	  for (uint32_t i=0; i<devices.GetN() ; i++)
	  {
		  Ptr <NetDevice> di = devices.Get (i);
		  Ptr <WaveNetDevice> wdi = DynamicCast <WaveNetDevice> (di);

		  Ptr <Packet> packet_i = Create<Packet> (400);

		  Ptr <Packet> low_priority_packet = Create <Packet> (250); // A low priority packet

		  TxInfo txi;
		  txi.channelNumber = CCH;
		  //We always need to set TxPower to something. Otherwise, default data rate would be used.
		  txi.txPowerLevel = 3;
		  switch (i)
		  {
		  case 0:
			  //I am going to make node 0 broadcast at 27Mbps, with priority 5.
			  txi.dataRate = WifiMode ("OfdmRate27MbpsBW10MHz");
			  txi.priority = 5; //check the pcap file for the TID value in the Wifi MAC header
			  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
			  //We also are going to schedule another packet with lowest priority
			  txi.priority = 1; //1 is for BK (Background) priority.
			  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, low_priority_packet, dest, protocol, txi);

			  break;

		  case 1:
			  //I am going to set only the data rate for packets sent by node1
			  txi.dataRate = WifiMode ("OfdmRate9MbpsBW10MHz");
			  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
			  break;

		  case 2:
			  /* I am not going to set data rate for packets out of node 2.
			   * The data rate will be whatever we used for NonUnicastMode when we set WifiRemoteStationManager
			   */
			  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
			  break;
		  }
	  }
  }
  /****** Unicast Example *******/
  //Let's send a Unicast packet from n0 to n2
  //Get the MAC address of the target node
  Ptr <WaveNetDevice> d2 = DynamicCast<WaveNetDevice>(devices.Get(2));
  Mac48Address target_mac = Mac48Address::ConvertFrom (d2->GetAddress());

  Ptr <Packet> unicast_packet = Create<Packet> (200);
  TxInfo tx_u;
  /*
   * Schedule sending from WaveNetDevice 0.
   * Since this is a unicast, the frame will be acknowledged with an acknowledgment frame
   */
  Simulator::Schedule ( Seconds(simTime-1) , &WaveNetDevice::SendX, wd0, unicast_packet, target_mac, protocol, tx_u );



  /* Using tracesources to trace some simulation events */

  /*
   * Connecting to a promiscous Rx trace source. This will invoke the 'Rx' function everytime a packet is received.
   *
   * The MonitorSnifferRx trace is defined in WifiPhy.
   */
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/MonitorSnifferRx", MakeCallback (&Rx) );

  //Set the number of power levels.
  Config::Set("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/TxPowerLevels", ns3::UintegerValue(7));


  /*
   * What if some packets were dropped due to collision, or whatever? We use this trace to fire RxDrop function
   */
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/PhyRxDrop", MakeCallback (&RxDrop) );

  /*
   * We can also trace some MAC layer details
   */
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/MacEntities/*/$ns3::OcbWifiMac/*/Queue/Enqueue", MakeCallback (&EnqueueTrace));

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/MacEntities/*/$ns3::OcbWifiMac/*/Queue/Dequeue", MakeCallback (&DequeueTrace));



  Simulator::Stop(Seconds(simTime));
  Simulator::Run();
  Simulator::Destroy();

}
