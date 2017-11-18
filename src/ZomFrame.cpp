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
    ID_RESET,
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame) // NOLINT
	EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
	EVT_MENU(wxID_NEW, ZomFrame::OnNew)
	EVT_MENU(ID_SIM_START, ZomFrame::OnSimStart)
	EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
    EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
    EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
    EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
    EVT_MENU(ID_RESET, ZomFrame::OnReset)
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
	mSimMenu->Append(ID_RESET, "Reset", "Reset");
    
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
    mLoadedhuman =false;
    mLoadedzombie=false;
    mSimMenu->Enable(ID_SIM_START, false);
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
    mTurnTimer->Stop();
    World::Get().GetHuman().clear();
    World::Get().GetZombie().clear();
    mPanel->mMonth = 0;
    World::Get().GetMyHumanMachine().ClearLoad();
    World::Get().GetMyZombieMachine().ClearLoad();
    mPanel->mHumanMachine="";
    mPanel->mZombieMachine="";
    mSimMenu->Enable(ID_SIM_START, false);
    mSimMenu->Enable(ID_RESET, false);
    mSimMenu->Enable(ID_RANDOMIZE, false);
    mLoadedhuman =false;
    mLoadedzombie=false;
    mPanel->PaintNow();
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
    wxString winner;
    if (World::Get().GetZombie().empty()){
        winner = "Humans won!";
    }
    else if (World::Get().GetHuman().empty()){
        winner = "Zombies won!";
    }
    else{
	// TEMP CODE: Take turn for zombie machine
        for(int i = 0; i<World::Get().GetZombie().size();i++){
            auto ms = World::Get().GetZombie().at(i);
            World::Get().GetMyZombieMachine().TakeTurn(*ms);
        }
        
        //Take turn for human machine
        for(int i = 0; i<World::Get().GetHuman().size();i++){
            auto ms = World::Get().GetHuman().at(i);
            World::Get().GetMyHumanMachine().TakeTurn(*ms);
        }
        mPanel->mMonth++;
        mPanel->PaintNow();
        return;
        // END TEMP CODE
    }
    mTurnTimer->Stop();
    wxMessageBox(winner, "");
}

void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
    try{
        wxFileDialog openFileDialog(this, _("Open file..."), "./zom", "",
                                    "ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openFileDialog.ShowModal() == wxID_OK)
        {
            std::string fileName = openFileDialog.GetPath().ToStdString();
            World::Get().GetMyZombieMachine().LoadMachine(fileName);
            mPanel->mZombieMachine = fileName;
            //mZombieMachine.BindState(mZombieTestState);
            //World::Get().AddZombie(std::make_shared<MachineState>(mZombieTestState));
        }
        else{
            throw FileLoadException();
        }
    }catch (FileLoadException exception)
    {
        wxMessageBox("Error loading file", "Error", wxOK | wxICON_ERROR);
    }
    mLoadedzombie=true;
    if(mLoadedzombie&&mLoadedhuman){
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_RESET, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
    }
    mPanel->PaintNow();
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
    try{
        wxFileDialog openFileDialog(this, _("Open file..."), "./zom", "",
                                    "ZOM Files|*.zom", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        
        if (openFileDialog.ShowModal() == wxID_OK)
        {
            std::string fileName = openFileDialog.GetPath().ToStdString();
            World::Get().GetMyHumanMachine().LoadMachine(fileName);
            mPanel->mHumanMachine = fileName;
            //mHumanMachine.BindState(mZombieTestState);
            //World::Get().AddZombie(std::make_shared<MachineState>(mZombieTestState));
        }
        else{
            throw FileLoadException();
        }
    }catch (FileLoadException exception)
    {
        wxMessageBox("Error loading file", "Error", wxOK | wxICON_ERROR);
    }
    mLoadedhuman =true;
    if(mLoadedzombie&&mLoadedhuman){
        mSimMenu->Enable(ID_SIM_START, true);
        mSimMenu->Enable(ID_RESET, true);
        mSimMenu->Enable(ID_RANDOMIZE, true);
    }
    mPanel->PaintNow();
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
    World::Get().GetZombie().clear();
    World::Get().GetHuman().clear();
    srand( time(nullptr) );
    std::vector<std::pair<int, int>> existedPoint;
    
    for (int i = 0; i < 20; i++) {
        MachineState zombie;
        zombie.mX= rand() % 20;
        zombie.mY = rand() % 20;
        auto point = std::make_pair(zombie.mX, zombie.mY);
        while(std::find(existedPoint.begin(), existedPoint.end(), point) != existedPoint.end()){
            zombie.mX = rand() % 20;
            zombie.mY = rand() % 20;
            point = std::make_pair(zombie.mX, zombie.mY);
        }
        existedPoint.push_back(point);
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
        World::Get().GetMyZombieMachine().BindState(zombie);
        World::Get().AddZombie(std::make_shared<MachineState>(zombie));
    }
    for (int i = 0; i < 10; i++) {
        MachineState human;
        human.mX = rand() % 20;
        human.mY = rand() % 20;
        auto point = std::make_pair(human.mX, human.mY);
        while(std::find(existedPoint.begin(), existedPoint.end(), point) != existedPoint.end()){
            human.mX = rand() % 20;
            human.mY = rand() % 20;;
            point = std::make_pair(human.mX, human.mY);
        }
        existedPoint.push_back(point);
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
        World::Get().GetMyHumanMachine().BindState(human);
        World::Get().AddHuman(std::make_shared<MachineState>(human));
    }
    mPanel->PaintNow();
}

void ZomFrame::OnReset(wxCommandEvent& event)
{
    mTurnTimer->Stop();
    World::Get().GetHuman().clear();
    World::Get().GetZombie().clear();
    mPanel->mMonth = 0;
    mPanel->PaintNow();
    
}


