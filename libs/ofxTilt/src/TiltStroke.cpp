#include "TiltStroke.h"

TiltStroke::TiltStroke() { }

TiltStroke::TiltStroke(ofColor c) {
    strokeColor = c;
}

TiltStroke::TiltStroke(float s) {
    strokeSize = s;
}

TiltStroke::TiltStroke(ofColor c, float s) {
    strokeColor = c;
    strokeSize = s;
}

TiltStroke::TiltStroke(vector<ofVec3f> pts, ofColor c) {
	points = pts;
	strokeColor = c;
}

void TiltStroke::update() { }

void TiltStroke::draw() {
	glPushMatrix();
	ofScale(globalScale, globalScale, globalScale);
    ofNoFill();
    ofSetLineWidth(strokeSize);
    ofSetColor(strokeColor);
    if (drawMesh) {
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        for (int i=1; i<points.size(); i++) {
            ofVec3f thisPoint = points[i-1];
            ofVec3f nextPoint = points[i];
            
            ofVec3f direction = (nextPoint - thisPoint);
            float distance = direction.length();
            ofVec3f unitDirection = direction.getNormalized();
            ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
            ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
            
            float thickness = ofMap(distance, 0, 60, strokeSize, 2, true);
            ofVec3f leftPoint = thisPoint + toTheLeft * thickness;
            ofVec3f rightPoint = thisPoint + toTheRight * thickness;
            
            mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
            mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
        }
        mesh.draw();
    } else {
        ofBeginShape();
        for (int i=0; i<points.size(); i++) {
            ofVertex(points[i].x, points[i].y, points[i].z);
        }
        ofEndShape();
    }
	glPopMatrix();
}

void TiltStroke::run() {
    update();
    draw();
}
