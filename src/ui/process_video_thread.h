/*
 * $Id: wxProcessVideoThread.h 117 2007-03-05 14:11:26Z johmathe $
 * $Date: 2007-03-05 15:11:26 +0100 (Mon, 05 Mar 2007) $
 */
#define Uses_wxThread
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/wx.h>

#include "src/film.h"
#include "src/ui/dialog_shotdetect.h"

#ifndef __WXPROCESSVIDEOTHREAD_H__
#define __WXPROCESSVIDEOTHREAD_H__


//http://www.dent.med.uni-muenchen.de/~wmglo/wxthread/CompleteExample.html


class wxProcessVideoThread:public wxThread
{
private:
  list < film > *films;
  void *Entry ();

public:
  wxWindow * dialogParent;

  void *Create (wxWindow * d, list < film > *films) {
    this->films = films;
    dialogParent = d;
    wxThread::Create ();
  };

  wxProcessVideoThread (wxThreadKind kind);

};



#endif
