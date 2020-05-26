#ifndef NS3_SCRATCH_AGGREGATE_EXAMPLE_USER_CLASS_H
#define NS3_SCRATCH_AGGREGATE_EXAMPLE_USER_CLASS_H

#include "ns3/object.h"
#include "ns3/nstime.h"

namespace ns3
{
    class UserClass : public ns3::Object
    {
        public:
            static TypeId GetTypeId();
            UserClass();
            virtual ~UserClass();

            void SetValue (double val);
            double GetValue ();

        private:
            double m_value;
    };
}

#endif 