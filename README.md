Halflife Sprite Decompiler
*****
This is a basic decompiler for Halflife (and probably Quake III, HL2, TF2, etc) sprite (.spr) files.

It renders the individual frames in a more modern format (essentially they're just pixelmap index frames with a common palette, this is a bit similar to how an animated gif is constructed)
It should also output a .qc file to go with them. Everything will be in your current directory. Be careful, if there are a lot of frames, it will make a mess.

Compiling
*********

You need libpng++ 2.5.1 for this. It's probably available in whatever package repository your OS distro uses. If you're on Windows, you'll have to install it yourself.

At a terminal window, enter:

	make sprdec

to build the decompiler.

If you just want to build the library, make sprlib.a


Reporting Bugs
**************

   We are always interested in hearing about Sprite Decompiler bugs.  Please send mail to `hordeking@users.noreply.github.com' and tell us about them.  Please include at least the following information:

   * your operating system

   * your host processor type

   * your C++ and C compiler make and version

   * the build of SprDec you are using

   * how you configured SprDec

   * how to trigger the bug

   * what you expected to see

   * how what you saw differed from what you expected to see

   * how you think it could be fixed (send a patch if you have one)

