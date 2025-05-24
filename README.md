# Windows-Threads
This C program uses threads (pthread) and mutexes to simulate multiple 
"parent processes", each running 3 worker threads that manipulate 
a shared file input.txt. 
It's designed to show thread synchronization and 
concurrency issues using mutual exclusion (mutex).

Reads an initial number from input.txt.
Spawns 3 "parent processes" (actually threads).
Each parent thread:

Spawns 3 worker threads:

Thread 1: multiplies file value by 4.

Thread 2: subtracts 2 from file value.

Thread 3: divides file value by 2.

These workers work sequentially, each accessing the shared file under a mutex lock.

Each parent thread waits for its workers to finish and reports the final value in the file.

After all parent threads finish, the final file value is printed.
