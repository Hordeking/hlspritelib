//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//
//		This is the frame object of the SPRLIB.

#include <vector>
#include <iostream>
#include <stdint.h>

#ifndef SPRLIB_SPRFRAME_H
	#define SPRLIB_SPRFRAME_H

class SprFrame {

protected:
	std::vector<uint8_t>	vData;

public:

	uint32_t	group;
	int32_t		originX;
	int32_t		originY;
	uint32_t	width;
	uint32_t	height;
	uint32_t	dataSize;

	SprFrame();
	~SprFrame();

	void SetPixel(size_t n, uint8_t color );
	void SetPixel(size_t row, size_t column, uint8_t color );
	uint8_t Pixel(size_t n);
	uint8_t Pixel(size_t row, size_t column);
	void resize(size_t size);
};

#endif
