# Digital Electronics and Microprocessors
## Final Project: SIMON (VHDL)
The final project consists on replicating the Simon game: **the board generates a certain sequence with LEDs that the user then replicates with the correspondant buttons**. 
<img width="1217" height="684" alt="Captura de pantalla 2025-12-03 010344" src="https://github.com/user-attachments/assets/594ac505-7d7d-4941-8c38-f8d2fdb351b4" /> 

The game controller **State Machine’s diagram** is the following:

<img width="1220" height="686" alt="Captura de pantalla 2025-12-03 010326" src="https://github.com/user-attachments/assets/1f672036-21c3-421b-a528-5a0b61371347" />

The project has been divided in 4 files (each of which is detailed in the report). There is a central one for the main Game Controller State Machine (Simon) and three smaller ones: 
- the Random number generator
- the Debouncer
- the Display. 

## Coursework

This course divided in two parts: 

1. VHDL design of the hardware components learned during the Bachelor's theory of digital circuits.
All the lab work is designed in Vivado and implemented using a Xilinx Zynq-7020 device integrated into a Digilent Pynq-Z1 prototyping board.

2. Full integration of Microprocessors: blocking, polling and interrupts. All the labwork is implemented in Arduino.
