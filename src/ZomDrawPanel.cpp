#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel) // NOLINT
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
: wxPanel(parent)
{
    mMonth=0;
    mZombieMachine="";
    mHumanMachine="";
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
    DrawMessage(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
    //Draw grid
    for (int i = 40; i < 610; i+= 30){
        dc.DrawLine(i, 10, i, 610);
        dc.DrawLine(10, i, 610, i);
        
    }
	//Draw zombies
    for(int i = 0; i<World::Get().GetZombie().size();i++){
        auto ms = World::Get().GetZombie().at(i);
        wxPoint ptup(0, 0);
        wxPoint ptleft(0, 0);
        wxPoint ptright(0, 0);
        switch (ms->mFacing)
        {
            case (MachineState::UP):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY + 1)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY + 1)+10);
                ptup = wxPoint(30*(ms->mX) + 15+10, 30*(ms->mY)+10);
                break;
            case (MachineState::RIGHT):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10);
                ptright = wxPoint(30*(ms->mX) + 10, 30*(ms->mY + 1)+10);
                ptup = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10+15);
                break;
            case (MachineState::LEFT):
                ptleft = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY+1)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10);
                ptup = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10+15);
                break;
            case (MachineState::DOWN):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10);
                ptup = wxPoint(30*(ms->mX) + 15 +10, 30*(ms->mY+1)+10);
                break;
                
        }
        wxPoint *list = new wxPoint[3];
        list[0] = ptup;
        list[1] = ptleft;
        list[2] = ptright;
        
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawPolygon(3, list);
    }
    //Draw Human
    for(int i = 0; i<World::Get().GetHuman().size();i++){
        auto ms = World::Get().GetHuman().at(i);
        wxPoint ptup(0, 0);
        wxPoint ptleft(0, 0);
        wxPoint ptright(0, 0);
        switch (ms->mFacing)
        {
            case (MachineState::UP):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY + 1)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY + 1)+10);
                ptup = wxPoint(30*(ms->mX) + 15+10, 30*(ms->mY)+10);
                break;
            case (MachineState::RIGHT):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10);
                ptright = wxPoint(30*(ms->mX) + 10, 30*(ms->mY + 1)+10);
                ptup = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10+15);
                break;
            case (MachineState::LEFT):
                ptleft = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY+1)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10);
                ptup = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10+15);
                break;
            case (MachineState::DOWN):
                ptleft = wxPoint(30*(ms->mX) + 10, 30*(ms->mY)+10);
                ptright = wxPoint(30*(ms->mX+1) + 10, 30*(ms->mY)+10);
                ptup = wxPoint(30*(ms->mX) + 15 +10, 30*(ms->mY+1)+10);
                break;
                
        }
        wxPoint *list = new wxPoint[3];
        list[0] = ptup;
        list[1] = ptleft;
        list[2] = ptright;
        
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawPolygon(3, list);
    }
}

void ZomDrawPanel::DrawMessage(wxDC& dc)
{
    dc.SetTextForeground(*wxRED);
    wxString text1 = "Zombie Machine: ";
    text1.Append(mZombieMachine);
    text1.Append("\nAlive: ");
    text1.Append(std::to_string(World::Get().GetZombie().size()));
    dc.DrawText(text1, 640, 10);
    
    dc.SetTextForeground(*wxGREEN);
    wxString human = "Human Machine: ";
    human.Append(mHumanMachine);
    human.Append("\nAlive: ");
    human.Append(std::to_string(World::Get().GetHuman().size()));
    dc.DrawText(human, 640, 110);
    
    dc.SetTextForeground(*wxBLACK);
    wxString month = "Month: ";
    month.Append(std::to_string(mMonth));
    dc.DrawText(month, 640, 210);
    
}


