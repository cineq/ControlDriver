// ControlMnager - main controller class, which provides initialization, configuration and work
// of Sensors, PID controller, Logger etc 
// v.0.1
// by Marcin Kurpiel
#ifndef _CONTROL_MANAGER_H__
#define _CONTROL_MANAGER_H__

#include "IMUSensor.h"
#include "LogUtil.h"
#include "PID.h"

//Serial logg(SERIAL_TX, SERIAL_RX);
//LogUtil logger(logg);

class ControlManager
{
public:
    ControlManager();

    bool initialize(void);
    void start(void);
    void readSensor(void);
    void computeControl(void);
    void sendControl(void);
    
private:
    IMUSensor*    sensor_ptr;
    PID*          regulator_ptr;
    float        i_angle;
    float        i_control;
};


#endif // _CONTROL_MANAGER_H__
