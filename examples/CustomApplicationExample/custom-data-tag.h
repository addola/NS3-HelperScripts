/*
 * This tag combines position, velocity, and acceleration in one tag.
 */

#ifndef CUSTOM_DATA_TAG_H
#define CUSTOM_DATA_TAG_H

#include "ns3/tag.h"
#include "ns3/vector.h"
#include "ns3/nstime.h"

namespace ns3
{
	/** We're creating a custom tag for simulation. A tag can be added to any packet, but you cannot add a tag of the same type twice.
	*/
class CustomDataTag : public Tag {
public:

	//Functions inherited from ns3::Tag that you have to implement. 
	static TypeId GetTypeId(void);
	virtual TypeId GetInstanceTypeId(void) const;
	virtual uint32_t GetSerializedSize(void) const;
	virtual void Serialize (TagBuffer i) const;
	virtual void Deserialize (TagBuffer i);
	virtual void Print (std::ostream & os) const;

	//These are custom accessor & mutator functions
	Vector GetPosition(void);
	uint32_t GetNodeId();
	Time GetTimestamp ();

	void SetPosition (Vector pos);
	void SetNodeId (uint32_t node_id);
	void SetTimestamp (Time t);



	CustomDataTag();
	CustomDataTag(uint32_t node_id);
	virtual ~CustomDataTag();
private:

	uint32_t m_nodeId;
	/** Current position */
	Vector m_currentPosition;
	/** Timestamp this tag was created */
	Time m_timestamp;

};
}

#endif 