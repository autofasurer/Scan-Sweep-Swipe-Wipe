/////////////////////////////////////////////////////////////////////////////////////
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License.
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

//// ofxaddon extension by Naoto HIEDA, 2014

//// simple phidgetEncoder positiont readout by Brecht Debackere 2014


#include <stdio.h>
#include <phidget21.h>

#include "ofMain.h"


class ofxPhidgets {
protected:
	static int CCONV AttachHandler(CPhidgetHandle phid, void *userptr);
	static int CCONV DetachHandler(CPhidgetHandle phid, void *userptr);
	static int CCONV ErrorHandler(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *unknown);
   	static int CCONV InputChangeHandler(CPhidgetEncoderHandle phid, void *usrptr, int Index, int State);
	static int CCONV OutputChangeHandler(CPhidgetEncoderHandle phid, void *usrptr, int Index, int State);
	static int CCONV SensorChangeHandler(CPhidgetEncoderHandle phid, void *usrptr, int Index, int Value);
    static int CCONV InputChangeHandler(CPhidgetInterfaceKitHandle phid, void *usrptr, int Index, int State);
	static int CCONV OutputChangeHandler(CPhidgetInterfaceKitHandle phid, void *usrptr, int Index, int State);
	static int CCONV SensorChangeHandler(CPhidgetInterfaceKitHandle phid, void *usrptr, int Index, int Value);
	bool initialized;

public:
	//virtual void init() = 0;
	//virtual void update() = 0;
	//virtual void exit() = 0;
};


class ofxPhidgetsEncoder : public ofxPhidgets {

public:
	void init();
	void update();
	void exit();
    ~ofxPhidgetsEncoder() { exit(); };
    int encPos, indexPos;

private:
	int display_properties(CPhidgetEncoderHandle phid);
	int result;
	const char *err;


	//Declare an encoder handle
	CPhidgetEncoderHandle enc = 0;
};

class ofxPhidgetsIfkit : public ofxPhidgets {

public:
    void init();
    void digiOut(int index, int state);
    void exit();
	~ofxPhidgetsIfkit() { exit(); };
    bool pushed = false;

private:
    static int CCONV InputChangeHandler(CPhidgetInterfaceKitHandle phid, void *usrptr, int index, int state);
    int display_properties(CPhidgetInterfaceKitHandle phid);
	int result, numSensors, i;
	const char *err;

	//Declare an InterfaceKit handle
	CPhidgetInterfaceKitHandle ifKit = 0;
};

