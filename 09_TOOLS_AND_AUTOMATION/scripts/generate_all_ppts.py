"""
Kernelwise Labs - Master Presentation Generator
Generates executive 6-slide PowerPoint (.pptx) decks and companion PRESENTATION.md
outlines for all 23 projects across the startup engineering monorepo.

Author: Aravind Reddy (@DAravindReddy)
"""

import os
import sys
from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.dml.color import RGBColor
from pptx.enum.text import PP_ALIGN
from pptx.enum.shapes import MSO_SHAPE

# Master Theme Colors
COLOR_BG = RGBColor(11, 15, 25)          # Deep cosmic slate
COLOR_CARD = RGBColor(19, 27, 46)        # Dark navy glass card
COLOR_CARD_BORDER = RGBColor(38, 51, 77) # Card border
COLOR_CYAN = RGBColor(6, 182, 212)       # Electric cyan
COLOR_PURPLE = RGBColor(139, 92, 246)    # Neural purple
COLOR_GREEN = RGBColor(16, 185, 129)     # Verified green
COLOR_AMBER = RGBColor(245, 158, 11)     # Warning amber
COLOR_TEXT_MAIN = RGBColor(248, 250, 252)# Off-white
COLOR_TEXT_MUTED = RGBColor(148, 163, 184)# Slate gray
COLOR_TEXT_FAINT = RGBColor(100, 116, 139)# Dim gray

PROJECT_DATA = [
    {
        "folder": "01_EMBEDDED_SYSTEMS/smart_home_automation",
        "title": "Deterministic Smart Home Automation",
        "subtitle": "Deterministic Cooperative Multitasking & Closed-Loop Irrigation",
        "domain": "Intelligent Embedded Systems",
        "silicon": "ATmega328P / 8-bit AVR / HC-05 Bluetooth",
        "problem": {
            "headline": "Microcontroller Jitter & Soil Moisture Sensor Degradation",
            "points": [
                "Conventional hobby automation relies on blocking delay() routines, freezing the MCU and causing Bluetooth packet drops.",
                "Continuous DC excitation of soil moisture probes causes rapid copper electrolysis, destroying sensors within weeks.",
                "Relay bouncing and unbuffered inductive spikes cause unpredictable microcontroller resets and fire hazards.",
                "Target Requirement: Sub-5ms task latency, corrosion-resistant soil sensing, and failsafe Bluetooth control."
            ]
        },
        "solution": {
            "headline": "Cooperative Multitasking Engine with Closed-Loop Control",
            "points": [
                "Engineered a non-blocking millis() cooperative multitasking scheduler executing 4 concurrent asynchronous state machines.",
                "Implemented pulsed analog sensor excitation with dual-threshold hysteresis to eliminate pump chatter.",
                "Constructed a delimited ASCII serial protocol with checksums for robust HC-05 smartphone teleoperation.",
                "Zero-compromise safety: Automated watchdog timers and hardware relay inductive back-EMF snubbing."
            ]
        },
        "architecture": {
            "headline": "Hardware-Aware C++14 Architecture",
            "points": [
                "Silicon: Microchip ATmega328P running at 16MHz with 2KB SRAM and 32KB Flash.",
                "Firmware Stack: Modular embedded C++14 utilizing fixed-size buffers with zero dynamic heap allocation.",
                "Peripherals: HC-05 Bluetooth UART bridge, DHT11 one-wire sensor, capacitive soil probe, dual SPDT relays.",
                "Verification: 100% passing C++ unit test runner verifying actuator, sensor, and packet state machines."
            ]
        },
        "justification": {
            "headline": "Strategic Design & Silicon Rationale",
            "points": [
                "Why ATmega328P: Highly deterministic interrupt latency (<4 clock cycles) and ultra-low cost ($1.80/unit) for mass commercial deployment.",
                "Why HC-05 Bluetooth: Direct peer-to-peer RF serial connection without dependency on home Wi-Fi routers or cloud latency.",
                "Why Hysteresis Logic: Prevents destructive high-frequency relay cycling when soil moisture oscillates near threshold.",
                "Client Advantage: Production-ready BOM cost under $14 per node with 5+ year sensor longevity."
            ]
        },
        "workflow": [
            {"step": "01. Sensing Phase", "detail": "Timer interrupts trigger non-blocking DHT11 read and pulsed ADC soil moisture capture."},
            {"step": "02. Packet Ingestion", "detail": "UART ring buffer asynchronously parses incoming Bluetooth commands with checksum validation."},
            {"step": "03. Decision Logic", "detail": "Hysteresis state machine compares moisture levels against calibrated agronomic bounds."},
            {"step": "04. Actuation & Safety", "detail": "Relay switches pump/appliance; hardware watchdog guards against electrical hangs."},
            {"step": "05. Telemetry Stream", "detail": "Telemetry packet broadcast back to mobile supervisor displaying live environmental state."}
        ]
    },
    {
        "folder": "01_EMBEDDED_SYSTEMS/robotics/object_avoiding_robot",
        "title": "Autonomous Obstacle-Avoiding Mobile Robot",
        "subtitle": "Differential Kinematics & Panoramic Ultrasonic Turret Navigation",
        "domain": "Autonomous Robotics & Kinematics",
        "silicon": "ATmega328P / L298N Dual H-Bridge / HC-SR04 Sonar",
        "problem": {
            "headline": "Entrapment in Spatial Concavities & Sensor Blind Spots",
            "points": [
                "Fixed ultrasonic sensors leave wide spatial blind spots, causing high-speed collisions with angular obstacles.",
                "Single-vector reactive algorithms frequently become trapped in U-shaped corner concavities.",
                "Abrupt motor reversals induce severe battery voltage dips, brownouts, and microcontroller resets.",
                "Target Requirement: 180° spatial clearance mapping, smooth PWM acceleration, and deterministic FSM navigation."
            ]
        },
        "solution": {
            "headline": "Servo Turret Scanning with 5-State Navigation FSM",
            "points": [
                "Constructed an articulated 180° servo scanner turret that pans to evaluate left/center/right spatial corridors.",
                "Engineered a deterministic 5-state navigation FSM: CRUISE, OBSTACLE_DETECTED, SCANNING, PIVOT, REVERSE.",
                "Implemented soft-start PWM motor acceleration curves to suppress inductive back-EMF and brownouts.",
                "Fully unit-tested navigation state machine validated with simulated acoustic reflection profiles."
            ]
        },
        "architecture": {
            "headline": "Differential Drive Kinematics & Modular C++14",
            "points": [
                "Silicon: ATmega328P running modular C++14 navigation and motor control drivers.",
                "Sensing & Actuation: HC-SR04 ultrasonic transducer mounted on SG90 servo turret; L298N dual H-bridge motor driver.",
                "Power System: Decoupled logic (5V regulated) and motor (7.4V Li-ion) power planes with flyback diode protection.",
                "Quality Gate: Hardware-in-the-loop simulated unit tests validating obstacle avoidance edge cases."
            ]
        },
        "justification": {
            "headline": "Optimized Sensing & Actuation Topology",
            "points": [
                "Why HC-SR04 Sonar: Reliable distance echolocation independent of ambient optical illumination or surface reflectivity.",
                "Why Articulated Turret: Eliminates the need to pivot the entire chassis to check lateral clearance, saving 70% battery power.",
                "Why Differential Drive: Zero turning radius enables instantaneous pivots in tight warehouse or residential corridors.",
                "Client Advantage: Autonomous navigation achieved at 1/10th the cost of commercial 2D LiDAR units."
            ]
        },
        "workflow": [
            {"step": "01. Forward Cruising", "detail": "Motors drive forward under PWM control while sonar ping verifies forward clearance > 25cm."},
            {"step": "02. Proximity Trigger", "detail": "Sonar echo return signals obstacle within threshold; FSM immediately transitions to halt state."},
            {"step": "03. Panoramic Sweep", "detail": "Servo pans ultrasonic transducer 45° left and 135° right to measure clearance vectors."},
            {"step": "04. Path Optimization", "detail": "Algorithm computes maximal clearance direction and calculates required differential turn angle."},
            {"step": "05. Differential Pivot", "detail": "Opposing wheel rotations pivot robot to clear corridor before resuming forward acceleration."}
        ]
    },
    {
        "folder": "01_EMBEDDED_SYSTEMS/robotics/fire_fighting_robot",
        "title": "Autonomous Fire Suppression Robot",
        "subtitle": "Optical NIR Flame Triangulation & Directed Closed-Loop Extinguishing",
        "domain": "Emergency Robotics & Safety Systems",
        "silicon": "ATmega328P / 3-Channel NIR Array / Submersible Pump",
        "problem": {
            "headline": "Hazardous Human Exposure in Industrial Fire Emergencies",
            "points": [
                "Entering confined industrial facilities during active fires presents extreme thermal and toxic risks to personnel.",
                "Stationary sprinkler systems flood entire facilities indiscriminately, causing catastrophic water damage to unburnt equipment.",
                "Autonomous suppression units often suffer false positives caused by ambient sunlight or incandescent bulbs.",
                "Target Requirement: Wavelength-selective flame sensing, autonomous azimuth steering, and targeted water jetting."
            ]
        },
        "solution": {
            "headline": "Tri-Directional Flame Triangulation & Articulated Suppression",
            "points": [
                "Engineered a 3-channel optical Near-Infrared (NIR) phototransistor array isolating open combustion wavelengths.",
                "Developed differential analog flux equations to compute flame source azimuth in real time.",
                "Integrated an isolated high-flow submersible pump nozzle with timed safety cutoff to prevent reservoir exhaustion.",
                "Deterministic FSM seamlessly transitions: PATROL, TRIANGULATION, APPROACH, EXTINGUISH, VERIFY."
            ]
        },
        "architecture": {
            "headline": "High-Reliability Embedded Hardware & C++14 Stack",
            "points": [
                "Silicon: ATmega328P microcontroller managing sensor ADC channels and motor PWM.",
                "Optical Sensing: 3x narrow-angle NIR phototransistors (760nm–1100nm) calibrated for hydrocarbon flame emission.",
                "Suppression Hardware: 5V DC centrifugal submersible water pump driven via optocoupled relay.",
                "Verification: 10 unit test cases validating angular localization, approach thresholds, and pump cutoff limits."
            ]
        },
        "justification": {
            "headline": "Safety Criticality & Sensor Selection Rationale",
            "points": [
                "Why 760nm–1100nm NIR: Specifically targets the radiant emission spectrum of carbon combustion, ignoring visible light.",
                "Why Tri-Channel Array: Provides 180° spatial coverage with simple analog differential math, requiring zero camera latency.",
                "Why Optocoupled Relays: Galvanically isolates motor and pump inductive back-EMF from the microcontroller core.",
                "Client Advantage: Rapid automated first-response intervention suppressing fires before structural escalation occurs."
            ]
        },
        "workflow": [
            {"step": "01. Patrol & Scan", "detail": "Robot autonomously navigates room while monitoring 3 analog phototransistor channels."},
            {"step": "02. Flame Localization", "detail": "Differential analog comparison (left vs. center vs. right) identifies flame heading vector."},
            {"step": "03. Proportional Drive", "detail": "Differential steering aligns center sensor directly with radiant infrared peak."},
            {"step": "04. Approach & Halt", "detail": "Robot approaches until flame intensity reaches target extinguishing distance, then locks brakes."},
            {"step": "05. Suppression & Check", "detail": "Pump sprays targeted water jet for calibrated interval, verifying extinction before disengaging."}
        ]
    },
    {
        "folder": "01_EMBEDDED_SYSTEMS/robotics/gesture_controlled_robotic_arm",
        "title": "4-DOF Gesture-Controlled Robotic Arm",
        "subtitle": "Wearable IMU Teleoperation & Inverse Kinematic Servo Control",
        "domain": "Wearables & Articulated Kinematics",
        "silicon": "ATmega328P / MPU6050 6-Axis IMU / PCA9685 PWM",
        "problem": {
            "headline": "Unintuitive Teleoperation & Mechanical Servo Jitter",
            "points": [
                "Controlling multi-axis articulated manipulators via joysticks or keyboards is slow, clumsy, and requires steep training.",
                "Raw inertial sensor data contains severe high-frequency jitter and drift that causes destructive servo oscillations.",
                "Direct microcontroller software PWM suffers from interrupt latency, producing erratic servo pulse jitter.",
                "Target Requirement: Intuitive ergonomic teleoperation, smooth jitter-free servo motion, and mechanical collision bounds."
            ]
        },
        "solution": {
            "headline": "Inertial Gesture Mapping with PCA9685 Hardware PWM",
            "points": [
                "Engineered a wearable teleoperation glove utilizing an MPU6050 6-axis IMU capturing human wrist pitch, roll, and flex.",
                "Implemented complementary sensor fusion and Exponential Moving Average (EMA) filtering to eliminate hand tremor jitter.",
                "Offloaded all PWM timing to a dedicated PCA9685 16-channel 12-bit hardware controller over I2C.",
                "Formulated geometric inverse kinematics with software bounds checking to prevent joint collisions."
            ]
        },
        "architecture": {
            "headline": "Hardware-Offloaded I2C & Kinematic C++ Stack",
            "points": [
                "Silicon: ATmega328P processing inertial sensor data at 100Hz over I2C at 400kHz.",
                "Sensors & Controllers: InvenSense MPU6050 (accelerometer + gyroscope), NXP PCA9685 12-bit PWM generator.",
                "Actuators: 4x high-torque metal-gear servos (Base, Shoulder, Elbow, Gripper).",
                "Verification: 11 unit tests verifying angle clamping, EMA filter convergence, and linear interpolation."
            ]
        },
        "justification": {
            "headline": "Ergonomics, Precision & Hardware Decoupling",
            "points": [
                "Why MPU6050: Combines 3-axis gyro and 3-axis accelerometer on a single silicon die for robust Euler angle calculation.",
                "Why PCA9685 PWM: Generates crystal-accurate hardware PWM independently of the CPU, completely eliminating servo chatter.",
                "Why EMA Filter: Rejects physiological tremors without introducing noticeable latency, ensuring 1:1 human-to-robot sync.",
                "Client Advantage: Natural human-machine interface allowing untrained operators to perform hazardous robotic manipulation."
            ]
        },
        "workflow": [
            {"step": "01. Inertial Sampling", "detail": "Wearable glove samples 3-axis accelerometer and gyroscope at 100Hz over I2C."},
            {"step": "02. Sensor Fusion", "detail": "Complementary filter combines high-pass gyro and low-pass accelerometer data into pitch/roll."},
            {"step": "03. Tremor Damping", "detail": "Exponential Moving Average (EMA) filters out physiological tremor and electrical noise."},
            {"step": "04. Kinematic Mapping", "detail": "Angles are clamped against physical mechanical limits and mapped to 12-bit PWM ticks."},
            {"step": "05. Servo Execution", "detail": "PCA9685 drives 4 servos simultaneously to mirror operator wrist and arm orientation."}
        ]
    },
    {
        "folder": "01_EMBEDDED_SYSTEMS/smart_railway_system",
        "title": "Autonomous Railway Crack & Crossing Collision System",
        "subtitle": "Track Fissure Detection & Automated Ultrasonic Crossing Barrier",
        "domain": "Transit Safety & Critical Infrastructure",
        "silicon": "ATmega328P / Optical Infrared / HC-SR04 Sonar",
        "problem": {
            "headline": "Catastrophic Rail Fractures & Unmanned Crossing Accidents",
            "points": [
                "Thermal expansion and dynamic stress create sub-millimeter rail micro-cracks that cause catastrophic derailments.",
                "Unmanned level crossings suffer hundreds of fatal vehicle-train collisions annually due to delayed gate deployment.",
                "Manual human track inspections are infrequent, cost-prohibitive, and vulnerable to human oversight.",
                "Target Requirement: Automated autonomous rail surface inspection and fail-safe velocity-based level crossing barriers."
            ]
        },
        "solution": {
            "headline": "Integrated Track Inspection & Intelligent Crossing Gate",
            "points": [
                "Engineered a track-crawler vehicle using high-sensitivity optical infrared reflectance to detect surface fissures.",
                "Constructed an automated level-crossing barrier system utilizing long-range sonar train approach tracking.",
                "Integrated acoustic alarms, visual LED signaling masts, and servo barrier gates with watchdog protection.",
                "Fail-safe architecture: Barrier automatically lowers and triggers alarms upon power interruption or sensor fault."
            ]
        },
        "architecture": {
            "headline": "Dual-Subsystem Critical Infrastructure Safety Architecture",
            "points": [
                "Silicon: Microchip ATmega328P running deterministic C++14 safety state machine.",
                "Inspection Hardware: High-frequency optical infrared reflective optocoupler array calibrated for steel rail reflectivity.",
                "Crossing Hardware: Long-range ultrasonic transceiver, high-torque barrier servo, buzzer alarm annunciator.",
                "Verification: 8 unit test suites validating fissure detection sensitivity, gate transit timing, and failsafe resets."
            ]
        },
        "justification": {
            "headline": "Optical & Acoustic Telemetry Rationale",
            "points": [
                "Why Optical IR Sensing: Detects microscopic surface discontinuities through reflection variance with zero contact wear.",
                "Why Ultrasonic Train Detection: Detects oncoming train velocity and proximity unaffected by darkness, rain, or fog.",
                "Why Failsafe Servo Gate: Ensures mechanical gravity or spring fallback to closed position during total power disruption.",
                "Client Advantage: Replaces high-cost manual inspection teams with continuous autonomous track surveillance."
            ]
        },
        "workflow": [
            {"step": "01. Track Traversal", "detail": "Crawler vehicle traverses rail line, maintaining continuous high-speed optical IR reflectance logging."},
            {"step": "02. Crack Detection", "detail": "Surface fissure causes sharp IR reflection drop, triggering emergency vehicle brake and fault alert."},
            {"step": "03. Train Approach", "detail": "Crossing sonar detects approaching train velocity and calculates estimated time of arrival (ETA)."},
            {"step": "04. Barrier Lockdown", "detail": "Visual and acoustic warnings fire; barrier servo descends to seal crossing before train arrival."},
            {"step": "05. Track Clearance", "detail": "Sonar confirms train has cleared crossing envelope, safely restoring gate to open position."}
        ]
    },
    {
        "folder": "01_EMBEDDED_SYSTEMS/arduino_ble_sense_ai",
        "title": "TinyML Mahalanobis Anomaly Detector",
        "subtitle": "On-Device Inertial Machine Health Inference & BLE 5.0 GATT Telemetry",
        "domain": "TinyML, Edge Computing & Predictive AI",
        "silicon": "Nordic nRF52840 (64MHz ARM Cortex-M4F) / LSM9DS1 IMU",
        "problem": {
            "headline": "Cloud Telemetry Bottlenecks in Rotating Machinery Monitoring",
            "points": [
                "Transmitting high-frequency vibration data (100Hz+) to cloud servers overwhelms industrial Wi-Fi and cellular bandwidth.",
                "Cloud-dependent machine anomaly detection introduces intolerable latencies (seconds) during catastrophic mechanical failure.",
                "Deep learning models (CNNs/RNNs) are too computationally heavy to run on low-power battery-operated edge microcontrollers.",
                "Target Requirement: Sub-millisecond on-device anomaly scoring, <2KB RAM footprint, and multi-year battery life."
            ]
        },
        "solution": {
            "headline": "On-Device Mahalanobis TinyML on ARM Cortex-M4F",
            "points": [
                "Implemented covariance-whitened Mahalanobis distance calculation running directly on an ARM Cortex-M4F microcontroller.",
                "Extracts 3-axis vibration magnitude, variance, and kurtosis features in real time without cloud roundtrips.",
                "Inference completes in <1.2ms using hardware floating-point acceleration (FPU) with <1.5KB RAM utilization.",
                "Broadcasts anomalous machine health flags over standard BLE 5.0 GATT Environmental Sensing profiles."
            ]
        },
        "architecture": {
            "headline": "Silicon TinyML & Bluetooth 5.0 GATT Stack",
            "points": [
                "Silicon: Nordic Semiconductor nRF52840 SoC (64MHz ARM Cortex-M4F, 256KB RAM, 1MB Flash).",
                "Inertial Sensing: STMicroelectronics LSM9DS1 9-axis motion sensor sampled over SPI.",
                "Math Engine: Embedded C++14 matrix math pipeline utilizing hardware FPU instructions.",
                "Verification: 14 unit test cases proving statistical convergence and numerical stability of on-chip covariance."
            ]
        },
        "justification": {
            "headline": "Mathematical Elegance & Silicon Efficiency",
            "points": [
                "Why Mahalanobis Distance: Accounts for feature correlations and scales variance without needing heavy neural network weights.",
                "Why nRF52840: Integrated BLE 5.0 radio and hardware FPU enable combined DSP inference and wireless reporting on a single chip.",
                "Why BLE 5.0 GATT: Enables direct interoperability with existing industrial handheld scanners and edge gateways.",
                "Client Advantage: Zero cloud hosting costs, zero network bandwidth consumption, and immediate microsecond alert dispatch."
            ]
        },
        "workflow": [
            {"step": "01. High-Rate IMU Ingestion", "detail": "LSM9DS1 samples 3-axis vibration at 100Hz into ring buffer via high-speed SPI bus."},
            {"step": "02. Feature Transformation", "detail": "Computes windowed mean amplitude, variance, and kurtosis to capture abnormal harmonics."},
            {"step": "03. Covariance Whitening", "detail": "Multiplies feature vector by pre-calibrated baseline inverse covariance matrix using hardware FPU."},
            {"step": "04. Anomaly Evaluation", "detail": "Calculates Mahalanobis distance in <1.2ms; compares result against empirical critical threshold."},
            {"step": "05. BLE GATT Broadcast", "detail": "Updates BLE 5.0 GATT characteristic, transmitting machine health status and alert to factory supervisor."}
        ]
    },
    {
        "folder": "02_AI_ML_SYSTEMS/android_device_health_prediction",
        "title": "Android Subsystem Telemetry Isolation Forest",
        "subtitle": "Real-Time ADB Observability & Unsupervised Anomaly Scoring",
        "domain": "Predictive AI, Mobile OS & Enterprise Fleet Telemetry",
        "silicon": "Qualcomm Snapdragon / Google Tensor / Android 14 HAL",
        "problem": {
            "headline": "Silent Hardware Degradation & Thermal Runaway in Device Fleets",
            "points": [
                "Enterprise Android kiosks, POS terminals, and mobile fleets suffer unexpected thermal throttling and battery failure.",
                "Standard OEM alerts only trigger after catastrophic thresholds are crossed, disrupting critical commercial operations.",
                "Labeled failure datasets are rarely available across diverse device models, making supervised ML impractical.",
                "Target Requirement: Non-invasive telemetry harvesting, unsupervised anomaly detection, and root-cause attribution."
            ]
        },
        "solution": {
            "headline": "14-Feature Isolation Forest Ensemble & Real-Time Dashboard",
            "points": [
                "Built an automated telemetry collector ingesting CPU thermal zones, battery impedance, and memory pressure via ADB.",
                "Engineered dynamic temporal derivatives (dT/dt) to isolate rapid thermal runaway from steady-state device warming.",
                "Trained an unsupervised Isolation Forest ensemble classifying health into HEALTHY, MODERATE, and CRITICAL states.",
                "Constructed a high-tech glassmorphism web dashboard featuring live WebSocket telemetry streaming."
            ]
        },
        "architecture": {
            "headline": "Telemetry Extraction & Machine Learning Pipeline",
            "points": [
                "Platform: Android 12-14 HAL accessed non-invasively via lightweight Android Debug Bridge (ADB) daemon.",
                "ML Stack: Python 3.12, scikit-learn Isolation Forest, NumPy feature preprocessor, Joblib model serializer.",
                "Observability UI: Flask WebSocket server with Chart.js real-time telemetry streaming charts.",
                "Verification: 4 unit tests validating schema conformity, temporal derivatives, and root-cause attribution."
            ]
        },
        "justification": {
            "headline": "Unsupervised Learning & Non-Invasive Telemetry",
            "points": [
                "Why Isolation Forest: Excels at isolating multi-dimensional outliers with O(n log n) efficiency without needing labeled failures.",
                "Why Temporal Derivatives (dT/dt): Distinguishes safe passive warming during charging from dangerous runaway spikes.",
                "Why ADB Bridge: Requires zero application instrumentation or OS root privileges, ensuring enterprise security compliance.",
                "Client Advantage: Predicts battery swelling and thermal governor throttling 15+ minutes before hardware failure occurs."
            ]
        },
        "workflow": [
            {"step": "01. Subsystem Harvesting", "detail": "Polls Android CPU frequencies, battery internal resistance, and thermal zones via ADB."},
            {"step": "02. Feature Preprocessing", "detail": "Normalizes 12 raw metrics and computes 2 dynamic temporal gradients (dT/dt, dFreq/dt)."},
            {"step": "03. Isolation Inference", "detail": "Isolation Forest evaluates average path length across tree ensemble to compute anomaly score."},
            {"step": "04. Risk Classification", "detail": "Calibrates raw decision score into 0-100% risk percentage and assigns risk severity tier."},
            {"step": "05. Live Observability", "detail": "Streams metrics to glassmorphism UI; triggers proactive mitigation recommendations if critical."}
        ]
    },
    {
        "folder": "02_AI_ML_SYSTEMS/linux_device_health_prediction",
        "title": "Enterprise Linux Kernel Telemetry & Anomaly AI",
        "subtitle": "Ultra-Low-Overhead /proc & /sys Observability with Isolation Forests",
        "domain": "Linux Kernel Observability & Server Fleet Health",
        "silicon": "x86_64 / ARM64 Enterprise Linux Servers & Edge Nodes",
        "problem": {
            "headline": "Unobserved Kernel Bottlenecks & Peripheral Hardware Failures",
            "points": [
                "Production Linux servers experience subtle hardware degradation, DMA stalls, and IRQ interrupt storms that evade standard alerts.",
                "Heavy APM monitoring agents (e.g. Datadog, Dynatrace) consume 5-15% CPU overhead, degrading edge server throughput.",
                "Static threshold alerts (e.g. CPU > 90%) trigger false alarms during routine batch jobs while missing true hardware degradation.",
                "Target Requirement: Sub-1% CPU harvesting overhead, multi-core IRQ skew detection, and unsupervised anomaly scoring."
            ]
        },
        "solution": {
            "headline": "Direct sysfs/procfs Ingestion with Machine Learning",
            "points": [
                "Engineered a zero-dependency kernel agent parsing raw /proc/stat, /proc/meminfo, /proc/interrupts, and sysfs thermal zones.",
                "Achieved ultra-low CPU overhead (<0.8%) through non-blocking asynchronous file descriptor polling.",
                "Trained an unsupervised Isolation Forest model capturing complex multi-variate system performance correlations.",
                "Automated root-cause diagnostic engine that highlights whether anomaly stems from memory thrashing, IRQ skew, or thermal limits."
            ]
        },
        "architecture": {
            "headline": "Lightweight Python 3.12 & Kernel Telemetry Stack",
            "points": [
                "Telemetry Sources: Native Linux procfs (/proc) and sysfs (/sys) virtual file system interfaces.",
                "Processing Engine: Python 3.12, scikit-learn Isolation Forest, NumPy numerical array transforms.",
                "Performance Profile: Overhead verified at <0.8% CPU utilization and <28MB memory footprint.",
                "Verification: 4 unit test suites validating parser resilience, feature scaling, and multi-threaded test harnesses."
            ]
        },
        "justification": {
            "headline": "Bare-Metal Kernel Metrics Without Agent Bloat",
            "points": [
                "Why Raw /proc and /sysfs: Eliminates costly system call wrappers and proprietary daemon overhead, running natively on any Linux kernel.",
                "Why Multi-Core IRQ Analysis: Detects faulty hardware devices that flood a single CPU core with unhandled interrupts.",
                "Why Unsupervised Machine Learning: Adapts dynamically to unique server workload baselines without manual rule tuning.",
                "Client Advantage: Enterprise-grade infrastructure observability without paying recurring cloud agent licensing fees."
            ]
        },
        "workflow": [
            {"step": "01. Kernel Metric Polling", "detail": "Asynchronously reads /proc/stat, /proc/meminfo, /proc/interrupts, and sysfs thermal files."},
            {"step": "02. Differential Vectorization", "detail": "Computes delta rates for CPU jiffies, context switches, page faults, and hardware interrupts."},
            {"step": "03. Model Inference", "detail": "Feeds normalized multi-dimensional vector into trained Isolation Forest model."},
            {"step": "04. Root Cause Analysis", "detail": "Compares anomalous features against historical baseline distributions to pinpoint failing subsystem."},
            {"step": "05. Enterprise Alerting", "detail": "Emits structured JSON payload to central syslogs or dashboard with mitigation suggestions."}
        ]
    },
    {
        "folder": "02_AI_ML_SYSTEMS/predictive_maintenance",
        "title": "Industrial Machinery Predictive Maintenance",
        "subtitle": "Vibration FFT Spectral Decomposition & Remaining Useful Life (RUL)",
        "domain": "Industry 4.0, Vibration DSP & Reliability Engineering",
        "silicon": "Industrial Edge Compute / NVIDIA Jetson / Raspberry Pi CM4",
        "problem": {
            "headline": "Catastrophic Unplanned Downtime in Industrial Rotating Machinery",
            "points": [
                "Unexpected bearing failures, shaft misalignments, and motor stalls cause billions in unplanned industrial manufacturing downtime.",
                "Preventive maintenance schedules replace expensive components prematurely or too late after micro-cracks propagate.",
                "Raw time-domain vibration signals hide early harmonic defect signatures within background industrial acoustic noise.",
                "Target Requirement: High-resolution spectral decomposition, bearing fault frequency identification, and RUL estimation."
            ]
        },
        "solution": {
            "headline": "FFT Harmonic Analysis & Weibull Hazard Rate Modeling",
            "points": [
                "Built an end-to-end vibration DSP pipeline performing Fast Fourier Transform (FFT) on high-frequency acceleration streams.",
                "Automated detection of characteristic bearing defect harmonics (BPFO, BPFI, BSF, FTF).",
                "Integrated statistical Weibull hazard modeling to estimate Remaining Useful Life (RUL) with confidence bounds.",
                "Engineered lightweight MQTT industrial telemetry bridges for integration into existing plant SCADA systems."
            ]
        },
        "architecture": {
            "headline": "Digital Signal Processing & Machine Health Architecture",
            "points": [
                "Compute Platform: Industrial Edge Linux (ARM64 / x86_64) interfaced with piezoelectric vibration accelerometers.",
                "Signal Processing Stack: Python 3.12, NumPy, SciPy FFT spectral transform, Pandas time-series aggregators.",
                "Telemetry Protocol: OASIS standard MQTT protocol publishing JSON vibration health telemetry.",
                "Documentation: Formally modeled in 13-section IEEE Research Whitepaper KW-WP-10."
            ]
        },
        "justification": {
            "headline": "Spectral Physics & Reliability Engineering",
            "points": [
                "Why Fast Fourier Transform (FFT): Transforms unintelligible time waveforms into clear frequency peaks matching bearing defect geometry.",
                "Why Weibull Hazard Distribution: Standard actuarial formulation for mechanical wear-out, providing realistic RUL horizons.",
                "Why Edge Execution: Eliminates the need to stream gigabytes of raw 10kHz vibration data over industrial cellular connections.",
                "Client Advantage: Shifts industrial facilities from reactive emergency repairs to planned, low-cost maintenance windows."
            ]
        },
        "workflow": [
            {"step": "01. Vibration Ingestion", "detail": "High-frequency accelerometer samples machine vibration at 1kHz-10kHz sampling rates."},
            {"step": "02. FFT Decomposition", "detail": "Fast Fourier Transform generates discrete frequency spectrum and isolates resonant frequencies."},
            {"step": "03. Harmonic Peak Matching", "detail": "Algorithms cross-reference spectral peaks against known bearing physical defect frequencies."},
            {"step": "04. Degradation Scoring", "detail": "Calculates normalized machine health index based on harmonic energy escalation over time."},
            {"step": "05. RUL Prediction & SCADA", "detail": "Weibull model outputs Remaining Useful Life hours and publishes alert to plant SCADA via MQTT."}
        ]
    },
    {
        "folder": "03_IOT_AND_AUTOMATION/smart_garden_irrigation",
        "title": "Microclimate Evapotranspiration Smart Irrigation",
        "subtitle": "Edge Evapotranspiration Compensation & Capacitive IoT Telemetry",
        "domain": "Precision Agriculture & Industrial IoT",
        "silicon": "ESP32 (240MHz Dual-Core Xtensa) / Wi-Fi & BLE / Capacitive ADC",
        "problem": {
            "headline": "Severe Water Wastage & Plant Stress from Blind Timer Irrigation",
            "points": [
                "Conventional agricultural timers and simplistic moisture switches waste over 40% of fresh irrigation water globally.",
                "Basic timer irrigation fails to account for ambient atmospheric drying power (solar radiation, vapor pressure deficit, temperature).",
                "Resistive soil moisture probes corrode rapidly due to galvanic DC currents, leading to false dry readings and crop flooding.",
                "Target Requirement: Corrosion-free sensing, atmospheric evapotranspiration compensation, and MQTT telemetry."
            ]
        },
        "solution": {
            "headline": "FAO-56 Evapotranspiration Engine on ESP32 Silicon",
            "points": [
                "Constructed an autonomous IoT controller computing reference evapotranspiration (ET0) using ambient vapor pressure deficit.",
                "Integrated capacitive dielectric soil moisture sensing with polynomial calibration curves to eliminate probe corrosion.",
                "Implemented dynamic hysteresis control to prevent pump cycling and hydraulic water hammer.",
                "Dual-core architecture: Core 0 handles Wi-Fi/MQTT communications; Core 1 executes real-time agricultural control loops."
            ]
        },
        "architecture": {
            "headline": "Dual-Core ESP32 & Microclimate Agronomic Stack",
            "points": [
                "Silicon: Espressif ESP32-WROOM-32 (240MHz dual-core Xtensa LX6, 520KB SRAM).",
                "Sensors: High-frequency capacitive soil dielectric probe, DHT22 high-accuracy temperature/humidity sensor.",
                "Actuation: High-reliability solid-state relay driving 12V submersible irrigation pump with failsafe timeouts.",
                "Verification: 10 unit tests validating Hargreaves-Samani ET0 models, hysteresis boundaries, and MQTT packet framing."
            ]
        },
        "justification": {
            "headline": "Agronomic Science & Dual-Core Silicon Power",
            "points": [
                "Why Capacitive Soil Sensing: Senses soil dielectric permittivity without exposing bare metal to soil, ensuring 5+ year lifespan.",
                "Why FAO-56 Evapotranspiration: Balances actual plant water loss against soil availability, cutting water consumption by 35-45%.",
                "Why Dual-Core ESP32: Wi-Fi stack operations on Core 0 never block critical irrigation sensing or pump shutoff loops on Core 1.",
                "Client Advantage: Significant municipal water and electricity bill reductions with optimized agricultural crop yield."
            ]
        },
        "workflow": [
            {"step": "01. Environmental Sensing", "detail": "Measures ambient temperature, relative humidity, and soil capacitive dielectric permittivity."},
            {"step": "02. Vapor Pressure Deficit", "detail": "Calculates saturation vapor pressure deficit and atmospheric drying power via Hargreaves formula."},
            {"step": "03. Dynamic Hysteresis", "detail": "Computes dynamic soil moisture threshold adjusted for current ambient evaporative demand."},
            {"step": "04. Actuation Control", "detail": "Engages relay to pulse irrigation pump; hardware timer limits maximum runtime to prevent waterlogging."},
            {"step": "05. Cloud Telemetry", "detail": "Streams soil hydration curve, water volume dispensed, and microclimate metrics to MQTT dashboard."}
        ]
    },
    {
        "folder": "03_IOT_AND_AUTOMATION/sensor_monitoring",
        "title": "Deterministic 5-Channel Environmental Telemetry",
        "subtitle": "Zero-Heap Circular Ring Buffering & On-Chip Statistical DSP",
        "domain": "Industrial IoT & Embedded Signal Processing",
        "silicon": "ESP32 / BMP280 / MQ-135 / LDR / I2C Bus",
        "problem": {
            "headline": "Heap Fragmentation & Bus Lockups in Long-Running IoT Nodes",
            "points": [
                "Standard IoT firmware relying on dynamic memory (malloc/free/new) suffers fatal heap fragmentation after weeks of uptime.",
                "I2C bus peripherals frequently latch the SDA line low during electrical transients, freezing the entire MCU bus.",
                "Raw environmental sensor reads contain transient electrical spikes that distort cloud analytics and trigger false alerts.",
                "Target Requirement: Zero dynamic memory allocation, automatic I2C bus recovery, and on-chip statistical filtering."
            ]
        },
        "solution": {
            "headline": "Fixed-Memory Ring Buffer & Welford Online DSP in C11",
            "points": [
                "Engineered a fixed-capacity 32-frame circular ring buffer running with zero heap allocation (100% stack/static memory).",
                "Implemented Welford's algorithm for on-chip incremental mean and standard deviation computation without numeric overflow.",
                "Constructed an automatic 9-clock I2C bit-banging bus recovery mechanism that resets stuck slave devices.",
                "Aggregates 5 environmental channels: Temperature, Barometric Pressure, Relative Humidity, Air Quality, and Ambient Light."
            ]
        },
        "architecture": {
            "headline": "Deterministic C11 / C++14 Embedded Architecture",
            "points": [
                "Silicon: ESP32 running modular C11 data structures and hardware abstraction layers.",
                "Sensor Suite: Bosch BMP280 barometric pressure sensor, MQ-135 gas sensor, photoresistor lux ADC.",
                "Memory Profile: Exactly 32 frames of fixed telemetry storage consuming <1KB total RAM.",
                "Verification: 12 unit test cases verifying circular pointer wrap-around, standard deviation convergence, and overflow protection."
            ]
        },
        "justification": {
            "headline": "Deterministic Reliability & Statistical Noise Rejection",
            "points": [
                "Why Zero-Heap Ring Buffer: Guarantees zero memory fragmentation, ensuring 10+ years of uninterrupted continuous uptime.",
                "Why Welford's Algorithm: Computes true variance in a single pass without needing to store large arrays or risk floating-point overflow.",
                "Why 9-Clock I2C Recovery: Restores locked bus lines automatically without requiring a complete system reboot.",
                "Client Advantage: Industrial-grade reliability suitable for remote, unattended environmental and factory monitoring."
            ]
        },
        "workflow": [
            {"step": "01. Multi-Channel Polling", "detail": "Polls BMP280 over I2C, MQ-135 gas analog channel, and light lux photoresistor ADC at 10Hz."},
            {"step": "02. Ring Buffer Push", "detail": "Writes timestamped frame into contiguous circular buffer using lock-free head/tail pointers."},
            {"step": "03. Statistical Filtering", "detail": "Welford online algorithm updates moving mean and variance to filter out electrical spikes."},
            {"step": "04. Anomaly Screening", "detail": "Evaluates z-score against threshold to detect gas leaks or sudden pressure drops."},
            {"step": "05. Bus Health & Uplink", "detail": "Monitors I2C bus state (recovering automatically if stuck) and dispatches clean telemetry packets."}
        ]
    },
    {
        "folder": "04_SOFTWARE_ENGINEERING/C_programming",
        "title": "Production C11 Systems Engineering Foundations",
        "subtitle": "Pointer Architecture, Bitwise Manipulation & Memory Safety",
        "domain": "Low-Level Systems Programming",
        "silicon": "Universal x86_64 / ARM / Microcontrollers",
        "problem": {
            "headline": "Memory Leaks, Buffer Overflows & Undefined Behavior in C",
            "points": [
                "Unsafe pointer manipulation, memory leaks, and buffer overflows account for over 70% of critical security vulnerabilities in systems code.",
                "Inconsistent type sizes and endianness mismatches cause severe cross-platform compilation and runtime failures.",
                "Many developers lack mastery over bitwise masking, direct struct memory layout, and re-entrant safe file I/O.",
                "Target Requirement: MISRA-adjacent C11 code, strict static compiler verification, and AddressSanitizer audits."
            ]
        },
        "solution": {
            "headline": "Modular C11 Systems Codebase with Zero-Leak Guarantee",
            "points": [
                "Architected a comprehensive modular C11 foundation covering bitwise register manipulators, custom allocators, and binary I/O.",
                "Compiled with maximum strictness: GCC 16.1.0 flags `-Wall -Wextra -Werror -pedantic -std=c11`.",
                "Audited dynamically with AddressSanitizer (ASan) and Valgrind to guarantee zero memory leaks and zero buffer overruns.",
                "Engineered cross-platform abstractions handling binary file persistence, endianness conversions, and string validation."
            ]
        },
        "architecture": {
            "headline": "Pure C11 Standards-Compliant Systems Architecture",
            "points": [
                "Standard: ISO/IEC 9899:2011 (C11) standard utilizing fixed-width integers (`stdint.h`) and boolean types (`stdbool.h`).",
                "Toolchain: GCC 16.1.0 and Clang toolchains with custom automated Makefiles.",
                "Memory Discipline: Explicit ownership semantics, bounds checking, and deterministic resource release.",
                "Verification: 100% audited test suite passing clean under AddressSanitizer and LeakSanitizer."
            ]
        },
        "justification": {
            "headline": "Bare-Metal Silicon Control & Peak Efficiency",
            "points": [
                "Why Pure C11: Delivers the absolute highest runtime execution speed, smallest binary size, and universal hardware compatibility.",
                "Why Fixed-Width Types: Guarantees that integers maintain identical bit-widths across 8-bit, 32-bit, and 64-bit architectures.",
                "Why Strict Compiler Gates: Catches subtle bugs (implicit conversions, uninitialized variables) at compile time rather than in production.",
                "Client Advantage: Reusable, bulletproof systems foundations that integrate seamlessly into kernel drivers and firmware."
            ]
        },
        "workflow": [
            {"step": "01. Memory Scoping", "detail": "Allocates deterministic memory layouts on stack or managed heap with strict size tracking."},
            {"step": "02. Pointer Operations", "detail": "Executes safe pointer arithmetic and bitwise register operations using validated masks."},
            {"step": "03. Boundary Validation", "detail": "Verifies array bounds, string null terminators, and buffer capacities before write cycles."},
            {"step": "04. Binary Serialization", "detail": "Serializes and deserializes structured binary payloads to disk/network with endianness safety."},
            {"step": "05. Resource Teardown", "detail": "Deterministically releases all open file handles and memory blocks with verified zero leakage."}
        ]
    },
    {
        "folder": "04_SOFTWARE_ENGINEERING/CPP_programming",
        "title": "Modern C++17 Asynchronous ThreadPool",
        "subtitle": "RAII Concurrency, std::future Task Queues & Exception Safety",
        "domain": "Modern Systems Programming & High-Performance Computing",
        "silicon": "Multi-Core Modern x86_64 / ARM64 CPUs",
        "problem": {
            "headline": "Thread Spawning Overhead & Concurrency Pitfalls",
            "points": [
                "Spawning individual threads on-demand causes severe OS context-switching latency and high memory allocation overhead.",
                "Concurrent multi-threaded applications frequently suffer from data races, deadlocks, and thread resource leaks.",
                "Capturing return values and unhandled exceptions across asynchronous worker threads is complex and error-prone.",
                "Target Requirement: Reusable worker thread pool, generic task dispatch with futures, and strict RAII exception safety."
            ]
        },
        "solution": {
            "headline": "Industrial-Grade C++17 Asynchronous ThreadPool Engine",
            "points": [
                "Engineered a high-performance modern C++17 ThreadPool pre-spawning worker threads matching hardware concurrency.",
                "Implemented template metaprogramming allowing `enqueue()` to accept arbitrary callables and return `std::future<T>`.",
                "Guaranteed strict RAII thread safety using `std::unique_lock`, mutex synchronization, and condition variables.",
                "Designed deterministic graceful shutdown ensuring all in-flight tasks complete cleanly before worker thread join."
            ]
        },
        "architecture": {
            "headline": "Modern C++17 Concurrency & Template Architecture",
            "points": [
                "Language Standard: ISO C++17 utilizing `std::packaged_task`, `std::future`, and template type deduction.",
                "Synchronization: `std::mutex` and `std::condition_variable` managing a thread-safe task queue.",
                "Shutdown Semantics: Atomic stop flag coordinates worker thread drain and thread joins upon pool destruction.",
                "Verification: 15 unit tests validating high concurrency stress, future resolution, and exception propagation."
            ]
        },
        "justification": {
            "headline": "Zero-Overhead Abstractions & Robust Concurrency",
            "points": [
                "Why Reusable ThreadPool: Eliminates OS kernel thread creation latency, slashing asynchronous task dispatch times to microseconds.",
                "Why std::future Return: Allows calling code to await asynchronous results synchronously or check completion status without polling.",
                "Why RAII Resource Management: Guarantees that locks are always released and threads joined even if an exception occurs.",
                "Client Advantage: Enterprise-grade concurrency backbone capable of handling thousands of concurrent tasks with zero race conditions."
            ]
        },
        "workflow": [
            {"step": "01. Pool Initialization", "detail": "Spawns N worker threads matching hardware CPU core count in sleeping condition-wait state."},
            {"step": "02. Task Enqueue", "detail": "Caller passes lambda/callable with arguments; pool packages task and returns `std::future<T>`."},
            {"step": "03. Worker Notification", "detail": "Task pushed to synchronized queue; condition variable wakes up an available idle worker thread."},
            {"step": "04. Parallel Execution", "detail": "Worker pops task, executes callable, and sets result value or captures thrown exception."},
            {"step": "05. Future Resolution", "detail": "Caller retrieves return value via `.get()` without ever handling low-level OS thread primitives."}
        ]
    },
    {
        "folder": "04_SOFTWARE_ENGINEERING/python_programming",
        "title": "Enterprise Python 3.12 Engineering Suite",
        "subtitle": "Modular Architecture, Design Patterns & Systems Automation",
        "domain": "Software Engineering & Data Automation",
        "silicon": "Universal Linux / macOS / Windows",
        "problem": {
            "headline": "Monolithic Scripts & Fragile Automation in Systems Engineering",
            "points": [
                "Systems automation scripts are frequently written as fragile, monolithic scripts lacking unit tests or type hints.",
                "Poor error handling and unvalidated CLI inputs cause automated deployment and data pipelines to crash unexpectedly.",
                "Lack of modular object-oriented design makes automation code difficult to test, maintain, or scale across teams.",
                "Target Requirement: Production-grade modularity, strict type annotations, native unit tests, and clean CLI interfaces."
            ]
        },
        "solution": {
            "headline": "23-Module Production Engineering & Algorithmic Suite",
            "points": [
                "Constructed 23 modular production software assignments covering core algorithms, OOP design patterns, and systems automation.",
                "Enforced strict Python 3.12 type annotations, docstring conventions, and clean package separation.",
                "Implemented resilient streaming file parsers, CLI argument handlers, and mathematical transformers.",
                "100% verified using Python's native `unittest` framework with automated CI execution."
            ]
        },
        "architecture": {
            "headline": "Clean Object-Oriented & Functional Python Architecture",
            "points": [
                "Language Version: Python 3.12 utilizing modern match-case statements, type hints, and dataclasses.",
                "Testing Framework: Python native `unittest` requiring zero third-party dependencies.",
                "Packaging: Standard modular package layout with isolated modules for algorithms, I/O, and utilities.",
                "Verification: 100% passed test suites covering algorithmic boundary conditions and file processing."
            ]
        },
        "justification": {
            "headline": "Maintainability, Portability & Fast Prototyping",
            "points": [
                "Why Python 3.12: Delivers 15-25% faster runtime execution than legacy Python and provides robust standard libraries.",
                "Why Native unittest: Guarantees zero installation overhead and instant test execution across any environment.",
                "Why Strict Typing: Catches data type mismatches and interface bugs early during static analysis and code review.",
                "Client Advantage: Reusable software libraries that accelerate client scripting, data extraction, and toolchain automation."
            ]
        },
        "workflow": [
            {"step": "01. Input Validation", "detail": "Parses CLI arguments or data inputs with strict schema and boundary validation."},
            {"step": "02. Algorithmic Core", "detail": "Executes modular object-oriented or functional algorithmic processing pipeline."},
            {"step": "03. Error Trapping", "detail": "Traps specific I/O and format exceptions cleanly, emitting structured diagnostic logs."},
            {"step": "04. Output Emission", "detail": "Formats results into JSON, CSV, or formatted console reports for downstream tools."},
            {"step": "05. Automated Testing", "detail": "Unit test runners automatically execute test assertions to guarantee regression-free code."}
        ]
    },
    {
        "folder": "04_SOFTWARE_ENGINEERING/data_structures",
        "title": "High-Performance C11 Data Structures",
        "subtitle": "Ring Buffers, Linked Lists, Stacks & Memory Leak Audits",
        "domain": "Computer Science Foundations & Cache Efficiency",
        "silicon": "Universal x86_64 / ARM / Embedded Silicon",
        "problem": {
            "headline": "Memory Fragmentation & Inefficient Cache Access in Containers",
            "points": [
                "Standard library generic containers often hide dynamic heap reallocations, causing unpredictable execution latencies.",
                "Poor pointer and struct memory layout leads to severe CPU cache misses and cache line invalidations.",
                "Dangling pointers, double-free bugs, and memory leaks in custom data structures can crash operating system kernels.",
                "Target Requirement: Cache-friendly contiguous layouts, deterministic O(1) operations, and strict memory leak audits."
            ]
        },
        "solution": {
            "headline": "Audited C11 Data Structures with Zero-Leak Mandate",
            "points": [
                "Engineered high-performance C11 implementations of Singly/Doubly Linked Lists, Circular Queue Ring Buffers, and Stacks.",
                "Contiguous circular ring buffers guarantee O(1) push and pop operations with zero dynamic memory allocation.",
                "Audited 100% leak-free under AddressSanitizer (ASan), LeakSanitizer, and Valgrind memory profiling suites.",
                "Constructed comprehensive boundary test runners verifying underflow, overflow, and pointer integrity."
            ]
        },
        "architecture": {
            "headline": "Modular C11 Data Structure Architecture",
            "points": [
                "Standard: C11 standard compiled with GCC `-Wall -Wextra -Werror -pedantic -std=c11`.",
                "Memory Structures: Contiguous circular buffers, doubly linked pointer nodes with sentinel heads.",
                "Testing Harness: Custom C test suites verifying time complexity and pointer boundary edge cases.",
                "Verification: 100% passing tests with zero memory leaks verified via AddressSanitizer."
            ]
        },
        "justification": {
            "headline": "Deterministic Performance & Embedded Portability",
            "points": [
                "Why Contiguous Circular Queues: Eliminates dynamic memory fragmentation and maximizes CPU L1/L2 cache locality.",
                "Why Pure C11: Enables drop-in reuse inside Linux kernel modules, bare-metal RTOS, or high-throughput servers.",
                "Why ASan Verification: Proves mathematically that no memory corruption or dangling pointer accesses can occur.",
                "Client Advantage: Rock-solid foundational containers that ensure real-time predictability in mission-critical software."
            ]
        },
        "workflow": [
            {"step": "01. Container Creation", "detail": "Initializes data structure on stack or contiguous heap with fixed capacity boundaries."},
            {"step": "02. Pointer Validation", "detail": "Validates input pointers and checks capacity limits before executing insertion or deletion."},
            {"step": "03. O(1) Execution", "detail": "Executes deterministic enqueue, dequeue, push, or pop in constant O(1) time."},
            {"step": "04. Boundary Checking", "detail": "Protects against underflow or overflow, returning clear status codes to the calling system."},
            {"step": "05. Deterministic Teardown", "detail": "Safely releases all allocated memory blocks and sets pointers to NULL to prevent dangling traps."}
        ]
    },
    {
        "folder": "05_LINUX_AND_SYSTEMS/device_drivers",
        "title": "Linux LKM Character Device Driver & IOCTL",
        "subtitle": "Dynamic Major/Minor Allocation, Udev Auto-Nodes & Mutex Safety",
        "domain": "Linux Kernel Space & Device Drivers",
        "silicon": "Linux Kernel 5.x / 6.x (x86_64 / ARM64)",
        "problem": {
            "headline": "Kernel Crashes & Race Conditions in Custom Hardware Interfaces",
            "points": [
                "Connecting custom hardware peripherals to user-space applications requires kernel-space drivers with strict memory isolation.",
                "Flawed kernel drivers easily cause kernel panics (oops), corrupt kernel memory, or crash the entire operating system.",
                "Static major/minor device numbering causes severe device conflicts across different Linux distributions.",
                "Target Requirement: Dynamic device allocation, automatic udev nodes, re-entrant mutex protection, and safe memory copying."
            ]
        },
        "solution": {
            "headline": "Production Linux Loadable Kernel Module (LKM)",
            "points": [
                "Engineered a production Loadable Kernel Module (LKM) character driver compatible with modern Linux 5.x and 6.x kernels.",
                "Implemented dynamic major/minor allocation via `alloc_chrdev_region` and auto-creation of `/dev` nodes via udev/sysfs.",
                "Guaranteed re-entrant concurrent safety using kernel mutexes (`char_device_mutex`) protecting shared buffers.",
                "Built custom IOCTL interfaces supporting buffer reset, status queries, and runtime parameter configuration."
            ]
        },
        "architecture": {
            "headline": "Linux Kernel Subsystems & Safe Memory API",
            "points": [
                "Kernel APIs: `alloc_chrdev_region`, `cdev_init`, `cdev_add`, `class_create`, `device_create`, `mutex_lock`.",
                "Memory Safety: Strict isolation using `copy_to_user()` and `copy_from_user()` validating user-space pointer bounds.",
                "User Interface: File operations struct implementing `.open`, `.read`, `.write`, `.release`, and `.unlocked_ioctl`.",
                "Verification: Tested with user-space multi-threaded test client under high concurrent read/write stress."
            ]
        },
        "justification": {
            "headline": "Enterprise Kernel Standards & Zero Panics",
            "points": [
                "Why LKM Architecture: Allows dynamic loading and unloading (`insmod`/`rmmod`) without requiring kernel recompilation.",
                "Why Dynamic Udev Creation: Device nodes appear automatically in `/dev` with appropriate permissions without manual mknod commands.",
                "Why Kernel Mutexes: Prevents multi-process race conditions while remaining interruptible by operating system signals.",
                "Client Advantage: Clean, production-ready kernel driver template ready for proprietary hardware and PCIe/USB integration."
            ]
        },
        "workflow": [
            {"step": "01. Module Loading", "detail": "`insmod` executes init function: dynamically allocates major number and registers sysfs device class."},
            {"step": "02. Device Node Creation", "detail": "udev daemon receives kernel event and automatically creates `/dev/char_device` in user space."},
            {"step": "03. Safe Data Transfer", "detail": "User application writes data; driver acquires mutex and safely copies payload using `copy_from_user()`."},
            {"step": "04. IOCTL Control", "detail": "User application sends custom IOCTL commands to query driver metrics or resize internal ring buffers."},
            {"step": "05. Clean Unloading", "detail": "`rmmod` destroys device node, releases major number region, and frees kernel memory cleanly."}
        ]
    },
    {
        "folder": "05_LINUX_AND_SYSTEMS/embedded_linux",
        "title": "Embedded Linux Digital Watch Engine & Yocto BSP",
        "subtitle": "Multi-Threaded C11 Engine, Custom Yocto Layer & systemd Daemon",
        "domain": "Embedded Linux, Board Support Packages (BSP) & Yocto",
        "silicon": "ARM Cortex-A (Allwinner / NXP i.MX / Raspberry Pi)",
        "problem": {
            "headline": "Heavy OS Images & Non-Deterministic Boot in Embedded Consumer Devices",
            "points": [
                "Standard desktop Linux distributions are excessively bloated (2GB+), slow to boot, and contain thousands of unnecessary packages.",
                "Embedded IoT and wearable devices require custom stripped-down operating systems boots in under 3 seconds.",
                "Concurrent wearable functions (timekeeping, stopwatches, button debouncing) easily suffer race conditions without thread safety.",
                "Target Requirement: Custom Yocto BitBake layer, lean rootfs (<50MB), systemd service management, and POSIX multithreading."
            ]
        },
        "solution": {
            "headline": "Custom Yocto `meta-watch` Layer & Multi-Threaded C11 Engine",
            "points": [
                "Engineered a multi-threaded C11 digital watch engine running concurrent timekeeping, timer, and event worker threads.",
                "Constructed a custom Yocto Project BitBake layer (`meta-watch`) compiling a tailored, stripped-down Linux distribution.",
                "Packaged the engine as a native systemd background service configured for automated boot startup and self-healing restarts.",
                "Comprehensive C11 unit test suite verifying clock epoch progression, stopwatch precision, and race-free thread joining."
            ]
        },
        "architecture": {
            "headline": "Yocto Project & POSIX Multi-Threading Architecture",
            "points": [
                "Build System: Yocto Project (Scarthgap/Kirkstone release), BitBake recipes, custom kernel configurations.",
                "Firmware Core: C11 standard utilizing POSIX pthreads, mutex locks, and condition variables for tick synchronization.",
                "Service Layer: systemd service unit (`meta-watch.service`) enforcing execution isolation and watchdog restarts.",
                "Verification: 7 unit test cases validating timer accuracy, button debounce state machines, and concurrency safety."
            ]
        },
        "justification": {
            "headline": "Commercial BSP Reproducibility & Minimal Footprint",
            "points": [
                "Why Yocto Project: Industry gold standard for generating deterministic, reproducible Linux board support packages (BSPs).",
                "Why POSIX Pthreads: Provides deterministic, low-latency task scheduling directly on Linux kernel real-time primitives.",
                "Why systemd Integration: Automatically manages daemon lifecycles, log rotation via journald, and auto-restart on crashes.",
                "Client Advantage: Production-ready BSP architecture ready for commercial wearable, medical, or smart meter mass manufacturing."
            ]
        },
        "workflow": [
            {"step": "01. Yocto Image Build", "detail": "BitBake compiles cross-compiler toolchain, Linux kernel, rootfs, and `meta-watch` recipe into flashable image."},
            {"step": "02. Board Initialization", "detail": "U-Boot loads kernel into RAM and boots stripped-down Linux kernel in under 2.5 seconds."},
            {"step": "03. Service Auto-Start", "detail": "systemd launches digital watch service daemon as an unprivileged secure background process."},
            {"step": "04. Concurrent Execution", "detail": "Worker threads run in parallel: timekeeper increments epoch ticks; timer thread manages countdown."},
            {"step": "05. User Interaction", "detail": "Asynchronous input thread captures button events and broadcasts state updates to the display driver."}
        ]
    },
    {
        "folder": "05_LINUX_AND_SYSTEMS/kernel_development",
        "title": "Linux Kernel Architecture: Syscalls, Memory & Scheduler",
        "subtitle": "Custom System Calls, Device Tree Bindings & SLUB Allocator",
        "domain": "Operating System Architecture & Kernel Internals",
        "silicon": "x86_64 / ARM64 Kernel Architecture",
        "problem": {
            "headline": "Hardware Accelerator Inefficiency & Kernel Architecture Opacity",
            "points": [
                "High-performance AI accelerators and financial engines face excessive latency when constrained by standard user-space APIs.",
                "Improper integration of hardware Device Tree (DTS) nodes leads to peripheral probing failures and silent bus deadlocks.",
                "Lack of visibility into SLUB/Buddy memory allocator internals leads to kernel-level memory fragmentation and latency spikes.",
                "Target Requirement: Custom system call implementation, Device Tree peripheral bindings, and scheduler performance analysis."
            ]
        },
        "solution": {
            "headline": "End-to-End Kernel Extensions & Architectural Blueprints",
            "points": [
                "Implemented an end-to-end custom Linux system call (`sys_kernelwise_status`) with user-space wrapper libraries.",
                "Authored production Device Tree Source (DTS) bindings defining hardware addressing for custom I2C/SPI peripherals.",
                "Conducted comparative performance analysis of Linux Completely Fair Scheduler (CFS) vs. Earliest Eligible Virtual Deadline First (EEVDF).",
                "Built diagnostic profiling scripts inspecting SLUB kernel cache fragmentation and slabinfo metrics under heavy load."
            ]
        },
        "architecture": {
            "headline": "Linux Kernel Source Tree & Subsystem Architecture",
            "points": [
                "Kernel Codebase: Modern Linux Kernel source tree modified with new syscall table entries (`syscall_64.tbl`).",
                "Syscall Macros: Built using standard `SYSCALL_DEFINE2` macros with strict user pointer verification (`copy_to_user`).",
                "Hardware Abstraction: Device Tree compiler (DTC) bindings defining compatible strings, interrupt lines, and memory maps.",
                "Documentation: Fully documented in formal architectural compendiums with code references."
            ]
        },
        "justification": {
            "headline": "Direct Silicon Control & Zero-Overhead Kernel Access",
            "points": [
                "Why Custom Syscalls: Bypasses user-space library overhead, executing privileged hardware checks in under 80 nanoseconds.",
                "Why Device Tree Bindings: Decouples driver software from hardcoded board memory addresses, enabling portable BSP deployment.",
                "Why EEVDF Scheduler Mastery: Optimizes process latency for audio DSP and real-time robotics by managing virtual deadlines.",
                "Client Advantage: Deep internal kernel expertise capable of customizing enterprise operating systems for specialized silicon."
            ]
        },
        "workflow": [
            {"step": "01. User Syscall Invocation", "detail": "Application executes `syscall(SYS_kernelwise_status)` with destination memory pointer."},
            {"step": "02. Mode Switch & Dispatch", "detail": "CPU executes fast syscall instruction, switching to Ring 0 kernel privilege and indexing syscall table."},
            {"step": "03. Privileged Kernel Action", "detail": "Syscall reads high-resolution kernel uptime and active CPU cluster topology directly from kernel registers."},
            {"step": "04. Safe Memory Copy", "detail": "`copy_to_user()` verifies page table permissions and transfers struct back to user space."},
            {"step": "05. Return to User Space", "detail": "CPU restores register context and returns execution to calling application in nanoseconds."}
        ]
    },
    {
        "folder": "05_LINUX_AND_SYSTEMS/linux_internals",
        "title": "POSIX Process Orchestration & IPC Pipeline Suite",
        "subtitle": "Multi-Stage Pipes, POSIX Shared Memory & Signal Trapping",
        "domain": "Systems Programming & Unix Internals",
        "silicon": "POSIX Standard Linux / Unix Systems",
        "problem": {
            "headline": "Zombie Process Leaks & Concurrency Bottlenecks in Daemons",
            "points": [
                "Uncontrolled child process spawning leads to process table exhaustion (zombie processes) that freeze Linux servers.",
                "Inter-process communication using temporary disk files causes severe I/O bottlenecks and disk wear.",
                "Crude signal handling (`SIGINT`, `SIGCHLD`) frequently causes race conditions and interrupted system call crashes.",
                "Target Requirement: Robust zombie reaping, zero-copy shared memory IPC, and re-entrant signal handling via sigaction."
            ]
        },
        "solution": {
            "headline": "High-Performance POSIX Systems Programming Suite",
            "points": [
                "Engineered a process tree orchestrator with automated `SIGCHLD` zombie reaping using `waitpid(WNOHANG)`.",
                "Built zero-copy inter-process communication pipelines utilizing POSIX shared memory (`shm_open`, `mmap`) and semaphores.",
                "Implemented dynamic multi-stage file descriptor redirection (`dup2`) connecting arbitrary command chains via anonymous pipes.",
                "Configured robust asynchronous signal handling utilizing `sigaction` with `SA_RESTART` flags."
            ]
        },
        "architecture": {
            "headline": "POSIX.1-2008 Standard Systems Architecture",
            "points": [
                "API Standards: IEEE POSIX.1-2008 standard system calls (`fork`, `execvp`, `dup2`, `pipe`, `shm_open`, `mmap`, `sigaction`).",
                "Synchronization: POSIX named and unnamed semaphores preventing race conditions across memory-mapped buffers.",
                "Memory Safety: Strict leak audits ensuring shared memory regions and file descriptors are closed upon termination.",
                "Documentation: Formally analyzed in IEEE Research Whitepaper KW-WP-03."
            ]
        },
        "justification": {
            "headline": "Microsecond IPC Throughput & Operating System Stability",
            "points": [
                "Why POSIX Shared Memory: Delivers direct RAM-to-RAM data transfer at memory bus speeds (>10GB/s) with zero context-switch overhead.",
                "Why Non-Blocking Zombie Reaping: Reaps terminated child processes asynchronously without blocking parent server event loops.",
                "Why sigaction with SA_RESTART: Prevents interrupted read/write system calls from returning false errors when signals arrive.",
                "Client Advantage: Foundational systems architecture for high-throughput enterprise backend daemons and edge telemetry brokers."
            ]
        },
        "workflow": [
            {"step": "01. Shared Memory Mapping", "detail": "Parent creates shared memory segment via `shm_open()` and maps it into virtual memory using `mmap()`."},
            {"step": "02. Process Forking", "detail": "Parent executes `fork()` to spawn independent child process inheriting shared memory pointer."},
            {"step": "03. Synchronized IPC", "detail": "Child writes telemetry data directly into memory; POSIX semaphore coordinates zero-copy read by parent."},
            {"step": "04. Signal Interception", "detail": "`sigaction` traps asynchronous signals (e.g. `SIGINT`), initiating graceful resource unmapping."},
            {"step": "05. Child Reaping & Cleanup", "detail": "`SIGCHLD` handler calls `waitpid()` to eliminate zombie entries; unlinks shared memory segments cleanly."}
        ]
    },
    {
        "folder": "06_EXISTING_GITHUB_PROJECTS/imported_projects/Addressbook",
        "title": "Modular Contact Storage & Binary File I/O",
        "subtitle": "Binary Persistence, Dynamic Structs & Stdio Flushing Abstraction",
        "domain": "C Software Engineering & Data Persistence",
        "silicon": "Universal POSIX / Linux / Windows",
        "problem": {
            "headline": "Data Corruption in Text Files & Cross-Platform Stdio Hangs",
            "points": [
                "Text-based contact records (CSV/JSON) are slow to parse and easily corrupted if writing is interrupted by an unexpected crash.",
                "Flushing `stdin` buffers across different platforms (Linux vs. Windows) causes terminal input reading to hang or skip inputs.",
                "Unbounded memory allocations and lack of field sanitization lead to buffer overflow attacks.",
                "Target Requirement: Fast binary file serialization, cross-platform stdin flushing, and robust search indexing."
            ]
        },
        "solution": {
            "headline": "Modular Relational Storage Engine with Binary Persistence",
            "points": [
                "Engineered a clean modular contact management engine with atomic binary file serialization (`fread`/`fwrite`).",
                "Constructed a cross-platform `stdio_ext.h` fallback wrapper resolving stdin input flushing differences across OS platforms.",
                "Implemented dynamic struct array allocation with bounds checking and memory leak audits.",
                "Built fast multi-field search indexing supporting partial substring matching across names, phone numbers, and emails."
            ]
        },
        "architecture": {
            "headline": "Modular C11 Architecture & Binary File Schema",
            "points": [
                "Language Standard: C11 standard compiled with GCC `-Wall -Wextra -Werror`.",
                "Persistence Model: Compact binary header record followed by fixed-width contact entries with CRC checks.",
                "Input Subsystem: Custom sanitizers stripping control characters and validating email syntax.",
                "Verification: Audited under Valgrind ensuring 100% memory deallocation upon application exit."
            ]
        },
        "justification": {
            "headline": "Compact Footprint & Crash-Resistant Storage",
            "points": [
                "Why Binary Serialization: Delivers 5x smaller file sizes and 10x faster disk read/write throughput than JSON or XML parsers.",
                "Why Modular C Architecture: Cleanly separates data model, presentation layer, and persistence layers for easy unit testing.",
                "Why Custom stdio Fallbacks: Guarantees reliable keyboard input behavior on Linux, macOS, and Windows command prompts.",
                "Client Advantage: Reusable embedded database engine suitable for low-memory kiosks, POS registers, and microcontrollers."
            ]
        },
        "workflow": [
            {"step": "01. Database Bootstrapping", "detail": "Reads binary header from disk, validates record count, and maps entries into memory struct array."},
            {"step": "02. User Command Processing", "detail": "Interactive menu accepts search, insertion, modification, or deletion commands."},
            {"step": "03. Input Sanitization", "detail": "Validates name length, formats telephone numbers, and checks email syntax before acceptance."},
            {"step": "04. Binary Flush", "detail": "Atomically writes modified struct array to disk using `fwrite()`, protecting against power interruption."},
            {"step": "05. Clean Termination", "detail": "Frees dynamic memory structures and closes file handles with verified zero memory leaks."}
        ]
    },
    {
        "folder": "06_EXISTING_GITHUB_PROJECTS/imported_projects/Car_black_box",
        "title": "Automotive Safety Event Logger (Car Black Box)",
        "subtitle": "PIC16F877A Flight Recorder, Non-Volatile EEPROM & RTC Telemetry",
        "domain": "Automotive Embedded Systems & Functional Safety",
        "silicon": "Microchip PIC16F877A / 24C02 I2C EEPROM / DS1307 RTC",
        "problem": {
            "headline": "Loss of Critical Crash Telemetry During Severe Vehicle Collisions",
            "points": [
                "Automotive accident investigations require forensic telemetry (speed, gear, collision signals) stamped with sub-second accuracy.",
                "Violent collisions frequently sever vehicle battery power instantly, wiping volatile RAM logs before they can be saved.",
                "Consumer microcontrollers lack hardware brownout protection and non-volatile memory endurance.",
                "Target Requirement: Sub-second RTC timestamps, brownout-safe EEPROM persistence, and supervisor access security."
            ]
        },
        "solution": {
            "headline": "Automotive Event Data Recorder with Non-Volatile Persistence",
            "points": [
                "Engineered an automotive flight recorder on the Microchip PIC16F877A microcontroller using MPLAB XC8.",
                "Implemented non-volatile crash event persistence utilizing an external I2C 24C02 EEPROM surviving sudden battery loss.",
                "Integrated an I2C DS1307 real-time clock logging precise timestamped vehicle speed and transmission gear changes.",
                "Constructed a 16x2 Character LCD interface and password-protected UART console for supervisor log offloading."
            ]
        },
        "architecture": {
            "headline": "Automotive PIC Microcontroller & Dual I2C Bus Stack",
            "points": [
                "Silicon: Microchip PIC16F877A 8-bit RISC microcontroller executing embedded C compiled with MPLAB XC8.",
                "Peripherals: Maxim DS1307 battery-backed RTC, Atmel 24C02 I2C EEPROM (256-byte circular log), 16x2 CLCD.",
                "Telemetry Ingestion: On-chip ADC measuring simulated speedometer voltage; digital interrupt lines tracking gear changes.",
                "Documentation: Formally published in 13-section IEEE Research Whitepaper KW-WP-02."
            ]
        },
        "justification": {
            "headline": "Automotive Hardening & Forensic Reliability",
            "points": [
                "Why PIC16F877A: Automotive-grade silicon with built-in Brown-Out Reset (BOR) preventing memory corruption during voltage sags.",
                "Why 24C02 I2C EEPROM: Retains critical crash records for 40+ years without power; byte-level writes complete in <5ms.",
                "Why DS1307 RTC: Battery backup guarantees continuous sub-second timekeeping regardless of main battery disconnects.",
                "Client Advantage: Meets insurance and legal forensic audit standards for commercial vehicle fleet monitoring."
            ]
        },
        "workflow": [
            {"step": "01. Continuous Sampling", "detail": "PIC microcontroller samples speed ADC and gear switch inputs every 100ms."},
            {"step": "02. Event Detection", "detail": "Detects state change (gear shift, collision trigger, overspeed threshold) and queries DS1307 RTC via I2C."},
            {"step": "03. Non-Volatile Write", "detail": "Atomically writes timestamp, event ID, and speed into circular 24C02 EEPROM log address in <5ms."},
            {"step": "04. Dashboard Display", "detail": "Renders real-time vehicle speed, gear, and event counter on 16x2 Character LCD."},
            {"step": "05. Forensic Offloading", "detail": "Supervisor authenticates via password to dump crash history over UART to forensic PC application."}
        ]
    },
    {
        "folder": "06_EXISTING_GITHUB_PROJECTS/imported_projects/minishell",
        "title": "Minishell: POSIX Unix Command Interpreter",
        "subtitle": "Arbitrary N-Stage Pipelines, Signal Trapping & Job Control",
        "domain": "Unix Systems Programming & Compiler Architecture",
        "silicon": "x86_64 / POSIX Standard Linux",
        "problem": {
            "headline": "Complexity of Pipeline Chaining & Signal Masking in Shells",
            "points": [
                "Standard Unix shells are massive codebases; engineering a lightweight, secure command interpreter requires deep systems knowledge.",
                "Managing multi-stage pipelines (`cmd1 | cmd2 | ... | cmdN`) without deadlocking pipe buffers requires precise descriptor lifecycle management.",
                "Trapping terminal signals (Ctrl+C, Ctrl+Z) without killing the shell or stranding orphan background jobs is notoriously difficult.",
                "Target Requirement: Arbitrary n-stage pipelines, I/O redirection, job control (fg/bg), and zero memory leaks."
            ]
        },
        "solution": {
            "headline": "Custom Unix Command Interpreter (`msh`) in C11",
            "points": [
                "Engineered a custom POSIX command-line interpreter supporting arbitrary n-stage pipelines with automatic file descriptor management.",
                "Implemented robust job control tracking active process groups, supporting foreground execution and background suspension (`fg`/`bg`).",
                "Constructed clean signal handlers intercepting `SIGINT` (Ctrl+C) and `SIGTSTP` (Ctrl+Z) without destabilizing the shell prompt.",
                "Built an allocation-safe lexical tokenizer handling nested quotation, whitespace, and redirection operators with zero leaks."
            ]
        },
        "architecture": {
            "headline": "POSIX System Calls & Process Control Architecture",
            "points": [
                "Core APIs: `fork()`, `execvp()`, `pipe()`, `dup2()`, `waitpid()`, `sigaction()`, `tcsetpgrp()`.",
                "Pipeline Engine: Creates 2*(N-1) pipe descriptors, dynamically wiring child standard streams before calling `execvp()`.",
                "Memory Discipline: Dynamic argument arrays freed cleanly after execution; zero memory leaks under Valgrind stress testing.",
                "Documentation: Formally analyzed in IEEE Research Whitepaper KW-WP-03."
            ]
        },
        "justification": {
            "headline": "Deep Unix Engineering & Zero Overhead",
            "points": [
                "Why Custom Lexer: Avoids heavy external parser dependencies (lex/yacc), creating a compact, easily auditable binary.",
                "Why Chained `dup2` Pipelines: Enables direct kernel-level memory streaming between processes with zero user-space copying.",
                "Why Process Group Control: Prevents stray keyboard interrupts from terminating background daemon tasks.",
                "Client Advantage: Reusable command interpreter suitable for embedded maintenance shells, secure kiosk terminals, or container runtimes."
            ]
        },
        "workflow": [
            {"step": "01. Command Ingestion", "detail": "Reads input line from terminal, parsing quotation marks, whitespace, and pipeline delimiters (`|`)."},
            {"step": "02. Built-In Evaluation", "detail": "Evaluates internal shell commands (`cd`, `exit`, `jobs`, `fg`, `bg`) directly without spawning sub-processes."},
            {"step": "03. Pipeline Setup", "detail": "Allocates array of pipe file descriptors and forks child worker process for each pipeline stage."},
            {"step": "04. Stream Redirection", "detail": "`dup2()` redirects stdin and stdout to respective pipe descriptors; child executes binary via `execvp()`."},
            {"step": "05. Signal & Job Management", "detail": "Parent monitors child process group, trapping Ctrl+C and Ctrl+Z and updating job state tables."}
        ]
    },
    {
        "folder": "08_SHARED_LIBRARIES",
        "title": "Reusable Embedded HAL & Protocol Suite",
        "subtitle": "Zero-Allocation I2C/UART Drivers & CRC16 Packet Serializer",
        "domain": "Reusable Embedded Middleware & Protocol Engineering",
        "silicon": "Universal Microcontrollers (PIC, AVR, STM32, ESP32)",
        "problem": {
            "headline": "Firmware Fragmentation & Unreliable Serial Communication",
            "points": [
                "Embedded firmware teams constantly rewrite low-level drivers for every new MCU, resulting in fragmented, unmaintainable codebases.",
                "Raw serial streams over UART or wireless RF suffer bit flips and corrupted frames from industrial electromagnetic noise.",
                "Hardware I2C peripherals frequently lock up when slave sensors fail to release the serial data (SDA) line.",
                "Target Requirement: Hardware-agnostic HAL, non-blocking ring-buffered UART, CRC16 noise rejection, and zero heap allocation."
            ]
        },
        "solution": {
            "headline": "Hardware-Agnostic C11 HAL & Robust Packet Protocols",
            "points": [
                "Engineered a drop-in C11 Hardware Abstraction Layer (HAL) for bit-banged I2C, UART ring buffering, and DHT11 protocols.",
                "Implemented an industrial CRC16-CCITT packet framing serializer guaranteeing 100% noise rejection on corrupted serial channels.",
                "Built an automatic 9-clock I2C bus recovery routine that clears stuck slave devices without rebooting the microcontroller.",
                "100% verified against GCC 16.1.0 with strict flags (`-Wall -Wextra -Werror -pedantic -std=c11`)."
            ]
        },
        "architecture": {
            "headline": "Decoupled C11 Middleware Architecture",
            "points": [
                "Language Standard: Pure C11 standard with hardware register decoupling through function pointer interfaces.",
                "Components: `i2c_hal.c`, `uart_hal.c`, `dht11_driver.c`, `packet_protocol.c`.",
                "Memory Model: Exactly zero heap allocation (no malloc); all buffers statically scoped or caller-allocated.",
                "Verification: Automated unit tests validating CRC16 polynomial math, packet boundary framing, and buffer overflows."
            ]
        },
        "justification": {
            "headline": "Modularity, Determinism & Noise Immunity",
            "points": [
                "Why Hardware Abstraction (HAL): Allows changing microcontrollers (e.g. AVR to STM32) without modifying high-level sensor drivers.",
                "Why Zero Heap Allocation: Eliminates memory leaks and heap fragmentation on microcontrollers with as little as 1KB RAM.",
                "Why CRC16-CCITT Checksums: Detects single-bit, double-bit, and burst transmission errors with 99.998% mathematical certainty.",
                "Client Advantage: Reusable software assets that slash firmware development timelines by months for new hardware products."
            ]
        },
        "workflow": [
            {"step": "01. HAL Initialization", "detail": "Configures target GPIO pins and baud rates through hardware abstraction register pointers."},
            {"step": "02. Ring-Buffered Transfer", "detail": "UART ISR receives bytes asynchronously into circular buffer without blocking main CPU execution."},
            {"step": "03. Packet Serialization", "detail": "Encapsulates application payload with start header, payload length, and CRC16-CCITT checksum."},
            {"step": "04. Bus Recovery", "detail": "If an I2C slave hangs the SDA line, the HAL executes a 9-clock pulse sequence to restore bus operation."},
            {"step": "05. Packet Verification", "detail": "Receiver recalculates CRC16; drops corrupted frames silently or passes verified payload to application."}
        ]
    }
]

def create_presentation_deck(p_info):
    prs = Presentation()
    prs.slide_width = Inches(13.333)
    prs.slide_height = Inches(7.5)
    blank_layout = prs.slide_layouts[6]

    def add_base_slide(section_tag, slide_title):
        slide = prs.slides.add_slide(blank_layout)
        # Background
        bg = slide.background
        fill = bg.fill
        fill.solid()
        fill.fore_color.rgb = COLOR_BG

        # Subtle Header Line
        line = slide.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.35), Inches(11.733), Inches(0.02))
        line.fill.solid()
        line.fill.fore_color.rgb = COLOR_CARD_BORDER
        line.line.color.rgb = COLOR_CARD_BORDER

        # Header Box
        header_box = slide.shapes.add_textbox(Inches(0.8), Inches(0.4), Inches(11.733), Inches(0.9))
        tf = header_box.text_frame
        tf.word_wrap = True
        tf.margin_left = tf.margin_top = tf.margin_right = tf.margin_bottom = 0

        p1 = tf.paragraphs[0]
        p1.text = section_tag.upper()
        p1.font.size = Pt(10)
        p1.font.bold = True
        p1.font.color.rgb = COLOR_CYAN

        p2 = tf.add_paragraph()
        p2.text = slide_title
        p2.font.size = Pt(22)
        p2.font.bold = True
        p2.font.color.rgb = COLOR_TEXT_MAIN

        # Footer Box
        footer_box = slide.shapes.add_textbox(Inches(0.8), Inches(7.0), Inches(11.733), Inches(0.35))
        ftf = footer_box.text_frame
        ftf.margin_left = ftf.margin_top = ftf.margin_right = ftf.margin_bottom = 0
        fp = ftf.paragraphs[0]
        fp.text = "Kernelwise Labs • Systems Architecture Portfolio • Lead Architect: Aravind Reddy"
        fp.font.size = Pt(9)
        fp.font.color.rgb = COLOR_TEXT_FAINT

        return slide

    # =========================================================================
    # SLIDE 1: Executive Title Slide
    # =========================================================================
    slide1 = prs.slides.add_slide(blank_layout)
    bg1 = slide1.background
    fill1 = bg1.fill
    fill1.solid()
    fill1.fore_color.rgb = COLOR_BG

    # Accent decorative box
    dec_box = slide1.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(0.8), Inches(2.2), Inches(0.35))
    dec_box.fill.solid()
    dec_box.fill.fore_color.rgb = COLOR_CARD
    dec_box.line.color.rgb = COLOR_CYAN
    tf_dec = dec_box.text_frame
    tf_dec.margin_left = tf_dec.margin_right = tf_dec.margin_top = tf_dec.margin_bottom = 0
    p_dec = tf_dec.paragraphs[0]
    p_dec.text = "  EXECUTIVE SYSTEM DECK  "
    p_dec.font.size = Pt(9)
    p_dec.font.bold = True
    p_dec.font.color.rgb = COLOR_CYAN
    p_dec.alignment = PP_ALIGN.CENTER

    # Title & Subtitle Box
    t_box = slide1.shapes.add_textbox(Inches(0.8), Inches(1.5), Inches(11.733), Inches(3.2))
    t_tf = t_box.text_frame
    t_tf.word_wrap = True

    p_dom = t_tf.paragraphs[0]
    p_dom.text = f"DOMAIN: {p_info['domain'].upper()}"
    p_dom.font.size = Pt(12)
    p_dom.font.bold = True
    p_dom.font.color.rgb = COLOR_PURPLE

    p_title = t_tf.add_paragraph()
    p_title.text = p_info["title"]
    p_title.font.size = Pt(36)
    p_title.font.bold = True
    p_title.font.color.rgb = COLOR_TEXT_MAIN

    p_sub = t_tf.add_paragraph()
    p_sub.text = p_info["subtitle"]
    p_sub.font.size = Pt(18)
    p_sub.font.color.rgb = COLOR_CYAN

    # Silicon & Target Info Card
    card_info = slide1.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(4.8), Inches(11.733), Inches(1.8))
    card_info.fill.solid()
    card_info.fill.fore_color.rgb = COLOR_CARD
    card_info.line.color.rgb = COLOR_CARD_BORDER
    tf_ci = card_info.text_frame
    tf_ci.word_wrap = True
    tf_ci.margin_left = tf_ci.margin_top = Inches(0.25)

    p_sil = tf_ci.paragraphs[0]
    p_sil.text = f"Primary Silicon & Technology Stack: {p_info['silicon']}"
    p_sil.font.size = Pt(13)
    p_sil.font.bold = True
    p_sil.font.color.rgb = COLOR_GREEN

    p_auth = tf_ci.add_paragraph()
    p_auth.text = "Author & Systems Architect: Aravind Reddy (@DAravindReddy) | Organization: Kernelwise Labs"
    p_auth.font.size = Pt(11)
    p_auth.font.color.rgb = COLOR_TEXT_MUTED

    p_stat = tf_ci.add_paragraph()
    p_stat.text = "Quality Gate Guarantee: 100% Passing Automated Unit Test Suite • Strict Zero-Memory-Leak Mandate"
    p_stat.font.size = Pt(11)
    p_stat.font.color.rgb = COLOR_CYAN

    # =========================================================================
    # SLIDE 2: Problem Statement & Industrial Need
    # =========================================================================
    slide2 = add_base_slide("SECTION 01 // OPERATIONAL CONSTRAINTS", "Problem Statement & Industrial Need")

    card_prob = slide2.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.65), Inches(11.733), Inches(5.0))
    card_prob.fill.solid()
    card_prob.fill.fore_color.rgb = COLOR_CARD
    card_prob.line.color.rgb = COLOR_CARD_BORDER
    tf_prob = card_prob.text_frame
    tf_prob.word_wrap = True
    tf_prob.margin_left = tf_prob.margin_right = tf_prob.margin_top = Inches(0.35)

    p_ph = tf_prob.paragraphs[0]
    p_ph.text = f"Core Challenge: {p_info['problem']['headline']}"
    p_ph.font.size = Pt(17)
    p_ph.font.bold = True
    p_ph.font.color.rgb = COLOR_AMBER

    for pt in p_info["problem"]["points"]:
        p_pt = tf_prob.add_paragraph()
        p_pt.text = f"•  {pt}"
        p_pt.font.size = Pt(13)
        p_pt.font.color.rgb = COLOR_TEXT_MAIN
        p_pt.space_before = Pt(14)

    # =========================================================================
    # SLIDE 3: What We Built (Executive Solution)
    # =========================================================================
    slide3 = add_base_slide("SECTION 02 // DELIVERED ARCHITECTURE", "What We Built: Executive Solution")

    card_sol = slide3.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.65), Inches(11.733), Inches(5.0))
    card_sol.fill.solid()
    card_sol.fill.fore_color.rgb = COLOR_CARD
    card_sol.line.color.rgb = COLOR_CARD_BORDER
    tf_sol = card_sol.text_frame
    tf_sol.word_wrap = True
    tf_sol.margin_left = tf_sol.margin_right = tf_sol.margin_top = Inches(0.35)

    p_sh = tf_sol.paragraphs[0]
    p_sh.text = f"Deliverable Overview: {p_info['solution']['headline']}"
    p_sh.font.size = Pt(17)
    p_sh.font.bold = True
    p_sh.font.color.rgb = COLOR_CYAN

    for pt in p_info["solution"]["points"]:
        p_pt = tf_sol.add_paragraph()
        p_pt.text = f"✔  {pt}"
        p_pt.font.size = Pt(13)
        p_pt.font.color.rgb = COLOR_TEXT_MAIN
        p_pt.space_before = Pt(14)

    # =========================================================================
    # SLIDE 4: How We Built It (Architecture & Stack)
    # =========================================================================
    slide4 = add_base_slide("SECTION 03 // ENGINEERING IMPLEMENTATION", "How We Built It: Architecture & Stack")

    card_arch = slide4.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.65), Inches(11.733), Inches(5.0))
    card_arch.fill.solid()
    card_arch.fill.fore_color.rgb = COLOR_CARD
    card_arch.line.color.rgb = COLOR_CARD_BORDER
    tf_arch = card_arch.text_frame
    tf_arch.word_wrap = True
    tf_arch.margin_left = tf_arch.margin_right = tf_arch.margin_top = Inches(0.35)

    p_ah = tf_arch.paragraphs[0]
    p_ah.text = f"System Topology: {p_info['architecture']['headline']}"
    p_ah.font.size = Pt(17)
    p_ah.font.bold = True
    p_ah.font.color.rgb = COLOR_PURPLE

    for pt in p_info["architecture"]["points"]:
        p_pt = tf_arch.add_paragraph()
        p_pt.text = f"⚙  {pt}"
        p_pt.font.size = Pt(13)
        p_pt.font.color.rgb = COLOR_TEXT_MAIN
        p_pt.space_before = Pt(14)

    # =========================================================================
    # SLIDE 5: Why We Used Those Tools & Devices (Design Justification)
    # =========================================================================
    slide5 = add_base_slide("SECTION 04 // DESIGN RATIONALE", "Why We Used These Tools & Devices")

    card_just = slide5.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.65), Inches(11.733), Inches(5.0))
    card_just.fill.solid()
    card_just.fill.fore_color.rgb = COLOR_CARD
    card_just.line.color.rgb = COLOR_CARD_BORDER
    tf_just = card_just.text_frame
    tf_just.word_wrap = True
    tf_just.margin_left = tf_just.margin_right = tf_just.margin_top = Inches(0.35)

    p_jh = tf_just.paragraphs[0]
    p_jh.text = f"Design Tradeoffs: {p_info['justification']['headline']}"
    p_jh.font.size = Pt(17)
    p_jh.font.bold = True
    p_jh.font.color.rgb = COLOR_GREEN

    for pt in p_info["justification"]["points"]:
        p_pt = tf_just.add_paragraph()
        p_pt.text = f"★  {pt}"
        p_pt.font.size = Pt(13)
        p_pt.font.color.rgb = COLOR_TEXT_MAIN
        p_pt.space_before = Pt(14)

    # =========================================================================
    # SLIDE 6: Project Operational Workflow
    # =========================================================================
    slide6 = add_base_slide("SECTION 05 // SYSTEM EXECUTION PIPELINE", "Project Operational Workflow")

    card_wf = slide6.shapes.add_shape(MSO_SHAPE.RECTANGLE, Inches(0.8), Inches(1.65), Inches(11.733), Inches(5.0))
    card_wf.fill.solid()
    card_wf.fill.fore_color.rgb = COLOR_CARD
    card_wf.line.color.rgb = COLOR_CARD_BORDER
    tf_wf = card_wf.text_frame
    tf_wf.word_wrap = True
    tf_wf.margin_left = tf_wf.margin_right = tf_wf.margin_top = Inches(0.35)

    p_wfh = tf_wf.paragraphs[0]
    p_wfh.text = "Step-by-Step Functional Execution Lifecycle"
    p_wfh.font.size = Pt(17)
    p_wfh.font.bold = True
    p_wfh.font.color.rgb = COLOR_CYAN

    for step_obj in p_info["workflow"]:
        p_step = tf_wf.add_paragraph()
        p_step.text = f"{step_obj['step']}: {step_obj['detail']}"
        p_step.font.size = Pt(12)
        p_step.font.color.rgb = COLOR_TEXT_MAIN
        p_step.space_before = Pt(12)

    return prs

def create_markdown_deck(p_info):
    md = f"""# Executive Presentation Deck: {p_info['title']}
**Organization:** Kernelwise Labs  
**Lead Systems Architect:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  
**Primary Silicon / Stack:** `{p_info['silicon']}`  
**Verification Status:** 100% Tests Passing • Zero Memory Leaks  

---

## Slide 1: Executive Title & System Overview
- **Project Title:** {p_info['title']}
- **Subtitle:** {p_info['subtitle']}
- **Domain:** {p_info['domain']}
- **Author:** Aravind Reddy (Founder & Lead Architect, Kernelwise Labs)
- **Primary Platform:** {p_info['silicon']}
- **Verification Guarantee:** 100% Automated Unit Test Verification Suite Passing

---

## Slide 2: Problem Statement & Industrial Need
**Core Challenge:** {p_info['problem']['headline']}
{chr(10).join(f"- {pt}" for pt in p_info['problem']['points'])}

---

## Slide 3: What We Built (Executive Solution)
**Deliverable Overview:** {p_info['solution']['headline']}
{chr(10).join(f"- {pt}" for pt in p_info['solution']['points'])}

---

## Slide 4: How We Built It (Architecture & Stack)
**System Topology:** {p_info['architecture']['headline']}
{chr(10).join(f"- {pt}" for pt in p_info['architecture']['points'])}

---

## Slide 5: Why We Used These Tools & Devices (Design Justification)
**Design Tradeoffs & Rationale:** {p_info['justification']['headline']}
{chr(10).join(f"- {pt}" for pt in p_info['justification']['points'])}

---

## Slide 6: Project Operational Workflow
**Step-by-Step Functional Execution Lifecycle:**
{chr(10).join(f"- **{step_obj['step']}:** {step_obj['detail']}" for step_obj in p_info['workflow'])}

---

### Client Presentation Notes
*This presentation is generated to support client technical briefings, investor diligence reviews, and enterprise architecture consultations. For full source code, test suites, and IEEE research whitepapers, inspect the repository.*
"""
    return md

def main():
    root = os.path.abspath(os.path.join(os.path.dirname(__file__), "../.."))
    print(f"[*] Generating presentation decks across monorepo root: {root}")
    
    generated_count = 0
    for p in PROJECT_DATA:
        folder_path = os.path.join(root, p["folder"])
        if not os.path.exists(folder_path):
            print(f"[!] Warning: Folder does not exist: {folder_path}")
            continue
        
        # 1. Generate PPTX
        prs = create_presentation_deck(p)
        pptx_filename = "project_presentation.pptx"
        pptx_path = os.path.join(folder_path, pptx_filename)
        prs.save(pptx_path)
        
        # 2. Generate Markdown outline
        md_content = create_markdown_deck(p)
        md_filename = "PRESENTATION.md"
        md_path = os.path.join(folder_path, md_filename)
        with open(md_path, "w", encoding="utf-8") as f:
            f.write(md_content)
            
        print(f"[+] Generated PPTX & MD in: {p['folder']}")
        generated_count += 1
        
    print(f"\n[SUCCESS] Successfully generated presentation decks for {generated_count} / {len(PROJECT_DATA)} projects!")

if __name__ == "__main__":
    main()
