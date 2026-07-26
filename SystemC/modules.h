// =========================================================================
// modules.h
// SystemC model of the Industry 5.0 Smart Collaborative Workstation
// Controller (SCWC). Mirrors the validated Embedded-C logic in
// Source_Code/controller.c so the same thresholds and priority order
// can be exercised as a cycle-accurate hardware/software co-simulation.
//
// Target environment: EDA Playground (systemc-2.3.3, gcc) or a local
// SystemC installation (https://systemc.org).
// =========================================================================
#ifndef MODULES_H
#define MODULES_H

#include <systemc.h>

// ---- Threshold configuration (identical to controller.h) --------------
static const double TEMP_WARNING_C   = 60.0;
static const double TEMP_CRITICAL_C  = 80.0;
static const double VIB_WARNING_MMPS = 4.0;
static const double VIB_CRITICAL_MMPS= 7.0;
static const double PROX_DANGER_CM   = 30.0;
static const double PROX_WARNING_CM  = 60.0;
static const double POWER_IDLE_W     = 5.0;

enum SystemState { NORMAL = 0, LOW_POWER, WARNING, CRITICAL, SAFETY_STOP };

// -------------------------------------------------------------------------
// SC_MODULE: SensorStimulus
// Drives deterministic test vectors (and an optional pseudo-random shift
// simulation) onto the sensor signal bundle, one set per clock edge.
// -------------------------------------------------------------------------
SC_MODULE(SensorStimulus) {
    sc_in<bool>      clk;
    sc_out<double>   temperature;
    sc_out<double>   vibration;
    sc_out<double>   proximity;
    sc_out<double>   power;

    void generate();

    SC_CTOR(SensorStimulus) {
        SC_METHOD(generate);
        sensitive << clk.pos();
    }
};

// -------------------------------------------------------------------------
// SC_MODULE: Controller
// Combinational-then-registered decision logic, safety-first priority.
// -------------------------------------------------------------------------
SC_MODULE(Controller) {
    sc_in<bool>      clk;
    sc_in<double>    temperature;
    sc_in<double>    vibration;
    sc_in<double>    proximity;
    sc_in<double>    power;

    sc_out<int>      state;
    sc_out<int>      motorSpeedPercent;
    sc_out<int>      alertLevel;
    sc_out<bool>     emergencyStop;

    void process();

    SC_CTOR(Controller) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

// -------------------------------------------------------------------------
// SC_MODULE: ActuatorMonitor
// Logs the applied control decision every clock edge (stand-in for the
// motor drive / alert tower / safety relay of the physical cell).
// -------------------------------------------------------------------------
SC_MODULE(ActuatorMonitor) {
    sc_in<bool>   clk;
    sc_in<double> temperature, vibration, proximity, power;
    sc_in<int>    state, motorSpeedPercent, alertLevel;
    sc_in<bool>   emergencyStop;

    void logCycle();

    SC_CTOR(ActuatorMonitor) {
        SC_METHOD(logCycle);
        sensitive << clk.pos();
    }
};

#endif // MODULES_H
