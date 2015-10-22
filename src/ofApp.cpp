#include "ofApp.h"
#include <ctime>

//--------------------------------------------------------------
void ofApp::setup(){
    srand(time(NULL));
    sonata.loadSound("sonata.wav");
    sonata.play();
    w = 1024;
    h = 768;
    Cre = -0.4;
    Cim = 0.6;
    beginH = -1.0;
    endH = 0;
    beginV = 0;
    endV = 0.75;
    width = endH - beginH;
    height = endV - beginV;
    bound = 4;
    max_step = 1000;
    img.allocate(w, h, OF_IMAGE_COLOR);
    img.setColor(ofColor::white);
    img.draw(0,0);
}
//--------------------------------------------------------------
void ofApp::update(){
    
    fftVals = ofSoundGetSpectrum(fftSize);
    resetColormap();
}
//--------------------------------------------------------------
void ofApp::draw(){
    c1 = clock();
    for (int i = 0; i < CC; i++) {
        MyHSL[i][0] = (i/(double)CC);
        MyHSL[i][1] = 1;
        MyHSL[i][2] = 0.5;
    }
    ofColor color;
    int index;
    //cout << "computing fractal" << endl;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            step = 0;
            Zre = beginH + ((double)j/w)*width;
            Zim = beginV + ((double)i/h)*height;
            while((step < max_step) && ((Zre*Zre + Zim*Zim) < bound)) {
                Zre_tmp = Zre;
                Zim_tmp = Zim;
                Zre = Zre_tmp * Zre_tmp - Zim_tmp * Zim_tmp + Cre;
                Zim = 2 * Zre_tmp * Zim_tmp + Cim;
                step++;
            }
            index = step % CC;
            ofColor color= ofColor(colormap[index][0], colormap[index][1], colormap[index][2]);
            img.setColor(j, i, color);
        }
    }
    //cout << "done computing fractal" << endl;
    img.update();
    //c2 = clock();
    //cout << "c1 = " << c1 << ", c2 = " << c2 << " and total = ";
    //cout << (c2 - c1)/(float)CLOCKS_PER_SEC << endl;
    //cout << "checking if" << endl;
    //if ((c2-c1)/(float)CLOCKS_PER_SEC < 8192.0/44100) {
    img.draw(0,0);
    //}
}
//--------------------------------------------------------------
void ofApp::resetColormap(){
    double minV, maxV, m;
    struct rgbStruct rgb;
    for (int j = 0; j < CC/8; j++) {
        minV = myMin(0, CC/8-1, CC/8);
        maxV = myMax(0, CC/8-1, CC/8);
        m = (fftVals[j] - minV)/(maxV - minV) * 0.65;
        MyHSL[j][2] = 0.35 + m;
    }
    for (int j = CC/8; j < CC/2; j++) {
        minV = myMin(j-15, j+63, (j+63) - (j-15));
        maxV = myMax(j-15, j+63, (j+63) - (j-15));
        m = (fftVals[j] - minV)/(maxV - minV) * 0.5;
        MyHSL[j][2] = 0.2 + m;
    }
    for (int j = CC/2; j < 3*CC/4; j++) {
        minV = myMin(j-7, j+31, (j+31) - (j-7));
        maxV = myMax(j-7, j+31, (j+31) - (j-7));
        m = (fftVals[j] - minV)/(maxV - minV) * 0.4;
        MyHSL[j][2] = 0.1 + m;
    }
    for (int j = 3*CC/4; j < CC-16; j++) {
        minV = myMin(j-3, j+16, (j+16) - (j-3));
        maxV = myMax(j-3, j+16, (j+16) - (j-3));
        m = (fftVals[j] - minV)/(maxV - minV) * 0.3;
        MyHSL[j][2] = m;
    }
    for (int i = 0; i < CC; i++) {
        rgb = hslTorgb(MyHSL[i][0], MyHSL[i][1], MyHSL[i][2]);
        colormap[i][0] = rgb.R;
        colormap[i][1] = rgb.G;
        colormap[i][2] = rgb.B;
    }
}
//--------------------------------------------------------------
double ofApp::myMin(double x, double y, int n) {
    double minV = 999999;
    for (int i = 0; i < CC/4; i++)
        if (fftVals[i] < minV) minV = fftVals[i];
    return minV;
}
//--------------------------------------------------------------
double ofApp::myMax(double x, double y, int n) {
    double maxV = -999999;
    for (int i = 0; i < CC/4; i++) {
        if (fftVals[i] > maxV) maxV = fftVals[i];
    }
    return maxV;
}
//--------------------------------------------------------------
struct rgbStruct ofApp::hslTorgb(double h, double sl, double l) {
    double v;
    double r,g,b;
    r = g = b = l;   // default to gray
    v = (l <= 0.5) ? (l * (1.0 + sl)) : (l + sl - l * sl);
    if (v > 0) {
        double m;
        double sv;
        int sextant;
        double fract, vsf, mid1, mid2;
        m = l + l - v;
        sv = (v - m ) / v;
        h *= 6.0;
        sextant = (int)h;
        fract = h - sextant;
        vsf = v * sv * fract;
        mid1 = m + vsf;
        mid2 = v - vsf;
        switch (sextant)
        {
            case 0:
                r = v;
                g = mid1;
                b = m;
                break;
            case 1:
                r = mid2;
                g = v;
                b = m;
                break;
            case 2:
                r = m;
                g = v;
                b = mid1;
                break;
            case 3:
                r = m;
                g = mid2;
                b = v;
                break;
            case 4:
                r = mid1;
                g = m;
                b = v;
                break;
            case 5:
                r = v;
                g = m;
                b = mid2;
                break;
        }
    }
    struct rgbStruct  rgb;
    rgb.R = round(r * 255);
    rgb.G = round (g * 255);
    rgb.B = round (b * 255);
    //cout << "r = " << rgb.R <<  ", g = " << rgb.G << " and b = " << rgb.B << endl;
    return rgb;
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    double tempLength, tempH, tempV, tempHeight;
    tempLength = endH - beginH;
    tempH = beginH;
    beginH = (x/w * tempLength)+tempH - 0.35*tempLength;
    endH = (x/w * tempLength)+tempH + 0.35*tempLength;
    
    tempHeight = endV - beginV;
    tempV = beginV;
    beginV = (y/h * tempHeight)+tempV - 0.35*tempHeight;
    endV = (y/h * tempHeight)+tempV + 0.35*tempHeight;
}