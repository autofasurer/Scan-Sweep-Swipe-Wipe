/////////////////////////////////////////////////////////////////////////////////////
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License.
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

//// ofxaddon extension by Naoto HIEDA, 2014

//// simple phidgetEncoder position readout by Brecht Debackere 2014

#include "ofxPhidgetsEnc.h"

int CCONV ofxPhidgets::AttachHandler(CPhidgetHandle phid, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(phid, &name);
	CPhidget_getSerialNumber(phid, &serialNo);

	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int CCONV ofxPhidgets::DetachHandler(CPhidgetHandle phid, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (phid, &name);
	CPhidget_getSerialNumber(phid, &serialNo);

	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int CCONV ofxPhidgets::ErrorHandler(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *unknown)
{
	printf("Error handled. %d - %s", ErrorCode, unknown);
	return 0;
}


//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.

int ofxPhidgetsEncoder::display_properties(CPhidgetEncoderHandle phid)
{
	int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);

	return 0;
}


void ofxPhidgetsEncoder::init()
{
	initialized = false;

	//create the InterfaceKit object
	CPhidgetEncoder_create(&enc);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)enc, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)enc, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)enc, ErrorHandler, NULL);

	//open the encoder for device connections
	CPhidget_open((CPhidgetHandle)enc, -1);

	//get the program to wait for an encoder  device to be attached
	printf("Waiting for encoder to be attached....");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)enc, 1000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached interface kit device
	display_properties(enc);
    CPhidgetEncoder_setEnabled (enc, 0 , PTRUE);
	//read interface kit event data
	printf("Reading.....\n");

	initialized = true;
}

void ofxPhidgetsEncoder::update()
{
    CPhidgetEncoder_getPosition (enc, 0, &encPos);
    CPhidgetEncoder_getIndexPosition(enc, 0, &indexPos);

    //printf("Encoder: %d\n", encPos);
}

void ofxPhidgetsEncoder::exit()
{
	if( !initialized ) return;

	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	printf("Closing...\n");
    CPhidgetEncoder_setEnabled (enc, 0 , PFALSE);
	CPhidget_close((CPhidgetHandle)enc);
	CPhidget_delete((CPhidgetHandle)enc);
}
/************************************************************************************************************/
/************************************************************************************************************/

int CCONV ofxPhidgetsIfkit::InputChangeHandler(CPhidgetInterfaceKitHandle phid, void *usrptr, int index, int state)
{
    printf("Digital Input: %d > State: %d\n", index, state);
    if (index && state) {
        reinterpret_cast<ofxPhidgetsIfkit*>(usrptr)->pushed=true;
    }
    return 0;
}

int ofxPhidgetsIfkit::display_properties(CPhidgetInterfaceKitHandle phid)
{
	int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
	CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
	CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
	CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
	printf("# Sensors: %d\n", numSensors);
	printf("Ratiometric: %d\n", ratiometric);

	for(i = 0; i < numSensors; i++)
	{
		CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);

		printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
	}

	return 0;
}


void ofxPhidgetsIfkit::init()
{
	initialized = false;

	//create the InterfaceKit object
	CPhidgetInterfaceKit_create(&ifKit);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, ErrorHandler, NULL);

	//open the interfacekit for device connections
	CPhidget_open((CPhidgetHandle)ifKit, -1);

	//get the program to wait for an encoder  device to be attached
	printf("Waiting for Interface Kit to be attached....");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 1000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached interface kit device
	display_properties(ifKit);

	//set input change handler
    CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, ofxPhidgetsIfkit::InputChangeHandler, this);

    //read interface kit event data
	printf("Reading.....\n");

	initialized = true;
}

void ofxPhidgetsIfkit::digiOut(int index, int state)
{
    CPhidgetInterfaceKit_setOutputState (ifKit, index,  state);
}

void ofxPhidgetsIfkit::exit()
{
	if( !initialized ) return;

	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)ifKit);
	CPhidget_delete((CPhidgetHandle)ifKit);
}
