#!/bin/sh

# Exécuter ce fichier pour lancer la configuration

srcdir=`dirname $0`

test -z "$srcdir" && srcdir=.

PKG_NAME="squelette"

.$srcdir/macros/autogen.sh
