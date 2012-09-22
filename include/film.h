/*
 * $Id: film.h 193 2010-09-30 23:02:58Z peterb $
 * $Date: 2010-10-01 01:02:58 +0200 (Fri, 01 Oct 2010) $
 */
#ifndef __FILM_H__
#define __FILM_H__

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif 

#define THUMB_HEIGHT 84
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

}

#ifdef BDD
#include <postgresql/libpq-fe.h>
#endif
#include <wx/wx.h>

#include "image.h"
#include "shot.h"
#include "xml.h"
#include "graph.h"

#include <string>
#include <iostream>

#define MAX_STR 254
#define MAX_CUTS 200
#define abs(x) ((x) < 0 ? - (x) : (x))
#define HAVE_THREADS
#define MAX_QUERY 400

#define DEFAULT_BDD_PORT 5432
#define DEFAULT_THUMB_HEIGHT 85
#define DEFAULT_THRESHOLD 60

#define RATIO 327
#define MIN_INT -32768
#define MAX_INT 32767



using namespace std;

class image;
class conf;
class shot;
class xml;
class DialogShotDetect_c;
class graph;
class film
{

    private:
        /* Variables d'état */
        bool first_img_set;
        bool last_img_set;
        bool audio_set;
        bool video_set;


        unsigned int ech;
        signed short int minright;
        signed short int maxright;
        signed short int minleft;
        signed short int maxleft;


        /* Various data for ffmpeg */
        AVFormatContext *pFormatCtx;
        AVCodecContext *pCodecCtx;
        AVCodecContext *pCodecCtxAudio;
        AVCodec *pCodec;
        AVCodec *pCodecAudio;
        AVFrame *pFrame;
        AVFrame *pFrameRGB;
        AVFrame *pFrameRGBprev;
        AVPacket packet;

        FILE *fd_xml_audio;

        int samples;
        int data_size;
        uint8_t *ptr;

        int len1;
        int len;
        int samplerate;
        int checknumber;

        /* Mem allocation */
        short *audio_buf;
        signed short int sample_right;
        signed short int sample_left;
        int ret;
        int audioStream;
        int videoStream;

        int samplearg;

        void do_stats (int frame);
        void CompareFrame (AVFrame * pFrame, AVFrame * pFramePrev);
        graph *g;

        void update_metadata();
        void init_xml (string filename);
        int close_xml ();

    public:
        bool thumb_set ;
        bool shot_set ;


        DialogShotDetect_c * dialogParent;
        struct
        {
            int id;
            string name;
            string surname;
        } author;

        struct
        {
            string video;
            string audio;
        } codec;

        struct
        {
            int hours;
            int mins;
            int secs;
            int us;
            double mstotal;
        } duration;

        int myid;
        static int idfilm;

        int progress_state_prev;
        /* Name of the movie */
        string film_name;
        /* Title of the movie */
        string title;
        /* Abstract of the movie */
        string abstract;
        /* Path to the film */
        string input_path;
        /* Film height */
        int height;
        /* Film width */
        int width;
        /* Film fps */
        double fps;
        /* Number of audio chanels */
        int nchannel;
        /* Year of the movie */
        int year;
        /* Shots */
        list < shot > shots;
        /* Prev Score in compare_frame */
        int prev_score;
        /* ID BDD Film */
        int id;
        /* id du plan en cours de traitement */
        int id_plan;
        /* Language code */
        string code_lang;
        /* Processing threshold */
        int threshold;
        /* Alphanumeric ID */
        string alphaid;
        /* Absolute path */
        string global_path;
        /* Percent accomplished */
        double percent;
        /* Showing state started */
        bool show_started;
        xml *x;
        bool display;

        int process ();
        void process_audio ();
        void shotlog(string message);
        void create_main_dir(void);

        /* Constructor */
        film ();
        film (DialogShotDetect_c *);

        /* Accessors */
        inline void set_first_img(bool val) {this->first_img_set = val; };
        inline void set_last_img(bool val) {this->last_img_set = val;   };
        inline void set_audio(bool val) {this->audio_set = val;   };
        inline void set_video(bool val) {this->video_set = val;   };
        inline void set_thumb(bool val) {this->thumb_set = val;   };
        inline void set_shot(bool val) {this->shot_set = val;     };
        inline void set_input_file(string input_file) { this->input_path = input_file; };
        inline void set_threshold(int threshold) { this->threshold=threshold; };
        inline void set_ipath(string path) { this->input_path = path; };
        inline void set_opath(string path) { this->global_path = path; };
        inline void set_year(int year) { this->year=year; };
        inline void set_alphaid(string alphaid) { this->alphaid = alphaid; };
        inline void set_title(string title) { this->title = title; };

        inline bool get_first_img(void) { return this->first_img_set ; };
        inline bool get_last_img(void)  { return this->last_img_set ;  };
        inline bool get_audio(void)     { return this->audio_set ;     };
        inline bool get_video(void)     { return this->video_set ;     };
        inline bool get_thumb(void)     { return this->thumb_set ;     };
        inline bool get_shot(void)      { return this->shot_set ;      };
        inline string get_ipath(void)   { return this->input_path ;    };
        inline string get_opath(void)   { return this->global_path ;   };
        inline double get_fps(void)     { return this->fps ;           };

};


#endif /* !__FILM_H__ */
