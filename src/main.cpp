/**
 * Reconnaissance de clignotement.
 *
 * Copyright (c) 2006-2015, Brain Vision Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Brain Vision Systems nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE  * OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "main.h"

using namespace std;

void onCmdReceptionEvent(InputStream *istream, unsigned char *buf, int blen)
{
    if(istream->type & STREAM_BVS)
    {
        int verbosity = app.verbosity;

        // Get the type of BVS message
        int pid = getPID(buf);
        // Process Message
        switch (pid)
        {
            
            case BVSMSG_BIPS_SYNCHRONISATION:
                verbosity = 0;
                break;
            
            case BVSMSG_TSK_GENTRACK_RES:
                app.board->pb.decodeGenericTaskResult(&gt_res, buf);
                cout << "GT:" << gt_res.tid << " (" << gt_res.numframe 
                     << ") -> " << gt_res.track_region.getCenterX() << "; " 
                     << gt_res.track_region.getCenterY()<< "   "
                     << gt_res.numpix
                     << endl;
                     
                verbosity = 0;
                break;

            case BVSMSG_TSK_GENTRACK_CCONF:
                verbosity = 0;
                break;

            case BVSMSG_TSK_GENTRACK_TCONF:
                verbosity = 0;
                break;



            case BVSMSG_TSK_COLOR_CONF:
                verbosity = 0;
                break;
            
            case BVSMSG_TSK_COLOR_RES:
                app.board->pb.decodeColorTaskResult(&ct_res, buf);
                cout << "CT:" << ct_res.tid << " (" << ct_res.numframe << 
                     ") -> " << ct_res.track_region.getCenterX() << "; " << 
                     ct_res.track_region.getCenterY() << endl;
                verbosity = 0;
                break;
            
            case BVSMSG_TSKMG_TG_RES_ALLOC:
                tgconf.receiveAlloc(buf);
                verbosity = 0;
                break;

            case BVSMSG_LOW_CAP:
                printf("Communication etablie !\n");
                verbosity = 0;
                break;      

            defaut:
                printf("Message non traite\n");
                break;
        }

        if(verbosity)
        {
            printf("< ");
            app.printBVSMSG(buf,blen);
        }
    }
    else
    {
        printf("no BVS stream < ");
        app.printBVSMSG(buf,blen);
    }
}

void onCmdSendEvent(OutputStream *ostream, unsigned char *buf, int blen)
{
      int verbosity =  app.verbosity; 
      if(verbosity)
      { 
        if (ostream->type & STREAM_BVS)
        {
          printf("> ");
          app.printBVSMSG(buf, blen);
        }
      }
}

// Render a frame with SDL.
void update_screen(SDL_Overlay *video, int w, int h)
{
    SDL_Rect video_rect = {0, 0, w, h}; 

    char text[255]; 

    if (video)
    {
        SDL_UnlockYUVOverlay(video);

        // ** SDL Manipulation here **

        // sprintf is used like printf but the resulting string will be put in the
        // first argument.
        sprintf(text, "exemple %s", (true ? "oui" : "non" ));
        // osd_drawText(SDL_Overlay sur lequel ecrire,
        //              char * à ecrire,
        //              int pos x,
        //              int pos y,
        //              int taille du texte,
        //              int couleur du texte [0-3]
        osd_drawText(video, text, 20, 20, 1, 0);

        SDL_DisplayYUVOverlay(video, &video_rect);
        SDL_LockYUVOverlay(video);
    }      
}


/// This function is called for each frame.
void computeFunc()
{

}

/// Function managing keys bindings
void handleKeys(int k, int m)
{ 
    switch(k)
    {
        case SDLK_s:
          app.video_parser.snapshot = 1;      
    }
}

void initGenericTracker()
{
    tgconf.tasktype = TASK_TYPE_GENERICTRACKER;
    tgconf.maxtask = 1;
    tgconf.conf = TG_CONF_ENABLED | 
                  TG_CONF_SHARE_ROI | 
                  TG_CONF_ALLOC_AUTO | 
                  TG_CONF_AUTOLINK_INHIBIT;

    if ((taskGroupID_GT = tgconf.allocTG(app)) > 0)
    {
        printf("Id of TG GT : %d\n",taskGroupID_GT);
    } 
    else // Error.
    {
        tgconf.state  =  TGSTATE_UNINITIALIZED;
    }  
    

    // Generic Tracker configuration.
    gt_conf.numBlocks = 1;
    gt_conf.xaclass = 1;
    gt_conf.yaclass = 1;
    gt_conf.xdelta = 50;
    gt_conf.ydelta = 50;

    gt_conf.cues[0].track_ca = 0; 
    gt_conf.cues[0].track_cb = 3;
    gt_conf.cues[0].track_flow = FLOW_VAR;

    // OPTIMISATION FOR GENERIC TRACKER.
    gt_conf.cues[0].class_ca = gt_conf.cues[0].track_ca; 
    gt_conf.cues[0].class_cb = gt_conf.cues[0].track_cb;
    gt_conf.cues[0].class_flow = gt_conf.cues[0].track_flow;

    gt_conf.cues[0].track_mode = 0;
    gt_conf.cues[0].track_delta = 0;
    gt_conf.cues[0].track_delay = 0;

    gt_conf.numpix_th = 40;
    gt_conf.confirmTracking_th = 60;

    app.board->taskSetConf(&gt_conf, taskGroupID_GT, 0xFF);  

    app.board->taskGroup_setROI(taskGroupID_GT, 0XFF, 10, 630, 10, 470);
}

int main(int argc, char * argv[])
{

    // --  Step 1: Application Configuration    
    app.name = "Projet_vide";
    // -Callback functions- link user defined function to the system.
    // Called when a new frame is decoded by the video parser 
    // (only if the video is enable).
    app.cb_newframe = update_screen; 
    // Called when a new frame is decoded by the video parser.
    app.cb_compute = computeFunc;
    // Called when a key is pressed.
    app.cb_handlekeys = handleKeys;
    // Called when a message is received.
    app.board->cb_receivePacket = onCmdReceptionEvent; 
    // Called when a message is send.
    app.board->cb_sendPacket = onCmdSendEvent; 
    
    // SDL initialisation.
    app.displayEnable = STDAPP_DISPLAY_SDL;
    app.displayResizable = 0;

    
    // -- Step 2: Application Initialisation
    if (!app.init(argc, argv)) 
        exit(-1); 


    // -- Step 3: Connection to the Bipcam.
    if (! app.connect(BIPCAM_ENABLE_BIPS))
        exit(-1);
    
    // -- Step 4: BIPS Configuration
    
    // Launch the video, mandatory if you want to see what the camera see.    
    app.startVideoCapture();    
    SDL_EnableKeyRepeat(100,80);
    // Select the flow to display.
    app.board->setOutFlow(FLOW_LUM); // FLOW_HUE    
    // Select the video format.
    app.board->setVideoMode(VIDSTD_640x480x50);

    // Commande to send a message :
    app.board->sendCmd(BVSMSG_LOW_GET_CAP,0,0);

    app.verbosity = 3;

    
    // -- Tracking allocation --
    // Here you can select between to types of task.
    
    // Generic Tracker allocation.
    initGenericTracker();

     
    // -- Step 5: Start Application Main Loop
    app.mainLoop();

    
    

    // -- Step 6: exit
    
    // Close the video
    app.stopVideoCapture();
    
    printf("Exiting...\n");
    return 0;
}
