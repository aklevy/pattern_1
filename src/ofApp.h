#pragma once

#include "ofMain.h"



class ofApp : public ofBaseApp{
public:
    void setup();

    void update();
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
    ofPoint     _origin{0};
    bool        _bStopUpdate{false};
    int         _circleRadius{10};
    int         _nbPointsOnCircle{30};

    ofVec2f     _windowSize{1};

    ofVboMesh   _vboMesh;
    std::vector<ofVec2f>    _circleCenters;

};
