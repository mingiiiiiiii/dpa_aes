# Assignment 2: CPA with real data

## Correlation Power Analysis (CPA) Attack on AES-128 S-Box

This guide explains how to perform a Correlation Power Analysis (CPA) attack on the AES-128 S-Box. This attack involves reading the captured power consumption data during the execution of the AES algorithm and analyzing it to recover the secret key. The power consumption collection environment is ChipWhisperer-Lite (MCU: Atmel ATxmega128D4). The attack consists of two main steps: read power consumption measured data and CPA.

### Prerequisites

Before you begin, make sure you have the following prerequisites:

1. C Compiler (e.g., gcc) installed on your system.
2. The power consumption file named "powerConsumption.trace".
3. Python installed on your system along with the NumPy and Matplotlib libraries.
4. The source code for the AES algorithm (AES.c, util.c, main.c).
5. A directory named "CPA" where you will store the power consumption data and the final CPA result.

### read power consumption measured file

The "powerConsumption.trace" file is a binary file and comprises the following components:
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
   
3. After running the simulator, you will have a file named `realPowerConsumption.txt`. Move this file to the "CPA" directory:
   ```shell
   cp realPowerConsumption.txt CPA/
   ```   
4. You can save the points of each X-th trace from `realPowerConsumption.txt` into individual files named `traceX.txt`. Run the graph maker to visualize the data:
   ```shell
   python drawtrace.py
   ```

### Correlation Power Analysis (CPA)

1. Change to the "CPA" directory:
   ```shell
   cd CPA
   ```

2. Compile the AES code again for the CPA attack:
   ```shell
   gcc AES.c util.c main.c -o a.out
   ```

3. Choose attack mode:
   `test1()` is designed for attacking all points.
   `test2()` is designed for attacking selected points. You can customize the points by modifying the `StartPoint` and `EndPoint` values within the code.

3. Run the CPA attack:
   ```shell
   ./a.out
   ```

4. After running the CPA attack, you will obtain a file named `CPAresult.txt`. This file contains the results of the CPA attack.

### Final Result

The final result of the CPA attack can be found in the `CPAresult.txt` file, which will contain the recovered key information.
