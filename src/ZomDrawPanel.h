#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include "World.hpp"
class ZomDrawPanel : public wxPanel
{
 
public:
	ZomDrawPanel(wxFrame* parent);
	void PaintNow();

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
	void DrawGrid(wxDC& dc);
    void DrawMessage (wxDC& dc);
	DECLARE_EVENT_TABLE()
	
public:
    int mMonth;
    std::string mZombieMachine;
    std::string mHumanMachine;
};

