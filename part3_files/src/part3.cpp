#include "functional_cache.h"
#include "knobs.h"
#include <fstream>

using namespace std;

int accesses, hits;
Functional_Cache *Cache;
ofstream OutputWriter;

void CacheInit(){
    accesses = 0;
    hits = 0;
    Cache = new Functional_Cache(NUM_SETS, NUM_WAYS);
}

void Simulate(uint64_t addr){
    if (Cache->Lookup(addr, LOOKUP_AND_UPDATE)){
        hits++;
    }
    else {
        OutputWriter << "0x" << hex << addr << endl;
        uint32_t L2_vic_way = Cache->Find_Victim(addr);
        Cache->Invalidate(addr, L2_vic_way);
        Cache->Fill(addr, L2_vic_way);
    }
    accesses++;
}

void PrintStats(){
    cout << right << setw(15) << "Cache simulation stats" << endl;
    cout << left;
    cout << setw(30) << "Lookups: " << accesses << endl;
    cout << setw(30) << "Hits: " << hits << endl;
    cout << setw(30) << "Misses: " << accesses - hits << endl;
}

int main(int argc, char *argv[]){
    string TraceFileName = argv[1];
    OutputWriter.open(TraceFileName + "_CacheMiss.trace", ofstream::out | ofstream::trunc);
    TraceFileName.append("_MachineAccess.trace");
    ifstream traceFile(TraceFileName);
    string line;

    CacheInit();

    while (getline(traceFile, line)) {
        uint64_t addr = stoull(line.substr(line.find("0x"), string::npos), NULL, 16);
        Simulate(addr);
    }
    PrintStats();

    return 0;
}