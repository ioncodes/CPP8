#include "Chip8.h"
#include "Instructions.h"
#include <array>
#include <iostream>
#include <fstream>
#include <vector>

Chip8::Chip8()
{
	Init();
	LoadFontSet();
}

void Chip8::Reset()
{
	programCounter = 0x200;
	opcode = 0x000;
	indexRegister = 0x000;
	stackPointer = 0x000;
	soundTimer = 0x000;
	delayTimer = 0x000;
	memory = {0x000};
	stack = {0x000};
	virtualRegisters = {0x000};
	m_screen = {0x000};
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

void Chip8::LoadRom(const char* file_path)
{
	// Load rom into memory
	std::cout << "Loading ROM: " << file_path << std::endl;
	std::ifstream rom;
	rom.open(file_path, std::ios::binary | std::ios::ate);

	std::cout << rom.is_open() << std::endl;

	std::streamsize size = rom.tellg();
	rom.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	rom.read(buffer.data(), size);

	rom.close();

	std::cout << size << std::endl;

	for (int i = 0; i < buffer.size(); i++)
	{
		memory.at(programCounter + i) = static_cast<unsigned int>(buffer.at(i));
	}
}

void Chip8::Cycle()
{
	opcode = memory.at(programCounter) << 8 | memory.at(programCounter + 1);

	if (opcode & 0xF000 == 0x0000)
	{
		switch (instructions.getInstruction(opcode & 0x000F))
		{
		case Instructions::Clear:
			m_screen = {0};
			programCounter += 2;
			break;

		case Instructions::ReturnToSubroutine:
			--stackPointer;
			programCounter = stack[stackPointer];
			programCounter += 2;
			break;
		case Instructions::InstructionUnknown:
			std::cout << "Opcode not found: " << std::hex << (opcode & 0x000F) << std::endl;
		default: 
			std::cout << "Unknown Error" << std::endl;
		}
	}
	else
	{
		switch (instructions.getInstruction(opcode & 0xF000))
		{
		case Instructions::JumpToAddress:
			programCounter = opcode & 0x0FFF;
			break;

		case Instructions::CallSubroutine:
			stack[stackPointer] = programCounter;
			++stackPointer;
			programCounter = opcode & 0x0FFF;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNN:
			if (virtualRegisters[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				programCounter += 4;
			else
				programCounter += 2;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotNN:
			if (virtualRegisters[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				programCounter += 4;
			else
				programCounter += 2;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsVirtual:
			if (virtualRegisters[(opcode & 0x0F00) >> 8] == virtualRegisters[(opcode & 0x00F0) >> 4])
				programCounter += 4;
			else
				programCounter += 2;
			break;

		case Instructions::SetVirtualToNN:
			virtualRegisters[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			programCounter += 2;
			break;

		case Instructions::AddNNToVirtual:
			virtualRegisters[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			programCounter += 2;
			break;

		case Instructions::Virtuals:
			switch (instructions.getVirtual(opcode & 0x000F))
			{
			case Instructions::SetVirtualToVirtual:
				virtualRegisters[(opcode & 0x0F00) >> 8] = virtualRegisters[(opcode & 0x00F0) >> 4];
				programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualOR:
				virtualRegisters[(opcode & 0x0F00) >> 8] |= virtualRegisters[(opcode & 0x00F0) >> 4];
				programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualAND:
				virtualRegisters[(opcode & 0x0F00) >> 8] &= virtualRegisters[(opcode & 0x00F0) >> 4];
				programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualXOR:
				virtualRegisters[(opcode & 0x0F00) >> 8] ^= virtualRegisters[(opcode & 0x00F0) >> 4];
				programCounter += 2;
				break;

			case Instructions::AddVirtualToVirtual:
				virtualRegisters[(opcode & 0x0F00) >> 8] += virtualRegisters[(opcode & 0x00F0) >> 4];
				if (virtualRegisters[(opcode & 0x00F0) >> 4] > (0xFF - virtualRegisters[(opcode & 0x0F00) >> 8]))
					virtualRegisters[0xF] = 1;
				else
					virtualRegisters[0xF] = 0;
				programCounter += 2;
				break;

			case Instructions::SubVirtualFromVirtual:
				if (virtualRegisters[(opcode & 0x00F0) >> 4] > virtualRegisters[(opcode & 0x0F00) >> 8])
					virtualRegisters[0xF] = 0;
				else
					virtualRegisters[0xF] = 1;
				virtualRegisters[(opcode & 0x0F00) >> 8] -= virtualRegisters[(opcode & 0x00F0) >> 4];
				programCounter += 2;
				break;

			case Instructions::ShiftVirtualRight:
				virtualRegisters[0xF] = virtualRegisters[(opcode & 0x0F00) >> 8] & 0x1;
				virtualRegisters[(opcode & 0x0F00) >> 8] >>= 1;
				programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualMinus:
				if (virtualRegisters[(opcode & 0x0F00) >> 8] > virtualRegisters[(opcode & 0x00F0) >> 4])
					virtualRegisters[0xF] = 0;
				else
					virtualRegisters[0xF] = 1;
				virtualRegisters[(opcode & 0x0F00) >> 8] = virtualRegisters[(opcode & 0x00F0) >> 4] - virtualRegisters[(opcode & 0x0F00) >> 8];
				programCounter += 2;
				break;

			case Instructions::ShiftVirtualLeft:
				virtualRegisters[0xF] = virtualRegisters[(opcode & 0x0F00) >> 8] >> 7;
				virtualRegisters[(opcode & 0x0F00) >> 8] <<= 1;
				programCounter += 2;
				break;
			case Instructions::VirtualUnknown:
				std::cout << "Opcode not found: " << std::hex << (opcode & 0x000F) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;;
			}
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotVirtual:
			if (virtualRegisters[(opcode & 0x0F00) >> 8] != virtualRegisters[(opcode & 0x00F0) >> 4])
				programCounter += 4;
			else
				programCounter += 2;
			break;

		case Instructions::SetIndexRegisterToNNN:
			indexRegister = opcode & 0x0FFF;
			programCounter += 2;
			break;

		case Instructions::JumpToAddressPlusV0:
			programCounter = (opcode & 0x0FFF) + virtualRegisters[0];
			break;

		case Instructions::SetVirtualToRandom:
			virtualRegisters[(opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (opcode & 0x00FF);
			programCounter += 2;
			break;

		case Instructions::DrawSprite:
			{
				unsigned short x = virtualRegisters[(opcode & 0x0F00) >> 8];
				unsigned short y = virtualRegisters[(opcode & 0x00F0) >> 4];
				unsigned short height = opcode & 0x000F;

				virtualRegisters[0xF] = 0;
				for (int yline = 0; yline < height; yline++)
				{
					unsigned short pixel = memory[indexRegister + yline];
					for (int xline = 0; xline < 8; xline++)
					{
						if ((pixel & (0x80 >> xline)) != 0)
						{
							if (m_screen[(x + xline + ((y + yline) * 64))] == 1)
							{
								virtualRegisters[0xF] = 1;
							}
							m_screen[x + xline + ((y + yline) * 64)] ^= 1;
						}
					}
				}

				programCounter += 2;
			}
			break;

		case Instructions::Keys:

			switch (instructions.getKey(opcode & 0x00FF))
			{
			case Instructions::SkipIfPressed:
				if (keypad[virtualRegisters[(opcode & 0x0F00) >> 8]] != 0)
					programCounter += 4;
				else
					programCounter += 2;
				break;

			case Instructions::SkipIfNotPressed:
				if (keypad[virtualRegisters[(opcode & 0x0F00) >> 8]] == 0)
					programCounter += 4;
				else
					programCounter += 2;
				break;

			case Instructions::KeyUnknown:
				std::cout << "Opcode not found: " << std::hex << (opcode & 0x00FF) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;
			}
			break;

		case Instructions::FXS:
			switch (instructions.getFX(opcode & 0x00FF))
			{
			case Instructions::SetVirtualToDelayTimer:
				virtualRegisters[(opcode & 0x0F00) >> 8] = delayTimer;
				programCounter += 2;
				break;

			case Instructions::WaitForKeyAndStore:
				{
					bool key_pressed = false;

					for (int i = 0; i < 16; ++i)
					{
						if (keypad[i] != 0)
						{
							virtualRegisters[(opcode & 0x0F00) >> 8] = i;
							key_pressed = true;
						}
					}

					if (!key_pressed)
						return;

					programCounter += 2;
				}
				break;

			case Instructions::SetDelayTimerToVirtual:
				delayTimer = virtualRegisters[(opcode & 0x0F00) >> 8];
				programCounter += 2;
				break;

			case Instructions::SetSoundTimerToVirtual:
				soundTimer = virtualRegisters[(opcode & 0x0F00) >> 8];
				programCounter += 2;
				break;

			case Instructions::AddVirtualToIndex:
				if (indexRegister + virtualRegisters[(opcode & 0x0F00) >> 8] > 0xFFF)
					virtualRegisters[0xF] = 1;
				else
					virtualRegisters[0xF] = 0;
				indexRegister += virtualRegisters[(opcode & 0x0F00) >> 8];
				programCounter += 2;
				break;

			case Instructions::SetIndexRegisterToVirtualSprite:
				indexRegister = virtualRegisters[(opcode & 0x0F00) >> 8] * 0x5;
				programCounter += 2;
				break;

			case Instructions::Decimals:
				memory[indexRegister] = virtualRegisters[(opcode & 0x0F00) >> 8] / 100;
				memory[indexRegister + 1] = (virtualRegisters[(opcode & 0x0F00) >> 8] / 10) % 10;
				memory[indexRegister + 2] = (virtualRegisters[(opcode & 0x0F00) >> 8] % 100) % 10;
				programCounter += 2;
				break;

			case Instructions::StoreVirtualToMemory1:
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
					memory[indexRegister + i] = virtualRegisters[i];

				indexRegister += ((opcode & 0x0F00) >> 8) + 1;
				programCounter += 2;
				break;

			case Instructions::StoreVirtualToMemory2:
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
					virtualRegisters[i] = memory[indexRegister + i];

				indexRegister += ((opcode & 0x0F00) >> 8) + 1;
				programCounter += 2;
				break;
			case Instructions::UnknownFX:
				std::cout << "Opcode not found: " << std::hex << (opcode & 0x00FF) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;;
			}
			break;
		case Instructions::InstructionUnknown:
			std::cout << "Opcode not found: " << std::hex << (opcode & 0xF000) << std::endl;
		default:
			std::cout << "Unknown Error" << std::endl;; ;
		}
	}
}

Chip8::~Chip8()
{
}
