/*
 * Copyright (C) 2007 Johan MATHE - johan.mathe@tremplin-utc.net - Centre
 * Pompidou - IRI This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either version
 * 2.1 of the License, or (at your option) any later version. This
 * library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details. You should have received a copy of the GNU
 * Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA $Id: image.cpp 115 2007-03-02 17:13:27Z
 * johmathe $
 */
#include "src/image.h"
#include "src/conf.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

int image::create_img_dir ()
{
  /*
   * Stats open file
   */

  ostringstream str;
  struct stat *buf;
  buf = (struct stat *) malloc (sizeof (struct stat));

  str.str ("");
  str << f->global_path << "/" << f->alphaid;
  ostringstream strthumb, strshot;
  strshot << str.str () << "/shots/";
  strthumb << str.str () << "/thumbs/";

  if (f->display || thumb_set) {
    if (stat (strthumb.str ().c_str (), buf) == -1) {
#if defined(__WINDOWS__ ) || defined(__MINGW32__)
      f->shotlog("Windows version : creating thumbs directory");
      mkdir (strthumb.str ().c_str ());
#else
      f->shotlog("Unix version : creating thumbs directory with 0777");
      mkdir (strthumb.str ().c_str (), 0777);
#endif
    }
  }

  if (f->display || shot_set) {
    if (stat (strshot.str ().c_str (), buf) == -1) {
#if defined(__WINDOWS__ ) || defined(__MINGW32__)
      f->shotlog("Windows version : creating shots directory");
      mkdir (strshot.str ().c_str ());
#else
      f->shotlog("Unix version : creating shots directory with 0777");
      mkdir (strshot.str ().c_str (), 0777);
#endif
    }

  }
  free (buf);
  return 0;
}

int image::SaveFrame (AVFrame * pFrame, int frame_number)
{
  // c->thumb_height set to 84
  int width_s = (THUMB_HEIGHT * this->width) / this->height;
  int height_s = THUMB_HEIGHT;
  FILE *jpgout;
  FILE *minijpgout;
  int y, x;
  ostringstream str;

  /*
   * Pointer to images
   */
  gdImagePtr im;
  gdImagePtr miniim;

  /*
   * Creating image
   */
  if ((void *) (im = gdImageCreateTrueColor (width, height)) == NULL) {
    cerr << "Problem Creating True color IMG" << endl;
    exit (EXIT_FAILURE);
  }

  /*
   * Creating mini image
   */
  if ((void *) (miniim = gdImageCreateTrueColor (width_s, height_s)) == NULL) {
    cerr << "Problem Creating True color IMG" << endl;
    exit (EXIT_FAILURE);
  }


  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      /*
       * concept : pix = r << 16 & g << 8 & b)
       */
      im->tpixels[y][x] = (((*(char *) (pFrame->data[0] + y * pFrame->linesize[0] + x * 3)) << 16) & 0xff0000) | (((*(char *) (pFrame->data[0] + y * pFrame->linesize[0] + x * 3 + 1)) << 8) & 0xff00) | ((*(char *) (pFrame->data[0] + y * pFrame->linesize[0] + x * 3 + 2)) & 0xFF);
    }

  }

  /* Pad numbers to constant string width: */
  char s_id[5];
  char s_frame_number[9];

  sprintf(s_id, "%05d", id);
  sprintf(s_frame_number, "%06d", frame_number);


  /* Creating file and saving it */
  if (f->get_thumb()) {

    /* Name of image file */
    str.str ("");
    if (this->type == BEGIN)
      str << f->alphaid << "/thumbs/" << f->alphaid << "_" << s_id << "-" << s_frame_number << "_in.jpg";
    else
      str << f->alphaid << "/thumbs/" << f->alphaid << "_" << s_id << "-" << s_frame_number << "_out.jpg";

    thumb = str.str ();
    str.str ("");
    str << f->global_path << "/" << thumb;

    /* Prepare file */
    if ((minijpgout = fopen (str.str().c_str (), "wb")) == NULL) {
      cerr << str.str () << endl;
      perror ("shotdetect ");
      exit (EXIT_FAILURE);
    }
    gdImageCopyResized (miniim, im, 0, 0, 0, 0, width_s, height_s, width, height);
    gdImageJpeg (miniim, minijpgout, 90);
    fclose (minijpgout);
  }

  if (f->get_shot()) {
    /* Name of image file */
    str.str ("");
    if (this->type == BEGIN)
      str << f->alphaid << "/shots/" << f->alphaid << "_" << s_id << "-" << s_frame_number << "_in.jpg";
    else
      str << f->alphaid << "/shots/" << f->alphaid << "_" << s_id << "-" << s_frame_number << "_out.jpg";

    img = str.str ();
    str.str ("");
    str << f->global_path << "/" << img;

    /* Prepare file */
    if ((jpgout = fopen (str.str ().c_str (), "wb")) == NULL) {
      cerr << str.str () << endl;
      perror ("shotdetect ");
      exit (EXIT_FAILURE);
    }
    gdImageJpeg (im, jpgout, 90);
    fclose (jpgout);
  }

  gdImageDestroy (im);
  gdImageDestroy (miniim);

  return 0;
}

image::image (film * _f, int _width, int _height, int _id, bool _type, bool _thumb_set, bool _shot_set):shot_set (_shot_set), thumb_set (_thumb_set), type (_type), f (_f), id (_id), height (_height), width (_width)
{
  this->height_thumb = 150;
  this->width_thumb = int (double (150 * width) / double (height));
}
