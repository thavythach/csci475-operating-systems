# Deadlock Detection & Recovery (Xinu)

## Authors
Matthew Moreno

Thavy Thach

Professor David Chiu

## Description:

### Background:
A deadlock is a system state in which several processes wait for resources in a circular chain.
It is desirable to avoid deadlocks because in a deadlocked situation, all involved processes are unable to make progress.
One way to mitigate deadlocks are to detect and recover from them.
When working with locks, which only dispense a single key, a system state can be modeled as a single-instance resource allocation graph (RAG).
A cycle in a RAG would indicate a deadlock condition in the system it represents.
Once a deadlock is detected, several options about how to proceed present themselves.
Among these, as implemented in this project, a deadlock can be rectified by killing one of the involved processes in order to break the wait chain.
Implementing a deadlock detection and recovery system, by preventing intermittent, invisible, and permanent seizing of process execution, leads to a more robust operating system.

### Program:
In XINU, the system state is modeled as a single-instance resource allocation graph.
A depth-first cycle-detection algorithm is implemented to detect deadlock conditions in the RAG.
Every fiftieth time that `resched()` is called, the deadlock detection process is run.
If a deadlock is detected, an arbitrary lock involved in the deadlock is selected and the process holding it is killed.
After cleaning up after the victim process, a message announcing the deadlock recovery is printed.

A test case for deadlock detection and recovery, run as part of a larger Xinu test suite.
This test, entitled "deadlock detection and recovery" is run and its status is printed upon launch of Xinu.
In this test, a counter is initialized to `n` and `n` processes are spawned.
The parent process then waits for all child process pids to have the status `PR_FREE` in the procent table maintained by Xinu.
Each process `i` acquires a lock `i`, decrements the counter, then waits on it to reach `0`.
Once the counter reaches `0`, each process `i` attempts to acquire lock `i + 1` with process `n` acquiring lock `0`.
This induces a deadlock condition, which Xinu should detect and fix.
Each remaining process then increments the counter before terminating.
The ending value of the counter should be `n-1`.
This condition is tested for by the parent process once all child process pids to have the status `PR_FREE` in the procent table maintained by Xinu.
As implemented, `n` is set to 5.

## Usage:

### Deadlock Detection (Off Xinu)
1. Navigate to `dd/`
2. `make clean`
3. `make test`
4. Read screen!

### Deadlock Detection and Recovery (On Xinu)
1. Start up develop-end
2. Navigate to `compile/`
3. `make clean`
4. `make`
5. `./upload.sh`
6. `sudo minicom`
7. Start up back-end
8. Read screen!

## Deadline
Monday, May 2
