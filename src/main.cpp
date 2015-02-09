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

// Dans SerialCommunication
#define ID_MASTER 102 

// Lit
Test_SerialCommunication serial("/dev/ttyAMA0");
string reception_message = "";

//bug
#define NB_ITERATION 20
#define DELAY_UART 100
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
                frame_sync = app.board->pb.decodeBIPSSynchronisation(buf);
                break;
            
            case BVSMSG_TSK_GENTRACK_RES:
                if(state == WAIT_BLINK)
                {
                    app.board->pb.decodeGenericTaskResult(&gt_res, buf);
                    frame_blink = gt_res.numframe;
                    /*
                    cout << "GT:" << gt_res.tid << " (" << gt_res.numframe 
                         << ") -> " << gt_res.track_region.getCenterX() << "; " 
                         << gt_res.track_region.getCenterY()<< "   "
                         << gt_res.numpix
                         << endl;
                    */
                }     
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

int average_vector(vector<int> vect)
{
    int sum = 0;
    for(int cpt = 0 ; cpt < vect.size() ; cpt ++)
    {
        sum += vect[cpt];
    }
    return (sum / vect.size());
}

vector<int> average_coords()
{
    coords_final.push_back(average_vector(Xmins));
    coords_final.push_back(average_vector(Xmaxs));
    coords_final.push_back(average_vector(Ymins));
    coords_final.push_back(average_vector(Ymaxs));
    
    return coords_final;
}

void appairage_ok(int id, vector<int> coords)
{
    cout << "Appairage ok" << endl;
    cout << "coords_final :" << endl;
    for(int cpt = 0 ; cpt < coords_final.size() ; cpt ++)
    {
        cout << coords_final[cpt] << " ";
    }
    cout << endl;
    cout << endl;
    // TODO : message appairage OK
    // Pas beau !! Faire proprement
    string strf = "12345";
    strf[0] = id_slave;
    strf[1] = ID_MASTER;
    strf[2] = 11;
    strf[3] = 255;
    strf[4] = 0;
    serial.writeWithDelay(strf, 40);
    for(int cpt = 0 ; cpt < NB_ITERATION ; cpt ++)
    {
        strf[4] = cpt;
        serial.writeWithDelay(strf, DELAY_UART);
        cout << (int)strf[0] << " " 
                         << (int)strf[1] << " "
                         << (int)strf[2] << " " 
                         << (int)strf[3] << " "
                         << (int)strf[4]
                         << endl;
    }
    
    // TODO :  Ajouter BdD
    // Update si existe deja.
    
    return_value = 0;
    app.mainLoopDone = 1;   
}
void appairage_non_ok(int id)
{
    cout << "Appairage PAS ok" << endl;
    cout << endl;
    cout << endl;
    // TODO : message appairage NON OK
    // Pas beau !! Faire proprement
    string strf = "12345";
    strf[0] = id_slave;
    strf[1] = ID_MASTER;
    strf[2] = 75;
    strf[3] = 255;
    strf[4] = 0;
    serial.writeWithDelay(strf, DELAY_UART);
    for(int cpt = 0 ; cpt < NB_ITERATION ; cpt ++)
    {
        strf[4] = cpt;
        serial.writeWithDelay(strf, 50);
        cout << (int)strf[0] << " " 
                         << (int)strf[1] << " "
                         << (int)strf[2] << " " 
                         << (int)strf[3] << " "
                         << (int)strf[4]
                         << endl;
    }
    
    return_value = 1;
    app.mainLoopDone = 1;
}

/// This function is called for each frame.
void computeFunc()
{   
    if(state == WAIT_MESSAGE)
    {
        // TODO
        // ATTENTION
        // Voir ce qui se passe au niveau du message de sortie UART
        // quand il y a un message et quand il n'y en a pas.
        // faire un do while
        
        
        serial.readMessage(reception_message); 
        long now = SDL_GetTicks();
        while ( (SDL_GetTicks() - now) < 40)
        {
            app.loopFunc();
        }       
        //reception_message = "12345";
        if(reception_message.size() > 0)
        {
            cout << "(" << reception_message.size() << ") "
                 << "\"" << reception_message << "\"" 
                 << (int)reception_message[0] << " " << (int)reception_message[1] << " "
                 << (int)reception_message[2] << " " << (int)reception_message[3] << " "
                 << (int)reception_message[4]
                 << endl;
        }
        while(reception_message.size() == TEST_NUMBER_BYTES) 
        {   
            cout << (int)reception_message[1] << " " << id_slave << endl;
            if((int)reception_message[1] == id_slave)
            {
                cout << "Message de l'esclave a appairer" << endl;
                
                // TODO : remplacer la valeur par variable/define/... 
                // voir Lucibel_SerialCommunication
                // DEMAND_PAIRING = 0x0A = 10
                if((int)reception_message[2] != 10)  
                {
                    cout << "La lampe est deja appairee" << endl;
                    return_value = 2;
                    app.mainLoopDone = 1;
                    return;
                }
                
                cout << "Debut de la manipulation (" << frame_sync << ")" << endl;
                // TODO : envoie du message !!
                // Pas beau !! Faire proprement
                string strf = "12345";
                strf[0] = id_slave;
                strf[1] = ID_MASTER;
                strf[2] = 26;
                strf[3] = 255;
                strf[4] = 255;
                serial.writeWithDelay(strf, DELAY_UART);
                cout << "(" << strf.size() << ") "
                     << "\"" << strf << "\"" 
                     << (int)strf[0] << " " 
                     << (int)strf[1] << " "
                     << (int)strf[2] << " " 
                     << (int)strf[3] << " "
                     << (int)strf[4]
                     << endl;
                for(int cpt = 0 ; cpt < NB_ITERATION ; cpt ++)
                {
                    strf[4] = cpt;
                    serial.writeWithDelay(strf, DELAY_UART);
                    cout << (int)strf[0] << " " 
                         << (int)strf[1] << " "
                         << (int)strf[2] << " " 
                         << (int)strf[3] << " "
                         << (int)strf[4]
                         << endl;
                }
                
                frame_manip_first = frame_sync;
                state = WAIT_BLINK;
            }  
            
            serial.readMessage(reception_message);
            now = SDL_GetTicks();
            while ( (SDL_GetTicks() - now) < 40)
            {
                app.loopFunc();
            }
            
            if(reception_message.size() > 0)
            {
            cout << "(" << reception_message.size() << ") "
                 << "\"" << reception_message << "\"" 
                 << (int)reception_message[0] << " " << (int)reception_message[1] << " "
                 << (int)reception_message[2] << " " << (int)reception_message[3] << " "
                 << (int)reception_message[4]
                 << endl;
            }
        }     
    }
    else if (state == WAIT_BLINK)
    {        
        if(frame_blink == frame_sync)
        {
            if(frame_blink_first == -1)
            {
                cout << "debut de clignotement repere" 
                     << "(" << frame_sync << ")" << endl;
                 
                frame_blink_first = frame_sync;                
            }
            else if((frame_blink_first + frame_blink_max) < frame_sync)
            {
                state = WAIT_NOTHING;
                cout << "temps de clignotement atteind." 
                     << "(" << frame_sync << ")" << endl;
                appairage_ok(id_slave, average_coords());
                return;
            }
            
            // Add tracking resultats
            Xmins.push_back(gt_res.track_region.x0);
            Xmaxs.push_back(gt_res.track_region.x1);
            Ymins.push_back(gt_res.track_region.y0);
            Ymaxs.push_back(gt_res.track_region.y1);
        }
        else // No movement
        {
            if(frame_blink_first != 0)
            {
                // Reset everything
                Xmins.clear();
                Xmaxs.clear();
                Ymins.clear();
                Ymaxs.clear();
                frame_blink_first = -1;                
            }
        }
        
        
        if((frame_manip_first + frame_manip_max) < frame_sync)
        {
            cout << "le temps d'appairage est fini sans resultat" 
                 << "(" << frame_sync << ")" << endl;
            
            state = WAIT_NOTHING;
            appairage_non_ok(id_slave);
            return;
        }        
    }
}

/// Function managing keys bindings
void handleKeys(int k, int m)
{ 
    switch(k)
    {
        case SDLK_s:
            app.video_parser.snapshot = 1;
            break;
        
        case SDLK_r:
            cout << reception_message << endl;
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
        exit(-1);
    }  
    

    // Generic Tracker configuration.
    gt_conf.numBlocks = 1;
    gt_conf.xaclass = 1;
    gt_conf.yaclass = 1;
    gt_conf.xdelta = 5;
    gt_conf.ydelta = 5;

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

    // Recuperation de l'id de la lampe.
    cout << "arguments : (" << argc << ")" << endl;
    for(int cpt = 0 ; cpt < argc ; cpt ++)
    {
        cout << argv[cpt] << endl;
    }
    
    if(argc != 2)
    {
        cout << "Pour le moment il faut 1 argument : l'id de la lampe a " 
             << "appairer." << endl;
            
        exit(-1);
    }
    id_slave = atoi(argv[1]);
    cout << "ID : " << id_slave << endl;
        
    // -- Step 2: Application Initialisation
    if (!app.init(argc, argv)) 
        exit(-1); 
           
    /*
    long now = 0;
    while(1)
    {
        serial.readMessage(reception_message);
        cout << "(" << reception_message.size() << ") "
             << "\"" << reception_message << "\"" 
             << (int)reception_message[0] << " " << (int)reception_message[1]
             << (int)reception_message[2] << " " << (int)reception_message[3]
             << (int)reception_message[4]
             << endl;
        
        now = SDL_GetTicks();
        while ( (SDL_GetTicks() - now) < 400)
        {
            app.loopFunc();
        }
    }
    
    
    string strf = "12345";
    strf[0] = id_slave;
    strf[1] = ID_MASTER;
    strf[2] = 26;
    strf[3] = 0;
    strf[4] = 0;
    //serial.writeWithDelay(strf, 40);
    cout << "(" << strf.size() << ") "
         << "\"" << strf << "\"" 
         << (int)strf[0] << " " 
         << (int)strf[1] << " "
         << (int)strf[2] << " " 
         << (int)strf[3] << " "
         << (int)strf[4]
         << endl;
         
    for(int cpt = 0 ; cpt < 10 ; cpt ++)
    {
        strf[4] = cpt;
        serial.writeWithDelay(strf, 40);
    }
    //exit(-1);
    
    */
    
    // -- Step 3: Connection to the Bipcam.
    if (! app.connect(BIPCAM_ENABLE_BIPS))
        exit(-1);
    
    // -- Step 4: BIPS Configuration
    
    
    // Launch the video, mandatory if you want to see what the camera see.    
    //app.startVideoCapture();    
    SDL_EnableKeyRepeat(100,80);
    // Select the flow to display.
    app.board->setOutFlow(FLOW_LUM); // FLOW_HUE    
    // Select the video format.
    //app.board->setVideoMode(VIDSTD_640x480x50);

    // Commande to send a message :
    app.board->sendCmd(BVSMSG_LOW_GET_CAP,0,0);

    app.verbosity = 3;

    app.board->setVarFlowSensitivity(40);
    
    // -- Tracking allocation --
    // Here you can select between to types of task.
    
    // Generic Tracker allocation.
    initGenericTracker();

    // Pour les tests.
    //state = WAIT_BLINK;
        
     
    // -- Step 5: Start Application Main Loop
    app.mainLoop();

    
    

    // -- Step 6: exit
    
    // Close the video
    //app.stopVideoCapture();
    
    printf("Exiting...\n");
    return return_value;
}
