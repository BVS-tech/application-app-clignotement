/**
 * Task Group structure for BIPCAM Demo
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

#ifndef TGCONFIG_H
#define TGCONFIG_H

#define TGSTATE_UNINITIALIZED 0
#define TGSTATE_INITIALIZED 1
#define TGSTATE_FLOW_SET 2
#define TGSTATE_STEP_INITIALIZED 4

#include <bvs_tasks.h>
#include <std_types.h>
#include <bvs_simpleapp.h>

/// Class helping for task group allocation
/**
 *
 * app is a BVSSimpleApp referencing the bipcam you want to allocate the 
 * task group in. 
 *
 * How to use :
 *
 * In the app.board->cb_receivePacket callback function wait for the
 * BVSMSG_TSKMG_TG_RES_ALLOC message with TGConfig::receiveAlloc(buf);
 *
 * set the tasktype, the maxtask and the conf according of your task group
 * configuration.
 * See "BIPcam_API.pdf" 2.3 Allocating a task Group for more details.
 * 
 * When you want to allocate a new task Group just call TGConfig::allocTG(app)
 */
class TGConfig
{
    public:
        /// Task type
        u08 tasktype;
        /// Max number of tasks managed by taskgroup
        u08 maxtask;        
        /// Taskgroup configuration
        /**
         * Possible configuration :
         * TG_CONF_ENABLED: Set this bit to enable the task group.
         * TG_CONF_SHARE_ROI: Tasks share the same ROI. This is useful for 
         * multi tasks in automatic allocation mode. 
         * TG_CONF_SHARE_TASK0_CONF: Tasks share the same parameters.
         * TG_CONF_ALLOC_NONE: Tasks are not allocated. User has to do it 
         * manually.
         * TG_CONF_ALLOC_AUTO: In auto allocation mode, task allocation is 
         * automatically managed by the task manager,depending on the scene.
         * TG_CONF_ALLOC_ALL: In this configuration, all tasks are immediately 
         * allocated.
         * TG_CONF_AUTOLINK_INHIBIT: Connect tasks inside the task group in 
         * order to track different objects.
         */
        u16 conf;
        /// Initialization state of TGconfig
        int state;

        TGConfig();

        /// Allocation of a TG.
        /** If return value is < 0 there is an error (p24).*/
        int allocTG(BVSSimpleApp& app, int timeout = 1000);
        /// This Function has to be called when BVSMSG_TSKMG_TG_RES_ALLOC is received
        void receiveAlloc(unsigned char *buf);

    private :
        /// Function waiting for the allocation.
        int waitAllocAnswer(BVSSimpleApp& app, int timeout);
        int lastAllocatedTGID;
        bool answerFlag;
};

#endif
