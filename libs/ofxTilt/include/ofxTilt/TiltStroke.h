#pragma once
#include "ofMain.h"

class TiltStroke {
    
    public:
        TiltStroke();
        TiltStroke(ofColor c);
        TiltStroke(float s);
        TiltStroke(ofColor c, float s);
        TiltStroke(vector<ofVec3f> pts, ofColor c);
		virtual ~TiltStroke(){};

        void update();
        void draw();
        void run();
    
        vector<ofVec3f> points;
        float strokeSize = 4.0;
        ofColor strokeColor = ofColor(0);
        int splitReps = 2;
        int smoothReps = 10;
        bool drawMesh = false;
		float globalScale = 1.0;

};
