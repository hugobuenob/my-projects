# Design of Embedded Systems
## Final Project
### Introduction
The final project aims to design a **hardware accelerator** that displays an **[STL](https://en.wikipedia.org/wiki/File:Utah_teapot_(solid).stl) format [teapot](https://en.wikipedia.org/wiki/STL_(file_format)) figure** in a VGA monitor. This project is implemented in a **Pynq board with a Zynq 7020 System On Chip (SoC)**. This has two main components, the Processing System, with two ARM cores and **the programmable logic, the FPGA fabric**.

<img width="842" height="352" alt="Captura de pantalla 2025-12-03 005331" src="https://github.com/user-attachments/assets/b6c4366d-8ff0-412a-a107-a48a464a55b4" /> <br/>

There are several tasks to be accomplished:
- VGA signal generation to drive a monitor from a video RAM.
- Reception of list of lines to be displayed from a PC via serial port
- Rendering of said lines into the video RAM buffer using the Bresenham’s line drawing
algorithm, both in software and hardware.
- Auxiliary functionality needed to feed the hardware Bresenham implementation with
the endpoints of the lines to be drawn and to copy the output pixels to the video RAM.
- Additionally, rotate points to be displayed in 3D and project them in 2D in order to be
able to rotate the figure being displayed.

### Solution II: Memory transfers with Direct Memory Address (DMA) - Xilinx Vivado IP integrator tool
The main components are:
- Video BRAM (blk_mem_gen_0): Holds the frame buffer, the image to be displayed. It is a dual port RAM,
which can be accessed both from the Processing System and from the VGA Generator IP.
- VGA Generator (vga_driver): Drives the display from the frame buffer and allows the Pixel Update IP
to access said buffer whenever it is not busy.
- Point Buffer BRAM (blk_mem_gen_1): Holds the endpoints of each line to be drawn.
- Point rotation & scaling (matrix_axi): Reads the endpoints and feeds them to the Bresenham IP, with
optional rotation and scaling , which have not been yet implemented in hardware It is
an AXI Lite peripheral, with registers to start line drawing, status and rotation and scaling
parameters.
- Bresenham line drawing (bresenham): Har d ware implementation of the Bresenham line drawing
algorithm, using only integer arithmetic . Once it receives the endpoints, outputs each
pixel of the corresponding line.
- Pixel update (pixel_update): Receives pixels to be written and sets the appropriate bit in the
appropriate word on the frame buffer.

<img width="800" height="326" alt="Captura de pantalla 2025-12-03 011924" src="https://github.com/user-attachments/assets/2e21a13b-8ab8-4264-9f88-7fefcf8cf6c1" />

Communication between IPs is done in a similar way to AXI, the source IP provides data and a “valid” signal, while the destination IP receives the data and provides a “ready” signal. The transaction takes place once both “ready” and “valid” are active.

The whole hardware processing chain is limited by the speed with which pixels can be written into video RAM, which takes two clock cycles (read and write) at a clock frequency of 100 MHz.

Two solutions are explored, using AXI L ite and DMA for data transfer. A timing comparison is performed between them.

### Example executions
<img width="457" height="222" alt="Captura de pantalla 2025-12-03 014231" src="https://github.com/user-attachments/assets/339b5a0e-7316-44dc-98a7-be104e229e92" /> <img width="338" height="194" alt="Captura de pantalla 2025-12-03 013206" src="https://github.com/user-attachments/assets/476f278e-f1b9-4e2e-b2c7-d66ac400c3c9" />

### Timing Analysis 
> [!Note]
> **The result**: its **hardware implementation with DMA is _11x times faster_ than its software implementation** in Vitis.

Figure below shows an execution time comparison between hardware and software implementations, with and without the use of DMA , for three figures with an increasing number of triangles. 

It can be seen that the hardware implementation provides a significant speedup. After the introduction of DMA, it is apparent that most of that speedup is due to the slow memory accesses on a per pixel basis that the software solution has to perform. Hardware has direct control of the frame buffer, so it can access it every cycle.

The use of DMA to copy endpoints from DRAM to the PL BRAM slows down the hardware implementation for a reduced number of lines (Cube figure) due to its overhead. This overhead becomes less significant as the number of lines increases with the teapot figure.

For the most complex figure, the medium resolution teapot, execution time is improved both by using the hardware implementation and DMA.

<img width="650" height="210" alt="Captura de pantalla 2025-12-03 011956" src="https://github.com/user-attachments/assets/3804cf1f-ee18-4869-8498-d016e580db01" />

Go check the final project's report for yourself!

## Coursework

This subject is the heart of the MSc, based on hardware/software co-design. 

**System-on-Programmable-Chip (SoPC)** configuration using Xilinx's commerical devices and tools. Concretely, we work with a **Zynq-7020 FPGA device integrated into a Digilent Pynq-Z1 prototyping board**. 
The **_Xilinx Vivado IP_ integrator tool** is used to build a processor-based design, and embedded software will be programmed and debugged with the **_Vitis_ unified software platform**.
