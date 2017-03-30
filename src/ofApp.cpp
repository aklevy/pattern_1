#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);
    ofSetCircleResolution(100);
    ofDisableAlphaBlending();
    ofSetBackgroundColor(255);
    // drawDynamicImage();

    _windowSize.x = ofGetWindowWidth();
    _windowSize.y = ofGetWindowHeight();

    int nbCirclesPerLine =_windowSize.x/_circleRadius;
    int nbCirclesPerColumn = _windowSize.y/_circleRadius;

    float x = 0.;
    float y = 0.;


    // setup vbo
    _vboMesh.setMode(OF_PRIMITIVE_LINES);
    _vboMesh.enableColors();
    _vboMesh.enableIndices();
    int iVertex = 0;

    int lineStartIndex = 0;
    for(int i = 0; i < nbCirclesPerLine; i++)
    {
        x = _circleRadius + i * _circleRadius;

        for(int j = 0; j < nbCirclesPerColumn; j++)
        {
            iVertex =  (i * nbCirclesPerLine + j);
            lineStartIndex = iVertex * _nbPointsOnCircle;

            y = _circleRadius +j * _circleRadius;

            _circleCenters.push_back(ofVec2f(x,y));

            _vboMesh.addVertex(ofVec2f(x,y));
            _vboMesh.addColor(ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

            for(int k = 1; k < _nbPointsOnCircle; k++)
            {

                float phi = k* 360/_nbPointsOnCircle;
                _vboMesh.addVertex(
                            ofVec2f(x +  _circleRadius * cos(phi),
                                    y + _circleRadius * sin(phi)));
                _vboMesh.addColor(ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

                _vboMesh.addIndex(lineStartIndex);
                _vboMesh.addIndex(lineStartIndex + (k));
            }

        }

    }
}



//--------------------------------------------------------------
void ofApp::update(){
    float fps= ofGetFrameRate();
    ofSetWindowTitle(ofToString(fps) );

    // choose a random point on the circle
    if(!_bStopUpdate)
    {

        float phi = 0;
        ofVec2f zero(1, 0);
        ofVec2f mousePos(ofGetMouseX(),ofGetMouseY());


        int i = -1;
        int circleCenterIndex = 0;
        ofVec2f circleCenter(0);
        for(auto & vertex: _vboMesh.getVertices())
        {
            i++;

            if(i % _nbPointsOnCircle != 0)
            {
                continue;
            }
            else
            {
                circleCenter = _circleCenters[i/30];

                phi = zero.angleRad(ofVec2f(mousePos.x - circleCenter.x,
                                            mousePos.y - circleCenter.y));
                vertex = circleCenter + _circleRadius * ofVec2f(cos(phi),sin(phi));

            }

        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{

    _vboMesh.draw();

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    _bStopUpdate = !_bStopUpdate;
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
