#pragma once

#include "ofMain.h"
#include "MyThread.h"
#include <stack>

#define CC 512
#define fftSize 2048

typedef struct rgbStruct
{
    int R;
    int G;
    int B;
    
} RGB;

class ofApp : public ofBaseApp
{
public:
		void setup();
		void update();
		void draw();
        void mousePressed(int x, int y, int button);
        struct rgbStruct hslTorgb(double h, double sl, double l);
        void resetColormap();
        double myMin(double x, double y, int n);
        double myMax(double x, double y, int n);
        void exit();
    
private:
        unsigned long c1;
        unsigned long c2;
        ofImage img;
        int colormap[CC][3];
        double MyHSL[CC][3];
        int w;
        int h;
        double beginH;
        double endH;
        double beginV;
        double endV;
        int max_step;
        double bound;
        int step;
        double width, height, Zre, Zim, Cre, Cim, Zre_tmp, Zim_tmp;
        float * fftVals;
        ofSoundPlayer sonata;
};


//class MyThreaded: public ofThread
//{
//public:
//    /// ...
//    void threadedFunction()
//    {
//        while(isThreadRunning())
//        {
//            /// Threaded function here.
//            
//        }
//    }
//};



MyThread thread;