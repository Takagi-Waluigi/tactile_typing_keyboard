#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	max_depth.set("max depth", -100, 0, -1000);
	coef_easing.set("coeffecient : easing", 0.01, 0, 0.5);
	coef_pow.set("coeffecient : power", 5, 0, 15);
	channel.set("channel", 0, 0, 2);
	autoChange.set("enable auto scene change", true);
	gui.add(max_depth);
	gui.add(coef_easing);
	gui.add(coef_pow);
	gui.add(channel);
	gui.add(autoChange);
	if (!csv.load("keycode.csv")) cout << "[ERROR]: csv load" << endl;
	n_w = csv.getNumCols();
	n_h = csv.getNumRows();
	n_w_highResolution = n_w * resolution;
	n_h_highResolution = n_h * resolution;

	gridSize = 75;

	for (int y = 0; y < n_h; y++)
	{
		ofxCsvRow rowData = csv.getRow(y);
		for (int x = 0; x < rowData.getNumCols(); x++)
		{
			vertices_keycode.push_back(rowData.getInt(x));
		}
	}

	vertices.resize(n_w_highResolution * n_h_highResolution);
	pressureValues.resize(n_w_highResolution * n_h_highResolution);
	vertices_flag.resize(n_w * n_h);

	for (int y = 0; y < n_h_highResolution; y++)
	{
		for (int x = 0; x < n_w_highResolution; x++)
		{
			vertices[y * n_w_highResolution + x] = ofVec3f(
				gridSize * (x - n_w_highResolution * 0.5),
				0,
				gridSize * (y - n_h_highResolution * 0.5)
			);
		}
	}	

	ofSetBackgroundColor(0);
	ofSetFullscreen(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!ofGetKeyPressed()) flaged_id = -1;

	for (int y = 0; y < n_h_highResolution; y++)
	{
		for (int x = 0; x < n_w_highResolution; x++)
		{
			if (x % resolution == 0 && y % resolution == 0 && vertices_flag[(y/resolution) * n_w + (x / resolution)])
			{
				pressureValues[y * n_w_highResolution + x] = max_depth;
			}
			else
			{
				if (flaged_id > 0)
				{
					float distance = ofDist(
						vertices[flaged_id * resolution].x,
						vertices[flaged_id * resolution].z,
						vertices[y * n_w_highResolution + x].x,
						vertices[y * n_w_highResolution + x].z);

					float attenuation = pow(((gridSize * n_w_highResolution) - distance) / (gridSize * n_w_highResolution), coef_pow);

					pressureValues[y * n_w_highResolution + x] = max_depth * attenuation;
				}
				else
				{
					pressureValues[y * n_w_highResolution + x] = 0;
				}
				
			}
		}
	}

	for (int i = 0; i < pressureValues.size(); i++)
	{
		vertices[i].y += (pressureValues[i] - vertices[i].y) * coef_easing;
	}

	if (autoChange && ofGetFrameNum() % 300 == 0) channel++;

	channel = channel % 3;

}

//--------------------------------------------------------------
void ofApp::draw(){

	ecam.begin();
	ofEnableDepthTest();
	ofPushMatrix();
	ofTranslate(gridSize * 0.5, 0, 0);

	//if (autoChange) ofRotateYRad(ofGetElapsedTimef() * 0.1);

	switch (channel % 3)
	{
	case 0:
		ofPushStyle();
		for (int y = 1; y < n_h_highResolution; y++)
		{
			for (int x = 1; x < n_w_highResolution; x++)
			{
				ofSetColor(255);
				ofNoFill();

				ofSetPolyMode(OF_POLY_WINDING_NONZERO);
				ofBeginShape();
				ofVertex(vertices[(y - 1) * n_w_highResolution + (x - 1)]);
				ofVertex(vertices[(y - 1) * n_w_highResolution + x]);
				ofVertex(vertices[y * n_w_highResolution + x]);
				ofVertex(vertices[y * n_w_highResolution + (x - 1)]);
				ofEndShape();

			}
		}
		ofPopStyle();
		break;

	case 1:
		ofPushStyle();
		for (int y = 0; y < n_h_highResolution; y++)
		{
			for (int x = 0; x < n_w_highResolution; x++)
			{
				ofSetColor(255);
				ofDrawLine(
					vertices[y * n_w_highResolution + x],
					ofVec3f(
						vertices[y * n_w_highResolution + x].x,
						max_depth * 1.25,
						vertices[y * n_w_highResolution + x].z
					)
				);

				ofDrawSphere(vertices[y * n_w_highResolution + x], gridSize * 0.3);
			}
		}
		ofPopStyle();
		break;

	case 2:
		ofPushStyle();
		for (int y = 0; y < n_h_highResolution; y++)
		{
			for (int x = 0; x < n_w_highResolution; x++)
			{
				float colExchange = ofMap(vertices[y * n_w_highResolution + x].y, 0, max_depth, 0, 255);
				ofFill();
				ofSetColor(colExchange);
				ofPushMatrix();
				ofTranslate(vertices[y * n_w_highResolution + x].x, 0, vertices[y * n_w_highResolution + x].z);

				ofRotateXDeg(90);

				ofDrawRectangle(0, 0, gridSize * 0.85, gridSize * 0.85);


				ofNoFill();
				ofSetColor(40);				
				ofDrawRectangle(0, 0, gridSize * 0.95, gridSize * 0.95);

				ofPopMatrix();
			}
		}
		ofPopStyle();
		break;

	}
	
	ofPopMatrix();
	ecam.end();
	ofDisableDepthTest();
	if(showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	int counter = 0;
	for (int i = 0; i < vertices_keycode.size(); i++)
	{
		if (key == vertices_keycode[i])
		{
			vertices_flag[i] = true;
			flaged_id = i;
			counter++;
			
		}
	}
	if (counter < 1) flaged_id = -1;
	
	if (key == OF_KEY_UP) showGui = !showGui;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	for (int i = 0; i < vertices_keycode.size(); i++)
	{
		if (vertices_flag[i] && key == vertices_keycode[i]) vertices_flag[i] = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
