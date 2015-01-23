/**
 *
 * Copyright (c) 2006-2010, Brain Vision Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the distribution.
 * - Neither the name of Brain Vision Systems nor the names of its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "TGGroup.h"

TGConfig::TGConfig()
{
      tasktype  = 0;
      maxtask = 0;
      state = TGSTATE_UNINITIALIZED;
       
      lastAllocatedTGID = -1;
      answerFlag = false;
}
 


// Allocation of a TG.
// If return value is < 0 there is an error (p24).
int TGConfig::allocTG(BVSSimpleApp& app, int timeout)
{
    int res = -1;

    if ((tasktype >= 0) && (maxtask > 0))
    {
        app.board->taskManager_allocTG(tasktype, maxtask, conf);
        res = waitAllocAnswer(app, timeout);
    }
    
    return res;
}

void TGConfig::receiveAlloc(unsigned char *buf)
{
    answerFlag = true;
    
    if (buf[8]!=0)
    {
        fprintf(stderr, "Allocation Error: %d\n", buf[8]);
        lastAllocatedTGID = -buf[8];
    }
    else
    {
        lastAllocatedTGID = buf[9];
    }
}


// ----- Private -----

// Function waiting for the allocation.
int TGConfig::waitAllocAnswer(BVSSimpleApp& app, int timeout)
{
    int res = -1;
    answerFlag = false;
    long now = SDL_GetTicks();

    if (app.verbosity>0)
        printf("Allocating Task Group: Waiting for tgid...\n");

    while (((SDL_GetTicks() - now) < timeout) && (answerFlag==false))
    {
        app.loopFunc();
    }

    if (answerFlag == true)
        res = lastAllocatedTGID;

    if (app.verbosity>0)
        printf("task Group ID : %d\n", res);


    return res;
}
