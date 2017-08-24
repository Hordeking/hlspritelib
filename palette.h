//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//
//		This is the generic palette object.

#include <vector>

#ifndef PALETTE_H
	#define PALETTE_H

template <class S, class T, unsigned int U = 3>
class Palette {

	protected:
		S nColors;
		std::vector<std::vector<T>> vColors;

	public:
		int AppendColor(std::vector<T> palColor);
		std::vector<T> GetColor(int index);
		bool PaletteLoaded();
		S SetPaletteSize(S size);
		S GetPaletteSize();
};

//S is whatever type we care to set the palette size to.
//T is the type that the individual R/G/B elements have. Eg if R,G,&B are each 8b, then this is a uchar/uint8_t...
//U is how many elements per color.

template <class S, class T, unsigned int U>
S Palette<S, T, U>::SetPaletteSize(S size) {
	nColors = size;
	vColors.reserve(nColors);
	return nColors;
}

template <class S, class T, unsigned int U>
S Palette<S, T, U>::GetPaletteSize() {
	return nColors;
}

template <class S, class T, unsigned int U>
bool Palette<S, T, U>::PaletteLoaded() {
	return ( vColors.size() >= GetPaletteSize() );	//vColors should never actually get to the > condition.
}

template <class S, class T, unsigned int U>
int Palette<S, T, U>::AppendColor(std::vector<T> palColor){

	if( PaletteLoaded()  ) return -1;	//Shouldn't shove any more into this palette, it's "full". Exception?

	if( palColor.size()<U ) return -1;	//Fix this to throw an exception? I need to learn exceptions.

	vColors.push_back(palColor);
	return 0;
}

template <class S, class T, unsigned int U>
std::vector<T> Palette<S, T, U>::GetColor(int index){
	return vColors.at(index);
}


//TODO: Write function to overwrite a color at a specific index.
//TODO: Write function to insert a color at a specific index, in front of the index.
//TODO: Write function to delete a color at a specific index.

#endif //PALETTE_H
