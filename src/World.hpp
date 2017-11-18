//
//  World.hpp
//  main
//
//  Created by Mengyao He on 11/8/17.
//

#ifndef World_hpp
#define World_hpp
//#define DECLARE_SINGLETON(World) friend class Singleton<World>;
#include <stdio.h>
#include "Singleton.h"
#include "Machine.h"
#include "Traits.h"
class World: public Singleton<World> {
public:
    void AddZombie(std::shared_ptr<MachineState> zombie);
    void AddHuman (std::shared_ptr<MachineState> human);
    std::vector<std::shared_ptr<MachineState>>& GetZombie (){return mZombies;}
    std::vector<std::shared_ptr<MachineState>>& GetHuman (){return mHumans; }
    Machine<ZombieTraits>& GetMyZombieMachine() {return mZombieMachine; }
    Machine<HumanTraits>& GetMyHumanMachine() {return mHumanMachine; }
    std::shared_ptr<MachineState> FindStateAt(int x, int y);
    std::shared_ptr<MachineState> FindStateAtFront(int x, int y, int f);
    
private:
    std::vector<std::shared_ptr<MachineState>> mZombies;
    std::vector<std::shared_ptr<MachineState>> mHumans;
    Machine<ZombieTraits> mZombieMachine;
    Machine<HumanTraits> mHumanMachine;
    
};

#endif /* World_hpp */
