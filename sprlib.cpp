//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//
//		This is the main library, SPRLIB

#include "sprlib.h"

//TODO: Other stuff to make a qc file.

SprImage::SprImage() {

	sprLoaded = false;
}

SprImage::SprImage(std::istream & inStream):SprImage(){

	load(inStream);

}

bool SprImage::load(std::istream & inStream){


	//Read in the header data.
	inStream.read((char *) &fileHeader, sizeof(fileHeader));

	//Need to confirm that the header says "IDSP" and it's version 2.
	if (*(int *) fileHeader.FileID != (('P'<<24)+('S'<<16)+('D'<<8)+'I')) throw "Sprlib: Not a valid sprite file.";

	if (fileHeader.version != 2) throw "Sprlib: Invalid Sprite version. Only version 2 allowed.";

	filePal.SetPaletteSize(fileHeader.nColors);
	for(size_t i = 0; i < fileHeader.nColors; ++i){
		std::vector<uint8_t> color = {0, 0, 0};
		uint8_t buffer[3] = {0};
		inStream.read( (char *) &buffer, sizeof(buffer) );

		color.at(0) = buffer[0];
		color.at(1) = buffer[1];
		color.at(2) = buffer[2];

		filePal.AppendColor(color);
	}


	//Now we have the palette. Let's go ahead and allocate enough frames to hold everything.
	myFrame.reserve(fileHeader.nFrames);

	//Note: each frame can vary in size, so we have to read the frame header before allocating space for the data.
	//Now we read in each frame.
	for(unsigned int i=0;i<fileHeader.nFrames;++i){

		SprFrame tempFrame;

		inStream.read( (char *) &(tempFrame.group), sizeof(tempFrame.group) );
		inStream.read( (char *) &(tempFrame.originX), sizeof(tempFrame.originX) );
		inStream.read( (char *) &(tempFrame.originY), sizeof(tempFrame.originY) );
		inStream.read( (char *) &(tempFrame.width), sizeof(tempFrame.width) );
		inStream.read( (char *) &(tempFrame.height), sizeof(tempFrame.height) );
		tempFrame.dataSize = tempFrame.width * tempFrame.height;
		tempFrame.resize(tempFrame.dataSize);

		//Slurp up that data!
		for(size_t j=0;j<tempFrame.dataSize;++j){
			uint8_t buffer = 0;
			inStream.read( (char *) &buffer, sizeof(buffer) );
			tempFrame.SetPixel(j, buffer);
		}

		myFrame.push_back( tempFrame );

	}

	sprLoaded = true;

	return sprLoaded;
}
