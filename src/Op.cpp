#include "Op.h"
#include "Machine.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	wxLogDebug("%d:%s,%d,%d,%d", state.mProgramCounter, mOpName, mParam,state.mX,state.mY);
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN):
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	case (MachineState::LEFT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
}

void OpForward::Execute(MachineState& state)
{
    DebugOutput(state);
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            state.mY--;
            break;
        case (MachineState::RIGHT) :
            state.mX++;
            break;
        case (MachineState::DOWN) :
            state.mY++;
            break;
        default:
        case (MachineState::LEFT) :
            state.mX--;
            break;
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}

void OpJe::Execute(MachineState& state)
{
    DebugOutput(state);
    if (state.mTest){
        state.mProgramCounter = mParam;
        
    }else{
        state.mProgramCounter++;
        
    }
}

void OpTest_wall::Execute(MachineState& state)
{
    DebugOutput(state);
    switch (state.mFacing)
    {
        case (MachineState::RIGHT) :
            if (state.mX == 19){
                state.mTest = true;
            }
            else{
                state.mTest = false;
            }
            break;
        case (MachineState::DOWN) :
            if (state.mY == 19){
                state.mTest = true;
            }
            else{
                state.mTest = false;
            }
            break;
        case (MachineState::LEFT) :
            if (state.mX == 0)
            {
                state.mTest = true;
            }
            else
            {
                state.mTest = false;
            }
            break;
        default:
        case (MachineState::UP) :
            if (state.mY == 0){
                state.mTest = true;
            }
            else{
                state.mTest = false;
            }
            break;
    }
    
    state.mProgramCounter++;
}

void OpTest_random::Execute(MachineState& state)
{
    DebugOutput(state);
    state.mProgramCounter++;
}
