/*
 * $Id: graph.h 191 2010-09-30 18:52:37Z peterb $
 * $Date: 2010-09-30 20:52:37 +0200 (Thu, 30 Sep 2010) $
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <algorithm>
#include <string>
#include <gd.h>
#include <gdfontl.h>
#include <iostream>

#include "src/film.h"
#define SIZE_DATA 180000
#define JPG 1
#define PNG 2
#define BMP 3


using namespace std;

struct dataframe {
  int global;
  int red;
  int green;
  int blue;
  float hue;
  float saturation;
  float value;
};

class film;

class graph
{

private:

  /* Private properties for class-internal functions */
  gdImagePtr im_colors;
  gdImagePtr im_motion_qty;
  gdImagePtr im_hsv;

  FILE *pngout;
  string filename_motion_qty;
  string filename_colors;
  string filename_hsv;
  string global_path;
  int size;
  string xtitle;
  string ytitle;

  int vline;
  int rline;
  int bline;
  int xoffset;
  int yoffset;
  int size_graduation;
  int grid_size;


  int threshold;

  int color;
  int bgcolor;
  int fmt;
  int xsize;
  int ysize;
  int pointnumber;
  int xaxis_offset;
  bool grid;
  int ptr;
  /* Graph file handles */
  FILE *fdmotion_qty;
  FILE *fdgraph_colors;
  FILE *fdgraph_hsv;
  /* XML file handles */
  FILE *fd_xml;

  film *f;
  vector < dataframe > data;
  /* Graph colors */
  int line_color;
  int background_color;
  int title_color;
  int legend_color;
  int grid_color;


  int red_color;
  int green_color;
  int blue_color;
  int threshold_color;
  int timecode_color;
  void draw_canvas (gdImagePtr im, string title);
  float MAX(float a, float b, float c);
  float MIN(float a, float b, float c);



public:
  void save ();
  void draw_all_canvas ();
  void draw_datas ();
  void draw_color_datas ();
  void init_gd ();
  void rgb_to_hsv(const float r,const float g,const float b, float *h, float *s, float *v );
  void hsv_to_rgb( float *r, float *g, float *b,const float h,const float s,const float v );
  void set_color (int, int, int);
  void write_xml(string filename);
  void set_title (string);
  ~graph ();
  graph (int x, int y, string filename,int threshold, film *farg);
  graph (int threshold, film *farg);
  inline void push_data (int val) {
    dataframe frame;
    frame.global = val;
    frame.red = 0;
    frame.blue = 0;
    frame.green = 0;
    data.push_back (frame);
  }

  inline void push_data (int val, int red, int green, int blue) {
    dataframe frame;
    frame.global = val;
    frame.red = red;
    frame.green = green;
    frame.blue = blue;
    rgb_to_hsv(float(red), float(green), float(blue), &(frame.hue), &(frame.saturation), &(frame.value));
    data.push_back (frame);
  }
};



#endif /* !__GRAPH_H__ */
