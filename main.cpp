#include "include/mainWindow.hpp"

class MyApp: public wxApp
{
public:
    virtual bool OnInit() ;
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

    MainWindow *main_window = new MainWindow(wxT("premiers tests"), wxDefaultPosition, wxSize(550,380));
    main_window->Show(true);
    SetTopWindow(main_window);

    return true;
}
