# Digital Signal Processing 
##  Final Project: Digital Filter
A widely-known course inside electronic engineering. In this scenario, the focus is to implement digital filters in VHDL, which are proven to be much more robust.  This robustness is due to the design of a sigma-delta converter block. 

The same filter is tested for a parallel and a serial architecture. Afterwards, **the filter efficiency is proven** with an oscilloscope to check **the steepness of the cut-off frequency**.

The experiments are carried out using an **older FPGA in ISE** instead of Vivado because of **compatibility issues with the lab's oscilloscope**.

### Filter simulation in ISE
<img width="513" height="128" alt="Captura de pantalla 2025-12-03 021532" src="https://github.com/user-attachments/assets/e8644a2c-f217-4f63-93cc-8e143a0bcfa6" />

### Filter implementation
The exit signal (blue) is clearly much cleaner than the entry signal (purple). <br/> <br/>
<img width="484" height="269" alt="Captura de pantalla 2025-12-03 021437" src="https://github.com/user-attachments/assets/23551b5b-d3f9-4b87-9af6-414f51cb293f" />

### Filter block design in Matlab
<img width="464" height="223" alt="Captura de pantalla 2025-12-03 021546" src="https://github.com/user-attachments/assets/fcb47d28-3b21-4df0-b5b9-b6af4487652e" />

Filter characteristics:
- Cut-off frequency = 1 kHz
- Stop frequency = 3 kHz
- Attenuation = 80 dB
- Sampling frequency = 100 kHz
- Order= 127
