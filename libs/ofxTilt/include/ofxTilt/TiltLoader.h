#pragma once

#include "ofMain.h"
#include "TiltJson.h"

class TiltLoader {

	public:
		TiltLoader();
		TiltLoader(string fileName);
		virtual ~TiltLoader() {};

		void run();
		bool checkInterval();
		void read(string fileName, bool clearExisting);
		void write(string fileName);

		TiltJson json;

		string fileName = "layer_test";
		float globalScale = 100.0;
		bool drawMesh = false;
		int startTime = 0;
		int lastMillis = 0;
		int timeInterval = 0;
		float fps = 12.0;
		int fpsInterval = int((1.0 / fps) * 1000.0);

		int currentLayer = 0;

};
