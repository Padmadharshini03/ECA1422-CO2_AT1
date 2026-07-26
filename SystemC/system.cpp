// =========================================================================
// system.cpp
// Top-level SystemC testbench for the SCWC Industry 5.0 controller model.
//
// EDA Playground setup:
//   - Language: SystemC
//   - Tools & Simulators: GCC/SystemC (2.3.3)
//   - Add system.cpp, modules.cpp, modules.h as the three design files
//   - Run
//
// Local build (example, Linux, SYSTEMC_HOME set to your install path):
//   g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 \
//       system.cpp modules.cpp -lsystemc -lm -o scwc_systemc
//   LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64 ./scwc_systemc
// =========================================================================
#include "modules.h"

int sc_main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    sc_clock clk("clk", 2, SC_NS);  // one clock edge = one control cycle

    sc_signal<double> temperature, vibration, proximity, power;
    sc_signal<int>    state, motorSpeedPercent, alertLevel;
    sc_signal<bool>   emergencyStop;

    SensorStimulus stim("stim");
    stim.clk(clk);
    stim.temperature(temperature);
    stim.vibration(vibration);
    stim.proximity(proximity);
    stim.power(power);

    Controller ctrl("ctrl");
    ctrl.clk(clk);
    ctrl.temperature(temperature);
    ctrl.vibration(vibration);
    ctrl.proximity(proximity);
    ctrl.power(power);
    ctrl.state(state);
    ctrl.motorSpeedPercent(motorSpeedPercent);
    ctrl.alertLevel(alertLevel);
    ctrl.emergencyStop(emergencyStop);

    ActuatorMonitor mon("mon");
    mon.clk(clk);
    mon.temperature(temperature);
    mon.vibration(vibration);
    mon.proximity(proximity);
    mon.power(power);
    mon.state(state);
    mon.motorSpeedPercent(motorSpeedPercent);
    mon.alertLevel(alertLevel);
    mon.emergencyStop(emergencyStop);

    std::cout << "=== SCWC SystemC test-vector simulation (10 cycles) ===" << std::endl;
    sc_start(20, SC_NS); // 10 clock periods -> all 10 test vectors

    return 0;
}
