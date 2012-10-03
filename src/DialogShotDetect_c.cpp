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
 * Boston, MA 02110-1301 USA $Id: DialogShotDetect_c.cpp 138 2007-03-28
 * 16:17:26Z johmathe $ $Date: 2007-03-28 18:17:26 +0200 (mer, 28 mar
 * 2007) $ 
 */

#include "DialogShotDetect_c.h"
#include "wxProcessVideoThread.h"
#include "DialogHelp.h"
//#include "icone_cgp.xpm"


class HelpFrame;
void
DialogShotDetect_c::OnQuit (wxCloseEvent &)
{
  // use Destroy instead.
  Destroy ();
}


DialogShotDetect_c::DialogShotDetect_c (wxWindow * parent, int id, const wxString & title, const wxPoint & pos, const wxSize & size, long style):
wxDialog (parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
  //SetIcon (wxICON (icone_cgp));

  list_films = new wxListCtrl (this, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxSUNKEN_BORDER);
  bouton_ajouer = new wxButton (this, ID_AJOUTER, wxT ("Add"));
  bouton_help = new wxButton (this, ID_HELP, wxT ("Help"));
  bouton_enlever = new wxButton (this, ID_ENLEVER, wxT ("Remove"));
  bouton_vider = new wxButton (this, ID_VIDER, wxT ("Clear"));
  label_chemin_sortie = new wxStaticText (this, -1, wxT ("Path"));
  text_chemin_sortie = new wxTextCtrl (this, -1, wxGetHomeDir ());
  label_id_film = new wxStaticText (this, -1, wxT ("Movie ID"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  text_id_film = new wxTextCtrl (this, -1, wxT ("movie"));
  label_id_auteur = new wxStaticText (this, -1, wxT ("author id"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
  text_id_auteur = new wxTextCtrl (this, -1, wxT ("author"));
  bouton_parcourir_sortie = new wxButton (this, ID_PARCOURIR, wxT ("Browse"));
  bouton_parcourir_xsl = new wxButton (this, ID_PARCOURIR_XSL, wxT ("Browse"));
  LabelXSL = new wxStaticText (this, -1, wxT ("Path to the XSLT stylesheet"));

  wxListItem NameCol;
  NameCol.SetText (_T ("Filename\0"));
  NameCol.SetImage (-1);
  NameCol.SetWidth (130);
  list_films->InsertColumn (0, NameCol);

  wxListItem ShotsCol;
  ShotsCol.SetText (_T ("#Shots\0"));
  ShotsCol.SetImage (-1);
  ShotsCol.SetWidth (60);
  list_films->InsertColumn (1, ShotsCol);

  wxListItem TimeCol;
  TimeCol.SetText (_T ("Duration"));
  TimeCol.SetImage (-1);
  TimeCol.SetWidth (60);
  list_films->InsertColumn (2, TimeCol);

  LabelSeuil = new wxStaticText (this, -1, wxT ("threshold"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
  text_seuil = new wxTextCtrl (this, -1, wxT ("60"));
  text_chemin_xsl = new wxTextCtrl (this, -1, _T ("xsl/default.xslt"));
  label_prenom = new wxStaticText (this, -1, wxT ("Author first name"));
  text_auteur_prenom = new wxTextCtrl (this, -1, wxT ("First Name"));
  label_nom = new wxStaticText (this, -1, wxT ("Author name"));
  text_auteur_nom = new wxTextCtrl (this, -1, wxT ("Name"));
  label_frames_offset = new wxStaticText (this, -1, wxT ("Offset Frames"));
  text_offset_frame = new wxTextCtrl (this, -1, wxT ("5"));
  label_titre = new wxStaticText (this, -1, wxT ("Title"));
  text_titre = new wxTextCtrl (this, -1, wxT ("Title"));
  label_resume = new wxStaticText (this, -1, wxT ("Abstract"));
  text_resume = new wxTextCtrl (this, -1, wxT ("Abstract"), wxPoint (180, 90), wxSize (200, 70), wxTE_MULTILINE | wxTE_PROCESS_TAB);
  label_annee = new wxStaticText (this, -1, wxT ("Year"));
  text_annee = new wxTextCtrl (this, -1, wxT ("2007"));
  bouton_lancer = new wxButton (this, ID_LANCER, wxT ("Launch"));
  progress_global = new wxGauge (this, -1, 100);
  progress_local = new wxGauge (this, -1, 100);
  label_percent = new wxStaticText (this, -1, wxT ("0.00 %"));
  label_g_percent = new wxStaticText (this, -1, wxT ("0.00 %"));
  label_time_elapsed = new wxStaticText (this, -1, wxT ("Time duration : 0 sec"));
  line_middle_1 = new wxStaticLine (this, -1);
  line_middle_2 = new wxStaticLine (this, -1);
  checkbox_1 = new wxCheckBox (this, -1, wxT ("First image"));
  checkbox_1->SetValue (true);
  checkbox_2 = new wxCheckBox (this, -1, wxT ("Last image"));
  checkbox_2->SetValue (true);
  hframe = new HelpFrame (this, -1, _T ("About Shotdetect"));
  progress_global->SetValue (0);
  progress_local->SetValue (0);

  const wxString radio_box_1_choices[] = {
    wxT ("Iri"),
    wxT ("Autres")
  };
  radio_box_1 = new wxRadioBox (this, -1, wxT ("Format"), wxDefaultPosition, wxDefaultSize, 2, radio_box_1_choices, 0, wxRA_SPECIFY_COLS);
  set_properties ();
  do_layout ();


  // end wxGlade
}


BEGIN_EVENT_TABLE (DialogShotDetect_c, wxDialog)
  // begin wxGlade: DialogShotDetect_c::event_table
  EVT_MENU (ID_CMD_LAUNCHTHREAD, DialogShotDetect_c::FinProcess) EVT_BUTTON (ID_AJOUTER, DialogShotDetect_c::AjouterFichier) EVT_BUTTON (ID_ENLEVER, DialogShotDetect_c::EnleverFichier) EVT_BUTTON (ID_VIDER, DialogShotDetect_c::ViderListe) EVT_BUTTON (ID_PARCOURIR, DialogShotDetect_c::ParcourirSortie) EVT_BUTTON (ID_PARCOURIR_XSL, DialogShotDetect_c::ParcourirXsl) EVT_BUTTON (ID_LANCER, DialogShotDetect_c::ProcessVideo) EVT_BUTTON (ID_HELP, DialogShotDetect_c::Help) EVT_CLOSE (DialogShotDetect_c::OnQuit)
  // end wxGlade
  END_EVENT_TABLE ();

     void DialogShotDetect_c::FinProcess (wxCommandEvent & event)
{

  wxMessageDialog MsgDlg (this, _T ("End of analyse"));
  MsgDlg.ShowModal ();

}


void
DialogShotDetect_c::AjouterFichier (wxCommandEvent & event)
{
  wxFileDialog dialog (this, _T ("Open file for an analyse"), wxEmptyString, wxEmptyString,
#ifdef __WXMOTIF__
		       _T ("Media files (*.avi;*.flv;*.mp4;*.mpg;*.mov;*.mp3;*.wav)|*.avi;*.flv;*.mp4;*.mpg;*.mov;*.mp3;*.wav")
#else
		       _T ("Media files (*.avi;*.flv;*.mp4;*.mpg;*.mov;*.mp3;*.wav)|*.avi;*.flv;*.mp4;*.mpg;*.mov;*.mp3;*.wav")
#endif
    );
  dialog.SetDirectory (wxGetHomeDir ());
  if (dialog.ShowModal () == wxID_OK)
    {
      long id = list_films->InsertItem (list_films->GetItemCount (),
					dialog.GetPath (), 0);
      list_films->SetItemData (id, 1);
      list_films->SetItem (id, 2, _T ("00:00:00"));
      list_films->SetItem (id, 1, _T ("0"));
    }
  event.Skip ();
}


void
DialogShotDetect_c::EnleverFichier (wxCommandEvent & event)
{
  long item = -1;
  for (;;)
    {
      item = list_films->GetNextItem (item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item == -1)
	break;

      // this item is selected - do whatever is needed with it
      list_films->DeleteItem (item);
      item = -1;
    }
  event.Skip ();
}


void
DialogShotDetect_c::ViderListe (wxCommandEvent & event)
{
  long item, nb_item = list_films->GetItemCount ();
  for (item = 0; item < nb_item; item++)
    {
      list_films->DeleteItem (0);
    }
  event.Skip ();
}


void
DialogShotDetect_c::Help (wxCommandEvent & event)
{
  hframe->ShowModal ();
  event.Skip ();
}

void
DialogShotDetect_c::ParcourirSortie (wxCommandEvent & event)
{

  wxDirDialog DirDlg (this, _("Choose a path"), wxGetCwd ());

  if (DirDlg.ShowModal () == wxID_OK)
    {
      text_chemin_sortie->SetValue (DirDlg.GetPath ());
    }

  event.Skip ();
}

void
DialogShotDetect_c::ParcourirXsl (wxCommandEvent & event)
{

  wxFileDialog dialog (this, _T ("Open an xsl stylesheet"), wxEmptyString, wxEmptyString, _T ("Xslt files (*.xsl;*.xslt)|*.xsl;*.xslt"));
  dialog.SetDirectory (wxGetHomeDir ());
  if (dialog.ShowModal () == wxID_OK)
    {
      text_chemin_xsl->SetValue (dialog.GetPath ());
    }
  event.Skip ();
}


void
DialogShotDetect_c::ProcessVideo (wxCommandEvent & event)
{
  /*
   * Copie des données pour le bon fonctionnement du film 
   */
  long item, nb_item = list_films->GetItemCount ();
  gettimeofday (&time_start, &time_zone);
  for (item = 0; item < nb_item; item++)
    {
      film f = film (this);
      f.idfilm = nb_item;
      f.id = int (item);
      f.code_lang = "fr";
      f.title = text_titre->GetLineText (0).ToAscii ();
      f.input_path = list_films->GetItemText (item).ToAscii ();
      f.abstract = text_resume->GetLineText (0).ToAscii ();
      f.year = atoi ((const char *) text_annee->GetLineText (0).ToAscii ());
      f.author.name = text_auteur_prenom->GetLineText (0).ToAscii ();
      f.author.surname = (const char *) text_auteur_nom->GetLineText (0).ToAscii ();
      f.threshold = int (atoi (text_seuil->GetLineText (0).ToAscii ()));
      f.alphaid = text_id_auteur->GetLineText (0).ToAscii ();
      f.alphaid += "_";
      f.alphaid += text_id_film->GetLineText (0).ToAscii ();
      f.global_path = text_chemin_sortie->GetLineText (0).ToAscii ();
      films.push_front (f);
      xml *x = new xml (&(films.front ()));
      x->xsl_path = text_chemin_xsl->GetLineText (0).ToAscii ();
      /*
       * Attention : sous windows, le séparateur est un \\ 
       */
#ifdef __WINDOWS__
      x->xsl_name.assign (x->xsl_path, x->xsl_path.rfind ("\\") + 1, x->xsl_path.rfind (".") - x->xsl_path.rfind ("\\") - 1);
#else
      x->xsl_name.assign (x->xsl_path, x->xsl_path.rfind ("/") + 1, x->xsl_path.rfind (".") - x->xsl_path.rfind ("/") - 1);
#endif
      struct stat *buf = NULL;
      buf = (struct stat *) malloc (sizeof (struct stat));
      if ((stat ((const char *) x->xsl_path.c_str (), buf) == -1) || !S_ISREG (buf->st_mode))
	{
	  wxMessageDialog MsgDlg (this, _T ("Unable to find an XSL stylesheet !"));
	  MsgDlg.ShowModal ();
	  free (buf);
	  films.clear ();
	  goto end;
	}
      films.front ().x = x;
    }

  vthread = new wxProcessVideoThread (wxTHREAD_DETACHED);
  vthread->Create (this, &films);
  vthread->Run ();
  vthread->Wait ();
end:
  ;
}



void
DialogShotDetect_c::set_properties ()
{
  SetTitle (wxT ("IRI::ShotDetect"));
  SetSize (wxSize (350, 700));
  list_films->SetMinSize (wxSize (250, 100));
  bouton_ajouer->SetMinSize (wxSize (70, 32));
  bouton_enlever->SetMinSize (wxSize (70, 32));
  bouton_vider->SetMinSize (wxSize (70, 32));
  text_chemin_sortie->SetMinSize (wxSize (150, 24));
  bouton_parcourir_sortie->SetMinSize (wxSize (90, 24));
  bouton_parcourir_xsl->SetMinSize (wxSize (90, 24));
  text_chemin_xsl->SetMinSize (wxSize (150, 24));
  text_auteur_prenom->SetMinSize (wxSize (40, 24));
  text_auteur_nom->SetMinSize (wxSize (40, 24));
}


void
DialogShotDetect_c::do_layout ()
{
  wxGridSizer *grid_meta = new wxGridSizer (2, 2, 0, 0);
  wxBoxSizer *SizerPrincipal = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_annee = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_resume = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_titre = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_offset = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_nom = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_prenom = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_horiz_1 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_horiz_2 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_horiz_3 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_horiz_4 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_horiz_5 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_horiz_6 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *sizer_check = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_seuil = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_xsl = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_output = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *SizerTop = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer *SizerBoutons = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_idfilm = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_idauteur = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *sizer_ids = new wxBoxSizer (wxHORIZONTAL);

  /*
   * Mise en place des boutons 
   */
  SizerBoutons->Add (bouton_ajouer, 0, wxADJUST_MINSIZE, 0);
  SizerBoutons->Add (bouton_enlever, 0, wxADJUST_MINSIZE, 0);
  SizerBoutons->Add (bouton_vider, 0, wxADJUST_MINSIZE, 0);

  /*
   * Mise en place du haut de la fenetre 
   */
  SizerTop->Add (list_films, 1, wxEXPAND, 0);
  SizerTop->Add (SizerBoutons, 0, wxADJUST_MINSIZE, 0);

  SizerPrincipal->Add (SizerTop, 0, wxEXPAND, 0);

  /*
   * Barres de progession 
   */
  SizerPrincipal->Add (progress_global, 0, wxEXPAND);
  SizerPrincipal->Add (label_g_percent, 0, wxRIGHT);
  SizerPrincipal->Add (progress_local, 0, wxEXPAND);
  SizerPrincipal->Add (label_percent, 0, wxRIGHT);

  SizerPrincipal->Add (label_time_elapsed, 0, wxCENTER | wxADJUST_MINSIZE, 0);

  SizerPrincipal->Add (label_chemin_sortie);
  sizer_horiz_1->Add (text_chemin_sortie, 1, wxADJUST_MINSIZE, 0);
  sizer_horiz_1->Add (bouton_parcourir_sortie, 0, wxADJUST_MINSIZE, 0);
  SizerPrincipal->Add (sizer_horiz_1, 1, wxEXPAND, 0);

  /*
   * Traitement XSL 
   */
  SizerPrincipal->Add (LabelXSL, 0, wxADJUST_MINSIZE, 0);
  sizer_horiz_2->Add (text_chemin_xsl, 1, wxADJUST_MINSIZE, 0);
  sizer_horiz_2->Add (bouton_parcourir_xsl, 0, wxADJUST_MINSIZE, 0);
  SizerPrincipal->Add (sizer_horiz_2, 1, wxEXPAND, 0);
  /*
   * seuil 
   */
  sizer_seuil->Add (LabelSeuil, 0, wxADJUST_MINSIZE, 0);
  sizer_seuil->Add (text_seuil, 0, wxRIGHT | wxADJUST_MINSIZE, 0);

  /*
   * Offset Frames 
   */
  sizer_offset->Add (label_frames_offset, 0, wxADJUST_MINSIZE, 0);
  sizer_offset->Add (text_offset_frame, 0, wxADJUST_MINSIZE, 0);

  /*
   * Ajout de tout cela au sizer 2 
   */
  sizer_check->Add (checkbox_1, 1, wxADJUST_MINSIZE, 0);
  sizer_check->Add (checkbox_2, 1, wxADJUST_MINSIZE, 0);
  sizer_horiz_3->Add (sizer_seuil, 1, wxADJUST_MINSIZE | wxALIGN_RIGHT, 0);
  sizer_horiz_3->Add (sizer_offset, 1, wxADJUST_MINSIZE, 0);
  sizer_horiz_3->Add (sizer_check, 0, wxADJUST_MINSIZE, 0);
  SizerPrincipal->Add (sizer_horiz_3, 1, wxEXPAND, 0);
  SizerPrincipal->Add (radio_box_1, 0, wxADJUST_MINSIZE, 0);


  /*
   * Mise en place des ids 
   */
  sizer_idauteur->Add (label_id_auteur, 0, wxADJUST_MINSIZE, 0);
  sizer_idauteur->Add (text_id_auteur, 0, wxADJUST_MINSIZE, 0);
  sizer_idfilm->Add (label_id_film, 0, wxADJUST_MINSIZE, 0);
  sizer_idfilm->Add (text_id_film, 0, wxADJUST_MINSIZE, 0);
  sizer_ids->Add (sizer_idfilm, 1, wxEXPAND, 0);
  sizer_ids->Add (sizer_idauteur, 1, wxEXPAND, 0);
  SizerPrincipal->Add (sizer_ids, 1, wxEXPAND, 0);

  /*
   * Méta données : prénom auteur 
   */
  sizer_prenom->Add (label_prenom, 0, wxADJUST_MINSIZE, 0);
  sizer_prenom->Add (text_auteur_prenom, 0, wxEXPAND, 0);

  /*
   * Méta données : nom auteur 
   */
  sizer_nom->Add (label_nom, 0, wxADJUST_MINSIZE, 0);
  sizer_nom->Add (text_auteur_nom, 0, wxEXPAND, 0);

  /*
   * Méta données : titre de l'oeuvre 
   */
  sizer_titre->Add (label_titre, 0, wxADJUST_MINSIZE, 0);
  sizer_titre->Add (text_titre, 0, wxEXPAND, 0);

  /*
   * Méta données : abstract 
   */



  /*
   * Méta données : année de l'oeuvre 
   */
  sizer_annee->Add (label_annee, 0, wxADJUST_MINSIZE, 0);
  sizer_annee->Add (text_annee, 0, wxEXPAND, 0);

  sizer_horiz_4->Add (sizer_prenom, 1, wxEXPAND, 0);
  sizer_horiz_4->Add (sizer_nom, 1, wxEXPAND, 0);
  sizer_horiz_5->Add (sizer_titre, 1, wxEXPAND, 0);
  sizer_horiz_5->Add (sizer_annee, 1, wxEXPAND, 0);

  /*
   * Bouton de lancement du process 
   */
  SizerPrincipal->Add (sizer_horiz_4, 1, wxEXPAND, 0);
  SizerPrincipal->Add (sizer_horiz_5, 1, wxEXPAND, 0);
  SizerPrincipal->Add (sizer_resume, 0, wxEXPAND, 0);
  SizerPrincipal->Add (label_resume, 0, wxEXPAND, 0);
  SizerPrincipal->Add (text_resume, 0, wxEXPAND, 0);
  sizer_horiz_6->Add (bouton_lancer, 0, wxEXPAND, 0);
  sizer_horiz_6->Add (bouton_help, 0, wxEXPAND, 0);
  SizerPrincipal->Add (sizer_horiz_6, 0, wxADJUST_MINSIZE | wxALIGN_CENTER, 0);

  SetAutoLayout (true);
  SetSizer (SizerPrincipal);
  Layout ();
}

void
DialogShotDetect_c::set_progress_local (double percent)
{
  wxString str;
  str << int (percent);
  str << wxT (".00 %");
  progress_local->SetValue (int (percent));
  label_percent->SetLabel (str);
}

void
DialogShotDetect_c::set_progress_global (double val_global)
{
  wxString str;
  str << int (val_global);
  str << wxT (".00 %");
  progress_global->SetValue (int (val_global));
  label_g_percent->SetLabel (str);
}

void
DialogShotDetect_c::set_time_elapsed (double time)
{
  wxString str;
  str << wxT ("temps \xE9\x63oul\xE9 : ");
  str << time;
  str << wxT (" secs ");
  label_time_elapsed->SetLabel (str);
}

int
DialogShotDetect_c::get_time_elapsed ()
{
  return (atoi (label_time_elapsed->GetLabel ().ToAscii ()));
}
