#pragma once

#include "ofMain.h"
#include "TiltJson.h"
#include "TiltStroke.h"

class TiltLoader {

	public:
		TiltLoader();
		TiltLoader(string fileName);
		virtual ~TiltLoader() {};

		void read(string fileName);

        string url;
		TiltJson json;
        vector<byte> bytes;
        int numStrokes;
        vector<TiltStroke> strokes;
    
    
    private:
        void parseTilt();
    
        int getUInt(vector<byte> _bytes, int _offset);
        int getInt(vector<byte> _bytes, int _offset);
        float getFloat(vector<byte> _bytes, int _offset);

        int asUInt(vector<byte> _bytes);
        int asInt(vector<byte> _bytes);
        float asFloat(vector<byte> _bytes);

        vector<byte> readEntryAsBytes(string entry);
        string readEntryAsString(string entry);
    
        string getFilePath();

};
