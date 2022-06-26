#include<iostream>
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <wx/gdicmn.h>
#include <wx/display.h>

using namespace std;

#define APP_NAME "imR tool"

enum {
    ID_QUIT = 1,
    ID_ABOUT,
	ID_LOAD,
	ID_SAVE,
	ID_PROCESS,
	ID_BEST_SIZE
};

class MainWindow: public wxPanel
{
public:
    MainWindow(wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size) ;
    ~MainWindow() ;
	void loadImage(wxString fileName);
	void saveImage(wxString fileName) ;
	void processImage();
	void BestSize() ;

private:
	int m_imageWidth ;
	int m_imageHeight ;
    wxBitmap m_imageBitmap ;	// used to display the image
	wxImage *m_imageRGB ;		// used to load the image
    unsigned char* m_myImage ;	// used to process the image

    void OnPaint(wxPaintEvent &event) ;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MainWindow, wxPanel)
	EVT_PAINT(MainWindow::OnPaint)
END_EVENT_TABLE()


MainWindow::MainWindow( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size )
        : wxPanel( parent, id, pos, size, wxSUNKEN_BORDER)

{
    m_myImage=NULL;
	m_imageRGB = NULL ;
}


MainWindow::~MainWindow()

{
	if (m_myImage)
		free(m_myImage) ;
	if (m_imageRGB)
		delete m_imageRGB ;
}


void MainWindow::loadImage(wxString fileName)

{
	if (m_myImage)
		free (m_myImage) ;
	if (m_imageRGB)
		delete m_imageRGB ;

// open image dialog box
	m_imageRGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1); // ANY => can load many image formats
	m_imageBitmap = wxBitmap(*m_imageRGB, -1); // ...to get the corresponding bitmap

	m_imageWidth = m_imageRGB->GetWidth() ;
	m_imageHeight = m_imageRGB->GetHeight() ;

	m_myImage = (unsigned char*)malloc(m_imageWidth * m_imageHeight * 3) ;
	memcpy(m_myImage, m_imageRGB->GetData(), m_imageWidth * m_imageHeight * 3) ;

// update GUI size
	SetSize(m_imageWidth, m_imageHeight) ;
	GetParent()->SetClientSize(GetSize()) ;

// create cursor 
	wxSetCursor(wxCURSOR_CROSS);


	const wxPoint pt = wxGetMousePosition();
	int mouse_x = pt.x - this->GetScreenPosition().x;
	int mouse_y = pt.y - this->GetScreenPosition().y;
	
	wxDisplay display(wxDisplay::GetFromWindow(this));
	wxRect screen = display.GetClientArea();
	int window_width, window_height;
	this->GetSize(&window_width, &window_height);
	// mouse_x = mouse_x < 0 ? 0 : (mouse_x > window_width ? window_width : mouse_x);
	cout << window_width << " " <<window_height << endl;

// update display
	Update();
}


void MainWindow::saveImage(wxString fileName)

{
	bool b ;

	wxImage* tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
	b = tempImage->SaveFile(fileName) ;
	delete(tempImage) ;		// buffer not needed any more

	if(!b)
		wxMessageBox(wxT("A problem occured during saving"));
}


void MainWindow::processImage()

// example of fast and trivial process (negative)
// you can replace it with your own
// you can also use methods from the wxImage class itself
{
	long int i = m_imageWidth*m_imageHeight*3 ;

// m_myImage is a monodimentional vector of pixels (RGBRGB...)
	while (i--)
		m_myImage[i] = 255 - m_myImage[i] ;

	Refresh(false); // update display
}


void MainWindow::BestSize()

{
	SetSize(m_imageWidth, m_imageHeight) ;	// ideal size for canvas
	GetParent()->SetClientSize(GetSize());	// force the main frame to show the whole canvas
}


void MainWindow::OnPaint(wxPaintEvent &WXUNUSED(event))

// update the main window content
{
    wxImage* tempImage;  // the bridge between my image buffer and the bitmap to display

	wxPaintDC dc(this);

	if (m_myImage)
	{
		tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
		m_imageBitmap = wxBitmap(*tempImage, -1); // ...to get the corresponding bitmap
		delete(tempImage) ;		// buffer not needed any more
		dc.DrawBitmap(m_imageBitmap, 0, 0) ;
	}
}

//************************************************************************
//************************************************************************
// Frame class (the main window)
//************************************************************************
//************************************************************************


class MyFrame: public wxFrame

{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

protected:
	void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnOpenImage(wxCommandEvent& WXUNUSED(event) ) ;
	void OnSaveImage(wxCommandEvent & WXUNUSED(event)) ;
	void OnProcessImage(wxCommandEvent& WXUNUSED(event) ) ;
	void OnClose(wxCloseEvent& event) ;
	void OnBestSize(wxCommandEvent& WXUNUSED(event)) ;

    MainWindow *m_canvas; // the canvas inside the main frame
	bool m_imageLoaded ;
    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_LOAD,  MyFrame::OnOpenImage)
	EVT_MENU(ID_SAVE,  MyFrame::OnSaveImage)
	EVT_MENU(ID_PROCESS,  MyFrame::OnProcessImage)
	EVT_MENU(ID_BEST_SIZE,  MyFrame::OnBestSize)
    EVT_MENU(ID_QUIT,  MyFrame::OnQuit)
    EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
	EVT_CLOSE(MyFrame::OnClose)
END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)

{
	wxMenu *file_menu = new wxMenu();
	file_menu->Append(ID_LOAD, _T("&Load image...\tCtrl-l"));
	file_menu->Append(ID_PROCESS, _T("&Process image"));
	file_menu->Append(ID_SAVE, _T("&Save image as..."));
	file_menu->Append(ID_BEST_SIZE, _T("&Best size"));
	file_menu->AppendSeparator();
	file_menu->Append(ID_ABOUT, _T("&About..."));
	file_menu->AppendSeparator();
	file_menu->Append(ID_QUIT, _T("&Exit"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(file_menu, _T("&File"));
	SetMenuBar( menuBar );

// create the canvas that will manage the image
	m_canvas = new MainWindow( this, -1, wxDefaultPosition, wxDefaultSize);
	m_imageLoaded = false ;
	Centre() ;
}



void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))

{
    Close(true) ;
}


void MyFrame::OnClose(wxCloseEvent& event)

{
	delete m_canvas ;
	event.Skip() ;
}


void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))

{
    wxMessageBox( _T("How to \n\n- load\n- display\n- process\n- save\n\nan image with wxWidgets (2.8.7)\n\nPascal Bertolino - GIPSA-lab, Grenoble - France\npascal.bertolino@gipsa-lab.fr"),
                  _T(APP_NAME), wxOK | wxICON_INFORMATION ) ;
}


void MyFrame::OnProcessImage(wxCommandEvent& WXUNUSED(event))

{
	if (m_imageLoaded)
	    m_canvas->processImage() ;
}


void MyFrame::OnOpenImage(wxCommandEvent& WXUNUSED(event) )

{
	wxBitmap bitmap;

	wxString filename = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*") );
	if ( !filename.empty() )
	{
		m_canvas->loadImage(filename) ;
		m_imageLoaded = true ;
	}
}


void MyFrame::OnSaveImage(wxCommandEvent & WXUNUSED(event))

{
//	char str[128] = "" ; // proposed file name

	if (!m_imageLoaded)
		return ;

	wxString filename = wxFileSelector(_T("Save image as"),_T(""),_T(""),_T("*.bmp"), _T("BMP files (*.bmp)|*.bmp|GIF files (*gif)|*.gif|JPEG files (*jpg)|*.jpg|PNG files (*png)|*.png|TIFF files (*tif)|*.tif|XPM files (*xpm)|*.xpm|All files (*.*)|*.*"), wxID_SAVE );
	if ( !filename.empty() )
		m_canvas->saveImage(filename) ;
}


void MyFrame::OnBestSize(wxCommandEvent& WXUNUSED(event))

{
    m_canvas->BestSize() ;
}

//************************************************************************
//************************************************************************
// Application class
//************************************************************************
//************************************************************************


class MyApp: public wxApp

{
    virtual bool OnInit() ;
};

IMPLEMENT_APP(MyApp) // macro that contains the main() function



bool MyApp::OnInit()

{
//support all available image formats
	wxInitAllImageHandlers() ;

    MyFrame *frame = new MyFrame(_T(APP_NAME), wxDefaultPosition, wxSize(400,300)) ;
    frame->Show(true) ;
    SetTopWindow(frame) ;
    return true ;
}
