#include <stdio.h>
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <Python.h>
// #include "matplotlibcpp.h"
using namespace std;

uint32_t BlockOffset = 6;

class AccessDistance
{
    int N, num_accesses;
    unordered_map <uint64_t, vector<int>> accessDist;
    unordered_map <uint64_t, int> lastAccess;
    map <int, float> cdf;
    vector<int> dValues;
    vector<float> fValues;

    public:
    AccessDistance()
    {
        N = 0, num_accesses = 0;
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
            if(cdf.find((num_accesses-lastAccess[blockAddress])) == cdf.end())
            {
                cdf[(num_accesses-lastAccess[blockAddress])] = 1;
            }
            else
            {
                cdf[(num_accesses-lastAccess[blockAddress])] = cdf[(num_accesses-lastAccess[blockAddress])]+1;
            }

            if(accessDist.find(blockAddress) == accessDist.end())
            {
                accessDist.insert(pair<uint64_t, vector<int>> (blockAddress, {num_accesses-lastAccess[blockAddress]}));
                lastAccess[blockAddress] = num_accesses;
            }
            else
            {
                accessDist[blockAddress].push_back(num_accesses-lastAccess[blockAddress]);
                lastAccess[blockAddress] = num_accesses;
            }
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

} accessDistance;


int main(int argc, char* argv[])
{
    // int tid; uint64_t addr;
    // FILE* fp = fopen(argv[1], "rb");
    // assert(fp != NULL);
    // while (!feof(fp)) 
    // {
    //     fread(&tid, sizeof(int), 1, fp);
    //     cout << tid << endl;
    //     // fread(&addr, sizeof(uint64_t), 1, fp);
    //     // cout  << tid << " : " << addr << endl; 
    // }
    // PyObject* pInt;
    // Py_Initialize();
    // FILE* fp = fopen("qwe.py","r");
    // PyRun_SimpleFile(fp,"qwe.py");
    // Py_Finalize();


    ifstream traceFile(argv[1]);
    string text;
    while (getline (traceFile, text)) {
        stringstream traceText(text); 
        string stringAddr; 
        while(getline(traceText, stringAddr, ' ')){} 
        // cout << stringAddr << endl;
        uint64_t addr;
        istringstream stream (stringAddr);
        stream >> hex >> addr;
        accessDistance.updateAccessDist(addr);
        // cout << addr << endl;
    }
    accessDistance.updateCDF();
    accessDistance.printdValues();
    accessDistance.printfValues();
}