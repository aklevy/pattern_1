#pragma once

#include "ofMain.h"



class ofApp : public ofBaseApp{
public:
    enum STATE{FOLLOW_MOUSE ,LAUNCH,EXPLODE,DISAPPEAR};

    void setup();
    void setupVbo(int nbCirclesPerLine,int nbCirclesPerColumn);
    void setupHanabiSeed();

    void update();
    void updateHanabiSeed();

    void draw();

    void switchState(STATE next);
    void toggleMouseMode();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


private:
    STATE       _state{FOLLOW_MOUSE};

    // background color
    float       _lerpBgColor{1.};
    bool        _bChangeBgColor{false};

    // hanabi seed
    ofVec2f     _hanabiSeed{-1000};
    float       _seedLighRadius{650};
    ofFloatColor    _seedColor;

    float       _angle{0};
    float       _timeStart{0};
    float       _hanabiPropagation{0};

    int         _circleRadius{50};
    int         _nbPointsOnCircle{30};

    float       _dist{500};

    ofShader    _shaderGradient;
    bool        _bShader{false};
    ofVec2f     _windowSize{1};

    ofVboMesh   _vboMesh;
    std::vector<ofVec2f>    _circleCenters;
    std::vector<ofVec2f>    _vertexFollow;

    ofFloatColor    _circleInColor;
    ofFloatColor    _circleOutColor;

};
