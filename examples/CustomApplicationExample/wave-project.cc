#include "ns3/wave-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "custom-application.h"
#include "wave-setup.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CustomApplicationExample");

int main (int argc, char *argv[])
{
  CommandLine cmd;

  //Number of nodes
  uint32_t nNodes = 5;
  double simTime = 4; //4 seconds
  double interval = 0.1;
  bool enablePcap = false;
  cmd.AddValue ("t","Simulation Time", simTime);
  cmd.AddValue ("i", "Broadcast interval in seconds", interval);
  cmd.AddValue ("n", "Number of nodes", nNodes);
  cmd.AddValue ("pcap", "Enable PCAP", enablePcap);
  cmd.Parse (argc, argv);

  NodeContainer nodes;
  nodes.Create(nNodes);

  /*
    You must setup Mobility. Any mobility will work. Use one suitable for your work
  */
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install(nodes);
  for (uint32_t i=0 ; i<nodes.GetN(); i++)
  {
    //set initial positions, and velocities
    Ptr<ConstantVelocityMobilityModel> cvmm = DynamicCast<ConstantVelocityMobilityModel> (nodes.Get(i)->GetObject<MobilityModel>());
    cvmm->SetPosition ( Vector (20+i*5, 20+(i%2)*5, 0));
    cvmm->SetVelocity ( Vector (10+((i+1)%2)*5,0,0) );
  }
 
  WaveSetup wave;
  NetDeviceContainer devices = wave.ConfigureDevices(nodes);

  //Create Application in nodes

  //Method 1
  /*
  for (uint32_t i=0; i<nodes.GetN(); i++)
  {
    Ptr<CustomApplication> app_i = CreateObject<CustomApplication>();
    app_i->SetBroadcastInterval (Seconds(interval));
    app_i->SetStartTime (Seconds (0));
    app_i->SetStopTime (Seconds (simTime));
    nodes.Get(i)->AddApplication (app_i);
  }
  */
 

  
  for (uint32_t i=0 ; i<nodes.GetN() ; i++)
  {
    ObjectFactory fact;
    fact.SetTypeId ("ns3::CustomApplication");
    fact.Set ("Interval", TimeValue (MilliSeconds(10)));
    Ptr<Application> appI = fact.Create <Application> ();
    appI->SetStartTime(Seconds(0));
    appI->SetStopTime (Seconds (0));
    nodes.Get(i)->AddApplication(appI);
  }
  
  //Now the nodes are ready with WaveNetDevice object attach to all of them, let's install applications
 

  Simulator::Stop(Seconds(simTime));
  Simulator::Run();
  Simulator::Destroy();

}
