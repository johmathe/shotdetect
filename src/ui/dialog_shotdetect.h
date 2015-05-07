#ifndef DIALOGSHOTDETECT_C_H
#define DIALOGSHOTDETECT_C_H

#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/app.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/listctrl.h>
#include <wx/gauge.h>
#include <wx/statline.h>
#include <wx/icon.h>
#include <sys/time.h>

#include <list>
#include <iterator>
#include <algorithm>

enum {
  ID_AJOUTER = 1,
  ID_ENLEVER,
  ID_VIDER,
  ID_PARCOURIR,
  ID_PARCOURIR_XSL,
  ID_LANCER,
  ID_CMD_LAUNCHTHREAD,
  ID_CMD_FILE_UNAVAILABLE,
  ID_HELP,
  ID_QUIT
};

#include "src/ui/dialog_help.h"
#include "src/ui/process_video_thread.h"
class film;
class HelpFrame;
class DialogShotDetect : public wxDialog {
 public:
  // begin wxGlade: DialogShotDetect::ids
  // end wxGlade
  DialogShotDetect(wxWindow *parent, int id, const wxString &title,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxDEFAULT_DIALOG_STYLE);
  film *f;

 private:
  void set_properties();
  void do_layout();
  void OnQuit(wxCloseEvent &);
  wxProcessVideoThread *vthread;

 protected:
  // begin wxGlade: DialogShotDetect::attributes
  wxButton *bouton_ajouer;
  wxButton *bouton_enlever;
  wxButton *bouton_vider;
  wxButton *bouton_parcourir_xsl;
  wxStaticText *label_chemin_sortie;
  wxTextCtrl *text_chemin_sortie;
  wxStaticText *label_id_auteur;
  wxTextCtrl *text_id_auteur;
  wxButton *bouton_parcourir_sortie;
  wxStaticText *LabelXSL;
  wxTextCtrl *text_chemin_xsl;
  wxStaticText *LabelSeuil;
  wxTextCtrl *text_seuil;
  wxStaticText *label_prenom;
  wxTextCtrl *text_auteur_prenom;
  wxStaticText *label_nom;
  wxTextCtrl *text_auteur_nom;
  wxStaticText *label_frames_offset;
  wxTextCtrl *text_offset_frame;
  wxStaticText *label_titre;
  wxTextCtrl *text_titre;
  wxStaticText *label_resume;
  wxTextCtrl *text_resume;
  wxStaticText *label_annee;
  wxTextCtrl *text_annee;
  wxButton *bouton_lancer;
  wxButton *bouton_help;
  wxStaticText *label_id_film;
  wxTextCtrl *text_id_film;
  wxStaticLine *line_middle_1;
  wxStaticLine *line_middle_2;
  wxRadioBox *radio_box_1;
  wxGauge *progress_local;
  wxGauge *progress_global;
  wxStaticText *label_percent;
  wxStaticText *label_g_percent;
  wxStaticText *label_time_elapsed;
  HelpFrame *hframe;
  DECLARE_EVENT_TABLE();

 public:
  wxListCtrl *list_films;
  wxCheckBox *checkbox_1;
  wxCheckBox *checkbox_2;
  void AjouterFichier(wxCommandEvent &event);   // wxGlade: <event_handler>
  void EnleverFichier(wxCommandEvent &event);   // wxGlade: <event_handler>
  void ViderListe(wxCommandEvent &event);       // wxGlade: <event_handler>
  void ParcourirSortie(wxCommandEvent &event);  // wxGlade: <event_handler>
  void ParcourirXsl(wxCommandEvent &event);
  void ProcessVideo(wxCommandEvent &event);  // wxGlade: <event_handler>
  void FinProcess(wxCommandEvent &event);
  void FileUnavailable(wxCommandEvent &event);
  void set_progress_local(double percent);
  void set_progress_global(double val_global);
  void set_time_elapsed(double time);
  int get_time_elapsed();

  void Help(wxCommandEvent &event);
  inline int GetGlobalProgress(void) { return progress_global->GetValue(); };
  struct timeval time_start;
  struct timezone time_zone;
  list<film> films;
};  // wxGlade: end class

#endif  // DIALOGSHOTDETECT_C_H
