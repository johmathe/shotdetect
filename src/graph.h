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

#define IM_CANVAS 0
#define IM_MOTION_QTY 1
#define IM_RGB_COLORS 2
#define IM_HSV_COLORS 3


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

/* Private properties for class-internal functions */
private:

  /* GD Image pointers */
  gdImagePtr im_motion_qty;
  gdImagePtr im_colors_rgb;
  gdImagePtr im_colors_hsv;
  gdImagePtr im_colors_yuv;

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

  /* Pixel color struct */
  struct pixel_color {
    int c1;
    int c2;
    int c3;
  };

  struct pixel_true_color {
    float c1;
    float c2;
    float c3;
  };

  /* Create variables for storing color values */
  vector < pixel_color > colors_yuv;
  vector < pixel_color > colors_rgb;
  vector < pixel_true_color > colors_hsv;

  /* Graph colors */
  struct graph_color {
    int background;
    int line;
    int title;
    int legend;
    int grid;
    int red;
    int green;
    int blue;
    int threshold;
    int timecode;
  };
  vector < graph_color > graph_colors;

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
  ~graph ();
  graph (int x, int y, string filename,int threshold, film *farg);
  graph (int threshold, film *farg);

  inline void push_data (int val) {
    dataframe frame;
    frame.global = val;
    data.push_back (frame);
  }

  inline void set_color(graph_color graph_color) {
    graph_colors.push_back (graph_color);
  }

  inline void push_yuv(int cy, int cu, int cv) {
    pixel_color yuv_components;
    yuv_components.c1 = cy;
    yuv_components.c2 = cu;
    yuv_components.c3 = cv;
    colors_yuv.push_back (yuv_components);
  }

  inline void push_rgb(int red, int green, int blue) {
    pixel_color rgb_components;
    rgb_components.c1 = red;
    rgb_components.c2 = green;
    rgb_components.c3 = blue;
    colors_rgb.push_back (rgb_components);
  }

  inline void push_rgb_to_hsv(int red, int green, int blue) {
    pixel_true_color hsv_components;
    rgb_to_hsv(float(red), float(green), float(blue), &(hsv_components.c1), &(hsv_components.c2), &(hsv_components.c3));
    colors_hsv.push_back (hsv_components);
  }

};



#endif /* !__GRAPH_H__ */
