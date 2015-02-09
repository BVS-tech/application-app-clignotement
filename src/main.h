/**
 * BIPCAM Autotrack Demo
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
#include <iostream>
#include <vector>

#include <bipcam_app.h>
#include <bvs_tasks.h>
#include <sdlosd.h>
#include <paramSet.h>

#include "TGGroup.h"
#include "Lucibel_SerialCommunication.h"

#define ANGLE_BVS 1024
#define PI 3.14159265


BipcamApp app;
TGConfig tgconf;

int taskGroupID_GT;
GenericTaskConf gt_conf;
GenericTaskResult gt_res;

/// Id of the slave wa want to appair.
int id_slave = -1;

/// State of appairing protocol.
enum State { WAIT_MESSAGE, WAIT_BLINK, WAIT_NOTHING};
State state = WAIT_MESSAGE;

/// Frame when we begin the manipulation.
int frame_manip_first = -1;
/// Maximum number of frame before ending the manipulation.
int frame_manip_max = 1200;
/// Frame when we begin to see the blinking.
int frame_blink_first = -1;
/// Number of frame to accept the blinking.
int frame_blink_max = 120;

/// Current frame.
int frame_sync = 0;
/// Last mouvement frame.
int frame_blink = 0;

/// Return value.
/**
 * 0 = OK
 * Si different de 0 -> pas bon
 * 1 = lampe non vue
 * 2 = lampe deja appairee
 */
int return_value = 0;

std::vector<int> Xmins;
std::vector<int> Xmaxs;
std::vector<int> Ymins;
std::vector<int> Ymaxs;
std::vector<int> coords_final;

