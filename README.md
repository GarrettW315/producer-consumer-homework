Producer–Consumer Problem (Shared Memory + Semaphores in C++)
Program Description:
This project implements the  Producer-Consumer problem using POSIX shared memory, named semaphores, and mutual exclusion in C++ in Linux. The producer creates items and places them on a shared table, while the consumer removes items from the table.
The shared “table” can only hold two items at a time, so synchronization is required:

-When the table is full, the producer must wait.
-When the table is empty, the consumer must wait.

Semaphores are used to control access, and a mutex ensures only one program accesses shared memory at a time.
The producer and consumer run concurrently and coordinate correctly without conflict or data corruption.
producer.cpp contains the producer program. Generates items and places them into shared memory. Uses semaphores for synchronization.
consumer.cpp contains the consumer program. Removes items from the shared table and prints them. Uses the same shared memory and semaphores.
table.hpp defines the shared buffer structure (table) and buffer size constant. Used by both programs.

Key Components and Concepts

1. Shared Memory (shm_open, mmap)
Shared memory allows both programs to access the same data area (the table).  
The table contains:
-A buffer of size 2
-An index for inserting (in)
-An index for removing (out)
This shared memory persists while programs run.

2. Semaphores (sem_open, sem_wait, sem_post)
I use three named semaphores:
|Semaphore Name|Type|Meaning|
|/empty_sem|Counting|Counts available empty slots (starts at 2)|
|/full_sem|Counting|Counts items available to consume (starts at 0)|
|/mutex_sem|Binary|Ensures mutual exclusion so only one program accesses the buffer at a time|

3. Mutual Exclusion (Critical Section)
A binary semaphore (mutex_sem) prevents the producer and consumer from modifying the shared table simultaneously.

Usage Instructions

1. Compile the Programs
Use g++ and include -pthread and -lrt:
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer
2. Run the Programs
Start both programs (order does not matter):
./producer & ./consumer &
The & runs each process in the background so they run concurrently.
3. Stopping the Programs
Hit:
ctrl + c
4. Cleanup Shared Memory and Semaphores
Run these commands if restarting the program:
pkill producer
pkill consumer
rm /dev/shm/shm_table
sem_unlink /empty_sem
sem_unlink /full_sem
sem_unlink /mutex_sem
