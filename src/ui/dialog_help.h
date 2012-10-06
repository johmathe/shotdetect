/*
 * $Id: DialogHelp.h 117 2007-03-05 14:11:26Z johmathe $
 */
#ifndef __DIALOGHELP_H__
#define __DIALOGHELP_H__
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/app.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/listctrl.h>
#include <wx/gauge.h>
#include <wx/statline.h>
#include <wx/icon.h>
#include <wx/frame.h>
#include <wx/dialog.h>
class HelpFrame:public wxDialog
{

public:
  HelpFrame (wxWindow * parent, wxWindowID id, const wxString & title,
             const wxPoint & pos = wxDefaultPosition,
             const wxSize & size = wxDefaultSize,
             long style = wxDEFAULT_FRAME_STYLE);

private:
  wxTextCtrl * m_text;
};

#endif
