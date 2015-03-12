// Base Class providing sensor interface
#ifndef _IMUSensor_H__
#define _IMUSensor_H__

#include "LSM9DS0.h"
#include "InterfaceSensor.h"
#include "IMUSensor.h"
#include "LogUtil.h"

//Serial logg(SERIAL_TX, SERIAL_RX);
//LogUtil logger(logg);


class IMUSensor : public LSM9DS0, public InterfaceSensor
{
public:
  IMUSensor();

    // Sensor Interface functions
    virtual bool initSensor(void);
    virtual int  gyroGetX(void);
    virtual int  gyroGetY(void);
  
}; 


#endif // _IMUSensor_H__