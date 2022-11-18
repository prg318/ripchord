# ripchord (with Linux and CLAP support)

Ripchord is a free MIDI plugin for Mac and PC that makes it easy to create, perform and remix chord progressions. The plugin manual and download can be found here: https://trackbout.com/ripchord

# Linux Support

This fork updates the build files to work on Linux.  There are two different
methods for building on Linux:  Projucer/make or FRUT/cmake.

## Projucer

Clone this repo and open the `Ripchord.jucer` file with `Projucer` (the official
JUCE project tool).

Next, save the project and close Projucer to generate the Makefiles.  You may
need to enable the JUCE splash screen if you aren't logged into a privileged
account.

Build using the generated Makefile:

    cd ripchord/Builds/LinuxMakefile
    make

## FRUT and cmake

FRUT (https://github.com/McMartin/FRUT) is a tool that can convert a Projucer
project to a cmake project.  FRUT was used to generate the starting point for
the `CMakeLists.txt` in this repo with this command:

    /path/to/FRUT/prefix/FRUT/bin/Jucer2CMake juce6 /path/to/ripchord/Ripchord.jucer

You'll need to manually set `CFLAGS` and `CXXFLAGS` to include `-fPIC`, or else
the linkage will fail.  The `CMakeLists.txt` can probably be patched to address
this, but I haven't looked into it yet.  Use the following to build:

    cd ripchord
    mkdir build && cd build
    export CFLAGS=-fPIC
    export CXXFLAGS=-fPIC
    cmake ../
    make

## CLAP Support

ripchord can be built as a CLAP unit with the help of https://github.com/free-audio/clap-juce-extensions

Follow the instructions on the clap-juce-extensions README and modify the
included `CMakeLists-CLAP.txt` file to build CLAP.

# open source

Ripchord is open source under the GNU General Public License v3.0. The license can be found here: https://github.com/trackbout/ripchord/blob/master/LICENSE
