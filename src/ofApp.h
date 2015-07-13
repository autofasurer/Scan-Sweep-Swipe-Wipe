#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxPhidgetsEnc.h"
#include "camThread.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void animate();
        void exit();
        void keyPressed(int key);
		void gotMessage(ofMessage msg);

	//the camera
    ofCamera            modelCam, maskCam;
    ofNode              globalRotation;

    //primitives
    ofSpherePrimitive   sphere;
    ofSpherePrimitive   sphere1, center;

    //model loaded from disk
    ofxAssimpModelLoader model;

    //mesh which will contain the model's data
    ofMesh              mesh;

    //misc variables
    float               y, distance, distanceGoal, slice, color, gap, gapOffset, throb;
    float               rotNow, rotThen, mRota;
    int                 encOffset;
    bool                showModel, showPlane;

    //Phidgets
    ofxPhidgetsEncoder  enc;
    ofxPhidgetsIfkit    ifKit;
    int index, state, turning;

    //Texture
    ofTexture           tex0;

    //Video Camera & cam thread
    ofTexture			videoTexture0, videoTexture1, videoTexture2;
    int 				camWidth;
    int 				camHeight;
    ofFbo               comboTex;
    MyThread            thread;
    ofVideoGrabber      uvcam;
    ofPixels            pixels;

    //FBO
    ofFbo fbo;

    vector<ofPoint> vertices0;
    float fader, brightness, contrast, saturation, x1, x2, y1, y2, z1, z2, dist1, dist2, scrDir, meshDir, startTime, timer;
    ofVec3f rotaVec;

    //Shader
    ofShader            brcosaShader;

    //Clip plane equations
    double eqn0[4] = {0., 0.1, -0.1, -23.};
    double eqn1[4] = {0., -0.1, 0.1, 23.};
    double eqn2[4] = {0.0, 1.0, 0.0, 0.0};
    double eqn3[4] = {0.0, 0.0, 0.0, 0.0};

};
