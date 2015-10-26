//
//  MyThread.h
//  MusicAndFractalsSynched
//
//  Created by Edward Zhao on 10/25/15.
//
//


#include "ofApp.h"

#ifndef MusicAndFractalsSynched_MyThread_h
#define MusicAndFractalsSynched_MyThread_h


class MyThread: public ofThread
{
public:
    // the thread function
    void threadedFunction()
    {
        
        // start
        
        while(isThreadRunning())
        {
            
            //display image 
            if(bReadyToProcess == true)
            {
                // lock access to the resource
                lock();
                
                // Access Resource Here
                
                
                // done with the resource
                unlock();

                bReadyToProcess = false;
            }
            
            //Sleeps the proceess while not displaying fractal
            else
            {
                // sleep the thread to give up some cpu
                sleep(2048/44100.0/1000.0);
            }
        }
        // while(isThreadRunning())
            
    }
    // done
    
    ofImage image;
    bool bReadyToProcess;
};

#endif
