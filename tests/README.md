# Integration Tests for System Monitor

## Overview
This directory contains integration tests that verify the correct interaction between components of the System Monitor application.

## Test Coverage

The integration tests validate:
1. **LinuxParser ↔ /proc filesystem** - File reading and parsing
2. **Processor ↔ LinuxParser** - CPU utilization calculation
3. **Process ↔ LinuxParser** - Process data retrieval
4. **System ↔ All Components** - Full system integration
5. **Data Flow** - End-to-end data flow verification

## Running the Tests

### Prerequisites
- CMake 3.16 or higher
- C++17 compatible compiler (g++ or clang++)
- Linux system with /proc filesystem

### Build and Run

```bash
# Navigate to project root
cd /path/to/System-Monitor

# Configure and build (from project root)
cmake -S . -B build
cmake --build build

# Run integration tests
./build/integration_test
```

### Using CTest (Alternative)

```bash
# From build directory
cd build
ctest -R IntegrationTests -V
```

## Test Output

The tests will display:
- Individual test results (PASS/FAIL)
- System summary information
- Process details
- Final test summary with pass rate

## Screenshot Points

For documentation, capture screenshots of:
1. Test execution showing all test groups
2. System summary output
3. Top processes list
4. Final test summary with pass/fail counts

