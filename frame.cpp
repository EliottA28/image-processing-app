#include "include/frame.hpp"

BEGIN_EVENT_TABLE(Frame, wxPanel)
	EVT_PAINT(Frame::OnPaint)
END_EVENT_TABLE()

Frame::Frame(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
			: wxPanel(parent, id, pos, size, wxSUNKEN_BORDER)
{
    m_img = nullptr;
	m_img_RGB = nullptr;
}

Frame::~Frame()
{
	if (m_img)
		free(m_img);
	if (m_img_RGB)
		delete m_img_RGB;
}

void Frame::loadImage(wxString fileName)

{
	if (m_img)
		free(m_img) ;
	if (m_img_RGB)
		delete m_img_RGB ;

	// open image dialog box
	m_img_RGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1); // ANY => can load many image formats
	m_img_bitmap = wxBitmap(*m_img_RGB, -1); // ...to get the corresponding bitmap

    m_img_height = m_img_RGB->GetHeight();
	m_img_width = m_img_RGB->GetWidth();

	m_img = (unsigned char *)malloc(m_img_width*m_img_height*3);
	memcpy(m_img, m_img_RGB->GetData(), m_img_width*m_img_height*3);

	// update GUI size
	SetSize(m_img_width, m_img_height);
	GetParent()->SetClientSize(GetSize());

	// create cursor 
	wxSetCursor(wxCURSOR_CROSS);

	const wxPoint pt = wxGetMousePosition();
	int mouse_x = pt.x - this->GetScreenPosition().x;
	int mouse_y = pt.y - this->GetScreenPosition().y;
	
	int window_width, window_height;
	this->GetSize(&window_width, &window_height);
	std::cout << window_width << " " <<window_height << std::endl;

	// update display
	Refresh(false);
}

void Frame::processImage()
{
	int i = m_img_height*m_img_width*3;

// m_myImage is a monodimentional vector of pixels (RGBRGB...)
	while (i--)
		m_img[i] = 255 - m_img[i];

	Refresh(false); // update display
}

void Frame::OnPaint(wxPaintEvent &WXUNUSED(event))

// update the main window content
{
    wxImage* tempImage;  // the bridge between my image buffer and the bitmap to display

	wxPaintDC dc(this);

	if (m_img)
	{
		tempImage = new wxImage(m_img_width, m_img_height, m_img, true); // lend my image buffer...
		m_img_bitmap = wxBitmap(*tempImage, -1); // ...to get the corresponding bitmap
		delete(tempImage);		// buffer not needed any more
		dc.DrawBitmap(m_img_bitmap, 0, 0);
	}
}