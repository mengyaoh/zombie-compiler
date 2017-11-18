//
//  World.hpp
//  main
//
//  Created by Mengyao He on 11/8/17.
//

#ifndef World_hpp
#define World_hpp
//#define DECLARE_SINGLETON(World) friend class Singleton<World>;
#include <cstdio>
#include "Singleton.h"
#include "Machine.h"
class World: public Singleton<World> {
public:
    void AddZombie(std::shared_ptr<MachineState> zombie);
    void AddHuman (std::shared_ptr<MachineState> human);
    std::vector<std::shared_ptr<MachineState>>& GetZombie (){return mZombies;}
    std::vector<std::shared_ptr<MachineState>>& GetHuman (){return mHumans; }
    std::shared_ptr<MachineState> FindStateAt(int x, int y);
    std::shared_ptr<MachineState> FindStateAtFront(int x, int y, int f);
private:
    std::vector<std::shared_ptr<MachineState>> mZombies;
    std::vector<std::shared_ptr<MachineState>> mHumans;
    
};

#endif /* World_hpp */
