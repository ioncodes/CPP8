#include "Chip8.h"
#include <array>

Chip8::Chip8()
{
	Init();
	LoadFontSet();
}

void Chip8::Reset()
{
	programCounter		= 0x200;
	opcode				= 0x000;
	indexRegister		= 0x000;
	stackPointer		= 0x000;
	soundTimer			= 0x000;
	delayTimer			= 0x000;
	memory				= { 0x000 };
	stack				= { 0x000 };
	virtualRegisters	= { 0x000 };
}

void Chip8::Init()
{
	Reset();
	LoadFontSet();
}

void Chip8::LoadFontSet()	
{
	// Load font into memory
	for (int i = 0; i < FONT_SIZE; i++)
	{
		memory.at(i) = static_cast<int>(FONT_SET.at(i));
	}
}

Chip8::~Chip8()
{
}
