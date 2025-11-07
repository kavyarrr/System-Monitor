# System Monitor Project Report

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architectural Prototype](#architectural-prototype)
3. [User Interface Prototype](#user-interface-prototype)
4. [Developed Functionalities](#developed-functionalities)
5. [Tools/Technologies Used](#toolstechnologies-used)
6. [SOLID Principles Application](#solid-principles-application)
7. [System Architecture](#system-architecture)
8. [Implementation Details](#implementation-details)
9. [Screenshots Required](#screenshots-required)

---

## Project Overview

The System Monitor is a real-time Linux system monitoring application built in C++ that provides comprehensive system information including CPU utilization, memory usage, process management, and system statistics. The application features a terminal-based user interface using the ncurses library and implements advanced process scheduling algorithms.

### Key Features:
- Real-time system monitoring
- CPU and memory utilization tracking
- Process management and monitoring
- Interactive process scheduling simulation
- Terminal-based graphical interface
- System overload detection and warnings

---

## Architectural Prototype

### System Architecture Overview

The System Monitor follows a modular, object-oriented design with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                    System Monitor                          │
├─────────────────────────────────────────────────────────────┤
│  Presentation Layer (ncurses_display.cpp)                  │
│  ├── System Information Display                            │
│  ├── Process List Display                                  │
│  ├── Progress Bars and Visualizations                     │
│  └── Interactive Scheduling Interface                     │
├─────────────────────────────────────────────────────────────┤
│  Business Logic Layer                                      │
│  ├── System Class (system.cpp)                            │
│  ├── Process Class (process.cpp)                          │
│  ├── Processor Class (processor.cpp)                      │
│  └── Format Utilities (format.cpp)                        │
├─────────────────────────────────────────────────────────────┤
│  Data Access Layer (linux_parser.cpp)                     │
│  ├── /proc filesystem parsing                              │
│  ├── System statistics extraction                          │
│  ├── Process information gathering                         │
│  └── CPU utilization calculations                         │
├─────────────────────────────────────────────────────────────┤
│  Linux Kernel Interface                                    │
│  ├── /proc/meminfo                                        │
│  ├── /proc/stat                                            │
│  ├── /proc/[pid]/stat                                     │
│  └── /proc/uptime                                          │
└─────────────────────────────────────────────────────────────┘
```

### Design Patterns Applied

1. **Composition Pattern**: System class contains Processor and Process objects
2. **Facade Pattern**: LinuxParser provides simplified interface to complex /proc filesystem
3. **Strategy Pattern**: Different scheduling algorithms (commented hybrid algorithm)
4. **Observer Pattern**: Real-time monitoring with periodic updates

---

## User Interface Prototype

### Terminal-Based Interface Design

The application provides a comprehensive terminal-based interface with three main windows:

#### 1. System Information Window
- Operating System details
- Kernel version
- CPU utilization with progress bar
- Memory utilization with progress bar
- Total and running process counts
- System uptime
- Warning alerts for high CPU usage (>80%)

#### 2. Process List Window
- Tabular display of top processes
- Columns: PID, USER, CPU%, RAM(MB), TIME+, COMMAND
- Real-time sorting by CPU utilization
- Color-coded headers

#### 3. Interactive Output Window
- Process scheduling simulation results
- Performance comparison metrics
- System overload detection timing
- Real-time CPU utilization monitoring

### Interface Features:
- Color-coded progress bars (blue for CPU/memory)
- Real-time updates every second
- Responsive layout adaptation
- Warning system for high resource usage
- Interactive process management

---

## Developed Functionalities

### Core System Monitoring

1. **Real-time System Statistics**
   - CPU utilization calculation using /proc/stat
   - Memory utilization from /proc/meminfo
   - System uptime tracking
   - Process count monitoring

2. **Process Management**
   - Individual process CPU utilization calculation
   - Process memory usage tracking
   - Process command and user identification
   - Process uptime calculation
   - Dynamic process list sorting

3. **Advanced Process Scheduling**
   - Interactive process creation and management
   - Round-robin scheduling simulation
   - Process scheduling comparison (with/without scheduling)
   - System overload detection and measurement
   - Signal-based process control (SIGCONT/SIGSTOP)

4. **Performance Monitoring**
   - CPU utilization threshold monitoring (80% warning)
   - Time-to-overload measurement
   - Scheduling algorithm performance comparison
   - Real-time performance metrics

### Technical Implementations

1. **Linux System Integration**
   - Direct /proc filesystem parsing
   - Real-time data extraction
   - Efficient file I/O operations
   - Process ID enumeration

2. **Mathematical Calculations**
   - CPU utilization percentage calculation
   - Memory usage percentage computation
   - Process CPU time calculations
   - System uptime conversions

3. **User Interface Components**
   - Progress bar generation
   - Color-coded displays
   - Multi-window layout management
   - Real-time screen updates

---

## Tools/Technologies Used

### Programming Languages
- **C++17**: Primary programming language with modern C++ features
- **C**: System-level programming for Linux integration

### Build System
- **CMake**: Cross-platform build system configuration
- **Make**: Traditional build automation
- **clang-format**: Code formatting and style enforcement

### Libraries and Frameworks
- **ncurses**: Terminal-based user interface library
- **Standard C++ Library**: STL containers, algorithms, and utilities
- **Linux System Libraries**: 
  - `<unistd.h>`: System calls and process management
  - `<signal.h>`: Signal handling for process control
  - `<dirent.h>`: Directory operations
  - `<fstream>`: File I/O operations

### Development Tools
- **GCC/Clang**: C++ compiler with C++17 support
- **Debug Build Support**: CMake debug configuration
- **Code Formatting**: Automated code style enforcement
- **Cross-platform Compatibility**: Linux-focused implementation

### System Integration
- **Linux /proc filesystem**: System information extraction
- **Process Management**: fork(), kill(), signal handling
- **File System Operations**: Real-time data reading
- **Threading**: std::thread for concurrent operations

---

## SOLID Principles Application

### 1. Single Responsibility Principle (SRP)

**Applied in:**
- `LinuxParser` class: Solely responsible for parsing Linux system files
- `Process` class: Handles only process-specific operations
- `Processor` class: Manages only CPU-related calculations
- `NCursesDisplay` class: Handles only display and UI logic

**Code Example:**
```cpp
// LinuxParser - Single responsibility for system data parsing
namespace LinuxParser {
    float MemoryUtilization();
    long UpTime();
    std::vector<int> Pids();
    // ... only parsing functions
}
```

### 2. Open/Closed Principle (OCP)

**Applied in:**
- Extensible scheduling algorithm framework (hybrid algorithm commented)
- Modular display system allowing new visualization types
- Pluggable process sorting strategies

**Code Example:**
```cpp
// System class can be extended without modification
class System {
public:
    Processor &Cpu();           // Extensible CPU interface
    std::vector<Process> &Processes(); // Extensible process management
    // New functionality can be added without changing existing code
};
```

### 3. Liskov Substitution Principle (LSP)

**Applied in:**
- Process objects maintain consistent interface contracts
- All Process objects can be used interchangeably in collections
- Consistent behavior across different process types

**Code Example:**
```cpp
// All Process objects maintain same interface
std::vector<Process> processes;
for (auto& process : processes) {
    process.Pid();           // Always returns int
    process.getCpuUtilization(); // Always returns float
    // Consistent behavior regardless of process type
}
```

### 4. Interface Segregation Principle (ISP)

**Applied in:**
- Separate interfaces for different concerns
- `LinuxParser` provides specific parsing functions
- Display functions separated from business logic
- Process interface contains only process-related methods

**Code Example:**
```cpp
// Segregated interfaces
class Process {
    // Only process-related methods
    int Pid() const;
    std::string User();
    float getCpuUtilization() const;
};

namespace NCursesDisplay {
    // Only display-related functions
    void Display(System &system, int n = 10);
    void DisplaySystem(System &system, WINDOW *window);
};
```

### 5. Dependency Inversion Principle (DIP)

**Applied in:**
- High-level modules depend on abstractions, not concretions
- System class depends on Processor abstraction
- Display layer depends on System interface
- LinuxParser abstracts complex filesystem operations

**Code Example:**
```cpp
// System depends on Processor abstraction, not concrete implementation
class System {
private:
    Processor cpu_ = {};  // Depends on Processor interface
    std::vector<Process> processes_ = {}; // Depends on Process interface
};

// Display depends on System interface
void NCursesDisplay::Display(System &system, int n = 10);
```

---

## System Architecture

### Component Diagram

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   main.cpp      │    │  NCursesDisplay │    │     System      │
│                 │────│                 │────│                 │
│ Entry Point     │    │ UI Management   │    │ System Logic    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                │                       │
                                │                       │
                       ┌─────────────────┐    ┌─────────────────┐
                       │    Process      │    │   Processor     │
                       │                 │    │                 │
                       │ Process Logic   │    │ CPU Logic       │
                       └─────────────────┘    └─────────────────┘
                                │                       │
                                └───────────┬───────────┘
                                            │
                                    ┌─────────────────┐
                                    │  LinuxParser    │
                                    │                 │
                                    │ Data Access     │
                                    └─────────────────┘
                                            │
                                    ┌─────────────────┐
                                    │  /proc filesystem│
                                    │                 │
                                    │ Linux Kernel    │
                                    └─────────────────┘
```

### Data Flow Architecture

```
Linux Kernel (/proc) → LinuxParser → System/Process/Processor → NCursesDisplay → Terminal UI
```

### Class Relationships

- **Composition**: System "has-a" Processor and "has-a" vector of Processes
- **Dependency**: NCursesDisplay depends on System interface
- **Aggregation**: System aggregates Process objects
- **Utility**: LinuxParser provides utility functions for data access

---

## Implementation Details

### Key Algorithms

1. **CPU Utilization Calculation**
   ```cpp
   // Calculates CPU usage percentage from /proc/stat
   float cpu_utilization = (active_time - prev_active_time) / 
                          (total_time - prev_total_time);
   ```

2. **Process CPU Utilization**
   ```cpp
   // Calculates individual process CPU usage
   const float TOTAL_TIME = (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
   const float ELAPSED_TIME = systime - (starttime / sysconf(_SC_CLK_TCK));
   cpu_utilization = TOTAL_TIME / ELAPSED_TIME;
   ```

3. **Process Scheduling Simulation**
   - Fork-based process creation
   - Signal-based process control (SIGCONT/SIGSTOP)
   - Round-robin time quantum implementation
   - Performance measurement and comparison

### Performance Optimizations

1. **Efficient File I/O**: Direct /proc filesystem access
2. **Memory Management**: RAII principles with automatic cleanup
3. **Real-time Updates**: Optimized refresh cycles
4. **Process Sorting**: Efficient algorithm for top processes

### Error Handling

1. **File Access Validation**: Checks for file stream validity
2. **Process Creation Safety**: Fork error handling
3. **Signal Handling**: Proper signal management
4. **Resource Cleanup**: Automatic ncurses cleanup

---

## Screenshots Required

### 1. Architectural Prototype Screenshots
- **Screenshot 1**: Project directory structure showing src/, include/, build/ folders
- **Screenshot 2**: CMakeLists.txt and Makefile showing build configuration
- **Screenshot 3**: Header files in include/ directory showing class definitions
- **Screenshot 4**: Source files in src/ directory showing implementation files

### 2. User Interface Prototype Screenshots
- **Screenshot 5**: Terminal showing the running System Monitor application
- **Screenshot 6**: System Information window showing OS, Kernel, CPU, Memory details
- **Screenshot 7**: Process List window showing PID, USER, CPU%, RAM, TIME+, COMMAND columns
- **Screenshot 8**: Progress bars for CPU and Memory utilization with color coding
- **Screenshot 9**: Warning message when CPU utilization exceeds 80%

### 3. Developed Functionalities Screenshots
- **Screenshot 10**: Real-time CPU utilization monitoring with live updates
- **Screenshot 11**: Memory utilization tracking with progress bar
- **Screenshot 12**: Process sorting by CPU usage (top processes displayed)
- **Screenshot 13**: System uptime and process count display
- **Screenshot 14**: Interactive process scheduling simulation (if enabled)
- **Screenshot 15**: Performance comparison output window
- **Screenshot 16**: System overload detection and timing measurements

### 4. Tools/Technologies Screenshots
- **Screenshot 17**: Terminal showing compilation with CMake and Make
- **Screenshot 18**: Code editor showing C++17 features and ncurses includes
- **Screenshot 19**: Build output showing successful compilation
- **Screenshot 20**: File structure showing Linux system integration (/proc access)

### 5. SOLID Principles Application Screenshots
- **Screenshot 21**: LinuxParser class showing Single Responsibility (parsing only)
- **Screenshot 22**: System class showing Open/Closed principle (extensible design)
- **Screenshot 23**: Process class interface showing Interface Segregation
- **Screenshot 24**: Class dependencies showing Dependency Inversion
- **Screenshot 25**: Process objects in vector showing Liskov Substitution
- **Screenshot 26**: Modular architecture in header files showing separation of concerns

### Additional Technical Screenshots
- **Screenshot 27**: Code formatting with clang-format
- **Screenshot 28**: Debug build configuration
- **Screenshot 29**: Real-time data from /proc filesystem
- **Screenshot 30**: Process management and signal handling code

---

## Conclusion

The System Monitor project demonstrates a well-architected, real-time system monitoring application that effectively applies object-oriented design principles, modern C++ practices, and Linux system integration. The modular design ensures maintainability and extensibility while providing comprehensive system monitoring capabilities through an intuitive terminal-based interface.

The implementation showcases advanced concepts including real-time data processing, process management, performance monitoring, and interactive system simulation, making it a comprehensive example of system programming and software architecture principles.

---

*Report generated for System Monitor Project - Comprehensive documentation of architecture, implementation, and SOLID principles application.*
