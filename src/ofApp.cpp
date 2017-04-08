#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(60);
    ofSetCircleResolution(100);
    ofDisableAlphaBlending();
    ofSetBackgroundColor(0);

    _circleInColor = ofFloatColor(99./255.,7./255.,32./255.,1.);
    _circleOutColor = ofFloatColor(0./255.,0./255.,0./255.,1.);
    _seedColor  = ofFloatColor(165./255.,57./255.,3./255.);

    // drawDynamicImage();

    _windowSize.x = ofGetWindowWidth();
    _windowSize.y = ofGetWindowHeight();


    int nbCirclesPerLine =(_windowSize.x )/(_circleRadius*2.);
    int nbCirclesPerColumn = (_windowSize.y )/(_circleRadius*2.);

    setupVbo(nbCirclesPerLine,nbCirclesPerColumn);
    setupHanabiSeed(3);
}
//--------------------------------------------------------------
void ofApp::setupVbo(int nbCirclesPerLine,int nbCirclesPerColumn)
{

    float x = 0.;
    float y = 0.;

    _vboMesh.clear();
    _vertexFollow.clear();
    _circleCenters.clear();

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
            _vertexFollow.push_back(ofVec2f(x,y));

            _vboMesh.addVertex(ofVec2f(x,y));
            _vboMesh.addColor(_circleOutColor);//ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

            for(int k = 0; k < _nbPointsOnCircle; k++)
            {

                float phi = k* 360/(_nbPointsOnCircle);
                _vboMesh.addVertex(
                            ofVec2f(x +  _circleRadius * cos(phi),
                                    y + _circleRadius * sin(phi)));
                _vboMesh.addColor(_circleInColor);//ofFloatColor(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));

                _vboMesh.addIndex(lineStartIndex);
                _vboMesh.addIndex(lineStartIndex + (k+1));
            }

        }

    }

#ifdef TARGET_OPENGLES
    ofLogError() << "no shader available";
#else
    if(ofIsGLProgrammableRenderer()){
        _shaderGradient.load("shadersGL3/shader");
    }else{
        _shaderGradient.load("shadersGL2/shader");
    }
#endif
}

//--------------------------------------------------------------
void ofApp::setupHanabiSeed(int nb)
{
    _hanabiSeed.set(ofRandom(50, (_windowSize.x - 50)),_windowSize.y);


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
        ofVec2f mousePos(_hanabiSeed.x,_hanabiSeed.y);//ofGetMouseX(),ofGetMouseY());


        int i = -1;
        // int circleCenterIndex = 0;
        //ofVec2f circleCenter(0);
        int index = 0.;
        for(auto & vFollow : _vertexFollow)
        {
            const auto & circleCenter = _circleCenters[index];
            float d = ofClamp(_dist / circleCenter.distance(mousePos),0.,1.);

            phi = zero.angleRad(ofVec2f(mousePos.x - circleCenter.x,
                                        mousePos.y - circleCenter.y));
            vFollow =  circleCenter + d*_circleRadius *  ofVec2f(cos(phi),sin(phi))
                    + (1-d) * (vFollow - circleCenter);
            _vboMesh.getVertices()[index * (_nbPointsOnCircle+1)] = vFollow;
            index ++;

        }
    }
    if(_bStart)
        updateHanabiSeed();
}
//--------------------------------------------------------------
void ofApp::updateHanabiSeed(){
    if(_bBoom)
    {
        float diff = ofGetElapsedTimef() - _timeStart;
        if(diff < 5)
            _hanabiPropagation = diff / 5.;
        else
        {
            // relaunch another seed
            _bStart = false;
            _bBoom = false;
            _hanabiPropagation = 0.;
            setupHanabiSeed(3);
            _bStart = true;

        }
    }
    else if(_hanabiSeed.y > _windowSize.y * 1./3.)
    {

        _hanabiSeed.y -= 15.;
    }
    else
    {
        _timeStart = ofGetElapsedTimef();
        _bBoom = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // ofBackground(255);

    if(!_bStopUpdate)
    {

        if(_bShader)
        {

            _shaderGradient.begin();
            int index = 0;
            _shaderGradient.setUniform1f("circleRadius",_circleRadius);
            _shaderGradient.setUniform4f("colorHanabi",_seedColor.r,_seedColor.g,_seedColor.b,_seedColor.a);
            _shaderGradient.setUniform4f("colorIn",_circleInColor.r,_circleInColor.g,_circleInColor.b,_circleInColor.a);
            _shaderGradient.setUniform4f("colorOut",_circleOutColor.r,_circleOutColor.g,_circleOutColor.b,_circleOutColor.a);//0./255.,0./255.,0./255,1.);

            for(auto & c : _circleCenters)
            {
                const auto & vertex = _vertexFollow[index];
                _shaderGradient.setUniform1f("ratioHanabi",_bBoom ?
                                                 _hanabiPropagation *  (1. - vertex.distance(_hanabiSeed)/_windowSize.x)
                                               : 0.);

                _shaderGradient.setUniform2f("vFollow", vertex.x,
                                             _windowSize.y - vertex.y);

                ofDrawCircle(c,_circleRadius);
                index++;
            }
            _shaderGradient.end();
        }

        _vboMesh.draw();

        // draw seed
        if(_bStart && !_bBoom)
        {

            ofPushStyle();
            ofSetColor(_seedColor);
            ofSetLineWidth(2.);

            ofFill();

            ofDrawCircle(_hanabiSeed,3.);
            float x0 = _hanabiSeed.x;
            float y0 = _hanabiSeed.y;
            float x1 = _hanabiSeed.x+10*sin(5*ofGetElapsedTimef());
            float y1 = _hanabiSeed.y+abs(30*sin(3.*ofGetElapsedTimef()/3.5f));
            float x2 = _hanabiSeed.x-5*sin(5*ofGetElapsedTimef());
            float y2 = _hanabiSeed.y+abs(80*sin(3.*ofGetElapsedTimef()));
            float y3 = _hanabiSeed.y + 100;



            ofNoFill();
            //ofSetHexColor(0xFF9933);
            ofBeginShape();
            ofVertex(x0,y0);
            ofBezierVertex(x1,y1,x2,y2,_hanabiSeed.x,y3);
            ofEndShape();

            ofPopStyle();
        }

    }
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f')
    {

        _bStopUpdate = true;
        ofToggleFullscreen();

        _windowSize.x = ofGetWindowWidth();
        _windowSize.y = ofGetWindowHeight();

        int nbCirclesPerLine =(_windowSize.x )/(_circleRadius*2.);
        int nbCirclesPerColumn = (_windowSize.y )/(_circleRadius*2.);

        setupVbo(nbCirclesPerLine,nbCirclesPerColumn);

        setupHanabiSeed(3);

        _bStopUpdate = false;

    }
    else if (key == 's')
    {
        // fill circles with gradient using shaders
        _bShader = !_bShader;
    }
    else if(key == 'l')
    {
        _bStart = false;
        _bBoom = false;
        setupHanabiSeed(3);
        _bStart = true;

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
