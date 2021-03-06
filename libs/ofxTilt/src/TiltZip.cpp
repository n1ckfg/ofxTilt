/*
based on ofxZipArchive
Created by Jeffrey Crouse on 3/19/15.
Copyright (c) 2015 __MyCompanyName__. All rights reserved.

https://github.com/jeffcrouse/ofxZipArchive/blob/master/LICENSE
The MIT License (MIT)

Copyright (c) 2015 Jeff Crouse

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "TiltZip.h"

typedef Poco::Delegate<TiltZipArchiveHandler, ZipErrorInfo> error_handler;
typedef Poco::Delegate<TiltZipArchiveHandler, ZipOkInfo> ok_handler;
typedef Poco::Delegate<TiltZipArchiveHandler, ZipDoneInfo> done_handler;


// ------------------------------------------------------
bool TiltZip::compress(string folderPath, string zipPath, bool recursive, bool excludeRoot, Poco::Zip::ZipCommon::CompressionLevel cl) {
    
    folderPath = ofToDataPath(folderPath);
    zipPath = ofToDataPath(zipPath);
    
    ofLogNotice() << "Compressing " << folderPath << " to " << zipPath;
    
    std::ofstream outfile(zipPath.c_str(), ios::binary);
    if (!outfile.good()) {
        ofLogError("TiltZip") << "Couldn't open " << zipPath;
        return false;
    }
    
    Poco::Zip::Compress c(outfile, true);
    
    TiltZipArchiveHandler handler;
    c.EDone += done_handler(&handler, &TiltZipArchiveHandler::onDone);
    
    c.addRecursive(Poco::Path(folderPath), cl, excludeRoot);
    c.close();
    c.EDone -= done_handler(&handler, &TiltZipArchiveHandler::onDone);
    
    return handler.isSuccessful;
}


// ----------------------------------------------------------
bool TiltZip::open(string zipPath) {
    zipPath = ofToDataPath(zipPath);
    ofLogNotice("TiltZip") << "Opening " << zipPath;
    
    infile.open(zipPath.c_str());
    if (!infile.good()) {
        ofLogError("TiltZip") << "Couldn't open " << zipPath;
        return false;
    }
    bOpened = true;
    return true;
}

// ----------------------------------------------------------
vector<string> TiltZip::list() {
    vector<string> files;
    if (!bOpened) {
        ofLogWarning("TiltZip") << "Archive not opened";
        return files;
    }
    
    infile.clear() ;
    infile.seekg(0, ios::beg);
    Poco::Zip::ZipArchive arch(infile);
    Poco::Zip::ZipArchive::FileInfos::const_iterator it;
    for (it = arch.fileInfoBegin(); it != arch.fileInfoEnd(); it++) {
        string fname = it->first;
        Poco::Zip::ZipFileInfo info = it->second;
        files.push_back( fname );
        ofLogNotice("TiltZip") << fname;
    }
    return files;
}


// ----------------------------------------------------------
ofBuffer TiltZip::getFile(string fileName) {
    if (!bOpened) {
        ofLogWarning("TiltZip") << "Archive not opened";
        return ofBuffer();
    }
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    // Find a file within the archive
    Poco::Zip::ZipArchive arch(infile);
    Poco::Zip::ZipArchive::FileHeaders::const_iterator it = arch.findHeader(fileName);
    if (it == arch.headerEnd()) {
        ofLogError() << fileName << " doesn't exist in archive";
        return ofBuffer();
    }
    
    ofLogNotice() << "Uncompressing " << it->second.getFileName() << " size = " << it->second.getUncompressedSize();
    
    Poco::Zip::ZipInputStream zipin (infile, it->second);
    
    ofBuffer buf(zipin);
    return buf;
}

// ----------------------------------------------------------
bool TiltZip::unzipTo(string destination) {
    destination = ofToDataPath(destination);
    
    if (!bOpened) {
        ofLogWarning("TiltZip") << "Archive not opened";
        return false;
    }
    infile.clear() ;
    infile.seekg(0, ios::beg);
    
    ofLogNotice("TiltZip") << "Unzipping archive to " << destination;
    
    Poco::Path d(destination);
    Poco::Zip::Decompress de(infile, d);
    
    TiltZipArchiveHandler handler;
    
    de.EError += error_handler(&handler, &TiltZipArchiveHandler::onError);
    de.EOk += ok_handler(&handler, &TiltZipArchiveHandler::onOk);
    
    de.decompressAllFiles();
    
    de.EError -= error_handler(&handler, &TiltZipArchiveHandler::onError);
    de.EOk -= ok_handler(&handler, &TiltZipArchiveHandler::onOk);
    
    return handler.isSuccessful;
}
