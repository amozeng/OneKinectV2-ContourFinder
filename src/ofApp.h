#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

    public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    
    void drawTextureAtRowAndColumn(const std::string& title,
                                   const ofTexture& tex,
                                   int row,
                                   int column);
    
    
    
    ofxKinectV2 kinectV2;
    
    ofTexture texRGB;
    ofTexture texRGBRegistered;
    ofTexture texIR;
    ofTexture texDepth;
    ofxCvGrayscaleImage grayImage;
    ofxCvContourFinder contourFinder;
    ofxCv::ContourFinder contourFinderAdv;
        
    int numRows = 2;
    int numColumns = 2;

    ofEasyCam cam;
    ofMesh pointCloud;
    bool showPointCloud = false;
    
    ofxPanel panel;
    ofxIntSlider cfMinArea, cfMaxArea, cfNum;
    
    
    // gui for advanced contour finder
    ofxPanel cvGui;
    ofParameter<float> blurLevel;
    ofParameter<int> threshold;
    ofParameter<bool> invert;
    ofParameter<int> erodeIterations;
    ofParameter<int> dilateIterations;
    
    ofParameter<float> contourMinArea;
    ofParameter<float> contourMaxArea;
    ofParameter<float> contourThreshold;
    ofParameter<bool> contourHoles;
    ofParameter<bool> contourSimplify;
};
