#include "Op.h"
#include "Machine.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>
#include <ctime>
#include "World.hpp"
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
    if(CanMove(state)){
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                state.mY = state.mY-1;
                break;
            case (MachineState::RIGHT) :
                state.mX = state.mX+1;
                break;
            case (MachineState::DOWN) :
                state.mY = state.mY+1;
                break;
            default:
            case (MachineState::LEFT) :
                state.mX = state.mX-1;
                break;
        }
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

void OpTestWall::Execute(MachineState& state)
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
            if (state.mX == 0){
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

void OpTestRandom::Execute(MachineState& state)
{
    DebugOutput(state);
    srand( time(nullptr) );
    int randNum = rand() % 2;
    //std::cout<<randNum<<"\n";
    state.mTest = static_cast<bool>(randNum);
    state.mProgramCounter++;
}

bool Op::CanMove(MachineState& state){
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            return(state.mY != 0 && World::Get().FindStateAt(state.mX, state.mY-1)==nullptr);
            break;
        case (MachineState::RIGHT) :
            return(state.mX != 19 && World::Get().FindStateAt(state.mX + 1, state.mY)==nullptr);
            break;
        case (MachineState::DOWN) :
            return(state.mY != 19 && World::Get().FindStateAt(state.mX, state.mY + 1)==nullptr);
            break;
        default:
        case (MachineState::LEFT) :
            return(state.mX != 0 && World::Get().FindStateAt(state.mX - 1, state.mY)==nullptr);
            break;
    }
}

void OpTestHuman::Execute(MachineState& state)
{
    DebugOutput(state);
    std::shared_ptr<MachineState> ms =World::Get().FindStateAtFront(state.mX, state.mY, mParam);
    if(ms==nullptr){
        state.mTest = false;
    }
    else if(ms->GetInfect()){
        state.mTest = false;
    }
    else if(!ms->GetInfect()){
        state.mTest = true;
    }
    state.mProgramCounter++;
}

void OpTestZombie::Execute(MachineState& state)
{
    DebugOutput(state);
    std::shared_ptr<MachineState> ms =World::Get().FindStateAtFront(state.mX, state.mY, mParam);
    if(ms==nullptr){
        state.mTest = false;
    }
    else if(ms->GetInfect()){
        state.mTest = true;
    }
    else if(!ms->GetInfect()){
        state.mTest = false;
    }
    state.mProgramCounter++;
}

void OpTestPassable::Execute(MachineState& state)
{
    DebugOutput(state);
    state.mTest = CanMove(state);
    state.mProgramCounter++;
}

void OpEndTurn::Execute(MachineState& state)
{
    DebugOutput(state);
    if (state.GetInfect()){
        state.mActionsTaken = 10;
    }
    else {
        state.mActionsTaken = 20;
    }
    state.mProgramCounter++;
}

void OpAttack::Execute(MachineState& state)
{
    DebugOutput(state);
    std::shared_ptr<MachineState> toattact = World::Get().FindStateAtFront(state.mX, state.mY,1);
    //if nothing in front
    if(toattact==nullptr){
        if (state.GetInfect()){
            state.mActionsTaken = 2;
        }
        else {
            state.mActionsTaken = 3;
        }
        state.mProgramCounter++;
        return;
    }
    //human kill zombie
    else if(!state.GetInfect()&&toattact->GetInfect()){
        //Iter: pointer to the share pointer of machinestate
        for (auto iter = World::Get().GetZombie().begin(); iter != World::Get().GetZombie().end(); iter++){
            if (*iter == toattact){
                World::Get().GetZombie().erase(iter);
                break;
            }
        }
    }
    //human attact human
    else if(!state.GetInfect()&&!toattact->GetInfect()){
        for (auto iter = World::Get().GetHuman().begin(); iter != World::Get().GetHuman().end(); iter++){
            if (*iter == toattact){
                World::Get().GetHuman().erase(iter);
                break;
            }
        }
    }
    //zombie kill human
    else if(state.GetInfect()&&!toattact->GetInfect()){
        for (auto iter = World::Get().GetHuman().begin(); iter != World::Get().GetHuman().end(); iter++){
            if (*iter == toattact){
                World::Get().GetHuman().erase(iter);
                break;
            }
        }
        //std::cout<<"before"<<toattact->mProgramCounter<<std::endl;
        World::Get().GetMyZombieMachine().BindState(*toattact);
        //std::cout<<toattact->mProgramCounter<<std::endl;
        World::Get().GetZombie().push_back(toattact);
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}
void OpRangeAttack::Execute(MachineState& state)
{
    DebugOutput(state);
    try{
        if (state.GetInfect())
        {
            // throws exception if zombie call range attack
            throw RangeAttackException();
        }
    }catch(RangeAttackException ex){
        ex.what();
    }
    std::shared_ptr<MachineState> toattact = World::Get().FindStateAtFront(state.mX, state.mY,mParam);
    //if nothing in front
    if(toattact==nullptr){
        if (state.GetInfect()){
            state.mActionsTaken = 2;
        }
        else {
            state.mActionsTaken = 3;
        }
        state.mProgramCounter++;
        return;
    }
    //human kill zombie
    else if(!state.GetInfect()&&toattact->GetInfect()){
        //Iter: pointer to the share pointer of machinestate
        for (auto iter = World::Get().GetZombie().begin(); iter != World::Get().GetZombie().end(); iter++){
            if (*iter == toattact){
                World::Get().GetZombie().erase(iter);
                break;
            }
        }
    }
    //human attact human
    else if(!state.GetInfect()&&!toattact->GetInfect()){
        for (auto iter = World::Get().GetHuman().begin(); iter != World::Get().GetHuman().end(); iter++){
            if (*iter == toattact){
                World::Get().GetHuman().erase(iter);
                break;
            }
        }
    }
    //zombie kill human
    else if(state.GetInfect()&&!toattact->GetInfect()){
        for (auto iter = World::Get().GetHuman().begin(); iter != World::Get().GetHuman().end(); iter++){
            if (*iter == toattact){
                World::Get().GetHuman().erase(iter);
                break;
            }
        }
        //std::cout<<"before"<<toattact->mProgramCounter<<std::endl;
        World::Get().GetMyZombieMachine().BindState(*toattact);
        //std::cout<<toattact->mProgramCounter<<std::endl;
        World::Get().GetZombie().push_back(toattact);
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}
