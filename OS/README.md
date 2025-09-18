# xv6-pi5 Documentation

## Overview
xv6-pi5 is a port of the MIT xv6 teaching operating system to the ARM architecture, with a focus on compatibility with the Raspberry Pi 5 platform. It provides a minimal Unix-like kernel, shell, file system, and basic user programs, serving as a hands-on resource for learning operating system fundamentals on ARM hardware.

## Getting Started

### Prerequisites
- **ARM GCC Toolchain**: `arm-none-eabi-gcc` (for ARMv6/ARMv7) or `aarch64-linux-gnu-gcc` (for ARMv8/AArch64, Pi 5).
- **QEMU**: For ARM system emulation and testing.
- **Make**: Standard build utility.

### Building xv6-pi5
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/IITGN-Operating-Systems/programming-assignment-1-decentteam.git
   cd programming-assignment-1-decentteam
   ```
2. **Build the Kernel and User Programs**:
   ```bash
   make clean
   make qemu
   ```
   You should see the xv6 shell prompt: `$`

## Implementing Boosting Schedular

### Changing sleep and wakeup mechanics

#### What is the problem?

- At every tick, the timer interrupt is called. It is implemented as isr_timer in device/timer.c
- This increments the tick and wakes up all the processes on the sleep channel - chan. Chan is the global address of ticks. It remains unchanged.
- wakeup1 goes through all the processes in the ptable and changes the status to RUNNING for all process proc->chan = chan.
- Now the implementation of sleep is such that it takes an argument int n, which is the number of ticks the process has to be put on sleep.
- sys_sleep in sysproc.c checks for every tick if the amount of time for the process to stay in sleep is completed. If not, it again calls sleep.
- sleep again changes the status of process to SLEEPING, and updates the proc->chan to chan. After it completes it again sets chan to 0.
- This cycle repeats until time for sleep is completed.

#### Solution

- We change the wakeup1 function in proc.c to change the status of a process to RUNNING if proc->chan == chan and if its sleep time is completed.
- To keep track of the sleep time, we change proc structure in proc.h to include another variable called duration_of_sleep which stores the number of ticks when the process should wake up.
- We update the sys_sleep function to update duration_of_sleep at the start.

### Boosting
