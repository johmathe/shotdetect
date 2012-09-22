# Fichier pour automake
INCLUDES= -I../include -I../resources/ `wx-config --cflags` `xslt-config --cflags`
bin_PROGRAMS = shotdetect
shotdetect_LDADD = -lgd -lz -lavformat -lavcodec -lavutil
shotdetect_LDFLAGS = `wx-config --libs` `xslt-config --libs` 
shotdetect_SOURCES = \
graph.cpp \
film.cpp   \
image.cpp  \
shot.cpp  \
xml.cpp \
wxProcessVideoThread.cpp \
DialogShotDetect_c.cpp \
DialogHelp.cpp \
main.cpp
