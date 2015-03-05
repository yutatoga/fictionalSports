#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

#define GROUND_MIDDLE_RATE 0.75

class SoundData {
public:
		int	 soundID;
		bool bHit;
		int teamID;
		int score;
};

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// this is the function for contacts
		void contactStart(ofxBox2dContactArgs &e);
		void contactEnd(ofxBox2dContactArgs &e);
		// when the ball hits we play this sound
		ofSoundPlayer  sound;
		
		int groundRes;
		float groundSpeed;
		float groundTimer;
		ofxBox2d box2d;
		ofPolyline groundLine;
		ofxBox2dEdge ground;
		vector<ofPtr<ofxBox2dCircle> >	circles;		  //	default box2d circles
		vector<ofPtr<ofxBox2dRect> >      boxes;			  //	defalut box2d rects
		
		// yuta
		int groundBoxNumber;
		vector<ofPtr<ofxBox2dRect> > groundBoxes, humanBoxes;
		ofTrueTypeFont font;
		float groundBoxWidth;
		float groundBoxHeight;
};
