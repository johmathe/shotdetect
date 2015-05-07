Shotdetect is a free software (LGPL) which detects shots and scenes from a video.
The result produced is a simple XML file containing all the informations about the movie generated.

# Install

## Dependencies
sudo apt-get install libavdevice-dev libavformat-dev libgd2-noxpm libgd2-noxpm-dev
## Graphical version
sudo apt-get install libwxgtk2.6-dev wx2.6-doc libwxgtk2.6-headers

## Building
mkdir -p build
cd  build
cmake ..

## Build command line version
mkdir -p build
cd build
cmake -D USE_WXWIDGETS:BOOL=OFF ..

# Example

## Shell

shotdetect -i myvideo.avi -o output_dir -s 60 -w -v -f -l -m -r

## Options details
-s : set threshold
The threshold is the level for shot detection. High will not detect a lot, low will detect a lot of false shots. A good choice is about 60.

-w : generates audio xml information
See the generated file for more details

-v : generates video xml informations
See the generated file for more details

-f : generates the first image of shots
-l : generates the last image of shots
-m : generates the thumbnails images
-r : generates the real size images

# Comments
johmathe@nonutc.fr
