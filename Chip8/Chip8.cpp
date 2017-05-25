#include "Chip8.h"
#include "Instructions.h"
#include <array>
#include <iostream>
#include <fstream>
#include <vector>
#include "CPU.h"

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
			CPU::ClearScreen(*this);
			break;

		case Instructions::ReturnToSubroutine:
			CPU::ReturnToSubroutine(*this);
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
			CPU::JumpToAddress(*this);
			break;

		case Instructions::CallSubroutine:
			CPU::CallSubroutine(*this);
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNN:
			CPU::SkipInstructionIfVirtualEqualsNN(*this);
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotNN:
			CPU::SkipInstructionIfVirtualEqualsNotNN(*this);
			break;

		case Instructions::SkipInstructionIfVirtualEqualsVirtual:
			CPU::SkipInstructionIfVirtualEqualsVirtual(*this);
			break;

		case Instructions::SetVirtualToNN:
			CPU::SetVirtualToNN(*this);
			break;

		case Instructions::AddNNToVirtual:
			CPU::AddNNToVirtual(*this);
			break;

		case Instructions::Virtuals:
			switch (m_instructions.getVirtual(m_opcode & 0x000F))
			{
			case Instructions::SetVirtualToVirtual:
				CPU::SetVirtualToVirtual(*this);
				break;

			case Instructions::SetVirtualToVirtualOR:
				CPU::SetVirtualToVirtualOR(*this);
				break;

			case Instructions::SetVirtualToVirtualAND:
				CPU::SetVirtualToVirtualAND(*this);
				break;

			case Instructions::SetVirtualToVirtualXOR:
				CPU::SetVirtualToVirtualXOR(*this);
				break;

			case Instructions::AddVirtualToVirtual:
				CPU::AddVirtualToVirtual(*this);
				break;

			case Instructions::SubVirtualFromVirtual:
				CPU::SubVirtualFromVirtual(*this);
				break;

			case Instructions::ShiftVirtualRight:
				CPU::ShiftVirtualRight(*this);
				break;

			case Instructions::SetVirtualToVirtualMinus:
				CPU::SetVirtualToVirtualMinus(*this);
				break;

			case Instructions::ShiftVirtualLeft:
				CPU::ShiftVirtualLeft(*this);
				break;
			case Instructions::VirtualUnknown:
				std::cout << "Opcode not found: " << std::hex << (m_opcode & 0x000F) << std::endl;
			default:
				std::cout << "Unknown Error" << std::endl;;
			}
			break;

		case Instructions::SkipInstructionIfVirtualEqualsNotVirtual:
			CPU::SkipInstructionIfVirtualEqualsNotVirtual(*this);
			break;

		case Instructions::SetIndexRegisterToNNN:
			CPU::SetIndexRegisterToNNN(*this);
			break;

		case Instructions::JumpToAddressPlusV0:
			CPU::JumpToAddressPlusV0(*this);

		case Instructions::SetVirtualToRandom:
			CPU::SetVirtualToRandom(*this);
			break;

		case Instructions::DrawSprite:
			CPU::DrawSprite(*this);
			break;

		case Instructions::Keys:

			switch (m_instructions.getKey(m_opcode & 0x00FF))
			{
			case Instructions::SkipIfPressed:
				CPU::SkipIfPressed(*this);
				break;

			case Instructions::SkipIfNotPressed:
				CPU::SkipIfNotPressed(*this);
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
				CPU::SetVirtualToDelayTimer(*this);
				break;

			case Instructions::WaitForKeyAndStore:
				CPU::WaitForKeyAndStore(*this);
				break;

			case Instructions::SetDelayTimerToVirtual:
				CPU::SetDelayTimerToVirtual(*this);
				break;

			case Instructions::SetSoundTimerToVirtual:
				CPU::SetSoundTimerToVirtual(*this);
				break;

			case Instructions::AddVirtualToIndex:
				CPU::AddVirtualToIndex(*this);
				break;

			case Instructions::SetIndexRegisterToVirtualSprite:
				CPU::SetIndexRegisterToVirtualSprite(*this);
				break;

			case Instructions::Decimals:
				CPU::Decimals(*this);
				break;

			case Instructions::StoreVirtualToMemory1:
				CPU::StoreVirtualToMemory1(*this);
				break;

			case Instructions::StoreVirtualToMemory2:
				CPU::StoreVirtualToMemory2(*this);
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
			std::cout << "Unknown Error" << std::endl;;;
		}
	}
}

Chip8::~Chip8()
{
}
