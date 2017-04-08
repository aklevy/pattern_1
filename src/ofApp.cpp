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

    int nbCirclesPerLine =(_windowSize.x )/(_circleRadius*2.);
    int nbCirclesPerColumn = (_windowSize.y )/(_circleRadius*2.);

    setupVbo(nbCirclesPerLine,nbCirclesPerColumn);
}
//--------------------------------------------------------------
void ofApp::setupVbo(int nbCirclesPerLine,int nbCirclesPerColumn)
{

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
        x = _circleRadius + i *2.*_circleRadius;

        for(int j = 0; j < nbCirclesPerColumn; j++)
        {
            iVertex =  (j * nbCirclesPerLine + i);
            lineStartIndex = iVertex * (_nbPointsOnCircle+1);

            y = _circleRadius + j * 2.*_circleRadius;

            _circleCenters.push_back(ofVec2f(x,y));

            _vboMesh.addVertex(ofVec2f(x,y));
            _vboMesh.addColor(ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

            for(int k = 0; k < _nbPointsOnCircle; k++)
            {

                float phi = k* 360/(_nbPointsOnCircle);
                _vboMesh.addVertex(
                            ofVec2f(x +  _circleRadius * cos(phi),
                                    y + _circleRadius * sin(phi)));
                _vboMesh.addColor(ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

                _vboMesh.addIndex(lineStartIndex);
                _vboMesh.addIndex(lineStartIndex + (k+1));
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
        // int circleCenterIndex = 0;
        //ofVec2f circleCenter(0);
        for(auto & vertex: _vboMesh.getVertices())
        {
            i++;

            if(i % (_nbPointsOnCircle+1) != 0)
            {
                continue;
            }
            else
            {
                const auto & circleCenter = _circleCenters[i / (_nbPointsOnCircle+1)];

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
    // ofBackground(255);

    if(!_bStopUpdate)
    {
        _vboMesh.draw();
    }else
    {
        drawCircleCenters();
    }
}

//--------------------------------------------------------------
void ofApp::drawCircleCenters()
{
    ofSetColor(0,200,200);

    int i = -1;
    // int circleCenterIndex = 0;
    //ofVec2f circleCenter(0);
    for(auto & vertex: _vboMesh.getVertices())
    {
        i++;

        if(i % (_nbPointsOnCircle+1) != 0)
        {
            continue;
        }
        else
        {
            const auto & circleCenter = _circleCenters[i / (_nbPointsOnCircle+1)];

            ofDrawCircle(circleCenter,20);
        }

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f')
    {

        _bStopUpdate = true;
        ofToggleFullscreen();

        _vboMesh.clear();

        _circleCenters.clear();
        _windowSize.x = ofGetWindowWidth();
        _windowSize.y = ofGetWindowHeight();

        int nbCirclesPerLine =(_windowSize.x )/(_circleRadius*2.);
        int nbCirclesPerColumn = (_windowSize.y )/(_circleRadius*2.);

        setupVbo(nbCirclesPerLine,nbCirclesPerColumn);
        _bStopUpdate = false;

    }

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
