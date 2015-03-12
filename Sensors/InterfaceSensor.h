// Inerfce for sensors using by Controll Manager

#ifndef _InterfaceSensor_H__
#define _InterfaceSensor_H__

class InterfaceSensor
{
public:

    virtual bool initSensor(void) = 0;
    
    // Getting gyro positions
    virtual int  gyroGetX(void) = 0;
    virtual int  gyroGetY(void) = 0;
    //virtual int  gyroGetZ(void) = 0;
    
    // Getting accelerometer positions
    //virtual int  accGetX(void) = 0;
    //virtual int  accGetY(void) = 0;
    //virtual int  accGetZ(void) = 0;
    
};
    
#endif // _InterfaceSensor_H__