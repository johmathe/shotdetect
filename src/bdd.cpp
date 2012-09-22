/*
 * Copyright (C) 2007 Johan MATHE - johan.mathe@tremplin-utc.net - Centre
 * Pompidou - IRI
 * 
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * * $Id: bdd.cpp 143 2007-04-03 14:43:56Z johmathe $ $Date: 2007-02-21
 * 10:58:30 +0100 (mer, 21 fÃ©v 2007) $ 
 */


#include "bdd.h"


// http://casteyde.christian.free.fr/cpp/cours/online/c5501.html#AEN5702
// http://docs.postgresqlfr.org/8.1/libpq-exec.html
// http://www.postgresql.org/docs/8.1/interactive/libpq.html#LIBPQ-CONNECT
// 
using namespace std;

bdd::bdd (const string host, const string login, const string pwd, int port, const string base)
{
  string str_cx;
  string query;
  str_cx = "host=" + host;
  str_cx += " user=";
  str_cx += login;
  str_cx += " password=";
  str_cx += pwd;
  str_cx += " port=";
  str_cx += port;
  str_cx += " dbname=";
  str_cx += base;

  conn = PQconnectdb (str_cx.c_str ());
  if (PQntuples (result) != 1)
    {
      fprintf (stderr, "query to find oid of geometry didnt return 1 row!");
      exit (1);
    }
  query = "SELECT MAX(id) FROM plan;";
  result = PQexec (conn, query.c_str ());
  f->id_plan = atoi (PQgetvalue (result, 0, 0));
  query = "SELECT * FROM auteur WHERE (prenom='";	// %s') AND
  // (nom='%s')",
  // f->auteur.prenom, film->auteur.nom);
  result = PQexec (conn, query.c_str ());

  /*
   * insertion auteur 
   */

  if (PQntuples (result))
    {
      fprintf (stderr, "L'auteur existe dans la base de données : récupération de l'id\n");
      f->auteur.id = atoi (PQgetvalue (result, 0, 0));
      fprintf (stderr, "ID auteur \t\t: %d\n", f->auteur.id);

    }
  else
    {
      fprintf (stderr, "L'auteur n'existe pas dans la base de données : insertion\n");
      query = "INSERT INTO auteur (prenom,nom) VALUES ('";	// %s','%s'); 
      // SELECT
      // MAX(id) 
      // FROM
      // auteur;",
      // f->auteur.prenom, f->auteur.nom);
      result = PQexec (conn, query.c_str ());
      f->auteur.id = atoi (PQgetvalue (result, 0, 0));
      fprintf (stderr, "ID auteur \t\t: %d\n", f->auteur.id);
    }


  /*
   * relations nn 
   */
  query = "INSERT INTO nn_auteur_film (id_local,id_foreign,priority) VALUES ";	// (%d,%d,0);",
  // f->auteur.id, f->id);
  result = PQexec (conn, query.c_str ());

  /*
   * Titre du film 
   */

  query = "INSERT INTO film_traduction (id,nom_lang,nom) VALUES (";	// %d,'%s','%s');",
  // film->id, film->code_lang, film->title);
  printf ("%s\n", query.c_str ());
  PQexec (conn, query.c_str ());

}

void
bdd::updatebdd ()
{
  int i;
  char query[MAX_QUERY];
  int min, sec, hr;
  int mind, secd, hrd;
  /*
   * Les transaction sont atomiques 
   */
  list < shot >::iterator il;

  for (il = f->shots.begin (); il != f->shots.end (); il++)
    {
      sec = (int) (il->msstart / 1000);
      min = sec / 60;
      hr = min / 60;
      min %= 60;
      sec %= 60;

      secd = (int) (il->msduration / 1000);
      mind = secd / 60;
      hrd = mind / 60;
      mind %= 60;
      secd %= 60;
      snprintf (query, MAX_QUERY, "INSERT INTO plan (id, id_film, timecode, duree, path_image) VALUES (%d,%d,'%d:%d:%d','%d:%d:%d','%d.jpg');INSERT INTO plan_traduction (id,id_film,nom_lang,titre) VALUES (%d,%d,'%s','plan %d');", i, f->id, hr, min, sec, hrd, mind, secd, i, i, f->id, f->code_lang.c_str (), i);
      result = PQexec (conn, query);
      free (result);
      i++;
    }
}

bdd::~bdd ()
{
}
