#!/usr/bin/env bash

# Setup for Ubuntu 18.04, 18.10, 19.04

# standard'ish GNU tools
sudo apt-get install libtool # stops configure.ac AC_ENABLE_SHARED, ... probs

# Other items seems required by ubuntu 17.10
sudo apt-get install nasm
sudo apt install libfontconfig1-dev intltool libxft-dev


# mandatory formats 
sudo apt-get install \
	libogg-dev \
	libvorbis-dev \
	libdv-dev \
	libtheora-dev \
	libjpeg-dev \
	libfaac-dev \
	libfaad-dev \
	libtiff-dev \
	libx264-dev \
	libfftw3-dev \
	libopenexr-dev \
	uuid-dev \
	libmjpegtools-dev \
	liba52-dev \
	libmp3lame-dev \
	libsndfile-dev 

#optional formats
sudo apt-get install \
	libraw1394-dev \
	libiec61883-dev \
	libavc1394-dev

# optional, but you might as well:
sudo apt-get install \
	libqt4-opengl-dev \
	libopencv-dev
	
# added 30-Jun-2018
sudo apt install \
	libasound-dev \
	libxv-dev \
	ffmpeg \
	ffmpeg-doc
	
