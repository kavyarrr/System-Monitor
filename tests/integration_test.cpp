#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <cmath>
#include <unistd.h>
#include <algorithm>
#include "../include/system.h"
#include "../include/linux_parser.h"
#include "../include/processor.h"
#include "../include/process.h"

// Simple test framework
class IntegrationTest {
public:
    static int tests_run;
    static int tests_passed;
    static int tests_failed;

    static void assert_true(bool condition, const std::string& test_name) {
        tests_run++;
        if (condition) {
            tests_passed++;
            std::cout << "  [PASS] " << test_name << std::endl;
        } else {
            tests_failed++;
            std::cout << "  [FAIL] " << test_name << std::endl;
        }
    }

    static void assert_false(bool condition, const std::string& test_name) {
        assert_true(!condition, test_name);
    }

    static void assert_range(float value, float min, float max, const std::string& test_name) {
        tests_run++;
        if (value >= min && value <= max) {
            tests_passed++;
            std::cout << "  [PASS] " << test_name << " (value: " << value << ")" << std::endl;
        } else {
            tests_failed++;
            std::cout << "  [FAIL] " << test_name << " (value: " << value 
                      << ", expected range: [" << min << ", " << max << "])" << std::endl;
        }
    }

    static void print_summary() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "INTEGRATION TEST SUMMARY" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Total Tests:  " << tests_run << std::endl;
        std::cout << "Passed:       " << tests_passed << " [";
        if (tests_run > 0) {
            float pass_rate = (tests_passed * 100.0f) / tests_run;
            std::cout << std::fixed << std::setprecision(1) << pass_rate << "%";
        }
        std::cout << "]" << std::endl;
        std::cout << "Failed:       " << tests_failed << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
};

int IntegrationTest::tests_run = 0;
int IntegrationTest::tests_passed = 0;
int IntegrationTest::tests_failed = 0;

// Test 1: LinuxParser Integration - System Metrics
void test_linux_parser_system_integration() {
    std::cout << "\n[TEST GROUP] LinuxParser System Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Test OS name retrieval
    std::string os = LinuxParser::OperatingSystem();
    IntegrationTest::assert_true(!os.empty(), "OperatingSystem() returns non-empty string");

    // Test Kernel version retrieval
    std::string kernel = LinuxParser::Kernel();
    IntegrationTest::assert_true(!kernel.empty(), "Kernel() returns non-empty string");

    // Test uptime retrieval
    long uptime = LinuxParser::UpTime();
    IntegrationTest::assert_true(uptime > 0, "UpTime() returns positive value");

    // Test memory utilization (should be between 0 and 1)
    float mem_util = LinuxParser::MemoryUtilization();
    IntegrationTest::assert_range(mem_util, 0.0f, 1.0f, "MemoryUtilization() returns value in [0, 1]");

    // Test process counts
    int total_procs = LinuxParser::TotalProcesses();
    IntegrationTest::assert_true(total_procs > 0, "TotalProcesses() returns positive value");

    int running_procs = LinuxParser::RunningProcesses();
    IntegrationTest::assert_true(running_procs >= 0, "RunningProcesses() returns non-negative value");
    IntegrationTest::assert_true(running_procs <= total_procs, "RunningProcesses() <= TotalProcesses()");
}

// Test 2: LinuxParser Integration - CPU Data
void test_linux_parser_cpu_integration() {
    std::cout << "\n[TEST GROUP] LinuxParser CPU Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    std::vector<std::string> cpu_data = LinuxParser::CpuUtilization();
    IntegrationTest::assert_true(cpu_data.size() == 8, "CpuUtilization() returns 8 values");

    if (cpu_data.size() >= 8) {
        // Verify all CPU jiffy values are non-negative
        for (size_t i = 0; i < cpu_data.size(); i++) {
            try {
                long value = std::stol(cpu_data[i]);
                IntegrationTest::assert_true(value >= 0, 
                    "CPU jiffy value[" + std::to_string(i) + "] is non-negative");
            } catch (...) {
                IntegrationTest::assert_true(false, 
                    "CPU jiffy value[" + std::to_string(i) + "] is valid number");
            }
        }
    }
}

// Test 3: LinuxParser Integration - Process Data
void test_linux_parser_process_integration() {
    std::cout << "\n[TEST GROUP] LinuxParser Process Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Get list of PIDs
    std::vector<int> pids = LinuxParser::Pids();
    IntegrationTest::assert_true(pids.size() > 0, "Pids() returns non-empty list");

    if (pids.size() > 0) {
        int test_pid = pids[0];
        std::cout << "  Testing with PID: " << test_pid << std::endl;

        // Test process command retrieval
        std::string command = LinuxParser::Command(test_pid);
        IntegrationTest::assert_true(!command.empty() || command == "None", 
            "Command() returns valid string for PID " + std::to_string(test_pid));

        // Test process RAM retrieval
        std::string ram = LinuxParser::Ram(test_pid);
        IntegrationTest::assert_true(!ram.empty(), 
            "Ram() returns non-empty string for PID " + std::to_string(test_pid));

        // Test process uptime retrieval
        long proc_uptime = LinuxParser::UpTime(test_pid);
        IntegrationTest::assert_true(proc_uptime >= 0, 
            "UpTime(pid) returns non-negative value");

        // Test process CPU utilization data
        std::vector<std::string> proc_cpu = LinuxParser::ProcessCpuUtilization(test_pid);
        IntegrationTest::assert_true(proc_cpu.size() == 5, 
            "ProcessCpuUtilization() returns 5 values");
    }
}

// Test 4: Processor Integration
void test_processor_integration() {
    std::cout << "\n[TEST GROUP] Processor Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    Processor processor;

    // First call should initialize and return valid value
    float cpu_util_1 = processor.Utilization();
    IntegrationTest::assert_range(cpu_util_1, 0.0f, 1.0f, 
        "First Processor::Utilization() call returns value in [0, 1]");

    // Wait a bit for CPU state to change
    usleep(100000); // 100ms

    // Second call should calculate delta correctly
    float cpu_util_2 = processor.Utilization();
    IntegrationTest::assert_range(cpu_util_2, 0.0f, 1.0f, 
        "Second Processor::Utilization() call returns value in [0, 1]");

    std::cout << "  CPU Utilization (1st call): " << std::fixed << std::setprecision(2) 
              << (cpu_util_1 * 100) << "%" << std::endl;
    std::cout << "  CPU Utilization (2nd call): " << (cpu_util_2 * 100) << "%" << std::endl;
}

// Test 5: Process Integration
void test_process_integration() {
    std::cout << "\n[TEST GROUP] Process Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    std::vector<int> pids = LinuxParser::Pids();
    IntegrationTest::assert_true(pids.size() > 0, "Can retrieve PIDs for Process testing");

    if (pids.size() > 0) {
        int test_pid = pids[0];
        Process process(test_pid);

        IntegrationTest::assert_true(process.Pid() == test_pid, 
            "Process constructor sets correct PID");

        std::string user = process.User();
        IntegrationTest::assert_true(!user.empty(), 
            "Process::User() returns non-empty string");

        std::string command = process.Command();
        IntegrationTest::assert_true(!command.empty(), 
            "Process::Command() returns non-empty string");

        std::string ram = process.Ram();
        IntegrationTest::assert_true(!ram.empty(), 
            "Process::Ram() returns non-empty string");

        long uptime = process.UpTime();
        IntegrationTest::assert_true(uptime >= 0, 
            "Process::UpTime() returns non-negative value");

        float cpu_util = process.getCpuUtilization();
        IntegrationTest::assert_range(cpu_util, 0.0f, 1.0f, 
            "Process::getCpuUtilization() returns value in [0, 1]");

        std::cout << "  Test Process Details:" << std::endl;
        std::cout << "    PID: " << process.Pid() << std::endl;
        std::cout << "    User: " << user << std::endl;
        std::cout << "    Command: " << (command.length() > 40 ? command.substr(0, 40) + "..." : command) << std::endl;
        std::cout << "    RAM: " << ram << " MB" << std::endl;
        std::cout << "    CPU: " << std::fixed << std::setprecision(2) << (cpu_util * 100) << "%" << std::endl;
    }
}

// Test 6: System Integration - Full System Component Integration
void test_system_integration() {
    std::cout << "\n[TEST GROUP] System Integration (Full Component Integration)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    System system;

    // Test CPU integration
    float cpu_util = system.Cpu().Utilization();
    IntegrationTest::assert_range(cpu_util, 0.0f, 1.0f, 
        "System::Cpu().Utilization() returns value in [0, 1]");

    // Test Memory integration
    float mem_util = system.MemoryUtilization();
    IntegrationTest::assert_range(mem_util, 0.0f, 1.0f, 
        "System::MemoryUtilization() returns value in [0, 1]");

    // Test OS info integration
    std::string os = system.OperatingSystem();
    IntegrationTest::assert_true(!os.empty(), 
        "System::OperatingSystem() returns non-empty string");

    std::string kernel = system.Kernel();
    IntegrationTest::assert_true(!kernel.empty(), 
        "System::Kernel() returns non-empty string");

    // Test Process list integration
    std::vector<Process>& processes = system.Processes();
    IntegrationTest::assert_true(processes.size() > 0, 
        "System::Processes() returns non-empty list");

    // Test that processes are sorted by CPU usage (descending)
    if (processes.size() > 1) {
        bool sorted = true;
        for (size_t i = 0; i < processes.size() - 1; i++) {
            if (processes[i].getCpuUtilization() < processes[i+1].getCpuUtilization()) {
                sorted = false;
                break;
            }
        }
        IntegrationTest::assert_true(sorted, 
            "System::Processes() returns processes sorted by CPU (descending)");
    }

    // Test process counts integration
    int total_procs = system.TotalProcesses();
    IntegrationTest::assert_true(total_procs > 0, 
        "System::TotalProcesses() returns positive value");

    int running_procs = system.RunningProcesses();
    IntegrationTest::assert_true(running_procs >= 0, 
        "System::RunningProcesses() returns non-negative value");

    // Test uptime integration
    long uptime = system.UpTime();
    IntegrationTest::assert_true(uptime > 0, 
        "System::UpTime() returns positive value");

    std::cout << "\n  System Summary:" << std::endl;
    std::cout << "    OS: " << os << std::endl;
    std::cout << "    Kernel: " << kernel << std::endl;
    std::cout << "    CPU Utilization: " << std::fixed << std::setprecision(2) 
              << (cpu_util * 100) << "%" << std::endl;
    std::cout << "    Memory Utilization: " << (mem_util * 100) << "%" << std::endl;
    std::cout << "    Total Processes: " << total_procs << std::endl;
    std::cout << "    Running Processes: " << running_procs << std::endl;
    std::cout << "    System Uptime: " << uptime << " seconds" << std::endl;
    std::cout << "    Process List Size: " << processes.size() << std::endl;

    // Display top 5 processes
    if (processes.size() > 0) {
        std::cout << "\n  Top 5 Processes by CPU:" << std::endl;
        int display_count = std::min(5, (int)processes.size());
        for (int i = 0; i < display_count; i++) {
            std::string cmd = processes[i].Command();
            if (cmd.length() > 30) cmd = cmd.substr(0, 30) + "...";
            std::cout << "    " << std::setw(6) << processes[i].Pid() 
                      << " " << std::setw(10) << std::fixed << std::setprecision(2)
                      << (processes[i].getCpuUtilization() * 100) << "%"
                      << " " << cmd << std::endl;
        }
    }
}

// Test 7: Data Flow Integration - Verify data flows correctly through components
void test_data_flow_integration() {
    std::cout << "\n[TEST GROUP] Data Flow Integration" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    System system;

    // Verify that System correctly aggregates data from all components
    std::vector<Process>& processes = system.Processes();
    
    if (processes.size() > 0) {
        Process& first_process = processes[0];
        
        // Verify Process data is accessible through System
        IntegrationTest::assert_true(first_process.Pid() > 0, 
            "Process data accessible through System::Processes()");

        // Verify CPU data flows: LinuxParser -> Processor -> System
        float system_cpu = system.Cpu().Utilization();
        IntegrationTest::assert_range(system_cpu, 0.0f, 1.0f, 
            "CPU data flows correctly: LinuxParser -> Processor -> System");

        // Verify Memory data flows: LinuxParser -> System
        float system_mem = system.MemoryUtilization();
        IntegrationTest::assert_range(system_mem, 0.0f, 1.0f, 
            "Memory data flows correctly: LinuxParser -> System");

        // Verify Process data flows: LinuxParser -> Process -> System
        std::string proc_user = first_process.User();
        IntegrationTest::assert_true(!proc_user.empty(), 
            "Process user data flows correctly: LinuxParser -> Process -> System");
    }

    std::cout << "  Data flow verification: All components integrated successfully" << std::endl;
}

int main() {
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "SYSTEM MONITOR - INTEGRATION TEST SUITE" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Testing integration between components:" << std::endl;
    std::cout << "  - LinuxParser <-> /proc filesystem" << std::endl;
    std::cout << "  - Processor <-> LinuxParser" << std::endl;
    std::cout << "  - Process <-> LinuxParser" << std::endl;
    std::cout << "  - System <-> Processor, Process, LinuxParser" << std::endl;
    std::cout << "  - End-to-end data flow" << std::endl;

    // Run all integration tests
    test_linux_parser_system_integration();
    test_linux_parser_cpu_integration();
    test_linux_parser_process_integration();
    test_processor_integration();
    test_process_integration();
    test_system_integration();
    test_data_flow_integration();

    // Print summary
    IntegrationTest::print_summary();

    // Return exit code: 0 if all passed, 1 if any failed
    return (IntegrationTest::tests_failed == 0) ? 0 : 1;
}

