#include "user-class.h"

namespace ns3 
{

UserClass::UserClass() {}

UserClass::~UserClass () {}

TypeId UserClass::GetTypeId ()
{
    static TypeId tid = TypeId("ns3::UserClass")
            .SetParent <Object> ()
            .AddConstructor <UserClass>()
            ;
    return tid;
}

double UserClass::GetValue()
{
    return m_value;
}

void UserClass::SetValue (double val)
{
    m_value = val;
}


}