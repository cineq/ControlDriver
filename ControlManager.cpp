// ControlMnager - main controller class, which provides initialization, configuration and work
// of Sensors, PID controller, Logger etc 
// v.0.1
// by Marcin Kurpiel

#include "ControlManager.h"
#include "LogUtil.h"


//---------------------------------------------------------------------------------------
ControlManager::ControlManager()
{
    sensor_ptr = new IMUSensor();
    regulator_ptr = new PID(1, 1, 1, 1);
}


//---------------------------------------------------------------------------------------
bool
ControlManager::initialize(void)
{
    bool isInitialized = false;
    
    // Initialize Sensors
    if(sensor_ptr->initSensor())
    {
        isInitialized += true;
        LOG("IMU Sensor initialized \r\n");
    }
    else
    {
        WARN("IMU Sensor not initialized \r\n");
    }
    
    // Initialize Motor Controler
    
    // Initialize Control Regulator
    
    return isInitialized;
}


//---------------------------------------------------------------------------------------
void 
ControlManager::start(void)
{
    // Initialize procedure
    initialize();
    
    // Create Thread
    
    
}

//---------------------------------------------------------------------------------------
void
ControlManager::readSensor(void)
{
    i_angle = sensor_ptr -> gyroGetX();
    LOG("Read angle: %f \r\n", i_angle);
}


//---------------------------------------------------------------------------------------
void
ControlManager::computeControl(void)
{
    regulator_ptr -> setProcessValue(i_angle);
    i_control = regulator_ptr -> compute();
}


//---------------------------------------------------------------------------------------
void
ControlManager::sendControl(void)
{
    LOG("Computed control: %f \r\n", i_control);
}
