#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

uint32_t BlockOffset = 6;

class AccessDistance
{
    int N, num_accesses;
    bool AccDistPerBlock;
    unordered_map <uint64_t, vector<int>> accessDist;
    unordered_map <uint64_t, int> lastAccess;
    map <int, float> cdf;
    vector<int> dValues;
    vector<float> fValues;

    public:
    AccessDistance()
    {
        N = 0, num_accesses = 0;
        AccDistPerBlock = false;
    }

    void updateAccessDist(uint64_t addr)
    {
        uint64_t blockAddress = (addr >> BlockOffset);
        if(lastAccess.find(blockAddress) == lastAccess.end())
        {
            lastAccess.insert(pair<uint64_t, int> (blockAddress, num_accesses));
        }
        else
        {
            int accessDistanceVal = num_accesses - lastAccess[blockAddress];
            if(cdf.find(accessDistanceVal) == cdf.end())
            {
                cdf.insert(pair<int, float> (accessDistanceVal, 1));
            }
            else
            {
                cdf[accessDistanceVal] += 1;
            }
            if (AccDistPerBlock)
            {
                if(accessDist.find(blockAddress) == accessDist.end())
                {
                    accessDist.insert(pair<uint64_t, vector<int>> (blockAddress, {accessDistanceVal}));
                }
                else
                {
                    accessDist[blockAddress].push_back(accessDistanceVal);
                }
            }
            lastAccess[blockAddress] = num_accesses;
            N++;
        }
        num_accesses++;
    }

    void updateCDF()
    {
        float prev = 0;
        map<int, float>::iterator itr;
        for(itr = cdf.begin(); itr != cdf.end(); itr++)
        {
            itr->second = (prev*N + itr->second)/N;
            prev = itr->second;
            dValues.push_back(itr->first);
            fValues.push_back(itr->second);
        }
    }

    void printdValues()
    {
        for(auto i = dValues.begin(); i != dValues.end(); i++)
        {
            cout << *i << " ";
        }
        cout << endl;
    }
    void printfValues()
    {
        for(auto i = fValues.begin(); i != fValues.end(); i++)
        {
            cout << *i << " ";
        }
        cout << endl;
    }

    void printCDFvalues(ofstream &OutputWriter)
    {
        for (unsigned long i = 0; i < fValues.size(); i++){
            OutputWriter << dValues[i] << ", " << fValues[i] << endl;
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
    accessDistance.updateCDF();
    accessDistance.printCDFvalues(CDFfile);
}