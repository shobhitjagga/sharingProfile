#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

uint64_t BlockOffset = 6;

class AccessDistance
{
    uint64_t num_accesses;
    uint64_t numReusedBlocks;
    map <uint64_t, uint64_t> lastAccess;
    map <uint64_t, uint64_t> cdf;

    public:
    AccessDistance()
    {
        num_accesses = 0;
        numReusedBlocks = 0;
    }

    void updateAccessDist(uint64_t addr)
    {
        num_accesses++;
        uint64_t blockAddress = (addr >> BlockOffset);
        if(lastAccess.find(blockAddress) == lastAccess.end())
        {
            lastAccess.insert(pair<uint64_t, uint64_t> (blockAddress, num_accesses));
        }
        else
        {
            numReusedBlocks++;
            uint64_t accessDistanceVal = num_accesses - lastAccess[blockAddress];
            if(cdf.find(accessDistanceVal) == cdf.end())
            {
                cdf.insert(pair<uint64_t, uint64_t> (accessDistanceVal, 1));
            }
            else
            {
                cdf[accessDistanceVal] += 1;
            }
            lastAccess[blockAddress] = num_accesses;
        }
    }

    void computeCDF(ofstream &OutputWriter)
    {
        uint64_t sum = 0;
        long double cdfVal;
        for(auto itr = cdf.begin(); itr != cdf.end(); itr++)
        {
            sum += itr->second;
            cdfVal = (long double)sum/(long double)numReusedBlocks;
            OutputWriter << itr->first << ", " << cdfVal << endl;
        }
    }

} accessDistance;


int main(int argc, char* argv[])
{
    string TraceFileName = argv[1];
    bool isCacheSim = false;

    if (argc > 2 && atoi(argv[2])){
        isCacheSim = true;
    }
    if (isCacheSim){
        TraceFileName.append("_CacheMiss.trace");
    }
    else {
        TraceFileName.append("_MachineAccess.trace");
    }

    ifstream traceFile(TraceFileName);
    string OutFile(argv[1]);
    ofstream CDFfile;
    if (isCacheSim){
        CDFfile.open(OutFile + "_CacheSimAccDist.cdf", ofstream::out | ofstream::trunc);
    }
    else {
        CDFfile.open(OutFile + "_AccDist.cdf", ofstream::out | ofstream::trunc);
    }

    string line;
    while (getline(traceFile, line)) {
        uint64_t addr = stoull(line.substr(line.find("0x"), string::npos), NULL, 16);
        accessDistance.updateAccessDist(addr);
    }
    accessDistance.computeCDF(CDFfile);

    return 0;
}