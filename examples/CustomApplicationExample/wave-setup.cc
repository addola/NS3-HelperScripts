#include "wave-setup.h"

namespace ns3
{

WaveSetup::WaveSetup(){}
WaveSetup::~WaveSetup () {}

NetDeviceContainer WaveSetup::ConfigureDevices (NodeContainer& nodes)
{
  /*
    Setting up WAVE devices. With PHY & MAC using default settings. 
  */
  YansWifiChannelHelper waveChannel = YansWifiChannelHelper::Default ();
  YansWavePhyHelper wavePhy =  YansWavePhyHelper::Default ();
  wavePhy.SetChannel (waveChannel.Create ());
  wavePhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
  wavePhy.Set ("TxPowerStart", DoubleValue (5) );
  wavePhy.Set ("TxPowerEnd", DoubleValue (33) );
  wavePhy.Set ("TxPowerLevels", UintegerValue (8));
  //Setup up MAC
  QosWaveMacHelper waveMac = QosWaveMacHelper::Default ();
  WaveHelper waveHelper = WaveHelper::Default ();

  waveHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
  						"DataMode", StringValue ("OfdmRate6MbpsBW10MHz"	),
  						"ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),
  						"NonUnicastMode", StringValue ("OfdmRate6MbpsBW10MHz"));

  NetDeviceContainer devices = waveHelper.Install (wavePhy, waveMac, nodes);


  //if you want PCAP trace, uncomment this!
  //wavePhy.EnablePcap ("custom-application", devices); //This generates *.pcap files

  return devices;
}

}