#include "catch.hpp"
#include <string>
#include <chrono>
#include "World.hpp"
#include "Op.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
    SECTION("Add Human")
    {
        MachineState ms;
        World::Get().AddHuman(std::make_shared<MachineState>(ms));
        REQUIRE(World::Get().GetHuman().size() == 1);
    }
    SECTION("Face Wall")
    {
        MachineState ms;
        ms.mX = 0;
        ms.mY = 19;
        OpTestWall otw = OpTestWall();
        otw.Execute(ms);
        REQUIRE(ms.mTest == true);
    }
    
    SECTION("Go Forward")
    {
        MachineState ms;
        ms.mX = 0;
        ms.mY = 16;
        OpForward otw = OpForward();
        otw.Execute(ms);
        REQUIRE(ms.mY == 17);
    }
    
    SECTION("find state")
    {
        MachineState ms1;
        ms1.mX=0;
        ms1.mY=17;
        MachineState ms;
        ms.mX = 0;
        ms.mY = 16;
        World::Get().AddHuman(std::make_shared<MachineState>(ms));
        World::Get().AddZombie(std::make_shared<MachineState>(ms1));
        REQUIRE(World::Get().FindStateAt(0, 17)!=nullptr);
        
    }
    
    SECTION("find front state")
    {
        MachineState ms1;
        ms1.mX=0;
        ms1.mY=17;
        MachineState ms;
        ms.mX = 0;
        ms.mY = 16;
        World::Get().AddHuman(std::make_shared<MachineState>(ms));
        World::Get().AddZombie(std::make_shared<MachineState>(ms1));
        REQUIRE(World::Get().FindStateAtFront(0, 16, 1)!=nullptr);
        
    }
}
