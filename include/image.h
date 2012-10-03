/* 
 * $Id: image.h 194 2010-09-30 23:35:11Z peterb $
 * $Date: 2010-10-01 01:35:11 +0200 (Fri, 01 Oct 2010) $
 */


#ifndef __IMAGE_H__
#define __IMAGE_H__

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif 

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string.h>
#include <string>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "film.h"

#include <gd.h>
#ifdef WXWIDGETS
#include <wx/wx.h>
#endif

#define BEGIN true
#define END false
using namespace std;

class conf;
class film;


class image
{
  film *f;
  bool thumb_set;
  bool shot_set;
public:
  int width;
  int height;
  int width_thumb;
  int height_thumb;
  string thumb;
  string img;
  int id;
  bool type;			// BEGIN || END
  int SaveFrame (AVFrame * pFrame, int frame_number);
  int create_img_dir ();
  image (film *, int, int, int, bool, bool, bool);

};

#endif /* !__IMAGE_H__ */
