#ifndef WAVE_SETUP_H
#define WAVE_SETUP_H
#include "ns3/core-module.h"
#include "ns3/wave-module.h"
#include "ns3/network-module.h"

namespace ns3
{
/** \brief This is a "utility class". It does not an extension to ns3. 
 */
  class WaveSetup
  {
    public:
      WaveSetup ();
      virtual ~WaveSetup ();

      NetDeviceContainer ConfigureDevices (NodeContainer &n);
  };
}

#endif 