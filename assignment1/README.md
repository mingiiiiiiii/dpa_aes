## Differential Power Analysis (DPA) Attack on AES-128 S-Box

This guide explains how to perform a Differential Power Analysis (DPA) attack on the AES-128 S-Box. The attack involves capturing power consumption data during the execution of the AES algorithm (via the Hamming Weight Power Model attack) and analyzing it to recover the secret key. The attack consists of two main steps: power consumption simulator and DPA.

### Prerequisites

Before you begin, make sure you have the following prerequisites:

1. C Compiler (e.g., gcc) installed on your system.
2. The source code for the AES algorithm (AES.c, util.c, main.c).
3. A directory named "DPA" where you will store the power consumption data and the final DPA result.

### power consumption simulator

1. Compile the simulator code:
   ```shell
   gcc AES.c util.c main.c -o a.out
   ```

2. Run the simulator:
   ```shell
   ./a.out
   ```

3. After running the simulator, you will have a file named `simulatedPowerConsumption.txt`. Move this file to the "DPA" directory:
   ```shell
   cp simulatedPowerConsumption.txt DPA/
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
