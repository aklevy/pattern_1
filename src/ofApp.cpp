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
    _seedColor  = ofFloatColor(195./255.,57./255.,3./255.);

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
void ofApp::setupHanabiSeed()
{
    _seedColor  = ofFloatColor(ofRandom(0.5,1.0),ofRandom(0.2,0.6),ofRandom(0.,0.2));//195./255.,57./255.,3./255.);

    _hanabiPropagation = 0.;

    _hanabiSeed.set(ofRandom(50, (_windowSize.x - 50)),_windowSize.y);


}

//--------------------------------------------------------------
void ofApp::update(){
    float fps= ofGetFrameRate();
    ofSetWindowTitle(ofToString(fps) );

    updateHanabiSeed();

    // choose a random point on the circle


    float phi = 0;
    ofVec2f zero(1, 0);
    ofVec2f mousePos = (_state == FOLLOW_MOUSE) ? ofVec2f(ofGetMouseX(),ofGetMouseY())
                                                : ofVec2f(_hanabiSeed.x,_hanabiSeed.y);


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
        phi += _angle;

        vFollow =  circleCenter + d*_circleRadius *  ofVec2f(cos(phi),sin(phi))
                + (1-d) * (vFollow - circleCenter);
        _vboMesh.getVertices()[index * (_nbPointsOnCircle+1)] = vFollow;
        index ++;

    }


}
//--------------------------------------------------------------
void ofApp::updateHanabiSeed(){

    if(_state == LAUNCH)
    {
        if(_hanabiSeed.y > _windowSize.y * 1./3.)
        {

            _hanabiSeed.y -= 15.;
        }
        else
        {
            switchState(EXPLODE);
        }
    }
    if(_state == EXPLODE)
    {
        if(_hanabiPropagation < 1.)
        {
            _hanabiPropagation = (ofGetElapsedTimef() - _timeStart) / 10. +0.5;
            _angle =  ofClamp(45. * (_hanabiPropagation - 0.5),0.,1.)* M_PI ;
            _angle -= /*_hanabiPropagation > 0.6*/(_angle -M_PI) < 0.001  ?
                        ofMap(_hanabiPropagation,1./45. + 0.5,0.9,0.,1.0,true) * M_PI : 0;

        }
        else
        {
            switchState(DISAPPEAR);
        }
    }
    if(_state == DISAPPEAR)
    {
        if (_hanabiPropagation >= 1.1)
        {

            switchState(LAUNCH);

        }
        else
        {
            _hanabiPropagation = (ofGetElapsedTimef() - _timeStart) / 10. +0.5;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw()
{
   if(_bChangeBgColor && _state == EXPLODE)
    {
        ofFloatColor res((1.-_hanabiPropagation)* _circleInColor.r + _hanabiPropagation* _circleOutColor.r,
                         (1.-_hanabiPropagation)* _circleInColor.g + _hanabiPropagation* _circleOutColor.g,
                         (1.-_hanabiPropagation)* _circleInColor.b + _hanabiPropagation* _circleOutColor.b);
        ofBackground( res );
        _lerpBgColor = 0.;
    }
    else
    {
        ofColor bgColor = ofColor::black;
        if(_lerpBgColor < 1.0)
        {
            bgColor = ofGetBackgroundColor();
            _lerpBgColor +=0.01;
            bgColor.lerp(ofColor::black,_lerpBgColor) ;
        }

        ofBackground(bgColor);
    }

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
            _shaderGradient.setUniform1f("ratioHanabi",
                                         _state == DISAPPEAR ? (_hanabiPropagation-1.) :
                                                               _state == EXPLODE ?
                                                                   2.*(1.-_hanabiPropagation) //* 5.* (1. - vertex.distance(_hanabiSeed)/(_seedLighRadius*0.1))
                                                                 :2. *(1. - ofClamp(vertex.distance(_hanabiSeed)/_seedLighRadius,0.,1.))) ;

            _shaderGradient.setUniform2f("vFollow", vertex.x,
                                         _windowSize.y - vertex.y);

            ofDrawCircle(c,_circleRadius);
            index++;
        }
        _shaderGradient.end();
    }

    _vboMesh.draw();

    // draw seed
    if(_state == LAUNCH)
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

//--------------------------------------------------------------
void ofApp::switchState(STATE next)
{
    switch(next)
    {
    case FOLLOW_MOUSE:
    {
        break;
    }
    case LAUNCH:
    {
        setupHanabiSeed();
        break;
    }
    case EXPLODE:
    {

        _timeStart = ofGetElapsedTimef();

        break;
    }
    case DISAPPEAR:
    {
        break;
    }
    }
    _state = next;
    ofLog() << "switched to state "<< _state;

}
//--------------------------------------------------------------
void ofApp::toggleMouseMode()
{
    switchState(_state == FOLLOW_MOUSE ? LAUNCH : FOLLOW_MOUSE);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f')
    {

        ofToggleFullscreen();

        _windowSize.x = ofGetWindowWidth();
        _windowSize.y = ofGetWindowHeight();

        int nbCirclesPerLine =(_windowSize.x )/(_circleRadius*2.);
        int nbCirclesPerColumn = (_windowSize.y )/(_circleRadius*2.);

        setupVbo(nbCirclesPerLine,nbCirclesPerColumn);


    }
    else if (key == 's')
    {
        // fill circles with gradient using shaders
        _bShader = !_bShader;
    }
    else if(key == 'l')
    {

        switchState(LAUNCH);

    }
    else if(key =='b')
    {
        _bChangeBgColor = !_bChangeBgColor;
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
    toggleMouseMode();
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
