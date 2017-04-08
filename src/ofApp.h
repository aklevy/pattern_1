#pragma once

#include "ofMain.h"



class ofApp : public ofBaseApp{
public:
    void setup();
    void setupVbo(int nbCirclesPerLine,int nbCirclesPerColumn);
    void setupHanabiSeed(int nb);

    void update();
    void updateHanabiSeed();

    void draw();

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
    ofVec2f     _hanabiSeed{0};
    bool        _bStart{false};
    ofFloatColor    _seedColor;

    bool        _bBoom{false};
    float       _timeStart{0};
    float       _hanabiPropagation{0};

    bool        _bStopUpdate{false};
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
