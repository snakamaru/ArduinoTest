#include "ofApp.h"

#define MINSCALE 0.3;
#define MAXSCALE 0.8;

//半径の初期値を定義
int inputVal = 50;
int inputMax =500;
int inputMin= 300;

float MINDIAMETER = 768 * MINSCALE;
float MAXDIAMETER = 768 * MAXSCALE;

//初期の半径の大きさと変数によるステップ値
float stepInit = (MAXDIAMETER-MINDIAMETER)/100;
float step = (MAXDIAMETER-MINDIAMETER)/80;
float level = 50;
float diameter = (MINDIAMETER + MAXDIAMETER) * 0.5;
//コマンドモードとアナログモードを切り替える, 1AnalogMode, 2CommandMode

int mode = 1;

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

    
    //Arduino Modeの時は値を更新する
    if (mode==1){
        inputVal = ard.getAnalog(0);
    }

    if (mode==3){
        ofCallibration();
    }
    //    右上にモードを表示する
    ofIndicator(mode);
    
    level = (inputVal-inputMin)/step;
    float diameter = MINDIAMETER + level * stepInit;
    // 円を描画する
    ofDrawCircle(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), diameter/2);

    std::cout<<"InputVal:"<< inputVal << endl;
    std::cout<<"Diameter:"<< diameter << endl;
    std::cout<<"InputMax:"<< inputMax << endl;
    std::cout<<"InputMin:"<< inputMin << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
            
            //        fでフルスクリーン
        case 'f':
            ofToggleFullscreen();
            break;
            //cを押すことでアナログとキー入力モードが切り替わる
        case 'c':
            if(mode == 1){
                mode = 2;}
            else if(mode==2){
                mode =3;
            }
            else if(mode==3){
                mode =1;
            }
            break;
   
//            lキーによりサークルをより大きく表示する
        case'l':
            ::inputVal += 5;
            break;
            
//  sキーによりサークルをより小さくする
        case 's':
            ::inputVal -= 5;
            break;
            
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

//現在のコマンドモードを表示する
void ofApp::ofIndicator(int mode){
    if(mode ==1){
//        inputVal = ard.getAnalog(0);
        ofDrawBitmapString("ARDUINO MODE", ofGetWidth()-120,20);
    }
    else if (mode ==2){
        ofDrawBitmapString("COMMAND MODE", ofGetWidth()-120,20);
    }
    else if (mode ==3){
        ofDrawBitmapString("CALLIBRATION MODE", ofGetWidth()-120,20);
    }
}

void ofApp::ofCallibration(){
//    センサの呼吸状態から最小最大の値を更新する
    if (inputMax < ard.getAnalog(0)){
        inputMax = ard.getAnalog(0);
    }
    else if (inputMin> ard.getAnalog(0)){
        inputMin=ard.getAnalog(0);
    }
    
    //Max, Minの値から円の大きさの刻み幅を指定
    step = (inputMax - inputMin) /100 ;
    
    }
