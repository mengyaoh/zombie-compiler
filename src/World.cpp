//
//  World.cpp
//  main
//
//  Created by Mengyao He on 11/8/17.
//

#include "World.hpp"

void World::AddZombie(std::shared_ptr<MachineState> zombie)
{
    mZombies.push_back(zombie);
}

void World::AddHuman(std::shared_ptr<MachineState> human)
{
    mHumans.push_back(human);
}

std::shared_ptr<MachineState> World::FindStateAt(int x, int y){
    for(int i=0 ; i<mZombies.size();i++){
        if(mZombies.at(i)->mX ==x && mZombies.at(i)->mY ==y){
            return mZombies.at(i);
        }
    }
    for(int i=0 ; i<mHumans.size();i++){
        if(mHumans.at(i)->mX ==x && mHumans.at(i)->mY ==y){
            return mHumans.at(i);
        }
    }
    return nullptr;
}
