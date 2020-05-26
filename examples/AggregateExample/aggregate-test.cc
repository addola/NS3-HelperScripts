#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "user-class.h"

using namespace ns3;

class DummyClass : public ns3::Object
{
    public:
        static TypeId GetTypeId();
        DummyClass();
        virtual ~DummyClass ();
};

TypeId DummyClass::GetTypeId ()
{
    static TypeId tid = TypeId("ns3::DummyClass")
                .SetParent <Object> ()
                .AddConstructor<DummyClass>();
    return tid;
}

DummyClass::DummyClass ()
{

}
DummyClass::~DummyClass ()
{

}

void Fun ()
{
    Ptr<Node> nnn = NodeList::GetNode (0);
    Ptr<UserClass> uuu = nnn->GetObject <UserClass> ();
    std::cout << Now() <<  "\tIn Fun(): " << uuu->GetValue() << std::endl;
}

int main (int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse (argc, argv);

    NodeContainer nodes;
    nodes.Create (2);

    Ptr <UserClass> user = CreateObject <UserClass> ();
    user->SetValue (12);

    Ptr<Node> n0 = nodes.Get(0);
    n0->AggregateObject (user);

    Ptr <Node> new_copy = ns3::CopyObject (n0);
    std::cout << "Org Node:\t" << n0 << "\nCpy Node:\t" << new_copy << std::endl;
    
    //The copy doesn't have the aggregate
    Ptr <UserClass> obj = new_copy->GetObject <UserClass> ();    
    //NULL
    std::cout << obj << std::endl;
    
    //We can only get the aggregated object from the original
    obj = n0->GetObject <UserClass> ();
    std::cout << obj << "\t" << obj->GetValue() << " user=" << user << std::endl;

    //We can also go the other way around 
    Ptr <Node> ptr_to_original = user->GetObject<Node> ();
    std::cout << "Org Node:\t" << n0 << "\nPtr. Orig:\t" << ptr_to_original << std::endl;
    
    Ptr<DummyClass> test2 = CreateObject <DummyClass> ();
    n0->AggregateObject (test2);

    Ptr<Node> test2_ptr = test2->GetObject <Node> ();
    std::cout << "Test 2 Ptr:\t" << test2_ptr << std::endl;

    Ptr<Packet> packet = Create <Packet> (100);
    std::cout << "Packet Size:" << packet->GetSize() << std::endl;

    Simulator::Schedule (Seconds (5), &Fun);


    Simulator::Stop (Seconds (60));
    Simulator::Run();
    Simulator::Destroy ();
    

}