/*
 * Robot Manager application
 *
 */
#include "mbed.h"
#include "config.h"
#include "LogUtil.h"
#include "qp_port.h"
#include "bsp.h"

using namespace QP;

Serial logg(SERIAL_TX, SERIAL_RX);
LogUtil logger(logg);

extern QActive * const AO_ControlManagerSM;
static QP::QEvt const * controlManagerQueue[10];
// QP Subscribe List
static QP::QSubscrList subscribeList[MAX_PUB_SIG];
static QF_MPOOL_EL(QP::QEvt) smlPoolSto[10];
//---------------------------------------------------------------------------------------
// I2C Configuration
DigitalOut pin(PC_8, 1);
DigitalOut pin2(PC_6, 1);

int main()
{
    // Starting up Robot Manager
    LOG("<<=====Starting up Control Manager====>>\r\n");
    LOG("<<=====System CLOCK: %i ====>>\r\n", SystemCoreClock);

    // Create instance of Control Manager

    // Create Control Manager QP State Machine


    // Initialize Board Support Package
    BSP::BSP_init();

    // Initialize QP framework
    QF::init();
    QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));
    QF::psInit(subscribeList, Q_DIM(subscribeList));
    QS_OBJ_DICTIONARY(controlManagerQueue);
    QS_OBJ_DICTIONARY(smlPoolSto);

    AO_ControlManagerSM->start(1U, controlManagerQueue, Q_DIM(controlManagerQueue), static_cast<void *>(0), 0U);

    //timer.attach_us(BSP::SysTick_Handler, 1000000);

    QF::run();

}
