#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include "Op.h"
#include "Exceptions.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
    // Machine can use Machinestate's private
    // one Machine for all zombie, one machinesate for each zombie
	template <typename MachineTraits>
	friend class Machine;
	
	enum Facing { UP, RIGHT, DOWN, LEFT};
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(DOWN)
		, mTest(false)
        , mX(0),mY(0)
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;

	int GetActionsPerTurn() const throw() { return mActionsPerTurn; }
	bool GetInfect() const throw() { return mInfectOnAttack; }
    int mX;
    int mY;
private:
	// Data which is set by the traits
	int mActionsPerTurn = 0;
	bool mInfectOnAttack = false;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Clear out the current machine
	void ClearMachine();
	
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);


	const std::string& GetName() const { return mName; }
private:
	std::vector<std::unique_ptr<Op>> mOps;
	std::string mName;
};

template <typename MachineTraits>
void Machine<MachineTraits>::ClearMachine()
{
	mOps.clear();
	mName.clear();
}

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	// TEMP CODE: Add your parsing code here!
    mOps.clear();
//    mOps.push_back(std::make_unique<OpRotate>(0));
//    mOps.push_back(std::make_unique<OpRotate>(0));
//    mOps.push_back(std::make_unique<OpRotate>(1));
//    mOps.push_back(std::make_unique<OpGoto>(1));
	// END TEMP CODE
    std::ifstream file (filename);
    if(file.is_open()){
        std::string op;
        while (!file.eof()){
            std::getline(file, op);
            std::size_t semicolonIndex = op.find(";");
            std::size_t commaIndex = op.find(",");
            int param=0;
            std::string comment;
            if(semicolonIndex!= std::string::npos){
                comment = op.substr(semicolonIndex+2);
                op.erase(semicolonIndex, comment.length()+2);
            }
            
            if(commaIndex!= std::string::npos){
                param = std::stoi(op.substr(commaIndex + 1));
                op.erase(commaIndex, op.length() - commaIndex);
            }
            op.erase(std::remove_if(op.begin(), op.end(), isspace), op.end());
            //std::cout<<op<<param<<comment<<"\n";
            if(op == "goto" ){
                 mOps.push_back(std::make_unique<OpGoto>(param));
            }
            if(op == "rotate" ){
                mOps.push_back(std::make_unique<OpRotate>(param));
            }
            else if( op == "forward"){
                mOps.push_back(std::make_unique<OpForward>());
            }
            
            else if( op == "je"){
                mOps.push_back(std::make_unique<OpJe>(param));
            }
            else if( op == "test_wall"){
                mOps.push_back(std::make_unique<OpTest_wall>());
            }
            else if( op == "test_random"){
                mOps.push_back(std::make_unique<OpTest_random>());
            }
        }
    }
    
}

//bind machinestate to zombie/human machine
template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	wxLogDebug("TAKING TURN");
    //std::cout<<mOps.size()<<"\n";
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
        //std::cout<<state.mProgramCounter<<"\n";
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}
