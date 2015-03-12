// Base Class providing sensor interface
#include "config.h"
#include "IMUSensor.h"
#include "LSM9DS0.h"
#include "InterfaceSensor.h"


//---------------------------------------------------------------------------------------
IMUSensor::IMUSensor() :
    LSM9DS0(SDA_IMU, SCL_IMU, CSG, CSXM)
{
    //IMUSensor Constructor
}


//---------------------------------------------------------------------------------------
bool
IMUSensor::initSensor(void)
{
    bool isOK = false;
    uint16_t status = begin();
    if(18900 == status)
    {
        isOK = true;
        LOG("WHO_AM_I received correctly \r\n");
    }
    else
    {
        WARN("WHO_AM_I from IMUSensor Error : 0x%x\r\n", status);
    }
    
    return isOK;
}


//---------------------------------------------------------------------------------------
int
IMUSensor::gyroGetX(void)
{
    readGyro();
    
    return gx;
}

//---------------------------------------------------------------------------------------
int
IMUSensor::gyroGetY(void)
{
    readGyro();
    
    return gy_raw;
}
