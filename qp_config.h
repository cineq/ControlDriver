//////////////////////////////////////////////////////////////////////////////
// Product: QP-mbed configuration file
// Last Updated for Version: 4.2.04
// Date of the Last Update:  Sep 25, 2011
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) 2002-2011 Quantum Leaps, LLC. All rights reserved.
//
// This software may be distributed and modified under the terms of the GNU
// General Public License version 2 (GPL) as published by the Free Software
// Foundation and appearing in the file GPL.TXT included in the packaging of
// this file. Please note that GPL Section 2[b] requires that all works based
// on this software must also be made publicly available under the terms of
// the GPL ("Copyleft").
//
// Alternatively, this software may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GPL and are specifically designed for licensees interested in
// retaining the proprietary status of their code.
//
// Contact information:
// Quantum Leaps Web site:  http://www.quantum-leaps.com
// e-mail:                  info@quantum-leaps.com
//////////////////////////////////////////////////////////////////////////////
#ifndef qp_config_h
#define qp_config_h
 
// enable the Q-SPY software tracing instrumentation
//#define Q_SPY
 
// enable preemptive QK kernel
#define QK_PREEMPTIVE
 
// The maximum number of active objects in the application (could be up to 63)
#define QF_MAX_ACTIVE      16

#define QF_MAX_TICK_RATE    2
 
#endif                                                          // qp_config_h
 
