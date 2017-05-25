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
	m_programCounter = 0x200;
	m_opcode = 0x000;
	m_indexRegister = 0x000;
	m_stackPointer = 0x000;
	m_soundTimer = 0x000;
	m_delayTimer = 0x000;
	m_memory = {0x000};
	m_stack = {0x000};
	m_virtualRegisters = {0x000};
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
		m_memory.at(i) = static_cast<int>(FONT_SET.at(i));
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
		m_memory.at(m_programCounter + i) = static_cast<unsigned int>(buffer.at(i));
	}
}

void Chip8::Cycle()
{
	m_opcode = m_memory.at(m_programCounter) << 8 | m_memory.at(m_programCounter + 1);

	if (m_opcode & 0xF000 == 0x0000)
	{
		switch (m_instructions.getInstruction(m_opcode & 0x000F))
		{
		case Instructions::Clear:
			m_screen = {0};
			m_programCounter += 2;
			break;

		case Instructions::ReturnToSubroutine:
			--m_stackPointer;
			m_programCounter = m_stack[m_stackPointer];
			m_programCounter += 2;
			break;
		case Instructions::InstructionUnknown:
			std::cout << "Opcode not found: " << std::hex << (m_opcode & 0x000F) << std::endl;
		default: 
			std::cout << "Unknown Error" << std::endl;
		}
	}
	else
	{
		switch (m_instructions.getInstruction(m_opcode & 0xF000))
		{
		case Instructions::JumpToAddress:
			m_programCounter = m_opcode & 0x0FFF;
			break;

		case Instructions::CallSubroutine:
			m_stack[m_stackPointer] = m_programCounter;
			++m_stackPointer;
			m_programCounter = m_opcode & 0x0FFF;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNN:
			if (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] == (m_opcode & 0x00FF))
				m_programCounter += 4;
			else
				m_programCounter += 2;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotNN:
			if (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] != (m_opcode & 0x00FF))
				m_programCounter += 4;
			else
				m_programCounter += 2;
			break;

		case Instructions::SkipInstructionIfVirtualEqualsVirtual:
			if (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] == m_virtualRegisters[(m_opcode & 0x00F0) >> 4])
				m_programCounter += 4;
			else
				m_programCounter += 2;
			break;

		case Instructions::SetVirtualToNN:
			m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = m_opcode & 0x00FF;
			m_programCounter += 2;
			break;

		case Instructions::AddNNToVirtual:
			m_virtualRegisters[(m_opcode & 0x0F00) >> 8] += m_opcode & 0x00FF;
			m_programCounter += 2;
			break;

		case Instructions::Virtuals:
			switch (m_instructions.getVirtual(m_opcode & 0x000F))
			{
			case Instructions::SetVirtualToVirtual:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				m_programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualOR:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] |= m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				m_programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualAND:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] &= m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				m_programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualXOR:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] ^= m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				m_programCounter += 2;
				break;

			case Instructions::AddVirtualToVirtual:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] += m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				if (m_virtualRegisters[(m_opcode & 0x00F0) >> 4] > (0xFF - m_virtualRegisters[(m_opcode & 0x0F00) >> 8]))
					m_virtualRegisters[0xF] = 1;
				else
					m_virtualRegisters[0xF] = 0;
				m_programCounter += 2;
				break;

			case Instructions::SubVirtualFromVirtual:
				if (m_virtualRegisters[(m_opcode & 0x00F0) >> 4] > m_virtualRegisters[(m_opcode & 0x0F00) >> 8])
					m_virtualRegisters[0xF] = 0;
				else
					m_virtualRegisters[0xF] = 1;
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] -= m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				m_programCounter += 2;
				break;

			case Instructions::ShiftVirtualRight:
				m_virtualRegisters[0xF] = m_virtualRegisters[(m_opcode & 0x0F00) >> 8] & 0x1;
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] >>= 1;
				m_programCounter += 2;
				break;

			case Instructions::SetVirtualToVirtualMinus:
				if (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] > m_virtualRegisters[(m_opcode & 0x00F0) >> 4])
					m_virtualRegisters[0xF] = 0;
				else
					m_virtualRegisters[0xF] = 1;
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = m_virtualRegisters[(m_opcode & 0x00F0) >> 4] - m_virtualRegisters[(m_opcode & 0x0F00) >> 8];
				m_programCounter += 2;
				break;

			case Instructions::ShiftVirtualLeft:
				m_virtualRegisters[0xF] = m_virtualRegisters[(m_opcode & 0x0F00) >> 8] >> 7;
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] <<= 1;
				m_programCounter += 2;
				break;
			case Instructions::VirtualUnknown:
				std::cout << "Opcode not found: " << std::hex << (m_opcode & 0x000F) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;;
			}
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotVirtual:
			if (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] != m_virtualRegisters[(m_opcode & 0x00F0) >> 4])
				m_programCounter += 4;
			else
				m_programCounter += 2;
			break;

		case Instructions::SetIndexRegisterToNNN:
			m_indexRegister = m_opcode & 0x0FFF;
			m_programCounter += 2;
			break;

		case Instructions::JumpToAddressPlusV0:
			m_programCounter = (m_opcode & 0x0FFF) + m_virtualRegisters[0];
			break;

		case Instructions::SetVirtualToRandom:
			m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (m_opcode & 0x00FF);
			m_programCounter += 2;
			break;

		case Instructions::DrawSprite:
			{
				unsigned short x = m_virtualRegisters[(m_opcode & 0x0F00) >> 8];
				unsigned short y = m_virtualRegisters[(m_opcode & 0x00F0) >> 4];
				unsigned short height = m_opcode & 0x000F;

				m_virtualRegisters[0xF] = 0;
				for (int yline = 0; yline < height; yline++)
				{
					unsigned short pixel = m_memory[m_indexRegister + yline];
					for (int xline = 0; xline < 8; xline++)
					{
						if ((pixel & (0x80 >> xline)) != 0)
						{
							if (m_screen[(x + xline + ((y + yline) * 64))] == 1)
							{
								m_virtualRegisters[0xF] = 1;
							}
							m_screen[x + xline + ((y + yline) * 64)] ^= 1;
						}
					}
				}

				m_programCounter += 2;
			}
			break;

		case Instructions::Keys:

			switch (m_instructions.getKey(m_opcode & 0x00FF))
			{
			case Instructions::SkipIfPressed:
				if (m_keypad[m_virtualRegisters[(m_opcode & 0x0F00) >> 8]] != 0)
					m_programCounter += 4;
				else
					m_programCounter += 2;
				break;

			case Instructions::SkipIfNotPressed:
				if (m_keypad[m_virtualRegisters[(m_opcode & 0x0F00) >> 8]] == 0)
					m_programCounter += 4;
				else
					m_programCounter += 2;
				break;

			case Instructions::KeyUnknown:
				std::cout << "Opcode not found: " << std::hex << (m_opcode & 0x00FF) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;
			}
			break;

		case Instructions::FXS:
			switch (m_instructions.getFX(m_opcode & 0x00FF))
			{
			case Instructions::SetVirtualToDelayTimer:
				m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = m_delayTimer;
				m_programCounter += 2;
				break;

			case Instructions::WaitForKeyAndStore:
				{
					bool key_pressed = false;

					for (int i = 0; i < 16; ++i)
					{
						if (m_keypad[i] != 0)
						{
							m_virtualRegisters[(m_opcode & 0x0F00) >> 8] = i;
							key_pressed = true;
						}
					}

					if (!key_pressed)
						return;

					m_programCounter += 2;
				}
				break;

			case Instructions::SetDelayTimerToVirtual:
				m_delayTimer = m_virtualRegisters[(m_opcode & 0x0F00) >> 8];
				m_programCounter += 2;
				break;

			case Instructions::SetSoundTimerToVirtual:
				m_soundTimer = m_virtualRegisters[(m_opcode & 0x0F00) >> 8];
				m_programCounter += 2;
				break;

			case Instructions::AddVirtualToIndex:
				if (m_indexRegister + m_virtualRegisters[(m_opcode & 0x0F00) >> 8] > 0xFFF)
					m_virtualRegisters[0xF] = 1;
				else
					m_virtualRegisters[0xF] = 0;
				m_indexRegister += m_virtualRegisters[(m_opcode & 0x0F00) >> 8];
				m_programCounter += 2;
				break;

			case Instructions::SetIndexRegisterToVirtualSprite:
				m_indexRegister = m_virtualRegisters[(m_opcode & 0x0F00) >> 8] * 0x5;
				m_programCounter += 2;
				break;

			case Instructions::Decimals:
				m_memory[m_indexRegister] = m_virtualRegisters[(m_opcode & 0x0F00) >> 8] / 100;
				m_memory[m_indexRegister + 1] = (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] / 10) % 10;
				m_memory[m_indexRegister + 2] = (m_virtualRegisters[(m_opcode & 0x0F00) >> 8] % 100) % 10;
				m_programCounter += 2;
				break;

			case Instructions::StoreVirtualToMemory1:
				for (int i = 0; i <= ((m_opcode & 0x0F00) >> 8); ++i)
					m_memory[m_indexRegister + i] = m_virtualRegisters[i];

				m_indexRegister += ((m_opcode & 0x0F00) >> 8) + 1;
				m_programCounter += 2;
				break;

			case Instructions::StoreVirtualToMemory2:
				for (int i = 0; i <= ((m_opcode & 0x0F00) >> 8); ++i)
					m_virtualRegisters[i] = m_memory[m_indexRegister + i];

				m_indexRegister += ((m_opcode & 0x0F00) >> 8) + 1;
				m_programCounter += 2;
				break;
			case Instructions::UnknownFX:
				std::cout << "Opcode not found: " << std::hex << (m_opcode & 0x00FF) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;;
			}
			break;
		case Instructions::InstructionUnknown:
			std::cout << "Opcode not found: " << std::hex << (m_opcode & 0xF000) << std::endl;
		default:
			std::cout << "Unknown Error" << std::endl;; ;
		}
	}
}

Chip8::~Chip8()
{
}
