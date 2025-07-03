#ifndef TLB_H
#define TLB_H

#include <cstdint>
#include <unordered_map>
#include <list>
#include <iostream>
#include <iomanip>
#include <mutex>

class TLB {
private:
    struct TLBEntry {
        uint32_t virtual_page;
        uint32_t physical_page;
        bool dirty;
    };

    static const size_t TLB_SIZE = 64;
    std::list<TLBEntry> lru_list;
    std::unordered_map<uint32_t, std::list<TLBEntry>::iterator> tlb_map;

    uint32_t hits;
    uint32_t misses;
    mutable std::mutex mtx; // Mutex for thread safety

public:
    TLB();

    bool lookup(uint32_t virtual_page, uint32_t& physical_page);
    void add(uint32_t virtual_page, uint32_t physical_page);
    void invalidate(uint32_t virtual_page);
    void invalidateAll();
    void getStats(uint32_t& hit_count, uint32_t& miss_count) const;
    void printStats() const;
};

#endif // TLB_H
