#include "include/config.hpp"
#include "include/mainWindow.hpp"
#include "include/frame.hpp"
#include <wx/gdicmn.h>

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_MENU(ID_LOAD, MainWindow::OnLoadImage)
	EVT_MENU(ID_PROCESS, MainWindow::OnProcessImage)
    EVT_MENU(ID_ABOUT, MainWindow::OnAbout)
    EVT_MENU(ID_QUIT, MainWindow::OnQuit)
	EVT_CLOSE(MainWindow::OnClose)
END_EVENT_TABLE()

MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
       : wxFrame((wxFrame *)nullptr, -1, title, pos, size)

{
	wxMenu *file_menu = new wxMenu();
	file_menu->Append(ID_LOAD, _T("&Load image...\tCtrl-l"));
	file_menu->Append(ID_PROCESS, _T("&Process image\tCtrl-p"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_ABOUT, _T("&About..."));
	file_menu->AppendSeparator();
	file_menu->Append(ID_QUIT, _T("&Exit"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(file_menu, _T("&File"));
	SetMenuBar(menuBar);

// create the canvas that will manage the image
	m_canvas = new Frame(this, -1, wxDefaultPosition, wxDefaultSize);
	m_img_loaded = false;
	Centre();
}



void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    Close(true);
}


void MainWindow::OnClose(wxCloseEvent &event)
{
	delete m_canvas;
	event.Skip();
}


void MainWindow::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxMessageBox(_T("small steps"), _T(APP_NAME), wxOK | wxICON_INFORMATION);
}


void MainWindow::OnProcessImage(wxCommandEvent &WXUNUSED(event))
{
	if (m_img_loaded)
	    m_canvas->processImage();
}


void MainWindow::OnLoadImage(wxCommandEvent &WXUNUSED(event))
{
	wxBitmap bitmap;

	wxString filename = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*"));
	if (!filename.empty())
	{
		m_canvas->loadImage(filename);
		m_img_loaded = true;
	}
}
