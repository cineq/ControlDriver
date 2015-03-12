//////////////////////////////////////////////////////////////////////////////
// Product: QP/C++ port to ARM Cortex, selectable Vanilla/QK, mbed compiler
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
#ifndef qp_port_h
#define qp_port_h

#include "qp_config.h"    // QP configuration defined at the application level

//............................................................................
// QF interrupt disable/enable and log2()...
#ifdef ARM_ARCH_V6M                          // Cortex-M0/M0+/M1 ?, see NOTE02

    #define QF_INT_DISABLE()        __asm volatile ("cpsid i")
    #define QF_INT_ENABLE()         __asm volatile ("cpsie i")

    // QF-aware ISR priority for CMSIS function NVIC_SetPriority(), NOTE2
    #define QF_AWARE_ISR_CMSIS_PRI  0

#else                                          // Cortex-M3/M4/M4F, see NOTE03

    #define QF_SET_BASEPRI(val_)    __asm volatile (\
        "movs r0,%0 \n\t" \
        "msr  BASEPRI,r0" :: "I" (val_) : "cc", "r0")
    #define QF_INT_DISABLE()        QF_SET_BASEPRI(QF_BASEPRI)
    #define QF_INT_ENABLE()         QF_SET_BASEPRI(0U)

    // NOTE: keep in synch with the value defined in "qk_port.s", see NOTE4
    #define QF_BASEPRI  (0xFF >> 2)

    // QF-aware ISR priority for CMSIS function NVIC_SetPriority(), NOTE5
    #define QF_AWARE_ISR_CMSIS_PRI  (QF_BASEPRI >> (8 - __NVIC_PRIO_BITS))

                 // Cortex-M3/M4/M4F provide the CLZ instruction for fast LOG2
    #define QF_LOG2(n_) ((uint8_t)(32U - __builtin_clz(n_)))
#endif

// QF critical section entry/exit
// QF_CRIT_STAT_TYPE not defined: "unconditional interrupt unlocking" policy
#define QF_CRIT_ENTRY(dummy)        QF_INT_DISABLE()
#define QF_CRIT_EXIT(dummy)         QF_INT_ENABLE()
#define QF_CRIT_EXIT_NOP()          __asm volatile ("nop")

#ifdef QK_PREEMPTIVE
                                                // QK interrupt entry and exit
#define QK_ISR_ENTRY() do { \
    QF_INT_DISABLE(); \
    ++QK_intNest_; \
    QF_QS_ISR_ENTRY(QK_intNest_, QK_currPrio_); \
    QF_INT_ENABLE(); \
} while (0)

#define QK_ISR_EXIT()  do { \
    QF_INT_DISABLE(); \
    QF_QS_ISR_EXIT(QK_intNest_, QK_currPrio_); \
    --QK_intNest_; \
    if (QK_schedPrio_() != (uint8_t)0) { \
        *Q_UINT2PTR_CAST(uint32_t, 0xE000ED04U) = (uint32_t)0x10000000U; \
    } \
    QF_INT_ENABLE(); \
} while (0)

#else
    #define QK_ISR_ENTRY() ((void)0)
    #define QK_ISR_EXIT()  ((void)0)
#endif

#ifdef Q_SPY
    #define QS_TIME_SIZE        4
    #define QS_OBJ_PTR_SIZE     4
    #define QS_FUN_PTR_SIZE     4
#endif

#include <stdint.h>    // mbed compiler supports standard exact-width integers

#include "qp.h"                           // QP platform-independent interface

#endif                                                            // qp_port_h
