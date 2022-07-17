#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCsv.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxPanel gui;
		ofxCsv csv;
		
		vector<ofVec3f> vertices;
		vector<int> vertices_keycode;
		vector<bool> vertices_flag;
		vector<float> pressureValues;
		ofEasyCam ecam;

		int flaged_id = -1;
		int n_w = 10;
		int n_h = 10;
		int gridSize = 100;
		int resolution = 1;
		int n_w_highResolution, n_h_highResolution;

		ofParameter<float> coef_easing;
		ofParameter<float> coef_pow;
		ofParameter<int> max_depth;
		ofParameter<int> channel;
		ofParameter<bool> autoChange;

		bool showGui;
};
