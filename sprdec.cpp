//Name: JD Fenech
//Date: 8/24/2017
//License: GPL3. See LICENSE.md
//
//Desc: Decompiles a halflife sprite file into individual
//		frames and saves them as png/animated gifs.
//		This is the driver file.



#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>
	using namespace std;

#include <png++/png.hpp>
#include <getopt.h>


#include "sprlib.h"
#include "palette.h"

void print_usage(void) {
    cerr << "Usage: "<< "sprdec" <<" [options] <Sprite File>" << endl;
    cerr << endl;
    cerr << "\t-v\t\tVerbose output" << endl;
    cerr << "\t-d\t\tDebugging output. Implies -v" << endl;
	cerr << "\t-b <basename>\tUse a different base name for output." << endl;
	cerr << "\t-q\t\tOutput a .qc file along with frames. (UNIMPLEMENTED)" << endl;
    cerr << "\t-n\t\tDry run, no file output" << endl;
    cerr << endl;
}

//Strip the file extension to get a base name. It only strips the last .ext out.
string basename(string infilename){

	size_t lastindex = infilename.find_last_of(".");

	//Probably should handle the case where a period is the very first character, too.
	if (lastindex == 0) return "default";

	if (lastindex != std::string::npos ){
		//If we found a '.', then do this.
		return infilename.substr(0, lastindex);
	}

	return infilename;
}


int main(int argc, char *argv[]){

	int option = 0;

	bool verbose = false;			// Print more information
	bool debug = false;				// Print debugging information, too. Implies -v
	bool testmode = false;			// Dry run. Don't write files or anything.
	bool defaultbasename = true;	// Use the base name of the input file
	bool transparentBG = false;		// Make the background (last color index) real-transparent in output frames.

	string BaseFileName;	//This is what we're naming the files.

	while ((option = getopt(argc, argv,"vdntqb:")) != -1) {
		switch (option) {
			case 'd' : //Debug mode
				debug = true;
				//break; //d-mode implies verbose. Uncomment the break to disable this behavior
				
			case 'v' : //Verbose mode
				verbose = true;
				break;
				
			case 'n' : //Dry run
				testmode = true;
				break;
				
			case 'b' : //User set the base output file name
				defaultbasename = false;
				BaseFileName = string(optarg);
			//	exit(EXIT_FAILURE);
				break;

			case 'q' : //Output a .qc file as well
				//TODO: Implement this.
				break;

			case 't' : //Enable remove background
				transparentBG = true;
				break;
				
			default: print_usage(); 
				exit(EXIT_FAILURE);
		}
	}

	int idxArgArchiveFile = optind;
	int numEx = argc-optind-1; 	//argc-optind-1 = numEx (the number of files to be extracting.)

	//		if numEx < 0, nothing has been specified at all.
	//		if numEx == 0, only ealib has been specified.
	//		if numEx > 0, numEx files have been specified.
	
	if (numEx < 0) {		//No files specified.
		cerr << "No file specified." << endl;
		print_usage(); 
		exit(EXIT_FAILURE);
	}

	string InFileName = string(argv[idxArgArchiveFile]);
	if (defaultbasename) BaseFileName = basename(InFileName);


	//This is where we input the file
	ifstream infile( InFileName.c_str() );

	//SprImage Myfile(foobar);
	SprImage Myfile;	//Use braces to disambiguate this. Google "Most Vexing Parse". Note: solved by not putting a ().
	Myfile.load(infile);
	infile.close();

	if(verbose){

		cout << endl << "Sprite File Info: " << InFileName << endl;
		//Output file info
		cout << "FileID: \t\t";
		for(unsigned int i=0;i<sizeof(Myfile.fileHeader.FileID);++i) cout << (Myfile.fileHeader.FileID)[i];
		cout << endl;

		cout << "Version\t\t\t" << Myfile.fileHeader.version << endl;
		cout << "Sprite Type\t\t" << Myfile.fileHeader.type << " (";
		   switch(Myfile.fileHeader.type){
			  case Myfile.SpriteType::VP_PARALLEL_UPRIGHT: cout << "VP_PARALLEL_UPRIGHT)"; break;
			  case Myfile.SpriteType::FACING_UPRIGHT: cout << "FACING_UPRIGHT)"; break;
			  case Myfile.SpriteType::VP_PARALLEL: cout << "VP_PARALLEL)"; break;
			  case Myfile.SpriteType::ORIENTED: cout << "ORIENTED)"; break;
			  case Myfile.SpriteType::VP_PARALLEL_ORIENTED: cout << "VP_PARALLEL_ORIENTED)"; break;
			  default: cout << "Unknown)";
		   }; cout << endl;
		cout << "Texture Format\t\t" << Myfile.fileHeader.textureFormat << " (";
		   switch(Myfile.fileHeader.textureFormat){
			  case Myfile.TextureType::SPR_NORMAL: cout << "SPR_NORMAL)"; break;
			  case Myfile.TextureType::SPR_ADDITIVE: cout << "SPR_ADDITIVE)"; break;
			  case Myfile.TextureType::SPR_INDEXALPHA: cout << "SPR_INDEXALPHA)"; break;
			  case Myfile.TextureType::SPR_ALPHTEST: cout << "PR_ALPHTESTD)"; break;
			  default: cout << "Unknown)";
		   }; cout << endl;

		cout << "Bounding Radius\t\t" << Myfile.fileHeader.boundingRadius << endl;
		cout << "MaxWidth\t\t" << Myfile.fileHeader.maxwidth << endl;
		cout << "MaxHeight\t\t" << Myfile.fileHeader.maxheight << endl;
		cout << "nframes\t\t\t" << Myfile.fileHeader.nFrames << endl;
		cout << "Beam Len\t\t" << Myfile.fileHeader.beamlength << endl;
		cout << "Sync Type\t\t" << Myfile.fileHeader.syncType << " (";
		   switch(Myfile.fileHeader.syncType){
			  case Myfile.SynchronizationType::SYNCHRONIZED: cout << "SYNCHRONIZED)"; break;
			  case Myfile.SynchronizationType::RANDOM: cout << "RANDOM)"; break;
			  default: cout << "Unknown)";
		   }; cout << endl;
		cout << "Palette Size\t\t" << Myfile.fileHeader.nColors << endl;
		cout << endl;

		if ( true || !defaultbasename ) cout << "Output frame name(s): " << BaseFileName << "_nnnnn" << endl << endl;
	}

	
	//At this point, we've read in the whole file. Let's kick out the frames in PNG!
	for(size_t n=0;n<Myfile.fileHeader.nFrames;++n){

		//Output frame info.
		if (debug){
			cout << "Frame " << n+1 << endl
				<< "\t.group =\t"	<< Myfile.Frame(n).group		<< "\tSize:\t" << sizeof(Myfile.Frame(n).group) << endl
				<< "\t.originX =\t"	<< Myfile.Frame(n).originX	<< "\tSize:\t" << sizeof(Myfile.Frame(n).originX) << endl
				<< "\t.originY =\t"	<< Myfile.Frame(n).originY	<< "\tSize:\t" << sizeof(Myfile.Frame(n).originY) << endl
				<< "\t.width =\t"	<< Myfile.Frame(n).width		<< "\tSize:\t" << sizeof(Myfile.Frame(n).width) << endl
				<< "\t.height =\t"<< Myfile.Frame(n).height		<< "\tSize:\t" << sizeof(Myfile.Frame(n).height) << endl
				<< endl;
		}

		//First, declare the image
		png::image<png::rgba_pixel> frame(Myfile.Frame(n).width, Myfile.Frame(n).height);

/*		//Create the palette object for png++
		png::palette pal( Myfile.GetPaletteSize() );

		//Load the palette into png++ palette.
		for(size_t i=0;i<Myfile.GetPaletteSize();++i)
			pal[i] = png::color(Myfile.Color(i).at(0), Myfile.Color(i).at(1), Myfile.Color(i).at(2) );

		//Apply the png++ palette to our image object.
		frame.set_palette(pal);
*/

		for(size_t i=0;i<Myfile.Frame(n).height;++i)
			for(size_t j=0;j<Myfile.Frame(n).width;++j)
		{
			size_t transparent_color = Myfile.GetPaletteSize()-1;	//Last color in the palette
			uint8_t pixval = Myfile.Frame(n).Pixel(Myfile.Frame(n).width*i+j);
			frame[i][j] = png::rgba_pixel( 
											Myfile.Color(pixval).at(0),
											Myfile.Color(pixval).at(1),
											Myfile.Color(pixval).at(2),
											(pixval == transparent_color)&&transparentBG?0x00:0xFF	//RGBA version
		);//
			//frame[i][j] = Myfile.Frame(n).Pixel(Myfile.Frame(n).width*i+j);	//index_pixel version
		}

		stringstream bar;

		//File Frame numbers start at 1.
		bar << BaseFileName << "_" << setfill('0') << setw(5) << n+1 << ".png";

		if (!testmode) frame.write(bar.str().c_str());

	}

	return 0;
}
