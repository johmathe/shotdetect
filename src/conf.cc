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
 * Boston, MA 02110-1301 USA $Id: conf.cpp 114 2007-02-21 09:58:30Z
 * johmathe $ $Date: 2007-04-03 16:43:56 +0200 (Tue, 03 Apr 2007) $
 */

#include "src/conf.h"

extern char *conf_file;

conf_data conf;
/*
 * the single conf used while parsing
 */
string_key_value_pair possible_values[] = {
    {"DB_PORT", &conf.portc, 7},
    {"DB_HOST", &conf.host, 7},
    {"DB_LOGIN", &conf.login, 8},
    {"DB_BASE", &conf.base, 7},
    {"DB_PWD", &conf.pwd, 6},
    {"SERVER_ROOT", &conf.serverroot, 11},
    {"IMG_PATH", &conf.img_path, 8},
    {"THUMB_PATH", &conf.thumb_path, 10},
    {"THUMB_HEIGHT", &conf.thumb_heightc, 12},
    {NULL, NULL}};

conf::conf() {
  FILE *fd;
  int size;
  int valuesize;
  char line[1024];
  printf("conf file \t\t: %s\n", conf_file);
  fd = fopen(conf_file, "r");
  if (!fd) {
    perror("open file ");
  }

  if (fd != NULL) {
    while (fgets((char *)line, sizeof(line), fd) != NULL) {
      size = strspn(line, " \t\n\v");
      if (line[size] == '#') {
      } else {
        for (sk = (struct string_key_value_pair *)possible_values;
             sk->sk_key != NULL; sk++) {
          /*
           * Compariaison et récupération de la clé
           */
          if (strncmp(line + size, sk->sk_key, sk->size) == 0) {
            valuesize = strcspn(line + size + sk->size + 1, " \n\t");
            /*
             * Allocation mémoire, bien sur, on fait ca
             * proprement, On checke s'il n'existe pas de
             * valeur par défaut
             */

            if (*(sk->sk_value) != NULL) {
              realloc(*(sk->sk_value), valuesize * sizeof(char) + 1);
            } else {
              *(sk->sk_value) = (char *)malloc(valuesize * sizeof(char) + 1);
            }

            /*
             * Mise en place de la valeur
             */

            strncpy(*(sk->sk_value), line + size + sk->size + 1, valuesize);
            (*(sk->sk_value))[valuesize] = 0;
          }
        }
      }
    }
  }
  if (conf.portc != NULL) {
    conf.port = atoi(conf.portc);
  } else {
    conf.port = DEFAULT_BDD_PORT;
  }
  if (conf.thumb_heightc != NULL) {
    conf.thumb_height = atoi(conf.thumb_heightc);
  } else {
    conf.thumb_height = DEFAULT_THUMB_HEIGHT;
  }
  fprintf(stderr, "thumb size \t\t: %d\n", conf.thumb_height);
  fprintf(stderr, "bdd host :\t\t: %s\nport \t\t\t: %d\n", conf.host,
          conf.port);
  fprintf(stderr, "parsing Conf file \t: OK.\n");
  fclose(fd);
}
