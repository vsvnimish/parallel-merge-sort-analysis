# Parallel Merge Sort using Multithreading in C

This project implements a high-performance parallel merge sort using **C multithreading (POSIX threads)** and explores two different concurrency strategies—**Method 1** (single-thread phased merging) and **Method 2** (multi-threaded phased merging). It evaluates how thread count (`2^p`), array size (`2^n`), and hardware constraints (number of cores) impact performance.

## 🔧 Key Features

* Fully implemented in **C (POSIX threads, mutexes, condition variables)**.
* Array of size `2^n` partitioned into `2^p` equal segments.
* Each segment is sorted in parallel using one thread per segment.
* Two merging strategies:

  * **Method 1:** Sequential phased merging (1 thread per phase).
  * **Method 2:** Parallel phased merging (multiple threads per phase).
* Complete performance analysis with graphs for:

  * Varying number of threads (`p`)
  * Varying array size (`n`)
* Demonstrates impact of:

  * Context switching overhead
  * Thread creation cost
  * Core count limitations
  * Parallel speedup vs diminishing returns

## 🧵 Concurrency Model

### **Method 1 (Phased Single-Thread Merging)**

1. `2^p` segments sorted in parallel using **pthread_create**.
2. Merging done in phases:

   * Phase 1: merge seg0+seg1
   * Phase 2: merge seg0+seg2
   * ... up to phase `(2^p – 1)`
3. Always requires **1 merging thread per phase**.

**Key insight:**
Method 1 performs better when threads > cores, because merging overhead is lower.

---

### **Method 2 (Phased Multi-Thread Merging)**

1. `2^p` segments sorted in parallel.
2. Each merging phase uses multiple threads:

   * Phase 1: merges (0,1), (2,3), (4,5), …
   * Phase 2: merges (0,2), (4,6), …
3. Number of segments halves each phase.

**Key insight:**
Higher overhead → Method 2 is slower when threads > cores.

---

## 📊 Performance Results

### **Graph 1 (fix n = 12, vary p = 2…6)**

* Peak speedup achieved for **p ≈ 3.3** (≈ 10 threads) on a 2-core CPU.
* After p > 3.3, overhead dominates → performance worsens.
* **Ranking:**

  1. Sequential (fastest overall when threads > cores)
  2. Method 1
  3. Method 2 (most overhead)

**Measured Insight:**
Method 1 improves sorting time until ~3 threads per core.
Beyond that, context switching causes performance to degrade.

---

### **Graph 2 (fix threads = 16, vary n = 7…12)**

* For both methods, time decreases initially (n=7→8 or 9→10) due to increased parallelism.
* Time increases after a threshold because array growth outweighs parallelism benefit.
* **Ranking:**

  1. Sequential
  2. Method 1
  3. Method 2

---

## 🧠 Key Learnings

* Parallel merge sort provides significant speedup **only while thread count ≤ cores**.
* Excess threads cause:

  * Higher context switching
  * Increased overhead
  * Worse performance than sequential sorting
* Method 1 is generally better due to lower merging overhead.
* Method 2 only becomes competitive for very large arrays (large `n`).

---

## 🛠 Technologies Used

* **C (POSIX threads, pthread_mutex, pthread_cond)**
* **Standard C library** (arrays, `malloc`, `free`, `qsort`, `memcpy`)
* **Linux (Ubuntu)**
* **gcc**, **gdb**, **valgrind**
