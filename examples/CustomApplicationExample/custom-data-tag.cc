/*
 * cacc-data-tag.cc
 *
 *  Created on: Oct 29, 2019
 *      Author: adil
 */
#include "custom-data-tag.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("CustomDataTag");
NS_OBJECT_ENSURE_REGISTERED (CustomDataTag);

CustomDataTag::CustomDataTag() {
	m_timestamp = Simulator::Now();
	m_nodeId = -1;
}
CustomDataTag::CustomDataTag(uint32_t node_id) {
	m_timestamp = Simulator::Now();
	m_nodeId = node_id;
}

CustomDataTag::~CustomDataTag() {
}

//Almost all custom tags will have similar implementation of GetTypeId and GetInstanceTypeId
TypeId CustomDataTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CustomDataTag")
    .SetParent<Tag> ()
    .AddConstructor<CustomDataTag> ();
  return tid;
}
TypeId CustomDataTag::GetInstanceTypeId (void) const
{
  return CustomDataTag::GetTypeId ();
}

/** The size required for the data contained within tag is:
 *   	size needed for a ns3::Vector for position +
 * 		size needed for a ns3::Time for timestamp + 
 * 		size needed for a uint32_t for node id
 */
uint32_t CustomDataTag::GetSerializedSize (void) const
{
	return sizeof(Vector) + sizeof (ns3::Time) + sizeof(uint32_t);
}
/**
 * The order of how you do Serialize() should match the order of Deserialize()
 */
void CustomDataTag::Serialize (TagBuffer i) const
{
	//we store timestamp first
	i.WriteDouble(m_timestamp.GetDouble());

	//then we store the position
	i.WriteDouble (m_currentPosition.x);
	i.WriteDouble (m_currentPosition.y);
	i.WriteDouble (m_currentPosition.z);

	//Then we store the node ID
	i.WriteU32(m_nodeId);
}
/** This function reads data from a buffer and store it in class's instance variables.
 */
void CustomDataTag::Deserialize (TagBuffer i)
{
	//We extract what we stored first, so we extract the timestamp
	m_timestamp =  Time::FromDouble (i.ReadDouble(), Time::NS);;

	//Then the position
	m_currentPosition.x = i.ReadDouble();
	m_currentPosition.y = i.ReadDouble();
	m_currentPosition.z = i.ReadDouble();
	//Finally, we extract the node id
	m_nodeId = i.ReadU32();

}
/**
 * This function can be used with ASCII traces if enabled. 
 */
void CustomDataTag::Print (std::ostream &os) const
{
  os << "Custom Data --- Node :" << m_nodeId <<  "\t(" << m_timestamp  << ")" << " Pos (" << m_currentPosition << ")";
}

//Your accessor and mutator functions 
uint32_t CustomDataTag::GetNodeId() {
	return m_nodeId;
}

void CustomDataTag::SetNodeId(uint32_t node_id) {
	m_nodeId = node_id;
}
Vector CustomDataTag::GetPosition(void) {
	return m_currentPosition;
}

Time CustomDataTag::GetTimestamp() {
	return m_timestamp;
}

void CustomDataTag::SetPosition(Vector pos) {
	m_currentPosition = pos;
}

void CustomDataTag::SetTimestamp(Time t) {
	m_timestamp = t;
}

} /* namespace ns3 */


