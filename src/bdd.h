/*
 * $Id: bdd.h 100 2007-01-29 11:10:37Z johmathe $
 * $Date: 2007-01-29 12:10:37 +0100 (Mon, 29 Jan 2007) $
 */

#ifndef __SHOTBDD_H__
#define __SHOTBDD_H__

#include <libpq-fe.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include "src/film.h"

// http://docs.postgresqlfr.org/8.1/libpq-exec.html
// http://www.postgresql.org/docs/8.1/interactive/libpq.html#LIBPQ-CONNECT

using namespace std;

class film;

class bdd {
 private:
  film *f;

  PGconn *conn;
  PGresult *result;
  string host;
  string login;
  string base;
  int port;

 public:
  bdd(const string host, const string login, const string pwd, int port,
      const string base);
  ~bdd();
  void init_bdd_values();
  void updatebdd();
};

#endif /* !__SHOTBDD_H__ */
