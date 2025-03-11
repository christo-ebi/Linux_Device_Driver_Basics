# Linux_Device_Driver_Basics
This repo contains various Linux Kernel Module examples to get started.

### Prerequisites:
- `sudo apt install raspberrypi-kernel-headers`

### Build (On Raspberry Pi)

1. `cd <Project>`
2. `make`
3. `insmod <project>.ko`
4. `sudo dmesg` (To view the driver logs)

### Projects:
1.  HelloWorld 
    -   Basic LDD with init and exit code.
    -   Logging with prinf macro (can be seen in dmesg).
2.  DevNumberAndFiles.
    - Register LLD under /dev with hardcoded major and minor number.
    - Register open/close system calls.
    - User space App example using these open/close system calls.
3.  AutoDevNo
    -   Auto creation of device number.
    -   Auto creation of char device file under /dev .
4.  Parameters 
    -   Kernel Module which can take input parameters.
5.  ReadWrite
    -   Register read and write system call.
    -   Copy data (char *) from user space to kernel space and vice-versa.
6.  KernelThreads
    -   Creation of two kenerl threads.
    -   Dmesg logs generation form each threads concurrently.
7.  Timer
    -   Sample usage of linux timer API in (kernel space).
8.  HighResTimer
    -   Sample usage of linux High resolution timer API in (kernel space).
9.  DevTreeProb
    -   Device tree overlay with new node.
    -   Usage of Platform device API's.
    -   Dynamic probing of device tree node.
10. PWMDriver.
    -   Device tree overlay for the Rpi 3 B+.
        -   Create myPWM node.
        -   Enalbe Rpi PWM controller.
    -   Dynamically allocate major and minor number.
    -   Register read, write system calls.
    -   Register as a Platform driver.
    -   Probe device Tree -> register & cofigure pin as PWM.
    -   Enable/Disable PWM based on inputs from user space using read/write sys calls.
