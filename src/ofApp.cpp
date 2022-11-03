#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Uncomment for verbose info from libfreenect2
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofBackground(0);
    
    //see how many devices we have.
    std::vector <ofxKinectV2::KinectDeviceInfo> deviceList = kinectV2.getDeviceList();
    
    panel.setup("", "settings.xml", 10, 100);
    
//    ofxKinectV2::Settings ksettings;
//    ksettings.enableRGB = true;
//    ksettings.enableIR = true;
//    ksettings.enableDepth = true;
//    ksettings.enableRGBRegistration = true;
//    ksettings.config.MinDepth = 0.5;
//    ksettings.config.MaxDepth = 8.0;
    
//    kinectV2.open(deviceList[0].serial, ksettings);
    kinectV2.open(deviceList[0].serial);

    grayImage.allocate(kinectV2.getDepthPixels().getWidth(), kinectV2.getDepthPixels().getHeight());
    panel.add(kinectV2.params);
    
    panel.loadFromFile("settings.xml");
    
    // for advanced contour finder
    contourFinderAdv.setMinAreaRadius(200);
    contourFinderAdv.setMaxAreaRadius(1000);
    contourFinderAdv.setSimplify(true);
    
    // gui for regular contour finder
    panel.add(cfMinArea.setup("cfMinArea", 5000, 1000, 100000));
    panel.add(cfMaxArea.setup("cfMaxArea", 50000, 5000, 200000));

    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinectV2.update();
    
    if (kinectV2.isFrameNew())
    {
        texRGB.loadData(kinectV2.getRgbPixels());
//        texRGBRegistered.loadData(kinectV2.getDepthPixels());
        texIR.loadData(kinectV2.getRgbPixels());
        texDepth.loadData(kinectV2.getDepthPixels());
        
        // grey image for contourFinder
        grayImage.setFromPixels(kinectV2.getDepthPixels().getData(), kinectV2.getDepthPixels().getWidth(), kinectV2.getDepthPixels().getHeight());
        
//        if( kinectV2.isRGBEnabled()) texRGB.loadData(kinectV2.getPixels());
//        if(kinectV2.getRegisteredPixels().getWidth() > 10) texRGBRegistered.loadData(kinectV2.getRegisteredPixels());
//        if(kinectV2.isIREnabled()) texIR.loadData(kinectV2.getIRPixels());
//        if(kinectV2.isDepthEnabled() ) texDepth.loadData(kinectV2.getDepthPixels());
        

//        if (showPointCloud)
//        {
//            pointCloud.clear();
//            for (std::size_t x = 0; x < texRGBRegistered.getWidth(); x++)
//            {
//                for (std::size_t y = 0; y < texRGBRegistered.getHeight(); y++)
//                {
//                    pointCloud.addVertex(kinectV2.getWorldCoordinateAt(x, y));
//                    pointCloud.addColor(kinectV2.getRegisteredPixels().getColor(x, y));
//                }
//            }
//        }
        // greyImage, minArea, maxArea, nConsidered
        contourFinder.findContours(grayImage, cfMinArea, (kinectV2.getDepthPixels().getWidth() * kinectV2.getDepthPixels().getHeight())/2, 1, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (!showPointCloud)
    {
        drawTextureAtRowAndColumn("RGB Pixels",texRGB, 0, 0);

        drawTextureAtRowAndColumn("RGB Pixels, Registered", texRGBRegistered, 1, 0);

        drawTextureAtRowAndColumn("Depth Pixels, Mapped", texDepth, 1, 1);

        drawTextureAtRowAndColumn("IR Pixels, Mapped", texIR, 0, 1);
    }
    else
    {
        cam.begin();
        ofPushMatrix();
        ofScale(100, -100, -100);
        //pointCloud.draw();
        pointCloud.drawVertices();
        ofPopMatrix();
        cam.end();
    }
    contourFinder.draw(0,0);
    panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'p')
    {
        showPointCloud = !showPointCloud;
    }
}

//--------------------------------------------------------------

void ofApp::drawTextureAtRowAndColumn(const std::string& title,
                                      const ofTexture& tex,
                                      int row,
                                      int column)
{
    float displayWidth = ofGetWidth() / numColumns;
    float displayHeight = ofGetHeight() / numRows;
    
    ofRectangle targetRectangle(row * displayWidth,
                                column * displayHeight,
                                displayWidth,
                                displayHeight);
    
    ofNoFill();
    ofSetColor(ofColor::gray);
    ofDrawRectangle(targetRectangle);
    
    ofFill();
    ofSetColor(255);
    if (tex.isAllocated())
    {
        ofRectangle textureRectangle(0, 0, tex.getWidth(), tex.getHeight());
        
        // Scale the texture rectangle to its new location and size.
        textureRectangle.scaleTo(targetRectangle);
        tex.draw(textureRectangle);
    }
    else
    {
        ofDrawBitmapStringHighlight("...",
                                    targetRectangle.getCenter().x,
                                    targetRectangle.getCenter().y);
    }
    
    ofDrawBitmapStringHighlight(title,
                                targetRectangle.getPosition() + glm::vec3(14, 20, 0));
}
