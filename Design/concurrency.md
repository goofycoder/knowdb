### Locking
- Avoid locking
    - The best way to avoid locking is to avoid shared mutable state. 
    - When shared mutable state is needed, use a mutex. 
    - If you experience lock contention, consider using more mutexes, each protecting less data 
    	(that is, make the locking finer-grained).
    *** A particularly important rule of thumb for deadlock avoidance is never hold a lock while invoking a callback. 
    
- Lock contention
	- partitioning locked data structures to avoid lock contention. 
	- For example, it is easier, more efficient, to build a high-concurrency hash table from many normal hash tables, 
	  each with its own lock, than to build one lock-free hash table using atomic operations.

- Two models to handle concurrency
    - these two models permit high-performance implementations that can use multiple CPUs and sharing of resources
    - 1. "threads + mutexes + condition-variables"
      2. "threads + message-passing". 
       *** These two can be used together, and often are.
    - Both models allow high-throughput implementations, and both can suffer from both races and deadlocks. 
        * Deadlocks can often be eliminated in the message-passing model by using unbounded queues and/or threadpools

- Message passing
    - Associate data with threads, so that each thread owns some variables and data structures
        *a variable is accessed only by its owning thread.
    - Other threads wishing to access the data then communicate with the owning thread via message passing

- When to use?
    * message-passing model tends to work well when 
        - there is a natural resource that already serializes accesses (such as an I/O device)
        - a linear state machine best expressed as a single routine, or 
        - when critical sections are long. 
    * Mutexes work well when 
        - critical sections are short and may be invoked in many places, 
        - or when reader-writer locks can be used effectively. 

-co-operative multithreading 
	- allows the programmer to use multiple threads on a single CPU. 
	- The scheduler guarantees
	    * no two threads can run concurrently
	    * never to pre-empt a thread that has not blocked.

-Single thread: 
	- A process that uses only a single thread requires no mutexes
		*  Often best approach for simple programs that do not require high performance or are inherently sequential. 
	- A single-threaded application can use only one CPU, which typically makes it far slower than other options
		*  usually not the best choice for large programs, or when high performance is required.
	- If application is simple enough, one may be able to run multiple copies of the same program on each machine, 
	  but this introduces two inefficiencies: 
	  	* cross-address-space context switches are more expensive than thread context switches because threads share TLB entries while address spaces do not; 
	  	* the address space separation precludes sharing some resources (caches, ports, etc.).

- Message passing 
    - a form of communication used in concurrent computing, parallel computing, object-oriented programming, and IPC. 
    - By waiting for messages, processes can also synchronize.

- Thread or process synchronization [or serialization]
    - the application of particular mechanisms to ensure that two concurrently-executing threads or processes do not execute specific portions of a program at the same time. 
    - If one thread has begun to execute a serialized portion of the program, any other thread trying to execute this portion must wait until the first thread finishes. 
    - Synchronization is used to control access to state both in small-scale multiprocessing systems and in distributed computers
        * in banking and database systems, in web servers, and so on.

- Lock:
    - a lock is a synchronization mechanism for enforcing limits on access to a resource in an environment where there are many threads of execution. 
    - A lock is designed to enforce a mutual exclusion concurrency control policy.
    - Generally, locks are advisory locks, where each thread cooperates by acquiring the lock before accessing the corresponding data. 
        * there are also mandatory locks, where attempting unauthorized access to a locked resource will force an exception in the entity attempting to make the access.
    - A (binary) semaphore is the simplest type of lock
    - Locks typically require hardware support for efficient implementation. 
      These instructions allow a single process to test if the lock is free, and if free, acquire the lock in a single atomic operation.
      This support usually takes the form of one or more atomic instructions. 
        Example: "test-and-set", "fetch-and-add" or "compare-and-swap" (CAS).

- Disadvantage of locks:
    - Lock-based resource protection and thread/process synchronization have many disadvantages:
        * Cause blocking:
            - some threads/processes have to wait until a lock (or a whole set of locks) is released.
        * Lock handling adds overhead for each access to a resource, even when the chances for collision are very rare. 
             (However, any chance for such collisions is a race condition.)
        * Locks can be vulnerable to failures and faults that are often very subtle and difficult to reproduce reliably.
            - Example: deadlock
        * If one thread holding a lock dies, stalls/blocks or goes into any sort of infinite loop
            0 other threads waiting for the lock may wait forever.
        * Lock contention limits scalability and adds complexity.
        * Priority inversion. 
            - High priority threads cannot proceed, if a low priority thread/process is holding the common lock.
        * Convoying. 
            - All threads have to wait, if a thread holding lock is descheduled due to time-slice interrupt/page fault.
        * Hard to debug: 
            - Bugs associated with locks are time dependent. They are extremely hard to replicate.

- Funnel
    - a funnel is a synchronization primitive used in kernel development to protect system resources.
    - A funnel is a mutex that prevents more than one thread from accessing certain kernel resources at the same time.
    - Funnels are now used in the Mac OS X kernel
    - Each thread acquires a funnel when it enters a synchronized portion of the kernel, and releases it when it leaves. 
        * If a thread blocks (sleeps) while holding a funnel, the kernel forces the thread to automatically drop the funnel, thereby allowing other threads to enter the synchronized portion of the kernel.

- Serializing tokens
    -  a concept in concurrency control arising from the ongoing development of DragonFly BSD
    - Tokens are similar to mutexes in that they can, if used correctly, prevent multiple threads from accessing a shared resource at the same time. 
      Unlike mutexes, however, they do NOT exclude other threads from accessing the resource while they are blocked or asleep. 
    
- Spinlock
    - A spinlock is a lock where the thread simply waits ("spins") until the lock becomes available. 
    - Pro: 
        * very efficient if threads are only likely to be blocked for a short period of time
        * as it avoids the overhead of operating system process re-scheduling. 
    - Con: It is wasteful if the lock is held for a long period of time.

- Race Condition
    - anything where the outcome depends on the relative ordering of execution of operations on two or more threads
        * timing sensitive
    - typically occur where completing an operation requires modification of two or more distinct pieces of data
        * such as the two link pointers in the linked list. 
    - Because the operation must access two separate pieces of data, these must be modified in separate instructions
      and another thread could potentially access the data structure when only one of them has been completed

- Avoid race condition
    - Option 1: (simplest: locking & mutex)
        * wrap your data structure with a protection mechanism
        * ensure that only the thread actually performing a modification can see the intermediate states
    - Option 2: (lock-free programming)
        * modify the design of your data structure
    - Option 3:  software transactional memory (STM)
        * handle the updates to the data structure as a transaction
        * just as updates to a database are done within a transaction.
        * The required series of data modifications and reads is stored in a transaction log and then committed in a single step.

### Deadlock
- Deadlock
    - occurs when an activity
        * attempts to acquire a limited resource that has been exhausted and 
        * cannot be replenished unless that activity makes progress.

- Self-deadlock
    - The simplest mutex-related deadlock is the self-deadlock: 
        mu.Acquire();
        mu.Acquire();      // BUG: deadlock: thread already holds mu
  
- Two-mutex deadlock
    - occurs when thread T0 attempts to acquire M1 while holding M0 at the same time that thread T1 attempts to acquire M0 while holding M1; each thread will wait indefinitely for the other.

- Deadlock debug/avoidance
    - Among the easiest bugs to debug and avoid. 
    - Debugging is typically easy because the address space stops exactly where the bug occurs. 
    - A stack trace of threads is usually all required to see what threads are waiting for and what resources they hold.

- Deadlocks Avoidance
    - disallow cycles in the resources' exhaust-before graph
        * by imposing a partial order on the graph. 
 

### Mutex 
- Mutex in C++0x
    - basic operations
        * create a mutex by constructing an instance of std::mutex, 
        * lock: call member function lock()
        * unlock: call member function unlock(). 
    - *** NOT recommended practice to call the member functions directly
        - because this means that you have to remember to call unlock() on every code path out of a function, 
          including those due to exceptions. 
    - STL provides the std::lock_guard class template, which implements that RAII idiom for a mutex; 
        * it locks the supplied mutex on construction and unlocks it on destruction, 
          thus ensuring a locked mutex is always correctly unlocked. 

- Mutex
    - Majority of cases: group the mutex and the protected data together in a class 
        * rather than use global variables. 

- System call 
    - how a program requests a service from an operating system's kernel. 
    - This may include hardware related services (e.g. accessing the hard disk), creating and executing new processes, and communicating with integral kernel services (like scheduling). 
    - System calls provide an essential interface between a process and the operating system.
