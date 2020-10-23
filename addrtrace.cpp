/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2018 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include <stdio.h>
#include "pin.H"

UINT32 BlockSize = (1 << 6);
FILE * trace;
PIN_LOCK pinLock;

// This function is called before every instruction is executed
// and prints the IP
VOID RecordMemAccess(VOID *ip, VOID *addr, THREADID tid, UINT32 memSize)
{ 
    PIN_GetLock(&pinLock, tid+1);

    UINT32 memSizeLeft = memSize;
    UINT32 rem = BlockSize - (UINT64)addr % BlockSize;
    UINT64 currAddr = (UINT64)addr;
    UINT32 allocBlock, blocksSize8, blocksSize4, blocksSize2, blocksSize1;
    while(memSizeLeft > 0)
    {
        allocBlock = (rem < memSizeLeft)?rem:memSizeLeft;
        blocksSize8 = allocBlock/8;
        blocksSize4 = (allocBlock-blocksSize8*8)/4;
        blocksSize2 = (allocBlock-blocksSize8*8-blocksSize4*4)/2;
        blocksSize1 = (allocBlock-blocksSize8*8-blocksSize4*4-blocksSize2*2);
        while(blocksSize8){
            fprintf(trace, "%d %p\n", tid, (void*)currAddr);
            currAddr += 8; blocksSize8--;
        }
        while(blocksSize4){
            fprintf(trace, "%d %p\n", tid, (void*)currAddr);
            currAddr += 4; blocksSize4--;
        }
        while(blocksSize2){
            fprintf(trace, "%d %p\n", tid, (void*)currAddr);
            currAddr += 2; blocksSize2--;
        }
        while(blocksSize1){
            fprintf(trace, "%d %p\n", tid, (void*)currAddr);
            currAddr += 1; blocksSize1--;
        }
        memSizeLeft = memSizeLeft - allocBlock;
        rem = BlockSize;
    }

    fflush(trace);
    PIN_ReleaseLock(&pinLock);
}

// This routine is executed every time a thread is created.
VOID ThreadStart(THREADID tid, CONTEXT *ctxt, INT32 flags, VOID *v)
{
    PIN_GetLock(&pinLock, tid+1);
    fprintf(stdout, "thread begin %d\n",tid);
    fflush(stdout);
    PIN_ReleaseLock(&pinLock);
}


// This routine is executed every time a thread is destroyed.
VOID ThreadFini(THREADID tid, const CONTEXT *ctxt, INT32 code, VOID *v)
{
    PIN_GetLock(&pinLock, tid+1);
    fprintf(stdout, "thread end %d code %d\n",tid, code);
    fflush(stdout);
    PIN_ReleaseLock(&pinLock);
}

// Pin calls this function every time a new instruction is encountered
VOID Instruction(INS ins, VOID *v)
{
    // Insert a call to printip before every instruction, and pass it the IP
    // INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)printip, IARG_INST_PTR, IARG_THREAD_ID, IARG_END);
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        if (INS_MemoryOperandIsRead(ins, memOp))
        {
            UINT32 memOpSize = INS_MemoryOperandSize(ins, memOp);
            // std::cout << memOpSize << ":" << (memOpSize%64) << std::endl;
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemAccess,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_THREAD_ID,
                IARG_UINT32, memOpSize,
                IARG_END);
        }

        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
            UINT32 memOpSize = INS_MemoryOperandSize(ins, memOp);
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordMemAccess,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_THREAD_ID,
                IARG_UINT32, memOpSize,
                IARG_END);
        }
    }

}

// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // fprintf(trace, "#eof\n");
    fclose(trace);
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    PIN_ERROR("This Pintool prints the IPs of every instruction executed\n" 
              + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    trace = fopen("addrtrace.out", "w");
    
    PIN_InitLock(&pinLock);

    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    PIN_AddThreadStartFunction(ThreadStart, 0);
    PIN_AddThreadFiniFunction(ThreadFini, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}