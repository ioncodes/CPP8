#pragma once
#include <array>
#include "Instructions.h"

class Chip8
{
	static const unsigned int MEMORY_SIZE = 4096; // Memory Size
	static const unsigned int GLOBAL_SIZE = 0x10; // General size is 16bit
	static const unsigned int FONT_SIZE = 0x50; // Font Size = 80
	static const unsigned int SCREEN_SIZE = 0x800; // Screen Size 2048; 32 * 64
	unsigned int m_programCounter = 0x200; // Program Counter
	unsigned int m_opcode = 0x000; // Current Opcode
	unsigned int m_stackPointer = 0x000; // Stack Pointer
	unsigned int m_indexRegister = 0x000; // I [Index Register]
	unsigned int m_soundTimer = 0x000; // Timer for the sound
	unsigned int m_delayTimer = 0x000; // Timer for the delay
	std::array<unsigned int, MEMORY_SIZE> m_memory = { 0x000 }; // Memory
	std::array<unsigned int, GLOBAL_SIZE> m_virtualRegisters = { 0x000 }; // Virtual Registers -> V[x]; 16 registers
	std::array<unsigned int, GLOBAL_SIZE> m_keypad = {0x000}; // The keypad
	std::array<unsigned int, GLOBAL_SIZE> m_stack = {0x000}; // 16bit Stack
	std::array<unsigned int, SCREEN_SIZE> m_screen = {0x000}; // The screen
	Instructions m_instructions; // Instructions
	const std::array<unsigned char, FONT_SIZE> FONT_SET = // The fontset
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80 //F
	};
public:
	Chip8();
	void Reset();
	void Init();
	void LoadFontSet();
	void LoadRom(const char* file_path);
	void Cycle();

	std::array<std::array<unsigned int, 32>, 64> screen() const
	{
		std::array<std::array<unsigned int, 32>, 64> map = { 1 };
		int j = 0;
		int k = 0;
		for (int i = 0; i < 2048; i++)
		{
			if (m_screen.at(i) == 0x00000000)
			{
				map[k][j] = 0;
			}
			else
			{
				map[k][j] = 1;
			}
			if (k == 63)
			{
				j++;
				k = 0;
			}
			else
			{
				k++;
			}
		}
		return map;
	}
	~Chip8();

	friend class CPU;
};
