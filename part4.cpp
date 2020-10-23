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

class SharingProfile
{
    int nMemBlocks[8];
    unordered_map <uint64_t, vector<int>> sharingProf;
    int blk;
    public:
    SharingProfile(){blk=0;}

    void updateSharingProf(int tid, uint64_t addr)
    {
        uint64_t blockAddress = (addr >> BlockOffset);
        if(sharingProf.find(blockAddress) == sharingProf.end())
        {
            blk++;
            sharingProf.insert(pair<uint64_t, vector<int>> (blockAddress, {tid}));
        }
        else
        {
            if(find(sharingProf[blockAddress].begin(), sharingProf[blockAddress].end(), tid) == sharingProf[blockAddress].end())
            {
                sharingProf[blockAddress].push_back(tid);
            }
        }
    }
    void updateMemBlocksArray()
    {
        for(auto itr : sharingProf)
        {
            nMemBlocks[itr.second.size()-1]++;
        }
    }
    void printSharingProf()
    {
        for(int i = 0; i < 8; i++)
        {
            cout << "Blocks shared by " << (i+1) << " threads: " << nMemBlocks[i] << endl;
        }
        cout << "Total number of accessed memory blocks: " << blk << endl;
    }

} sharingProfile;

int main(int argc, char* argv[])
{
    ifstream traceFile(argv[1]);
    string text;
    while (getline (traceFile, text)) {
        stringstream traceText(text); 
        string stringAddr; 
        int tid = (text.at(0)-'0');
        while(getline(traceText, stringAddr, ' ')){} 
        uint64_t addr;
        istringstream stream (stringAddr);
        stream >> hex >> addr;

        sharingProfile.updateSharingProf(tid, addr);
    }
    sharingProfile.updateMemBlocksArray();
    sharingProfile.printSharingProf();
    return 0;
}