# Master Thesis: GNSS Anti-jamming signal processing with a HackRF One-based Software-Defined Receiver

> [!IMPORTANT]
> Software used: <br/>
> - **GNU Radio Companion**: GNSS signal capture <br/>
> - **MATLAB**: SDR signal processing

This is my current Master Thesis scope. Let me break it down:

- "GNSS": the satellite systems that tell us our position and time (e.g. GPS for the US or Galileo for Europe), used for navigation in application like Google Maps or autonomous driving.
- "Anti-jamming signal processing": mitigation techniques against attacks concerning the signal waveform properties (signal power, signal-to-noise ratio SNR, etc.).
- "with a HackRF One-based": a state-of-the-art platform known for its wide sampling rate properties and its low-cost applications.
- "Software-Defined Receiver": a recent strategy that implements the hardware features of the receiver's front-end in software, giving more design flexibility against emerging evolving attacks.

## Signal processing steps:
1. _Setup_: Get out of the building, connect the HackRF One and a GNSS antenna, launch GNU Radio Companion and start capturing the signal.
2. _Capturing_: Ensure you are capturing what you expect with a live frequency plot and save your capture in a .bin format.
3. _Pre-processing_: Run your code in Matlab and ensure the signal's integrity on the pre-processing stage. If not, correct this part.
4. _Acquisiton_: locate satellites available in the sky. Need at least 4.
5. _Tracking_: lock into the satellite's message and demodulate it.
6. _Navigation_: obtain results - latitude, longitude and altitude. Your rover (e.g. an autonomous car) can now take decisions according to its position and time.

<img width="330" height="390" alt="Flowgraph_Poster_Thesis_blackfont" src="https://github.com/user-attachments/assets/80b9192b-ec33-425b-b55c-986000032716" />
<img width="661" height="390" alt="Captura de pantalla 2025-12-09 151204" src="https://github.com/user-attachments/assets/37f81d72-e0ec-48f1-a0e2-87ae40c1cc7d" />
<img width="455" height="402" alt="Captura de pantalla 2025-12-09 141206" src="https://github.com/user-attachments/assets/87fa8c6f-57e3-43d8-b4f3-36549940b158" /> <img width="455" height="402" alt="Captura de pantalla 2025-12-09 152747" src="https://github.com/user-attachments/assets/141d1595-7f97-468c-a5b5-d28c16cd196b" />
<img width="455" height="402" alt="Captura de pantalla 2025-12-09 141518" src="https://github.com/user-attachments/assets/2f470e2f-9041-403d-8e77-85bc29f889df" /> <img width="455" height="402" alt="Captura de pantalla 2025-12-08 120305" src="https://github.com/user-attachments/assets/50dbc8ed-9c9c-4712-bf51-27bf634e21cf" />



## What are my objectives? 
- Generate three different types of attacks: carrier-wave, pulsed noise and PRN spoofing.
- Configurate my Matlab code to detect these attacks and switching off further processing.
- Adapt the code to mitigate them and keep it working properly.
