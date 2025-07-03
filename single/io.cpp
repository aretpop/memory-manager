#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <regex>
#include <cstdint>
#include "task.h" // Task class (from task.cpp)
#include "../common/config.h" // Configuration constants (from config.h)
#include "../common/memory_manager.h"
#include <thread>
#include <vector>

using namespace std;

// Parses size string like "16KB" or "4MB" into bytes
uint32_t parseSize(const string& sizeStr) {
    uint32_t multiplier = 1;
    if (sizeStr.find("KB") != string::npos) {
        multiplier = 1024;
    } else if (sizeStr.find("MB") != string::npos) {
        multiplier = 1024 * 1024;
    } else {
        cerr << "Unknown size format: " << sizeStr << endl;
        return 0;
    }

    uint32_t number = stoi(sizeStr);
    return number * multiplier;
}

// Process a single trace line and store accesses
void processLine(const string& line, map<string, vector<uint32_t>>& accessMap) {
    stringstream ss(line);
    string taskStr, addrStr, sizeStr;

    // Split line by ':' (format: T1: 0x4000:16KB)
    getline(ss, taskStr, ':');
    getline(ss, addrStr, ':');
    getline(ss, sizeStr);

    if (taskStr.empty() || addrStr.empty() || sizeStr.empty()) {
        cerr << "Malformed line: " << line << endl;
        return;
    }

    string task_id = taskStr;
    uint32_t logical_address = stoul(addrStr, nullptr, 16);
    uint32_t size_in_bytes = parseSize(sizeStr);

    uint32_t page_size = MIN_PAGE_SIZE_KB * 1024;
    uint32_t num_pages = (size_in_bytes + page_size - 1) / page_size;

    for (uint32_t i = 0; i < num_pages; ++i) {
        uint32_t page_address = logical_address + i * page_size;
        accessMap[task_id].push_back(page_address);
    }
}

// Reads the entire trace file and processes each line
void readTraceFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    map<string, vector<uint32_t>> accessMap;
    map<string, task*> taskMap;

    while (getline(infile, line)) {
        if (line.empty()) continue;
        processLine(line, accessMap);
    }

    // Create tasks
    for (const auto& pair : accessMap) {
        taskMap[pair.first] = new task(pair.first);
    }

    // Launch threads for each task
    vector<std::thread> threads;
    for (const auto& pair : accessMap) {
        threads.emplace_back([&taskMap, &pair]() {
            for (uint32_t addr : pair.second) {
                taskMap[pair.first]->accessMemory(addr);
            }
        });
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    // Print stats for all tasks after threads complete
    cout << "\n=== Memory Access Summary ===\n";
    for (auto it : taskMap) {
        it.second->printStats();
        delete it.second; // clean up
    }
}

int main(){
    readTraceFile("trace.txt");
}
