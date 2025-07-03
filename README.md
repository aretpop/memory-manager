# Virtual Memory Simulator

A C++ implementation of virtual memory management simulation featuring both single-level and multi-level page tables with TLB caching.

**With parallelism:** Each task is executed in its own thread, allowing all tasks to process their memory accesses concurrently. This simulates a multi-process environment and leverages multi-core CPUs for faster and more realistic simulation. Thread safety is ensured using mutexes for all shared resources.

## Description

This project simulates how modern operating systems manage virtual memory using:
- ✅ Single-Level Page Table
- ✅ Multi-Level Page Table (2-level)
- ✅ Translation Lookaside Buffer (TLB) for faster translation

The simulation models the process of mapping logical/virtual addresses to physical addresses using paging and TLB caching, providing insights into memory management mechanisms.

## Project Structure

```
.
├── common/
│   ├── memory_manager.h    # Memory management interface
│   ├── memory_manager.cpp  # Memory allocation implementation
│   ├── tlb.h              # TLB interface
│   ├── tlb.cpp            # TLB implementation
│   └── config.h           # Configuration constants
├── single/
│   ├── task.h             # Single-level task interface
│   ├── task.cpp           # Single-level task implementation
│   └── io.cpp             # Single-level I/O operations
├── multi/
│   ├── taskmulti.h        # Multi-level task interface
│   ├── taskmulti.cpp      # Multi-level task implementation
│   └── iomulti.cpp        # Multi-level I/O operations
├── test.cpp               # Trace file generator
├── makefile               # Build automation
└── README.md             # This file
```

## Requirements

- g++ compiler (C++11 support)
- Make (for building)
- Windows or Linux (WinAPI/Windows threads used)

## Build Instructions

1. Clone the repository:
```bash
git clone <repository-url>
cd memory_manager
```

2. Build the project:
```bash
# Build multilevel page table (default)
make

# Build single-level page table
make single

# Generate trace file
make trace

# Clean build files
make clean
```

## Running the Simulator

1. Generate a trace file (optional, uses default 1M lines, 10 tasks):
```bash
make trace
```

2. Or generate a custom trace file:
```bash
./test <total_lines> <num_tasks>
```

3. Run the simulators:
```bash
# Run multilevel page table simulator
./multilevel_pagetable trace.txt

# Run single-level page table simulator
./single_pagetable trace.txt
```

## Features

- 🧠 TLB cache simulation with hit/miss tracking
- 🧮 Physical memory page allocation and deallocation
- 📊 Per-task statistics (page hits/misses, TLB hits/misses)
- 🧵 Multi-threaded trace file generation
- 🛠️ Cross-platform build system (Windows/Linux)

## Trace File Format

The trace file contains memory access patterns in the format:
```
T<task_id>: 0x<address>: <size>KB
```

Example:
```
T1: 0x00423000: 8KB
T2: 0x00A31000: 4KB
```

## Implementation Details

### Multi-level Page Table
- Two-level page table structure
- Page directory (10 bits) + Page table (10 bits) + Offset (12 bits)
- 4KB page size
- TLB caching for faster translation

### Single-level Page Table
- Direct mapping from virtual to physical pages
- 4KB page size
- TLB caching for faster translation

## Known Limitations

- This is a **simulation**, not actual OS memory management
- Physical memory is simulated in software
- TLB is simplified to basic key-value storage
- Limited to 32-bit addressing
- **Platform Dependency**: The trace generator uses Windows-specific threading (`windows.h` and WinAPI). This means:
  - The code will not compile on Linux systems without modification
  - To make it work on Linux, you would need to:
    1. Replace `windows.h` with `pthread.h`
    2. Replace Windows thread functions (`CreateThread`, `WaitForMultipleObjects`) with pthread equivalents
    3. Replace Windows synchronization primitives (`CRITICAL_SECTION`) with pthread mutexes
    4. Modify the thread function signatures to match pthread requirements

## Author

**Shashwat Saini**  
Roll No: 2023AIB1015  

**Vaibhav Gupta**

Roll No:2023AIB1019

AI Lab 3

## License

This project is part of the AI Lab 3 coursework.

## How the Project Works

This simulator models virtual memory management by simulating memory accesses from multiple tasks (processes). The workflow is as follows:

1. **Trace File Input**: The simulator reads a trace file (`trace.txt`) containing memory access patterns for multiple tasks. Each line specifies a task, a memory address, and the size of the access.
2. **Task Creation**: For each unique task in the trace, a corresponding task object is created. Each task maintains its own page table and TLB (Translation Lookaside Buffer).
3. **Parallel Execution**: Each task is executed in its own thread, allowing all tasks to process their memory accesses in parallel. This simulates a multi-process environment and leverages multi-core CPUs for faster simulation.
4. **Thread Safety**: Shared resources, such as the physical memory manager and TLB structures, are protected using mutexes (`std::mutex`). This ensures that memory allocation, deallocation, and TLB updates are thread-safe, preventing race conditions and data corruption.
5. **Memory Access Simulation**: Each thread processes its assigned memory accesses, performing TLB lookups, page table lookups, and physical memory allocations as needed. TLB hits/misses and page hits/misses are tracked for each task.
6. **Statistics Reporting**: After all threads have completed their work (using thread joining to ensure all processing is finished), the simulator prints per-task statistics, including TLB and page table hit/miss rates.

### Key Points
- **Parallelism**: Each task runs in its own thread, processing memory accesses concurrently with other tasks.
- **Race Condition Avoidance**: All access to shared resources (physical memory manager, TLB) is protected by mutexes, ensuring thread safety.
- **Accurate Simulation**: The simulator models both single-level and multi-level page tables, as well as TLB caching, to provide a realistic view of virtual memory management.
- **Output**: Statistics are only printed after all threads have finished, preventing interleaved or corrupted output.
