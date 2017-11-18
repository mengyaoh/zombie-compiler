#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include <memory>
#include <algorithm>
enum
{
	ID_SIM_START = 1000,
	ID_TURN_TIMER,
    ID_LOAD_ZOMBIE,
    ID_LOAD_SURVIVOR,
    ID_RANDOMIZE,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIM_START, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
    EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
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
    mSimMenu->Append(ID_RANDOMIZE,"Randomize");
	
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
	//mZombieMachine.LoadMachine("zom/basic_movement.zom");
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
		mTurnTimer->Start(100);
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
    for(int i = 0; i<World::Get().GetZombie().size();i++){
        auto ms = World::Get().GetZombie().at(i);
        mZombieMachine.TakeTurn(*ms);
    }
    
    //Take turn for human machine
    for(int i = 0; i<World::Get().GetHuman().size();i++){
        auto ms = World::Get().GetHuman().at(i);
        mHumanMachine.TakeTurn(*ms);
    }
    mPanel->PaintNow();
	// END TEMP CODE
}

void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
    try{
        wxFileDialog openFileDialog(this, _("Open file..."), "./zom", "",
                                    "ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openFileDialog.ShowModal() == wxID_OK)
        {
            std::string fileName = openFileDialog.GetPath().ToStdString();
            mZombieMachine.LoadMachine(fileName);
            //mZombieMachine.BindState(mZombieTestState);
            //World::Get().AddZombie(std::make_shared<MachineState>(mZombieTestState));
            mPanel->PaintNow();
        }
    }catch (FileLoadException exception)
    {
        wxMessageBox("Error loading file", "Error", wxOK | wxICON_ERROR);
    }
    
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    try{
        wxFileDialog openFileDialog(this, _("Open file..."), "./zom", "",
                                    "ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openFileDialog.ShowModal() == wxID_OK)
        {
            std::string fileName = openFileDialog.GetPath().ToStdString();
            mHumanMachine.LoadMachine(fileName);
            //mHumanMachine.BindState(mZombieTestState);
            //World::Get().AddZombie(std::make_shared<MachineState>(mZombieTestState));
        }
    }catch (FileLoadException exception)
    {
        wxMessageBox("Error loading file", "Error", wxOK | wxICON_ERROR);
    }

}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
    World::Get().GetZombie().clear();
    World::Get().GetHuman().clear();
    srand( time(nullptr) );
    std::vector<std::pair<int, int>> existedPoint;
    
    for (int i = 0; i < 20; i++) {
        MachineState zombie;
        int x = rand() % 20;
        int y = rand() % 20;
        auto point = std::make_pair(x, y);
        while(std::find(existedPoint.begin(), existedPoint.end(), point) != existedPoint.end()){
            x = rand() % 20;
            y = rand() % 20;
            point = std::make_pair(x, y);
        }
        existedPoint.push_back(point);
        zombie.mX = x;
        zombie.mY = y;
        int facing = rand() % 4;
        switch (facing) {
            case 0:
                zombie.mFacing = MachineState::UP;
                break;
            case 1:
                zombie.mFacing = MachineState::DOWN;
                break;
            case 2:
                zombie.mFacing = MachineState::LEFT;
                break;
            case 3:
                zombie.mFacing = MachineState::RIGHT;
                break;
        }
        mZombieMachine.BindState(zombie);
        World::Get().AddZombie(std::make_shared<MachineState>(zombie));
    }
    for (int i = 0; i < 10; i++) {
        MachineState human;
        int x = rand() % 20;
        int y = rand() % 20;
        auto point = std::make_pair(x, y);
        while(std::find(existedPoint.begin(), existedPoint.end(), point) != existedPoint.end()){
            x = rand() % 20;
            y = rand() % 20;;
            point = std::make_pair(x, y);
        }
        existedPoint.push_back(point);
        human.mX = x;
        human.mY = y;
        int facing = rand() % 4;
        switch (facing) {
            case 0:
                human.mFacing = MachineState::UP;
                break;
            case 1:
                human.mFacing = MachineState::DOWN;
                break;
            case 2:
                human.mFacing = MachineState::LEFT;
                break;
            case 3:
                human.mFacing = MachineState::RIGHT;
                break;
        }
        mHumanMachine.BindState(human);
        World::Get().AddHuman(std::make_shared<MachineState>(human));
    }
    mPanel->PaintNow();
}

