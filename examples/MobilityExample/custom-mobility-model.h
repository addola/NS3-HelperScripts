/*
 * NOTE : Using a custom Mobility Model works, but have issues working with PyViz due to lack of Python Bindings.
 * therefore, work on this is halted for now.
 */

#ifndef NS3_SCRATCH_CUSTOM_MOBILITY_MODEL_H
#define NS3_SCRATCH_CUSTOM_MOBILITY_MODEL_H

#include "ns3/constant-acceleration-mobility-model.h"
#include "ns3/mobility-model.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
namespace ns3
{
/** \brief An implementation of longitudinal vehicular mobility on an x-axis only. This implementation is based on ns3::ConstantAccelerationMobilityModel, but it adds limits on velocity and acceleration.
 * 
 * You can set an upper and lower limits for acceleration. The default values are 3.6576 & -3.6576 in m/s^2
 * You can also set the max velocity. Default value is 30 m/s.
 * 
 * If you set the acceleration to a positive value, it calculates the time to reach the maximum velocity, and schedules an event to set the acceleration to zero at that time.
 * If acceleration is set to a negative value, it would calculate the time to reach zero velocity and schedule and event to set the acceleration to zero at that time.
 * 
 * The event scheduled to set acceleration to zero is cancelled and replaced by another event everytime the accelration value is set again.
 * 
 * 
 */
class CustomMobilityModel : public MobilityModel
{
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);


  virtual TypeId GetInstanceTypeId() const;
  /**
   * Create position located at coordinates, velocity & acceleration set to (0,0,0) 
   */
  CustomMobilityModel ();
  virtual ~CustomMobilityModel ();
  /**
   * Set the model's velocity and acceleration
   * \param velocity the velocity (m/s) of type ns3::Vector
   * \param acceleration the acceleration (m/s^2) of type ns3::Vector
   */
  void SetVelocityAndAcceleration (const Vector &velocity, const Vector &acceleration);
  /** \brief Change the acceleration. If the value is greater than the max limit, then it's set to that limit. If it's less than the lowest limit, it's set to the lower limit.
   */
  void SetAccelerationValue (double value);

  /** \brief Returns the acceleration as a ns3::Vector form
   */ 
  Vector GetAcceleration();

  /** \brief Returns the acceleration as a double
   */
  double GetAccelerationValue ();

  /** \brief Sets the minimum and maximum acceleration values in m/s^2. This depends on the vehicle.
   * \param min the minimum acceleration value. This is a negative value for deceleration.
   * \param max the maximum acceleration value.
   */
  void SetAccelerationLimits (double min, double max);
  /** \brief Sets the maximum speed. This is usually the speed limit of the road.
   */
  void SetMaxVelocity (double max);

  /** \brief Sanity check for debugging
   */
  void SanityCheck ();

private:

  /* \brief A function that is scheduled to set acceleration to zero when max velocity is reached.
  */
  void SetAccelerationToZeroMaxVelocity ();
  /* \brief A function that is scheduled to set acceleration to zero when zero velocity is reached. 
  */
  void SetAccelerationToZeroZeroVelocity ();

  virtual Vector DoGetPosition (void) const;
  virtual void DoSetPosition (const Vector &position);
  virtual Vector DoGetVelocity (void) const;

  Time m_baseTime;  //!< the base time
  Vector m_basePosition; //!< the base position
  Vector m_baseVelocity; //!< the base velocity
  Vector m_acceleration;  //!< the acceleration

  Time m_updatePeriod; //!< How often to update
  Time m_timeToSpeedlimit; //!< When acceleration is adjusted, we compute the time at which the vehicle would reach max allowed velocity. We do this so that we would set the acceleration to 0 as vehicles must not exceed the speed limit


  EventId m_limit_event; //!< An event that's scheduled in anticipation of reaching max speed. If acceleration changes before then, it's cancelled, and a new one is scheduled
  double m_theta; //!< Heading angle. If vehicle is moving along x-axis, forward it would be 0, if opposite direction, it's PI (3.14)
  double m_max_velocity;
  double m_min_velocity;
  double m_min_acceleration;
  double m_max_acceleration;
  
};

}


#endif /* NS_3_30_1_CONTRIB_CACC_MOBILITY_VEHICULAR_MOBILITY_MODEL_H_ */
