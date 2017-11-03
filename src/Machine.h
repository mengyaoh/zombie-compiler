#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
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
	template <typename MachineTraits>
	friend class Machine;
	
	enum Facing { UP, RIGHT, DOWN, LEFT};
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
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
	mOps.push_back(std::make_unique<OpRotate>(0));
	mOps.push_back(std::make_unique<OpRotate>(0));
	mOps.push_back(std::make_unique<OpRotate>(1));
	mOps.push_back(std::make_unique<OpGoto>(1));
	// END TEMP CODE
}

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
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}
