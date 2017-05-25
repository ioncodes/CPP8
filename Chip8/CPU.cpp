#include "CPU.h"
#include "Chip8.h"

void CPU::ReturnToSubroutine(Chip8 chip8) const
{
	--chip8.m_stackPointer;
	chip8.m_programCounter = chip8.m_stack[chip8.m_stackPointer];
	chip8.m_programCounter += 2;
}
