# High-Level Description of Systems 

HLS is conceived to help conventional programmers to design hardware, not at its best efficiency but fairly well. Using Vitis HLS (before Vivado HLS), C code is transformed into VHDL, to later verify the correct execution and improve its performance (througput and latency) through benchmarking techniques like pipelining. 

1. **Hardware Implementation (C-Based)**：The final project consists on translating a MATLAB image texture classification algorithm into hardware.

<img width="364" height="299" alt="Captura de pantalla 2025-12-03 003430" src="https://github.com/user-attachments/assets/6f648901-0c54-46ab-a9d1-00f1a8024b82" /> 

2. **[SystemC](https://systemc.org/) Simulation**：An extra project consists on the Electronic System-Level (ESL) replication in SystemC of this algorithm.
   
<img width="450" height="220" alt="Captura de pantalla 2025-12-03 003326" src="https://github.com/user-attachments/assets/4a61b1d0-47d0-42c9-85a4-c7b9c38d3ce1" /> <br/>

>[!Note]
> SystemC is a C++ library for system-level modeling, simulation, and verification of hardware/software systems. It enables design of digital systems (e.g., SoCs, embedded systems) at abstract or detailed levels, supporting:
>- **Hardware modeling**: Modules, signals, clocks, and concurrency (via processes).
>- **Software/hardware co-simulation**: Integrating C/C++ software with hardware models.
>- **High-level synthesis (HLS)**: Bridging abstract system design and RTL implementation.
>
> Used in electronics design to validate architecture, test inter-module communication, and optimize systems before physical implementation—critical for complex projects like embedded systems or SoCs.

# Final Project: Image Texture Classification

## Introduction
Texture is a critical visual attribute of objects, materials, and scenes, making it indispensable for numerous computer vision applications. This project aims to implement a hardware-accelerated classification system that identifies image textures.
 
## System Overview
The classification system comprises two primary modules:
- **Feature Extraction**: Prepares input data for classification by extracting texture-relevant features—specifically, Local Binary Patterns (LBP). This stage takes an image as input and outputs a 256-element feature vector.
- **Classifier**: Uses the LBP feature vector to predict the texture category of the input image. As a supervised machine learning component, it is trained on labeled data samples; here, a Support Vector Machine (SVM) is employed for multi-class texture classification.

>[!TIP]
>The available **folders** contain the following:
>- **MATLAB**：the original MATLAB files that compose the image texture classification algorithm.
>- **C_code**: the adapted source code to be introduced into Vitis HLS.
>- **Vitis_HLS**: these files already into
>- **SystemC**: the system-level design simulation files.
>- **lab5**: basic SystemC modules like a Counter, a Multiplexer and a Finite State Machine.
>- **benchmarks_VitisHLS**: benchmark results for every block under several pipelining configurations.
>
>  You can also find the final project report and the Pareto diagrams.

<img width="369" height="226" alt="Captura de pantalla 2025-12-03 003407" src="https://github.com/user-attachments/assets/14b4376c-ee27-4398-84d2-b2abbce642fe" />

### Image Data Set
The project utilizes the [Outex color texture set](http://lagis-vi.univ-lille1.fr/datasets/outex.html), containing 4,320 128x128 grayscale images across 24 texture categories. The dataset includes variations in illumination, spatial resolution, and camera rotation.

First, download the Outex TC 00010-r test suite. Upon extraction, the folder contains two subdirectories: 000 (text files describing image sets) and images (the actual image data). The 000 folder includes train.txt (480 training images) and test.txt (3,840 inference images), each line specifying an image and its ground-truth label.

To use the provided Matlab scripts, **remove the first line** from both train.txt and test.txt (these lines describe the dataset, not image instances). Training labels guide classifier learning, while inference labels validate prediction accuracy.

### Local Binary Patterns (LBP)
[Local Binary Patterns (LBP)](https://www.pyimagesearch.com/2015/12/07/local-binary-patterns-with-pythonopencv/) are texture descriptors for image classification, operating as follows:

1. The image is processed in raster scan order (pixel-by-pixel).
2. For each pixel, its 8-neighbor neighborhood is extracted. Each neighbor is thresholded against the central pixel: a value of ‘1’ is assigned if the neighbor is brighter, ‘0’ otherwise. This generates an 8-bit binary code, converted to a decimal value (0–255).
3. A histogram of these decimal values (256 bins) is computed across the entire image—this histogram serves as the texture feature vector.
 
You may analyze and run the custom Matlab script LBPFeatures.m to understand LBP implementation.

<img width="297" height="128" alt="Captura de pantalla 2025-12-02 222045" src="https://github.com/user-attachments/assets/a584395a-eaba-4783-8b38-a6ce47caabd5" /> <br/>
<img width="393" height="292" alt="Captura de pantalla 2025-12-03 003341" src="https://github.com/user-attachments/assets/916b75e3-0222-4edc-9aed-1696368d165a" />

### Support Vector Machine (SVM)
[Support vector machines (SVM)](https://en.wikipedia.org/wiki/Support) are supervised learning models for classification/regression. For binary classification, the SVM trains a hyperplane to maximize the margin between support vectors (closest class samples), classifying new samples based on their side of the hyperplane (labeled -1/+1).

For multi-class texture classification (24 categories), a one-vs-rest approach is used: 24 independent binary SVMs are trained (each distinguishing one class from the rest). During inference:
- If one SVM returns a positive distance, its class is predicted.
- If multiple SVMs return positive distances, the class with the largest distance is selected.
- If all distances are negative, the class with the smallest (least negative) distance is chosen.

Training is pre-completed via Matlab’s Classification Learner, with the model saved as Trained SVM Model.mat. Use Predict SVM.m to test inference: it takes an LBP feature vector, computes distances to all 24 hyperplanes, and outputs the predicted class.

<img width="373" height="219" alt="Captura de pantalla 2025-12-03 003355" src="https://github.com/user-attachments/assets/57523a57-a8e4-46f0-b084-f3eacf1710f0" />

### Overall Processing System (TOP)
The system operates in two phases:

- **Training**: The ExtractFeatures4training.m script reads train.txt, computes LBP histograms for all training images, and stores features/labels in a table. This table trains the SVM (pre-executed via Matlab’s Classification Learner).
- **Inference**: The Inference Dataset.m script reads test.txt, computes LBP features for each image, and uses the pre-trained SVM to predict labels. Inference on train.txt should yield 100% accuracy (it uses training data); test.txt (unseen data) will show real-world performance.

The ExportDataStructures.m script exports classifier data to binary files for HLS simulation—adapt it to your solution’s format.

## System Implementation

**Part 1: Hardware Implementation (C-Based)**
A hardware-accelerated texture classification system was developed in C, integrating both Local Binary Patterns (LBP) feature extraction and Support Vector Machine (SVM) classification—either as a unified accelerator or two separate modules.

>[!NOTE]
>Key optimizations were applied using hardware-specific directives (aligned with lab methodologies), including:
>- Strategic data transfer design to minimize latency at critical pipeline stages.
>- A comprehensive design space exploration to evaluate tradeoffs across metrics (e.g., latency, resource utilization, throughput) by varying data types, parallelism levels, and interface configurations.
>- A testbench was created to validate consistency between hardware outputs and Matlab reference results, ensuring functional accuracy.

**Part 2: SystemC Simulation**
The classification system was modeled and simulated in SystemC, with distinct modules defined for LBP and SVM components.

>[!NOTE]
>Focus was placed on formalizing data transfer protocols between modules to reflect real-world hardware communication, rather than full implementation. The SystemC model prioritizes behavioral accuracy and inter-module interaction, serving as a high-level validation of the system architecture.
