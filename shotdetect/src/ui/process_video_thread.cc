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
 * Boston, MA 02110-1301 USA $Id: wxProcessVideoThread.cpp 139 2007-03-29
 * 14:37:40Z johmathe $
 */
#include "process_video_thread.h"

void* wxProcessVideoThread::Entry() {
  list<film>::iterator il;
  for (il = films->begin(); il != films->end(); il++) {
    (*il).process();
    string xml_path = (*il).alphaid;
    xml_path += "_";
    xml_path += (*il).x->xsl_name;
    xml_path += ".xml";
    string xml_own_path = (*il).alphaid;
    xml_own_path += ".xml";
    (*il).x->write_data(xml_own_path);
    (*il).x->apply_xsl(xml_path);
  }

  wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, ID_CMD_LAUNCHTHREAD);
  wxPostEvent(dialogParent, event);
  return NULL;
}

wxProcessVideoThread::wxProcessVideoThread(
    wxThreadKind kind)
    : wxThread(kind) {}
