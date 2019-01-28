#include "ofApp.h"

//半径の初期値を定義
int inputVal = 10;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    ofBackground(0,0,0);
    
    // positionの初期値、最小値、最大値を設定
    ofVec2f initPos = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
    ofVec2f minPos = ofVec2f(0, 0);
    ofVec2f maxPos = ofVec2f(ofGetWidth(), ofGetHeight());
    
    //    buttonState = "digital pin:";
    //    potValue = "analog pin:";
    
    //    Arduinoの接続に必要なポート情報
    ard.connect("/dev/cu.usbmodem142101", 57600);
    
    // 初期接続の確認フラグ
    // listen for EInitialized notification. this indicates that
    // the arduino is ready to receive commands and it is safe to
    // call setupArduino()
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino    = false;    // flag so we setup arduino when its ready, you don't need to touch this :)
    
}

//--------------------------------------------------------------
void ofApp::update(){
    updateArduino();
    
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version){
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)
    
    //    ピンのアサインを決める
    for (int i=0; i<6; i++){
        //A0-A5をアナログインプットに指定
        ard.sendAnalogPinReporting(i, ARD_ANALOG);
    }
    
    //set up 状態の判定
    bSetupArduino = true;
    
    
}
//--------------------------------------------------------------
void ofApp::updateArduino(){
    //
    ard.update();
    
}

//--------------------------------------------------------------

void ofApp::draw(){
    //ANALOG INの値を記載する
    //ard.getAnalog(0)-ard.getAnalog(5)でA0-A5の値を読み取る
    
    //メッセージ出力
    string msg ="";
    for(int i=0; i<6;i++){
        msg += "Analog IN" + ofToString(i,0) + ":" + ofToString(ard.getAnalog(i),0)+'\n';
    }
    ofSetHexColor(0xFFFFFF);
    ofDrawBitmapString(msg, 20,20);
    
//    ArduinoのA0の値に応じてサークルの形を変形させる
//        ofDrawCircle(ofVec2f(position), radius);
        ofDrawCircle(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ::inputVal);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
            
            //        fでフルスクリーン
        case 'f':
            ofToggleFullscreen();
        case 'u':
            ::inputVal += 10;
        case 'd':
            ::inputVal --;
            
            break;
    }}

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
