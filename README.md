# Parallel Merge Sort using Multithreading in C

This project implements a **high-performance parallel merge sort in C** using **POSIX threads (pthreads)**. It explores two different concurrency strategies—**Method 1** (single-thread phased merging) and **Method 2** (multi-threaded phased merging)—and provides detailed performance analysis based on array size, thread count, and hardware constraints.

---

## 🔧 Key Features

* Fully implemented in **C** with **POSIX threads** (`pthread_create`, `pthread_join`, mutexes, condition variables).
* Dynamically generated array of size `2^n` from an input file.
* Array is divided into `2^p` segments, each of size `2^(n-p)`.
* Random values assigned to array elements.
* Two merging strategies:

  * **Method 1:** Sequential phased merging (one thread per phase).
  * **Method 2:** Parallel phased merging (multiple threads per phase).
* Performance evaluated with graphs by:

  * Varying **number of threads** (`p`)
  * Varying **array size** (`n`)
* Demonstrates impact of:

  * Context switching overhead
  * Thread creation cost
  * Core count limitations
  * Parallel speedup and diminishing returns

---

## 🧵 Low-Level Design

### **Array Segmentation**

* `2^p` threads are created: `thread0` → `thread(2^p-1)`
* Each thread sorts one segment of size `2^(n-p)`
* Segment `k` starts at index `k * 2^(n-p)`
* Threads exit after sorting their segments

---

### **Method 1: Phased Single-Thread Merging**

1. Segments: `segment0, segment1, ..., segment(2^p-1)`
2. Merge in phases:

   * Phase 1: merge `segment0 + segment1` → update `segment0`
   * Phase 2: merge `segment0 + segment2` → update `segment0`
   * Continue until `segment0` contains the whole array
3. **Key insight:** Only 1 thread is used for merging in each phase → less overhead

---

### **Method 2: Phased Multi-Thread Merging**

1. Merge in parallel:

   * Phase 1: thread0 merges `(0,1)`, thread2 merges `(2,3)`, …
   * Phase 2: thread0 merges `(0,2)`, thread4 merges `(4,6)`, …
   * Each phase halves the number of segments
2. Threads exit after completing their merge in each phase
3. **Key insight:** More threads → higher overhead → slower when threads > cores

---

## 📊 Performance Analysis

### **Graph 1: Fix n = 12, vary p = 2…6**

* **X-axis:** number of threads `p`
* **Y-axis:** sorting time
* **Insights:**

  * Optimal thread count `k` ≈ 3.3 (Method 1), 2.1 (Method 2) on a 2-core CPU
  * Beyond `k`, overhead dominates → sequential method performs better
* **Ranking:**

  1. Sequential
  2. Method 1
  3. Method 2

---

### **Graph 2: Fix threads = 16, vary n = 7…12**

* **X-axis:** array size `n`
* **Y-axis:** sorting time
* **Insights:**

  * For Method 1 and 2, sorting time decreases initially due to parallelism
  * Time increases when array growth outweighs parallelism benefits
* **Ranking:**

  1. Sequential
  2. Method 1
  3. Method 2

---

## 🧠 Key Learnings

* Parallel merge sort is effective **only while threads ≤ cores**
* Excess threads increase:

  * Context switching
  * Thread creation overhead
  * Sorting time
* Method 1 generally outperforms Method 2 due to lower merging overhead
* Method 2 may become competitive for **very large arrays**
* Sequential sorting is better for small arrays or excessive threads

---

## ✅ Conclusion

* **Sequential > Method 1 > Method 2** for most cases
* For extremely large arrays, **Method 2** may outperform sequential and Method 1
* **Method 1** is more efficient than Method 2 in typical scenarios

---

## 🛠 Technologies Used

* **C (POSIX threads)**
* **Standard C library:** arrays, `malloc`, `free`, `qsort`, `memcpy`
* **Linux (Ubuntu)**
* **gcc**, **gdb**, **valgrind**

---

## ⚙️ Usage

1. Provide an input file with values for `n` and `p`.
2. Compile with:

   ```bash
   gcc -pthread -o parallel_merge_sort parallel_merge_sort.c
   ```
3. Run:

   ```bash
   ./parallel_merge_sort input.txt
   ```
4. Program outputs sorted array and performance metrics for both methods.
