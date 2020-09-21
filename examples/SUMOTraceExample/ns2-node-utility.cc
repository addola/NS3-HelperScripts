#include "ns2-node-utility.h"

#include <regex>

namespace ns3
{
  Ns2NodeUtility::Ns2NodeUtility (std::string name)
  {
    m_file_name = name;
    m_input_file.open(m_file_name);

    std::string line;

    std::vector <uint32_t> node_ids;

    while (std::getline (m_input_file, line))
      {
	std::smatch sm;
	std::regex r("\\$ns_ at (\\d*.\\d) \"\\$node_\\((\\d*)\\)");
	if (std::regex_search(line, sm, r))
	  {
	    //std::cout << "MATCH!" << std::endl;

	    uint32_t id = std::stoi(sm[2]);
	    double new_latest = std::stof(sm[1]);

	    if ( std::find (node_ids.begin(), node_ids.end(), id) != node_ids.end())
	      {
		double entry_time = std::get<0> (m_node_times [id]);
		m_node_times [id] = std::make_pair(entry_time, new_latest);
	      }
	    else
	      {
		m_node_times [id] = std::make_pair(new_latest, new_latest);
	      }
	    node_ids.push_back(id);
	  }
	else
	  {
	    //std::cout << "No Match!" << std::endl;
	  }
      }
  }
  uint32_t
  Ns2NodeUtility::GetNNodes ()
  {
    return m_node_times.size();
  }
  double
  Ns2NodeUtility::GetEntryTimeForNode (uint32_t nodeId)
  {
    return std::get<0>(m_node_times [nodeId]);
  }
  double
  Ns2NodeUtility::GetExitTimeForNode (uint32_t nodeId)
  {
    return std::get<1>(m_node_times [nodeId]);
  }
  double
  Ns2NodeUtility::GetSimulationTime()
  {
    double time = 0;

    for (uint32_t i=0 ; i<m_node_times.size(); i++)
      time = std::max ( time,  std::get<1>(m_node_times[i]) );

    return time;
  }


  void
  Ns2NodeUtility::PrintInformation()
  {
    uint32_t s = m_node_times.size();
    for (uint32_t i=0 ; i<s; i++)
      {
	std::cout << "Node " << i << " started " << std::get<0>(m_node_times[i]) << " and ended " << std::get<1>(m_node_times[i]) << std::endl;
      }
  }
}

