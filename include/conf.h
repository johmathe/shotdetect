/* 
 * $Id: conf.h 100 2007-01-29 11:10:37Z johmathe $
 * $Date: 2007-01-29 12:10:37 +0100 (Mon, 29 Jan 2007) $
 */


#ifndef __CONF_H__
#define __CONF_H__

#include "film.h"

class conf
{

public:
/* Config struct */
  string portc;
  string host;
  string login;
  string base;
  string pwd;
  string serverroot;
  string img_path;
  string thumb_path;
  string thumb_heightc;
  int thumb_height;
  int port;


    conf ();
   ~conf ();

};

#endif /* !__CONF_H__ */
