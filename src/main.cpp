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
 * Boston, MA 02110-1301 USA $Id: main.cpp 194 2010-09-30 23:35:11Z peterb $ 
 */

#include <film.h>
#include <stdlib.h>
#include <xml.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include "DialogShotDetect_c.h"

#define APP_VERSION "SVN-r$Revision: 194 $-$Author: peterb $"

class xml;
class film;

class ShotApp:public wxApp
{
    public:
        virtual bool OnInit ();

};

    IMPLEMENT_APP_NO_MAIN (ShotApp)
bool ShotApp::OnInit ()
{
    DialogShotDetect_c *DialogShotDetect = new DialogShotDetect_c (0, -1, wxT (""));
    SetTopWindow (DialogShotDetect);
    DialogShotDetect->Show ();
}

void show_help (char **argv)
{
    printf ("\nShotdetect version \"%s\" ($Date: 2010-10-01 01:35:11 +0200 (Fri, 01 Oct 2010) $), Copyright (c) 2007-2010 Johan Mathe\n\n"
            "Usage: %s \n"
            "-h           : show this help\n"
            "-n           : commandline mode (disable GUI)\n"
            "-s threshold : threshold\n"
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
            "-r           : generate the images in native resolution\n",
            APP_VERSION,
            argv[0]
    );
}



int main (int argc, char **argv)
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

    for (;;)
    {
        int c = getopt (argc, argv, "?hnt:y:i:o:a:x:s:flwvmr");

        if (c < 0)
        {
            break;
        }

        switch (c)
        {
            case '?':
            case 'h':
                show_help (argv);
                exit (EXIT_SUCCESS);
                break;

                /* Draw first and/or last image of scene cut? */
            case 'f':
                f.set_first_img(true);
                break;

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

            case 'n':
                gui = false;
                break;

                /* Set the title */
            case 't':
                f.set_title(optarg);
                break;

                /* Set the year */
            case 'y':
                f.set_year(atoi (optarg));
                break;

            case 'i':
                f.set_ipath(optarg);
                ifile_set = true;
                break;

            case 'o':
                f.set_opath(optarg);
                ofile_set = true;
                break;

            default:
                break;
        }

    }

    if (!gui)
    {
        // Error handling
        if ( !ifile_set || !ofile_set || !id_set )
        {
            if (!ifile_set)
            {
                cerr << "please specify an input file" << endl;
                show_help (argv);
            }
            if (!ofile_set)
            {
                cerr << "please specify an output path" << endl;
                show_help (argv);
            }
            if (!id_set)
            {
                cerr << "please specify an alphanumeric id" << endl;
                show_help (argv);
            }
            exit (EXIT_FAILURE);
        }


        xml *x = new xml (&f);
        f.x = x;

        f.shotlog("Processing movie.");
        f.process ();

        string xml_path = f.alphaid;
        xml_path += "_";
        xml_path += f.x->xsl_name;
        xml_path += ".xml";

        string xml_own_path = f.alphaid;
        xml_own_path += ".xml";
        f.x->write_data (xml_own_path);

        // Optionally, apply XSLT: --------------
        if (xsl_path_set)
        {
            x->set_xsl_path (xsl_path);
            // Extract name of XML stylesheet (XSLT) without extension
#ifdef __WINDOWS__
            x->xsl_name.assign (x->xsl_path, x->xsl_path.rfind ("\\") + 1, x->xsl_path.rfind (".") - x->xsl_path.rfind ("\\") - 1);
#else
            x->xsl_name.assign (x->xsl_path, x->xsl_path.rfind ("/") + 1, x->xsl_path.rfind (".") - x->xsl_path.rfind ("/") - 1);
#endif

            f.x->apply_xsl (xml_path);
        }
        // --------------------------------------

        string path = f.global_path;
        path += "/";
        path += f.alphaid ;
        path += "/finished";

        FILE *fd_finished = fopen(path.c_str(),"w");
        fprintf(fd_finished, "0\n");
        fclose(fd_finished);
    }
    else
    {
        /*
         * Enter GUI version
         */
        wxEntry (argc, argv);
        return true;
    }
    exit (0);
}
