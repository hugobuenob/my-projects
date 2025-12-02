# OpenMP

OpenMP also optimizes matrix operations through parallelization, but it is more concise than Pthread. <br/>
The number of parallel regions can be controlled through instructions, thereby reducing the running time and achieving a more suitable use.

This graph is given per # of threads and per size of matrix.
<img width="509" height="282" alt="Captura de pantalla 2025-12-03 024609" src="https://github.com/user-attachments/assets/2969bfae-f93b-4be7-b7fa-450374d01042" />

## Pthreads vs OpenMP

While both Pthreads and OpenMP may be used for this parallelization strategy, they have some differences that make them more suitable depending on the situation:
- OpenMP provides a set of easy-to-use compilation instructions that **make it easier to parallelize programs without having to manually manage thread creation and synchronization**. The tasks in the parallel area can be automatically assigned to have different threads for execution and optimized for different hardware platforms, so they can generally achieve a high performance. **However, the parallelization control it provides is relatively simple** and may not provide enough flexibility for some complex parallel modes.
- On the other hand, Pthreads provides more flexibility and control, **being suitable for scenarios that require higher concurrency details, whereas have higher coding complexity.**

