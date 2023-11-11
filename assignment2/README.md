# Assignment 2: DPA with real data

## Differential Power Analysis (DPA) Attack on AES-128 S-Box

This guide explains how to perform a Differential Power Analysis (DPA) attack on the AES-128 S-Box. This attack involves reading the captured power consumption data during the execution of the AES algorithm and analyzing it to recover the secret key. The power consumption collection environment is ChipWhisperer-Lite (MCU: Atmel ATxmega128D4). The attack consists of two main steps: read power consumption measured data and DPA.

### Prerequisites

Before you begin, make sure you have the following prerequisites:

1. C Compiler (e.g., gcc) installed on your system.
2. The power consumption file named "CW_Lite_powerConsumption.trace".
3. Python installed on your system along with the NumPy and Matplotlib libraries.
4. The source code for the AES algorithm (AES.c, util.c, main.c).
5. A directory named "DPA" where you will store the power consumption data and the final DPA result.

### read power consumption measured file

The "CW_Lite_powerConsumption.trace" file is a binary file and comprises the following components:
| type              | detail                       | size                              |
|-------------------|------------------------------|-----------------------------------|
| header            | file descryption (string)    | 20 bytes                          |
| header            | num of trace (int)           | 4 bytes                           |
| header            | num of points (int)          | 4 bytes                           |
| header            | "END!" (string)              | 4 bytes                           |
| power consumption | points of each trace (float) | numOfTrace * numOfPoint * 4 bytes |


1. Compile the reader code:
   ```shell
   gcc readtrace.c -o a.out
   ```
   
2. Run the reader:
   ```shell
   ./a.out
   ```
   
3. After running the simulator, you will have a file named `realPowerConsumption.txt`. Move this file to the "DPA" directory:
   ```shell
   cp realPowerConsumption.txt DPA/
   ```   
4. You can save the points of each X-th trace from `realPowerConsumption.txt` into individual files named `traceX.txt`. Run the graph maker to visualize the data:
   ```shell
   python drawtrace.py
   ```

### Differential Power Analysis (DPA)

1. Change to the "DPA" directory:
   ```shell
   cd DPA
   ```

2. Compile the AES code again for the DPA attack:
   ```shell
   gcc AES.c util.c main.c -o a.out
   ```

3. Run the DPA attack:
   ```shell
   ./a.out
   ```

4. After running the DPA attack, you will obtain a file named `DPAresult.txt`. This file contains the results of the DPA attack.

### Final Result

The final result of the DPA attack can be found in the `DPAresult.txt` file, which will contain the recovered key information.
