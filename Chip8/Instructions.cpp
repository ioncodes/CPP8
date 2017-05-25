#include "Instructions.h"

Instructions::Instruction Instructions::getInstruction(unsigned int opcode) const
{
	auto instruction = instructions.find(opcode);
	if (instruction != instructions.end())
		return instruction->second;
	return InstructionUnknown;
}

Instructions::Virtual Instructions::getVirtual(unsigned int opcode) const
{
	auto instruction = virtuals.find(opcode);
	if (instruction != virtuals.end())
		return instruction->second;
	return VirtualUnknown;
}

Instructions::Key Instructions::getKey(unsigned int opcode) const
{
	auto instruction = keys.find(opcode);
	if (instruction != keys.end())
		return instruction->second;
	return KeyUnknown;
}

Instructions::FX Instructions::getFX(unsigned int opcode) const
{
	auto instruction = fxs.find(opcode);
	if (instruction != fxs.end())
		return instruction->second;
	return UnknownFX;
}
