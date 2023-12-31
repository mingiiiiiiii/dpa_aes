# Assignment 4: Countermeasure of SCA : Hiding

## AES-128 Hiding using shuffling techniques and DPA (Differential Power Analysis) attack 

This guide explains how to counteract side-channel analysis on the AES-128 S-Box using a hiding technique and practically execute a Differential Power Analysis (DPA) attack. First, a random operation order is assigned to AES s-box operations through Fisher-Yates shuffling. This gives it randomness, making it guess the operation with a probability of 1/16. In theory, this shuffling increases the number of waveforms required for side-channel analysis by 16^2. This guide involves analyzing AES implemented with stealth via the DPA method, which consists of two main steps: implementing AES with shuffling and performing DPA. The DPA attack is performed similarly to [Assignment 1].

### Prerequisites

Before you begin, make sure you have the following prerequisites:

1. C Compiler (e.g., gcc) installed on your system.
2. The source code for the AES algorithm (AES.c, util.c, main.c).
3. A directory named "DPA" where you will store the power consumption data and the final DPA result.

### power consumption simulator (simulating shuffled AES)

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

The final result of the DPA attack can be found in the `DPAresult.txt` file, which will contain the recovered key information. By applying the Hiding technique, you can confirm that you have countered to the DPA attack.

[Assignment 1]: https://github.com/mingiiiiiiii/side-channel_attack/tree/main/assignment1
