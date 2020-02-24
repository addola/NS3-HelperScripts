#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "custom-mobility-model.h"
using namespace ns3;
void PrintInfo ()
{
    Ptr<Node> n0 =  ns3::NodeList::GetNode(0);
    Ptr<Node> n1 =  ns3::NodeList::GetNode(1);

    Ptr<MobilityModel> m0 = n0->GetObject<MobilityModel> ();
    Ptr<MobilityModel> m1 = n1->GetObject<MobilityModel> ();
    
    std::cout << "n0 Vel:" << m0->GetVelocity() << "\t\tn1 Vel: " << m1->GetVelocity() << std::endl;

    if (Now() == Seconds (3))
    {
        Ptr<CustomMobilityModel> cmm = DynamicCast<CustomMobilityModel> (m0);
        cmm->SetAccelerationValue (0.5);
    }

    Simulator::Schedule (Seconds (1), &PrintInfo);

}
int main (int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse (argc, argv);

    NodeContainer nodes;
    nodes.Create (2);

    MobilityHelper mob;
    mob.SetMobilityModel ("ns3::CustomMobilityModel");
    mob.Install (nodes);

    Ptr<CustomMobilityModel> m0 = DynamicCast<CustomMobilityModel>(nodes.Get(0)->GetObject<MobilityModel> ());
    Ptr<CustomMobilityModel> m1 = DynamicCast<CustomMobilityModel>(nodes.Get(1)->GetObject<MobilityModel> ());

    //Initial positions
    m0->SetPosition (Vector (50,10,0));
    m1->SetPosition (Vector (75,20,0));

    m0->SetVelocityAndAcceleration (Vector (0,0,0), Vector (3,0,0));
    m1->SetVelocityAndAcceleration (Vector (10,0,0), Vector (-3,0,0));

    Simulator::Schedule (Seconds (1), &PrintInfo);
    Simulator::Stop (Seconds (60));
    Simulator::Run();
    Simulator::Destroy ();
    

}