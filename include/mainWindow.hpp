#ifndef __FRAME__
#define __FRAME__

#include <wx/wx.h>
#include "frame.hpp"

enum {
    ID_QUIT = 1,
    ID_ABOUT,
	ID_LOAD,
	ID_PROCESS,
};

class MainWindow: public wxFrame
{
public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
protected:
	void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
	void OnLoadImage(wxCommandEvent &WXUNUSED(event));
	void OnProcessImage(wxCommandEvent &WXUNUSED(event));
	void OnClose(wxCloseEvent &event);

    Frame *m_canvas; // the canvas inside the main frame
	bool m_img_loaded;

    DECLARE_EVENT_TABLE()
};

#endif