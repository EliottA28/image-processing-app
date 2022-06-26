#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <iostream>

class Frame : public wxPanel
{
public:
    Frame(wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size);
    ~Frame();

    void loadImage(wxString file_name);
	void processImage();
private:
    wxBitmap m_img_bitmap;	// display image
	wxImage *m_img_RGB;    // load image
    unsigned char* m_img;	// process image

	int m_img_height;
	int m_img_width;

    void OnPaint(wxPaintEvent &event);
    
    DECLARE_EVENT_TABLE()
};

#endif