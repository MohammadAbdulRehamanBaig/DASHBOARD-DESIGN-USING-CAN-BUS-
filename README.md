# DASHBOARD-DESIGN-USING-CAN-BUS
## Overview

Dashboard Design Using CAN Bus is a distributed embedded system developed using LPC2129 ARM7 microcontrollers and CAN protocol.

The project simulates a vehicle dashboard where multiple Electronic Control Units (ECUs) communicate through a CAN network.

The system consists of three independent CAN nodes:

1. Main Node
2. Indicator Node
3. Fuel Node

The Main Node reads engine temperature, receives fuel information from the Fuel Node, and sends indicator commands to the Indicator Node.

---

# Features

* CAN Bus Communication
* Multi-node Architecture
* Engine Temperature Monitoring
* Fuel Percentage Monitoring
* Left Indicator Control
* Right Indicator Control
* Real-Time Dashboard Display
* Interrupt-Based Indicator Switching
* Distributed ECU Communication

---
## Block Diagram
<img width="643" height="459" alt="image" src="https://github.com/user-attachments/assets/4f54c280-198e-44b8-9a32-8ba905b7585a" />
---

# System Architecture

MAIN NODE
|
|---- CAN BUS ----|
|
INDICATOR NODE
|
FUEL NODE

Communication Medium:
CANH and CANL

Transceiver:
DS18B20

Microcontroller:
LPC2129

---


# Node Description

## 1. Main Node

Responsibilities:

* Read Engine Temperature using DS18B20
* Display Temperature on LCD
* Read Left Indicator Switch
* Read Right Indicator Switch
* Send Indicator Commands through CAN
* Receive Fuel Percentage from Fuel Node
* Display Fuel Percentage on LCD

### Peripherals Used

* LCD
* DS18B20
* Left Switch
* Right Switch
* CAN Controller

---

## 2. Indicator Node

Responsibilities:

* Receive CAN Data from Main Node
* Decode Indicator Command
* Control Indicator LEDs

Left Indicator:
LEDs scroll from Right → Left

Right Indicator:
LEDs scroll from Left → Right

### Peripherals Used

* 8 LEDs
* CAN Controller

---

## 3. Fuel Node

Responsibilities:

* Read Fuel Sensor using ADC
* Convert ADC Reading into Fuel Percentage
* Send Fuel Data to Main Node through CAN

### Peripherals Used

* Fuel Gauge Sensor
* ADC
* CAN Controller

---

# CAN Communication

## Message Flow

Fuel Node
|
| Fuel Percentage
v

Main Node
|
| Indicator Command
v

Indicator Node

---

# CAN Identifiers

Example:

Fuel Node ID      : 0x100
Indicator Node ID : 0x200
Main Node ID      : 0x300

(Modify according to source code)

---

# Hardware Requirements

* LPC2129
* MCP2551 CAN Transceiver
* LCD 16x2
* DS18B20 Temperature Sensor
* Fuel Gauge Sensor
* Indicator LEDs
* Switches
* USB-UART Converter

---

# Main Node Connections

## LCD

D0-D7  -> P0.8 to P0.15
RS     -> P0.16
RW     -> P0.17
EN     -> P0.18

## DS18B20

DATA -> P0.7

## Left Indicator Switch

SW1 -> EINT0

## Right Indicator Switch

SW2 -> EINT1

## CAN Interface

CANRX -> RD1
CANTX -> TD1

Connected to MCP2551

---

# Indicator Node Connections

## LED Connections

LED0 -> P0.8
LED1 -> P0.9
LED2 -> P0.10
LED3 -> P0.11
LED4 -> P0.12
LED5 -> P0.13
LED6 -> P0.14
LED7 -> P0.15

## CAN Interface

CANRX -> RD1
CANTX -> TD1

Connected to MCP2551

---

# Fuel Node Connections

## Fuel Sensor

Fuel Output -> ADC Channel

Example:

Fuel Sensor -> AD0.1

## CAN Interface

CANRX -> RD1
CANTX -> TD1

Connected to MCP2551

---

# CAN Bus Wiring

 Node1 CANH ---- CANH Node2 ---- CANH Node3

 Node1 CANL ---- CANL Node2 ---- CANL Node3

120Ω Termination Resistor at both ends of CAN Bus.

---

# Project File Structure

Major_Project

├── Major_Main.c
├── Major_Fuel.c
├── Major_Indicator.c
├── Major_CAN.c
├── Major_CAN.h
├── Major_ADC.c
├── Major_DS18B20.c
├── lcd.c
├── delay.c
├── types.h
└── defines.h

---


# Running Individual Nodes in Keil

The project contains source code for all 3 nodes in a single project.

Only one node must be compiled and flashed at a time.

## To Run Main Node

Enable:

Major_Main.c

Disable:

Major_Fuel.c
Major_Indicator.c

Build and Flash Board-1

---

## To Run Fuel Node

Enable:

Major_Fuel.c

Disable:

Major_Main.c
Major_Indicator.c

Build and Flash Board-2

---

## To Run Indicator Node

Enable:

Major_Indicator.c

Disable:

Major_Main.c
Major_Fuel.c

Build and Flash Board-3

---

# Keil Configuration

1. Open Project
2. Right Click Source Group
3. Exclude Unwanted Node Files
4. Build Project
5. Generate HEX File
6. Flash Using Flash Magic

---

# Project Workflow

1. Fuel Node reads fuel sensor.
2. Fuel Node sends fuel percentage through CAN.
3. Main Node receives fuel percentage.
4. Main Node reads DS18B20 temperature.
5. Main Node updates LCD.
6. User presses Left or Right Switch.
7. Main Node sends indicator command through CAN.
8. Indicator Node receives command.
9. Corresponding indicator LEDs blink.

---

# Expected Output

LCD Display:

TEMP : 32°C
FUEL : 78%

LEFT IND : ON

or

RIGHT IND : ON

Indicator Node:

Sequential LED scrolling for vehicle indicators.

---

# Learning Outcomes

* CAN Protocol
* CAN Arbitration
* CAN Frames
* CAN Error Handling
* LPC2129 Programming
* ADC Interface
* DS18B20 Interface
* External Interrupts
* Multi-Node Embedded Systems

---

# Author

Mohammad Abdul Rehaman Baig

B.Tech – Electronics and Communication Engineering

Embedded Systems Project using LPC2129 and CAN Bus.
