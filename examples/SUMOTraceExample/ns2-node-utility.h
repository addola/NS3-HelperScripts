/*
 * ns2-node-utility.h
 *
 *  Created on: Sep 14, 2020
 *      Author: addola
 */

#ifndef SUMOTRACEEXAMPLE_NS2_NODE_UTILITY_H_
#define SUMOTRACEEXAMPLE_NS2_NODE_UTILITY_H_

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>

namespace ns3
{
  class Ns2NodeUtility
  {
  public:
    Ns2NodeUtility (std::string file_name);
    /** Prints information. For debugging
    */
    void PrintInformation ();
    /**
     * \return the number of nodes in the ns-2 trace
     */
    uint32_t GetNNodes ();
    /**
     * \param nodeId of the node.
     * \return the time the node has entered into the ns-2 simulation trace in double.
     */
    double GetEntryTimeForNode (uint32_t nodeId);
    /** Get the last time stamp of a node. At this point we can assume a node has left the simulation environment so we can turn its applications or communication devices off.
     * \param nodeId of the node
     * \return the last timestamp the node has done something during the ns-2 simulation trace in double.
     */
    double GetExitTimeForNode (uint32_t nodeId);
    /**
     * \return The total simulation time of the ns-2 simulation in seconds as double
     */
    double GetSimulationTime ();

  private:
    std::string m_file_name; /**< File name of the ns-2 mobility trace */
    std::ifstream m_input_file; /**< input file stream object */
    std::map <uint32_t, std::pair<double, double>> m_node_times; /**< a map to keep track of nodes entry & exit times. */

  };
}

#endif /* NS_3_30_1_SCRATCH_YOUTUBEQUESTIONS_NS2_NODE_UTILITY_H_ */
