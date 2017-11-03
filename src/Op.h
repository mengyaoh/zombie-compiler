// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const wchar_t* OpName, int parameter)
		: mParam(parameter)
		, mOpName(OpName)
	{ }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;
protected:
	int mParam;
	const wchar_t* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op(L"OpRotate", parameter)
	{ }

	void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op(L"OpGoto", parameter)
	{ }

	void Execute(MachineState& state) override;
};
