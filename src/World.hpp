//
//  World.hpp
//  main
//
//  Created by Mengyao He on 11/8/17.
//

#ifndef World_hpp
#define World_hpp
#define DECLARE_SINGLETON(World) friend class Singleton<World>;
#include <stdio.h>
//#include "Singleton.h"
#include "Machine.h"
class World {
public:
    
private:
    
    std::vector<std::shared_ptr<MachineState>> mZombies;
    std::vector<std::shared_ptr<MachineState>> mHumans;
    
};

#endif /* World_hpp */
