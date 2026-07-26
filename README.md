# Industry 5.0 Smart Collaborative Workstation Controller (SCWC)
### Embedded C / SystemC Simulation & Performance Evaluation

## 1. Overview
This project implements and simulates a **smart manufacturing-cell controller**
aligned with the three pillars of **Industry 5.0**:

| Pillar | Feature implemented |
|---|---|
| **Human-centricity** | Proximity-based human-robot collaboration safety interlock (highest-priority emergency stop) |
| **Resilience** | Predictive maintenance via continuous temperature & vibration health scoring |
| **Sustainability** | Adaptive low-power / idle mode when the cell is unoccupied and the motor is unloaded |

The controller reads four simulated sensor channels every control cycle,
runs them through a priority-based decision engine, drives simulated
actuators (motor speed, alert tower light, safety relay), logs every
cycle to CSV, and reports real-time **performance metrics** (worst-case
execution time, throughput, safety-response latency).

## 2. Repository Structure
```
EmbeddedSystem_Assessment/
│── README.md
│── Report.pdf / Report.docx
│── Source_Code/            <- Embedded C implementation (primary deliverable)
│   ├── main.c
│   ├── sensor.c / sensor.h
│   ├── controller.c / controller.h
│   ├── actuator.c / actuator.h
│   ├── performance.c / performance.h
│   └── simulation_log.csv  <- generated data trace (200-cycle run)
│── SystemC/                <- SystemC model (EDA Playground / local SystemC)
│   ├── system.cpp
│   ├── modules.cpp
│   └── modules.h
│── Diagrams/
│   ├── architecture.png / architecture.dot
│   ├── flowchart.png / flowchart.dot
│   └── performance_chart.png
│── Screenshots/
│   ├── output1_test_case_validation.png
│   └── output2_performance_summary.png
│── TestCases/
│   ├── test_cases.pdf
│   └── full_console_output.txt
```

## 3. Building & Running (Embedded C)
Any C compiler works; no external libraries are required beyond `libm`.

```bash
cd Source_Code
gcc -Wall -Wextra -O2 -o scwc_sim main.c sensor.c controller.c actuator.c performance.c -lm
./scwc_sim
```

The program runs in two phases:
1. **Deterministic test-case validation** — 10 fixed test vectors checked
   against expected states (PASS/FAIL harness).
2. **Continuous 200-cycle shift simulation** — a random-walk sensor model
   representing a realistic working shift, with full CSV logging and a
   performance summary printed at the end.

### Online simulators
The same source compiles unmodified on:
- **OnlineGDB** (C, gcc) — paste all `.c`/`.h` files, set `main.c` as entry.
- **Replit** (C template) — same as above.
- **Wokwi** — for a hardware-in-the-loop variant, the sensor-reading
  functions in `sensor.c` can be re-pointed to `analogRead()` calls on a
  DHT22 (temperature), an SW-420 (vibration), an HC-SR04 (proximity) and
  an ACS712 (current/power) wired to an Arduino Uno/ESP32 board.

## 4. Building & Running (SystemC)
`SystemC/` contains an equivalent cycle-accurate SystemC model (same
thresholds, same priority logic) for EDA Playground or a local SystemC
installation.

**EDA Playground:** Language = SystemC, Tools = GCC/SystemC 2.3.3, add the
three files, click *Run*.

**Local install:**
```bash
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 \
    system.cpp modules.cpp -lsystemc -lm -o scwc_systemc
LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64 ./scwc_systemc
```

## 5. Test Cases
10 deterministic test vectors validate every branch of the priority
decision tree, including edge cases and a multi-fault case that proves
the safety interlock always overrides machine-health alarms. See
`TestCases/test_cases.pdf` for the full table with expected vs. actual
results (10/10 PASS).

## 6. Performance Evaluation Summary (200-cycle run)
| Metric | Value |
|---|---|
| Average loop execution time | 0.00144 ms |
| Worst-case execution time (WCET) | 0.01200 ms |
| Controller throughput | ~694,000 iterations/sec |
| Worst-case safety-response latency | 0.01200 ms |
| State distribution | NORMAL 88.0% · LOW_POWER 10.5% · SAFETY_STOP 1.5% |

Full discussion and interpretation is in `Report.pdf`.

## 7. Author / Academic Integrity
This is original work prepared for the Embedded C/SystemC Simulation
Assessment. External libraries used: none beyond the C standard library
and (for the SystemC variant) the standard SystemC class library.
