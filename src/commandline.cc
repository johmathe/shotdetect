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

class xml;
class film;

//Example
//Put the file you want to analyse in the same directory of shotdetect (it not mandatory but it’s easier with the command line)

//shotdetect -i myvideo.avi -o output_dir -s 60 -w -v -f -l -m -r

//Options details
//-s : set threshold
//The threshold is the level for shot detection. High will not detect a lot, low will detect a lot of false shots. A good choice is about 60.

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
  printf ("usage: %s \n"
          "Shotdetect (IRI - johmathe - $Id: main.cpp 164 2007-10-13 23:53:21Z johmathe $)\n"
          "-h           : show this help\n"
          "-i file      : input file path\n"
          "-o path      : output path\n"
          "-s threshold : threshold\n"
          "-w           : generate xml of waveform\n"
          "-v           : generate xml of video infos\n"
          "-f           : generate first img for each shot\n"
          "-l           : generate last img for each shot\n"
          "-m           : generate the thumb img\n"
          "-r           : generate the real img\n", argv[0]);
}



int
main (int argc, char **argv)
{
  extern char *optarg;
  extern int optind, opterr, optopt;


  film f = film ();

  // Initialize threshold to a sensible default value
  f.threshold=60;

  for (;;) {
    int c = getopt (argc, argv, "?ht:i:o:s:flwvmr");

    if (c < 0) {
      break;
    }

    switch (c) {
    case '?':
    case 'h':
      show_help (argv);
      exit (EXIT_SUCCESS);
      break;

      /*	 choix des thumbs   */
    case 'f':
      f.set_first_img(true);
      break;

    case 'l':
      f.set_last_img(true);
      break;

      /* generer l'image en resolution native */
    case 'r':
      f.set_shot(true);
      break;

      /* generer l'image en miniature */
    case 'm':
      f.set_thumb(true);
      break;

      /* generer le xml pour les donnees video */
    case 'v':
      f.set_video(true);
      break;

      /* generer le xml pour les donnees audio */
    case 'w':
      f.set_audio(true) ;
      break;

      /*  Seuil */
    case 's':
      f.set_threshold(atoi (optarg));
      break;

    case 'i':
      f.set_ipath(optarg);
      break;

    case 'o':
      f.set_opath(optarg);
      break;

    default:
      break;
    }

  }

  /*  Traitement des erreurs */
  if (f.get_ipath().empty()) {
    cerr << "Please specify an input file" << endl;
    show_help (argv);
    exit(EXIT_FAILURE);
  }
  if (f.get_opath().empty()) {
    cerr << "Please specify an output path" << endl;
    show_help (argv);
    exit(EXIT_FAILURE);
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
