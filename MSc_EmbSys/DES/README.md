# Design of Embedded Systems

This subject is the heart of the MSc, based on hardware/software co-design. 

System-on-Programmable-Chip (SoPC) configuration using Xilinx's commerical devices and tools. Concretely, we work with a Zynq-7020 FPGA device integrated into a Digilent Pynq-Z1 prototyping board. 
The Xilinx Vivado IP integrator tool is used to build a processor-based design, and embedded software will be programmed and debugged with the Vitis unified software platform.

The final project consists on a hardware accelerator that displays a teapot image in a VGA monitor. The display updates while moving the image with the cursor, connecting the composing dots and forming triangles through Bresenham's algorithm.
The result: its hardware implementation, including a Direct Memory Access block (DMA), is 11x times faster than its software imlpementation in Vitis.

Check the final project's report for yourself!
