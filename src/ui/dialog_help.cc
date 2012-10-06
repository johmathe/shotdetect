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
 * Boston, MA 02110-1301 USA
 */
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/textctrl.h>

#include "src/ui/dialog_help.h"

using namespace std;
HelpFrame::HelpFrame (wxWindow * parent, wxWindowID id, const wxString & title, const wxPoint & position, const wxSize & size, long style):
  wxDialog (parent, id, title, position, size, style)
{
  m_text = new wxTextCtrl (this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  // m_text->LoadFile (_T ("help.txt"));
  Layout ();
}
