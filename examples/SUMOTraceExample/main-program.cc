#include "../SUMOTraceExample/custom-application.h"
#include "../SUMOTraceExample/wave-setup.h"
#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns2-node-utility.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  std::string mobility_file = "scratch/SUMOTraceExample/ns2mobility.tcl";

  //A tool I created so that we only start the applications within nodes when they actually enter the simulation.
  Ns2NodeUtility ns2_utility (mobility_file);

  uint32_t nnodes = ns2_utility.GetNNodes();
  double sim_time = ns2_utility.GetSimulationTime();

  NodeContainer nodes;
  nodes.Create (nnodes);

  //Using the bulit-in ns-2 mobility helper
  Ns2MobilityHelper sumo_trace (mobility_file);
  sumo_trace.Install(); //install ns-2 mobility in all nodes

  //To write shorter code, I put the code to setup WaveNetDevice in a separate file.
  WaveSetup wave;
  wave.ConfigureDevices(nodes);

  std::cout << "NS2 mobility & devices configured..." << std::endl;
  //Let's install my CustomApplication to all nodes and start them at the appropriate time using my utilitiy.
  for (uint32_t i=0 ; i<nnodes; i++)
    {
      Ptr<Node> n = nodes.Get(i);
      Ptr<CustomApplication> app = CreateObject <CustomApplication>  ();
      app->SetStartTime(Seconds (ns2_utility.GetEntryTimeForNode(i)));
      app->SetStopTime (Seconds (ns2_utility.GetExitTimeForNode(i)));
      n->AddApplication(app);
    }

  std::cout << "Applications setup done!" << std::endl;
  Simulator::Stop(Seconds (sim_time)); //because this is the last timestamp in your ns-2 trace
  Simulator::Run ();

  std::cout << "End of Program" << std::endl;

}
