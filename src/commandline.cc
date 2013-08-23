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
 * Boston, MA 02110-1301 USA $Id: main.cpp 164 2007-10-13 23:53:21Z johmathe $
 */
#include <stdlib.h>

#include "src/film.h"
#include "src/xml.h"

#define APP_VERSION "2.0.0-git"     // This is also being declared in "main.cc"

class xml;
class film;

//Example
//Put the file you want to analyse in the same directory of shotdetect (it not mandatory but it’s easier with the command line)

//shotdetect -i myvideo.avi -o output_dir -s 75 -w -v -f -l -m -r

//Options details
//-s : set threshold
//The threshold is the level for shot detection. High will not detect a lot, low will detect a lot of false shots. A good choice is about 75.

//-w : generates audio xml informations
//See the generated file for more details

//-v : generates video xml informations
//See the generated file for more details

//-f : generates the first image of shots
//-l : generates the last image of shots
//-m : generates the thumbnails images
//-r : generates the real size images

void
show_help (char **argv)
{
  printf ("\nShotdetect version \"%s\", Copyright (c) 2007-2013 Johan Mathe\n\n"
          "Usage: %s \n"
          "-h           : show this help\n"
          "-n           : commandline mode (disable GUI)\n"
          "-s threshold : threshold (Default=%d)\n"
          "-i file      : input file path\n"
          "-o path      : output path\n"
          "-y year      : set the year\n"
          "-a id        : id of the movie\n"
          "-x xsl path  : path of the xsl stylesheet\n"
          "-w           : generate xml of audio\n"
          "-v           : generate xml of video infos\n"
          "-f           : generate first image for each shot\n"
          "-l           : generate last image for each shot\n"
          "-m           : generate the thumbnail image\n"
          "-r           : generate the images in native resolution\n"
          "-c           : print timecode on x-axis in graph\n",
          APP_VERSION,
          argv[0],
          DEFAULT_THRESHOLD
         );
}



int
main (int argc, char **argv)
{
  film f = film ();
  bool gui = true;
  bool ifile_set = false;
  bool ofile_set = false;
  bool id_set = false;
  bool xsl_path_set = false;
  string xsl_path = "Not set";

  extern char *optarg;
  extern int optind, opterr, optopt;


  // Initialize threshold to a sensible default value
  f.threshold=DEFAULT_THRESHOLD;

  for (;;) {
    int c = getopt (argc, argv, "?ht:y:i:o:a:x:s:flwvmrc");

    if (c < 0) {
      break;
    }

    switch (c) {
    case '?':
    case 'h':
      show_help (argv);
      exit (EXIT_SUCCESS);
      break;

     /* Draw first image of scene cut? */
    case 'f':
      f.set_first_img(true);
      break;

      /* Draw last image of scene cut? */
    case 'l':
      f.set_last_img(true);
      break;

      /* Generate images with native resolution? */
    case 'r':
      f.set_shot(true);
      break;

      /* Generate thumbnails? */
    case 'm':
      f.set_thumb(true);
      break;

      /* Generate XML with video metadata? */
    case 'v':
      f.set_video(true);
      break;

      /* Generate XML with audio metadata? */
    case 'w':
      f.set_audio(true);
      break;

      /* Set the threshold */
    case 's':
      f.set_threshold(atoi (optarg));
      break;

      /* Embed timecode in graph  */
    case 'c':
      f.set_show_timecode(true);
      break;

      /* Id for path creation */
    case 'a':
      f.set_alphaid (optarg);
      id_set = true;
      break;

      /* Set the XSLT path */
    case 'x':
      xsl_path = optarg;
      xsl_path_set = true;
      break;

      /* Set the title */
    case 't':
      f.set_title(optarg);
      break;

      /* Set the year */
    case 'y':
      f.set_year(atoi (optarg));
      break;

      /* Set the input file */
    case 'i':
      f.set_ipath(optarg);
      if (!f.get_ipath().empty()) {
        ifile_set = true;
      } 
      break;

      /* Set the output file */
    case 'o':
      f.set_opath(optarg);
      if (!f.get_opath().empty()) {
        ofile_set = true;
      } 
      break;


    default:
      break;
    }
  }

  // Error handling
  if ( !ifile_set || !ofile_set || !id_set ) {
    if (!ifile_set) {
      cerr << "ERROR: Input filename is missing or empty. See argument '-i'" << endl;
    }
    if (!ofile_set) {
      cerr << "ERROR: Output path is missing or empty. See argument '-o'" << endl;
    }
    if (!id_set) {
      cerr << "ERROR: please specify an alphanumeric id. See argument '-a'" << endl;
    }
    show_help (argv);
    exit (EXIT_FAILURE);
  }

  xml *x = new xml (&f);
  f.x = x;

  f.shotlog("Processing movie.");
  f.process ();
  string xml_path  = f.global_path;
  xml_path += "/result.xml";
  f.x->write_data (xml_path);
  /*string finished_path = f.global_path;
  finished_path += "/finished";
  FILE *fd_finished = fopen(finished_path.c_str(),"w");
  fprintf(fd_finished, "0\n");
  fclose(fd_finished);*/
  exit (0);
}
