// =========================================================================
// modules.cpp
// Implementation of the SystemC modules declared in modules.h
// =========================================================================
#include "modules.h"
#include <cstdlib>

// Same 10 deterministic test vectors used by the Embedded-C harness
// (Source_Code/sensor.c) so both models can be cross-validated.
struct Vec { double t, v, p, w; const char *label; };
static const Vec VECTORS[10] = {
    {42.0, 1.2, 150.0, 320.0, "TC0 Nominal"},
    {62.5, 1.5, 150.0, 340.0, "TC1 Temp warning boundary"},
    {85.0, 2.0, 150.0, 360.0, "TC2 Temp critical"},
    {45.0, 4.5, 150.0, 330.0, "TC3 Vibration warning boundary"},
    {46.0, 8.2, 150.0, 335.0, "TC4 Vibration critical"},
    {40.0, 1.1, 55.0,  310.0, "TC5 Proximity warning"},
    {41.0, 1.0, 18.0,  310.0, "TC6 Proximity danger (E-stop)"},
    {90.0, 7.5, 12.0,  355.0, "TC7 Multi-fault + safety priority"},
    {30.0, 0.2, 400.0, 3.0,   "TC8 Idle -> low power"},
    {60.0, 4.0, 60.0,  325.0, "TC9 Exact-threshold edge case"}
};

void SensorStimulus::generate() {
    static int cycle = 0;
    int idx = cycle % 10;
    temperature.write(VECTORS[idx].t);
    vibration.write(VECTORS[idx].v);
    proximity.write(VECTORS[idx].p);
    power.write(VECTORS[idx].w);
    cycle++;
}

void Controller::process() {
    double t = temperature.read();
    double v = vibration.read();
    double p = proximity.read();
    double w = power.read();

    // Priority 1: human safety -- always wins
    if (p <= PROX_DANGER_CM) {
        state.write(SAFETY_STOP);
        motorSpeedPercent.write(0);
        alertLevel.write(2);
        emergencyStop.write(true);
        return;
    }
    // Priority 2: critical machine health
    if (t >= TEMP_CRITICAL_C || v >= VIB_CRITICAL_MMPS) {
        state.write(CRITICAL);
        motorSpeedPercent.write(20);
        alertLevel.write(2);
        emergencyStop.write(false);
        return;
    }
    // Priority 3: warning
    if (t >= TEMP_WARNING_C || v >= VIB_WARNING_MMPS || p <= PROX_WARNING_CM) {
        state.write(WARNING);
        motorSpeedPercent.write(60);
        alertLevel.write(1);
        emergencyStop.write(false);
        return;
    }
    // Priority 4: low power / sustainability
    if (w <= POWER_IDLE_W) {
        state.write(LOW_POWER);
        motorSpeedPercent.write(0);
        alertLevel.write(0);
        emergencyStop.write(false);
        return;
    }
    // Default: normal
    state.write(NORMAL);
    motorSpeedPercent.write(100);
    alertLevel.write(0);
    emergencyStop.write(false);
}

static const char *stateName(int s) {
    switch (s) {
        case NORMAL: return "NORMAL";
        case LOW_POWER: return "LOW_POWER";
        case WARNING: return "WARNING";
        case CRITICAL: return "CRITICAL";
        case SAFETY_STOP: return "SAFETY_STOP";
        default: return "UNKNOWN";
    }
}

void ActuatorMonitor::logCycle() {
    std::cout << "@" << sc_time_stamp()
               << " T=" << temperature.read() << "C"
               << " Vib=" << vibration.read() << "mm/s"
               << " Prox=" << proximity.read() << "cm"
               << " P=" << power.read() << "W"
               << " | STATE=" << stateName(state.read())
               << " Speed=" << motorSpeedPercent.read() << "%"
               << " Alert=" << alertLevel.read()
               << " EStop=" << emergencyStop.read()
               << std::endl;
}
