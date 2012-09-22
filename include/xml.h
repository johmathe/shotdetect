/*
 * $Id: xml.h 142 2007-04-02 16:18:49Z johmathe $
 * $Date: 2007-04-02 18:18:49 +0200 (Mon, 02 Apr 2007) $
 */

#ifndef __XML_H__
#define __XML_H__

#include <unistd.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "film.h"
#include "shot.h"

#include <string.h>
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/debugXML.h>
#include <libxml2/libxml/HTMLtree.h>
#include <libxml2/libxml/xmlIO.h>
#include <libxml2/libxml/xinclude.h>
#include <libxml2/libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#define MY_ENCODING "ISO-8859-1"

using namespace std;

class film;

class xml
{

private:
  /* Chemin relatif du fichier xml propre à l'application */
  string xml_own_path;
  /* Chemin relatif du fichier xml après application du xsl */
  string xml_out_path;
  /* Chemin de la feuille xsl  */
  xmlChar *ConvertInput (const char *in, const char *encoding);
  film *f;


public:
  string xsl_path;
  string xsl_name;
  void write_data (string &);
  void apply_xsl (string & xml_out);
    xml (film *);
   ~xml ();
 inline void set_xsl_path(const string & arg) { xsl_path = arg ;};
};

#endif /* !__XML_H__ */
