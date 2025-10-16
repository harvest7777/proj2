# Group Project 2: Synchronization
Authors: Phu Quach (ID: 029475548) and Ryan Tran (ID: 031190716)

Course: CECS 326 SEC 02 - Operating Systems

Assignment: Group Project 2: Synchronization


### Project Description
This project implements the classic Dining Philosophers problem using POSIX mutex locks and condition variables in C. The solution prevents deadlock and starvation while allowing philosophers to alternate between thinking and eating.

## How to Compile

To compile the program, use the following command in your terminal:

```bash
make
```

## How to Run

### Basic Usage:
```bash
./dining
```

To remove compiled program:
```bash
make clean
```

## System Requirements
- Unix-like operating system (Linux or macOS)
- GCC compiler
- Standard C library

## Program Features
- Implements the dining philosphers problem with POSIX threads
- Demonstrates mutex locks to prevent deadlocks
- Runs the philospher problem for 40 seconds while printing intermediate outputs
