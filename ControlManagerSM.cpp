

#include "qp_port.h"
#include "bsp.h"
#include "ControlManager.h"

//Q_DEFINE_THIS_FILE

using namespace QP;

// local objects -------------------------------------------------------------
class ControlManagerSM : public QActive // extend the QActive class
{                      
    // Variables
    ControlManager manager;
    
public:
    ControlManagerSM() : 
        QActive((QStateHandler)&ControlManagerSM::initial)     
     { }
     
private:
    // Member functions


    
    // QStates
    static QState initial  (ControlManagerSM *me, QEvt const *e);
    static QState active   (ControlManagerSM *me, QEvt const *e);
    
};

static ControlManagerSM l_ControlManagerSM;             // the sole instance of the ControlManager active object

// global objects ------------------------------------------------------------
QActive * AO_ControlManagerSM = &l_ControlManagerSM;                // opaque pointer to ControlManager AO

// HSM definition ------------------------------------------------------------
//............................................................................
QState ControlManagerSM::initial(ControlManagerSM *me, QEvt const *) {

    me->subscribe(TIME_TICK_SIG);           // subscribe for time event


    QS_OBJ_DICTIONARY(&l_ControlManagerSM);           // object dictionary for ControlManager object
    QS_FUN_DICTIONARY(&ControlManagerSM::initial);   // function dictionaries for ControlManager HSM
    QS_FUN_DICTIONARY(&ControlManagerSM::active);
    QS_SIG_DICTIONARY(TIME_TICK_SIG, &l_ControlManagerSM);
    QS_SIG_DICTIONARY(Q_CONFIG_SIG, &l_ControlManagerSM);         // local signals

    return Q_TRAN(&ControlManagerSM::active);               // top-most initial transition
}
//............................................................................
QState ControlManagerSM::active(ControlManagerSM *me, QEvt const *e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: 
        {
            me->manager.initialize();

            
            return Q_SUPER(&QHsm::top);
            break;
        }
        case Q_CONFIG_SIG: 
        {    
            //configure(e->cfg);// configure functions tbd
            
            return Q_HANDLED();
        }
        case TIME_TICK_SIG: 
        {
            me->manager.readSensor();
            me->manager.computeControl();
            me->manager.sendControl();
                            
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm::top);
}
