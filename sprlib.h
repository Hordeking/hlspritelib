//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//
//		This is the main library, SPRLIB

#include <stdint.h>
#include <iostream>
#include <vector>

#ifndef SPRLIB_SPRIMAGE_H
	#define SPRLIB_SPRIMAGE_H

#include "palette.h"
#include "sprframe.h"

class SprImage{

//Forward Declarations here to keep things decluttered.
protected:

	uint16_t backgroundColor;	//Later use when converting to RGBA. Whatever index is named will be rendered with A0.
	bool sprLoaded;

public:

	enum SpriteType:uint32_t { VP_PARALLEL_UPRIGHT = 0, FACING_UPRIGHT, VP_PARALLEL, ORIENTED, VP_PARALLEL_ORIENTED };
	enum TextureType:uint32_t { SPR_NORMAL = 0, SPR_ADDITIVE, SPR_INDEXALPHA, SPR_ALPHTEST };
	enum SynchronizationType:uint32_t { SYNCHRONIZED = 0, RANDOM };

#pragma pack(push, 1)
	class SprHeader {		//The only thing that should go in here is the actual data in the file header.
	public:
		uint8_t				FileID[4];
		uint32_t			version;
		SpriteType			type;
		TextureType			textureFormat;
		float				boundingRadius;
		uint32_t			maxwidth;
		uint32_t			maxheight;
		uint32_t			nFrames;
		float				beamlength;
		SynchronizationType	syncType;
		uint16_t			nColors;
	};
#pragma pack(pop)

	SprHeader fileHeader;						//Static header data. Can be used directly
	Palette<uint16_t, uint8_t, 3> filePal;
	std::vector<SprFrame> myFrame;

public:

	SprImage();
	SprImage(std::istream & inStream);

	int load(std::istream & inStream);
	size_t GetPaletteSize() { return fileHeader.nColors; }
	void SetBGColor(uint16_t n) { backgroundColor = n; return; }
	std::vector<uint8_t> Color(size_t n) { return filePal.GetColor(n); }

	SprFrame Frame(size_t n){ return myFrame.at(n); }
};

#endif
