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

        // unordered_map<int, vector<int>>::iterator itr;
        // for(itr = sharingProf.begin(); itr != sharingProf.end(); itr++)
        // {
        //     nMemBlocks[itr->second.size()]++;
        // }
    }
    void printSharingProf()
    {
        for(int i = 0; i < 8; i++)
        {
            cout << nMemBlocks[i] << endl;
        }
        cout << blk << endl;
    }

} sharingProfile;

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
        int tid = (text.at(0)-'0');
        while(getline(traceText, stringAddr, ' ')){} 
        uint64_t addr;
        istringstream stream (stringAddr);
        stream >> hex >> addr;

        sharingProfile.updateSharingProf(tid, addr);
        // cout << addr << endl;
    }
    sharingProfile.updateMemBlocksArray();
    sharingProfile.printSharingProf();
    return 0;
}