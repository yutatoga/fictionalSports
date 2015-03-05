#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
		ofDisableAntiAliasing();
		ofSetFrameRate(30);
		ofBackground(200, 220, 255);
		ofSetLogLevel(OF_LOG_NOTICE);
		
		// scrolling ground
		groundSpeed = 0.02;
		groundTimer = 0;
		groundRes   = 30;
		
		// build the ground first
		for(int i=0; i<groundRes; i++) {
				float x = ofMap(i, 0, groundRes-1, 0, ofGetWidth());
				float y = ofGetHeight()*GROUND_MIDDLE_RATE + ofRandom(-30, 100);
				groundLine.addVertex(x, y);
		}
		
		box2d.init();
		box2d.enableEvents();
		box2d.setGravity(0, 20);
		box2d.setFPS(30.0);
		box2d.registerGrabbing();
		// register the listener so that we get the events
		ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
		ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);
		
		sound.loadSound("4.mp3");
		sound.setMultiPlay(true);
		sound.setLoop(false);
		
		groundBoxNumber = groundRes;
		groundBoxWidth = ofGetWidth()/(float)groundBoxNumber;
		groundBoxHeight = 100;
		for (int i=0; i<groundBoxNumber; i++) {
				// ground
				groundBoxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
				ofxBox2dRect *rect = groundBoxes.back().get();
				rect->setPhysics(0, 0, 0);
				rect->setup(box2d.getWorld(), groundBoxWidth*(i+0.5), ofGetHeight()/2.0, groundBoxWidth, groundBoxHeight);
				
				// human
				float humanBoxHeight = ofRandom(50, 100);
				humanBoxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
				ofxBox2dRect *rect2 = humanBoxes.back().get();
				rect2->setPhysics(0, 0, 0);
				rect2->setup(box2d.getWorld(), groundBoxWidth*(i+0.5), ofGetHeight()/2.0, groundBoxWidth*0.5, humanBoxHeight);
				
				// listner
				rect2->setData(new SoundData());
				SoundData * sd = (SoundData*)rect2->getData();
				sd->soundID = 4;
				sd->teamID = (int)ofRandom(0, 10)%2;
				ofLogNotice(ofToString(sd->teamID));
				sd->bHit	= false;
		}
}

//--------------------------------------------------------------
void ofApp::contactStart(ofxBox2dContactArgs &e) {
		if(e.a != NULL && e.b != NULL) {
				
				// if we collide with the ground we do not
				// want to play a sound. this is how you do that
//				if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) {
				
						SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
						SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();

						if(aData) {
								aData->bHit = true;
								sound.play();
						}
						
						if(bData) {
								bData->bHit = true;
								sound.play();
						}
//				}
		}
}

//--------------------------------------------------------------
void ofApp::contactEnd(ofxBox2dContactArgs &e) {
		if(e.a != NULL && e.b != NULL) {

				SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
				SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
				
				if(aData) {
						aData->bHit = false;
				}
				
				if(bData) {
						bData->bHit = false;
				}
		}
}

//--------------------------------------------------------------
void ofApp::update(){
//		ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
//		ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
		
		box2d.update();
		
		ground.clear();
		ground.addVertexes(groundLine);
		ground.create(box2d.getWorld());
}

//--------------------------------------------------------------
void ofApp::draw(){
		float   t = ofGetElapsedTimef() * 1.2;
		float   y = ofSignedNoise(t) * 200 + (sin(t)* 100);
		
		if(ofGetElapsedTimef() - groundTimer > groundSpeed) {
				float newHeight = ofGetHeight()*GROUND_MIDDLE_RATE + y;
				int n = (int)groundLine.size();
				groundTimer = ofGetElapsedTimef();
				for(int i=n-1; i>=1; i--) {
						groundLine[i].y = groundLine[i-1].y;
						if(i%2){
								ofSetColor(32, 127, 32);
						}else{
								ofSetColor(35, 130, 35);
						}
						ofRect(groundBoxes[i].get()->getPosition().x-0.5*groundBoxWidth, groundLine[i-1].y, groundBoxWidth, ofGetHeight());
						groundBoxes[i].get()->setPosition(groundBoxes[i].get()->getPosition().x, groundLine[i-1].y);
						ofSetColor(255, 0, 0);
						if (i > n*0.5) {
								humanBoxes[i].get()->setPosition(groundBoxes[i].get()->getPosition().x, groundLine[i-1].y-(groundBoxHeight+humanBoxes[i].get()->getHeight())/2.0-1);
						}else{
								humanBoxes[i].get()->setPosition(groundBoxes[i].get()->getPosition().x, ofGetHeight()+groundBoxHeight); // hide
						}
				}
				groundLine[0].y = newHeight;
				ofSetColor(35, 130, 35);
				ofRect(groundBoxes[0].get()->getPosition().x-0.5*groundBoxWidth, newHeight, groundBoxWidth, ofGetHeight());
				groundBoxes[0].get()->setPosition(groundBoxes[0].get()->getPosition().x, newHeight);
				humanBoxes[0].get()->setPosition(groundBoxes[0].get()->getPosition().x, ofGetHeight()+groundBoxHeight); // hide
		}
		ofSetColor(255);
		ground.updateShape();
		ground.draw();
		
		
		string info = "";
		info += "Press [c] for circles\n";
		info += "Press [b] for blocks\n";
		info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
		info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
		info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
		ofSetColor(255);
		ofDrawBitmapString(info, 30, 30);
		
		//draw box and circles
		for(int i=0; i<circles.size(); i++) {
				ofFill();
				ofSetColor(250, 160, 0);
				circles[i].get()->draw();
		}
		
		for(int i=0; i<boxes.size(); i++) {
				ofFill();
				ofSetHexColor(0xe63b8b);
				boxes[i].get()->draw();
		}
		
		for (int i=0; i<groundBoxes.size(); i++) {
				ofFill();
				if(i%2){
						ofSetColor(32, 127, 32);
				}else{
						ofSetColor(35, 130, 35);
				}
				groundBoxes[i].get()->draw();
		}
		
		for (int i=0; i<humanBoxes.size(); i++){
				ofFill();
				SoundData * data = (SoundData*)humanBoxes[i].get()->getData();
				if (data && data->bHit) {
						ofSetColor(255, 255, 0);
				}else{
						if (data->teamID == 0) {
								ofSetColor(255, 0, 0);
						}else{
								ofSetColor(0, 0, 255);
						}
				}
				humanBoxes[i].get()->draw();
		}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		if(key == 'c') {
				float r = 20;
				if (circles.size() != 0) {
						circles.pop_back();
				}
				circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
				ofxBox2dCircle * circle = circles.back().get();
				circle->setPhysics(3.0, 0.53, 0.1);
				circle->setup(box2d.getWorld(), mouseX, mouseY, r);
				circle->setVelocity(ofRandom(10, 15), ofRandom(-20, -10));
				
				// listner
//				circle->setData(new SoundData());
//				SoundData * sd = (SoundData*)circle->getData();
//				sd->soundID = 4;
//				sd->bHit	= false;
		}
		
		if(key == 'b') {
				float w = ofRandom(20, 12);
				float h = ofRandom(40, 50);
				boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
				ofxBox2dRect * rect = boxes.back().get();
				rect->setPhysics(3.0, 0.53, 0.1);
				rect->setup(box2d.getWorld(), mouseX, mouseY, w, h);
		}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
