# TestSamples
Trivial audio sample player for Amiga with Vampire V4 hardware

Simple test program to play different kinds of samples on a vampirized Amiga.<br>
Debug binary prints information about audio sample, you'd better try it first.<br>
Tested on Amiga 500 with Firebird and OS3.2.1.

Usage: SAMPLE,PAULA/S,VOLUME/K/N

  * SAMPLE: if set, tries to load and play this sample file, else enters interactive mode
  * PAULA: if set, tries to use original Paula hardware to play the sample
  * VOLUME: if set, is followed by a volume value which will be constrained to a byte value

Includes basic decoding of WAV, AIFF and 8SVX files and can also play RAW ones.<br>
Written in C, built with vbcc 0.9h and SDK3.2R4.

Feel free to reuse, get inspired or ignore, it's just knowledgeware.
