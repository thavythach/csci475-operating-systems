# prisched

## Authors
Matthew Moreno

Thavy Thach

Professor David Chiu

## Description:

### Background:
Timer interrupts, which allows processes that have entered a long-running or infinite stretch of execution to be preempted so that control is returned to the kernel, is an essential component of modern operating systems.
These interrupts prevent a sloppily, or even maliciously, written piece of code from completely hijacking a machine.
This approach, relying upon preemption via timer interrupts, is known as a time-sharing system.

### Program:
The first component of this project is configuration of the hardware timer.
The hardware timer was configured to emit interrupt signals every millisecond.

The second component of this project is implementing mutex locks.
These locks are implemented using atomic assembly commands.

The third component of this project is Djikstra's dining philosophers synchronization problem.
In this problem, `N` philosophers are seated around a circular table with
`N` forks.
One fork is placed between each pair of adjacent philosophers.
At random intervals, each philosopher attempts to grab the fork situated on their right and the fork situated on their left in order to dine.
It is important to the safety and wellbeing of the philosophers that our implementation allow each fork to be acquired by only one philosopher at a time.
In order to prevent deadlocks, we do not allow the philosophers to hold on to one fork while waiting on another.
The philosophers dine indefinitely.
(The dining philosophers simulation is run inside an infinite loop).

## Usage:

1. Start up develop-end
2. Navigate to `compile/`
3. `make clean`
4. `make`
5. `./upload.sh`
6. `sudo minicom`
7. Start up back-end
8. Read screen!

## Deadline
Monday, April 10
