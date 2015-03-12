//////////////////////////////////////////////////////////////////////////////
// Product: DPP example, configurable Vanilla/QK kernel
// Last Updated for Version: 4.5.02
// Date of the Last Update:  Sep 04, 2012
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// Quantum Leaps Web sites: http://www.quantum-leaps.com
//                          http://www.state-machine.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#include "bsp.h"
#include "mbed.h"             // mbed is used only for the built-in serial
#include "LogUtil.h"

//////////////////////////////////////////////////////////////////////////////
namespace BSP {

Q_DEFINE_THIS_FILE

enum ISR_Priorities {      // ISR priorities starting from the highest urgency
    SYSTICK_PRIO
    // ...
};

static DigitalOut myLed(LED2);

#ifdef Q_SPY
    QP::QSTimeCtr l_tickTime;
    QP::QSTimeCtr l_tickPeriod;
    static uint8_t l_SysTick_Handler;   

    #define QSPY_BAUD_RATE  9600U

    enum AppRecords {                    // application-specific trace records
        PHILO_STAT = QP::QS_USER
    };

    Serial l_qspy(SERIAL_TX, SERIAL_RX);
#endif

//............................................................................
extern "C" void SysTick_Handler(void) __attribute__((__interrupt__));
extern "C" void SysTick_Handler(void)
{
    QK_ISR_ENTRY();                // inform the QK kernel of entering the ISR
    LOG("<<=====Sys Tick====>>\r\n");
#ifdef Q_SPY
    uint32_t volatile dummy = SysTick->CTRL; // clear the COUNTFLAG in SysTick
    l_tickTime += l_tickPeriod;              // account for the clock rollover
#endif

    //QP::QF::TICK(&l_SysTick_Handler);         // process all armed time events
    static QP::QEvt const tickEvt = QEVT_INITIALIZER(TIME_TICK_SIG);
    QP::QF::PUBLISH(&tickEvt,&l_SysTick_Handler);
    
    QK_ISR_EXIT();                  // inform the QK kernel of exiting the ISR
    LOG("<<=====Sys Tick performed====>>\r\n");
}

//............................................................................
void BSP_init(void) {
    // set the system clock as specified in lm3s_config.h (20MHz from PLL)
    //SystemInit();
    //SystemCoreClockUpdate();
    //SetSysClock();

    // initialize the QS software tracing...
    Q_ALLEGE(QS_INIT(static_cast<void *>(0)));
    QS_RESET();
    QS_OBJ_DICTIONARY(&l_SysTick_Handler);
}
//............................................................................
void BSP_terminate(int16_t const result) {
    (void)result;
}
//............................................................................

//............................................................................

//............................................................................

//............................................................................

//............................................................................
extern "C" void Q_onAssert(char_t const * const file, int_t const line) {
    (void)file;                                      // avoid compiler warning
    (void)line;                                      // avoid compiler warning
    QF_INT_DISABLE();            // make sure that all interrupts are disabled
                                                          // light up all LEDs


    for (;;) {          // NOTE: replace the loop with reset for final version
        LOG("assertion error\r\n");
        __NOP();
        __NOP();
    }
}

}                                                             // namespace DPP
//////////////////////////////////////////////////////////////////////////////

namespace QP {

//............................................................................
void QF::onStartup(void) {
// set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate

// set priorities of all interrupts in the system...
(void)SysTick_Config((SystemCoreClock - 2) / BSP::BSP_TICKS_PER_SEC);

NVIC_SetPriority(SysTick_IRQn, BSP::SYSTICK_PRIO);
//NVIC_SetPriority(EINT0_IRQn,   BSP::GPIOPORTA_PRIO);
//NVIC_EnableIRQ(SysTick_IRQn);

}
//............................................................................
void QF::onCleanup(void) {
}
//............................................................................
#ifdef QK_PREEMPTIVE

void QK::onIdle(void) {

    QF_INT_DISABLE();
    BSP::myLed = 1;                    // turn the LED4 on
    __NOP();                        // delay a bit to see some light intensity
    __NOP();
    __NOP();
    __NOP();
    BSP::myLed = 0;                    // turn the LED4 off
    QF_INT_ENABLE();

#ifdef Q_SPY
    if (BSP::l_qspy.writeable()) {
    
        QF_INT_DISABLE();
        uint16_t b = QS::getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            BSP::l_qspy.putc((uint8_t)b);
        }
    }
#else    
    // Put the CPU and peripherals to the low-power mode. You might need to
    // customize the clock management for your application, see the datasheet
    // for your particular Cortex-M3 MCU.
    //
    // Specifially for the mbed board, see the articles:
    // * "Power Management" http://mbed.org/cookbook/Power-Management; and
    // * "Interface Powerdown" at 
    //   http://mbed.org/users/simon/notebook/interface-powerdown/
    // 
    //__WFI();
#endif
}

#else                                         // non-preemptive Vanilla kernel

void QF::onIdle(void) {               // NOTE: called with interrupts DISABLED



#ifdef Q_SPY
    QF_INT_ENABLE();
    if (BSP::l_qspy.writeable()) {
    
        QF_INT_DISABLE();
        uint16_t b = QS::getByte();
        QF_INT_ENABLE();

        if (b != QS_EOD) {
            BSP::l_qspy.putc((uint8_t)b);
        }
    }
#else    
    // Put the CPU and peripherals to the low-power mode. You might need to
    // customize the clock management for your application, see the datasheet
    // for your particular Cortex-M3 MCU.
    //
    // Specifially for the mbed board, see the articles:
    // * "Power Management" http://mbed.org/cookbook/Power-Management; and
    // * "Interface Powerdown" at 
    //   http://mbed.org/users/simon/notebook/interface-powerdown/
    // 
    __WFI();
    QF_INT_ENABLE();
#endif
}

#endif                                                        // QK_PREEMPTIVE

//----------------------------------------------------------------------------
#ifdef Q_SPY
//............................................................................
bool QS::onStartup(void const *arg) {
    static uint8_t qsBuf[6*256];                     // buffer for Quantum Spy
    initBuf(qsBuf, sizeof(qsBuf));
    
    BSP::l_qspy.baud(QSPY_BAUD_RATE);
    
    BSP::l_tickPeriod = SystemCoreClock / BSP::BSP_TICKS_PER_SEC;
    BSP::l_tickTime   = BSP::l_tickPeriod;   // to start the timestamp at zero

                                                    // setup the QS filters...
    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);

//    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
//    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
//    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
//    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
//    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
//    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
//    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
//    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
//    QS_FILTER_OFF(QS_QF_MPOOL_GET);
//    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
//    QS_FILTER_OFF(QS_QF_PUBLISH);
//    QS_FILTER_OFF(QS_QF_NEW);
//    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
//    QS_FILTER_OFF(QS_QF_GC);
//    QS_FILTER_OFF(QS_QF_TICK);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
//    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
//    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
//    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
//    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    return true;                                             // return success
}
//............................................................................
void QS::onCleanup(void) {
}
//............................................................................
QSTimeCtr QS::onGetTime(void) {              // invoked with interrupts locked
    if ((SysTick->CTRL & 0x00000100U) == 0U) {            // COUNTFLAG no set?
        return BSP::l_tickTime - (QSTimeCtr)SysTick->VAL;
    }
    else {        // the rollover occured, but the SysTick_ISR did not run yet
        return BSP::l_tickTime + BSP::l_tickPeriod - (QSTimeCtr)SysTick->VAL;
    }
}
//............................................................................
void QS::onFlush(void) {
    uint16_t b;
    QF_INT_DISABLE();
    while ((b = QS::getByte()) != QS_EOD) {
        while (!BSP::l_qspy.writeable()) {    // wait until serial is writable
        }
        BSP::l_qspy.putc((uint8_t)b);
    }
    QF_INT_ENABLE();
}
#endif                                                                // Q_SPY
//----------------------------------------------------------------------------

}                                                              // namespace QP
