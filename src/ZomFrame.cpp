#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"

enum
{
	ID_SIM_START = 1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIM_START, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(nullptr, wxID_ANY, title, pos, size)
, mPanel(nullptr)
, mSimMenu(nullptr)
, mIsActive(false)
{
	// File menu
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);
	
	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_SIM_START, "Start/stop\tSpace", "Start or stop the simulation");
    mSimMenu->Append(ID_LOAD_ZOMBIE,"Load Zombie");
    mSimMenu->Append(ID_LOAD_SURVIVOR,"Load Human");
	
    wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	wxFrame::SetMenuBar(menuBar);
	wxFrame::CreateStatusBar();
	
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);
	
	wxFrame::SetSizer(sizer);
	wxFrame::SetAutoLayout(true);
	wxFrame::Show(true);
	
	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);

	// TEMP CODE: Initialize zombie test machine
	mZombieMachine.LoadMachine("zom/basic_movement.zom");
	mZombieMachine.BindState(mZombieTestState);
	// END TEMP CODE
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New

}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	// TEMP CODE: Take turn for zombie machine
	mZombieMachine.TakeTurn(mZombieTestState);
	// END TEMP CODE
}

void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
    
    wxFileDialog openFileDialog(this, _("Open file..."), "./zom", "",
                                "ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        std::string fileName = openFileDialog.GetPath().ToStdString();
        mZombieMachine.LoadMachine(fileName);
        mPanel->PaintNow();
    }
    
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{

}
