Digital Signal Processing is a widely-known course for every discipline inside electronic engineering. In this scenario, the focus is to implement digital filters in VHDL, which are proven to be much more robust. 

This time, however, the experiments are carried out using an older FPGA in ISE instead of Vivado because of compatibility issues with the lab's oscilloscope. More concretely, this robustness is due to the design of a sigma-delta converter block.

The same filter is tested for a parallel and a serial architecture. Afterwards, the filter efficiency is proven with an oscilloscope to check how steep the cut-off frequency is.
