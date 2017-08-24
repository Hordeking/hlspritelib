//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//
//		This is the frame object of the SPRLIB

#include "sprframe.h"

SprFrame::SprFrame() { }

SprFrame::~SprFrame() { }

void SprFrame::SetPixel(size_t n, uint8_t color ) { vData.at(n) = color; return; }

void SprFrame::SetPixel(size_t row, size_t column, uint8_t color ) { vData.at(width*row + column) = color; return; }

uint8_t SprFrame::Pixel(size_t n) { return vData.at(n); }

uint8_t SprFrame::Pixel(size_t row, size_t column) { return vData.at(width*row + column); }

void SprFrame::resize(size_t size) { vData.resize(size); return; }
