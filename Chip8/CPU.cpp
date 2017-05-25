#include "CPU.h"
#include "Chip8.h"

void CPU::ReturnToSubroutine(Chip8 &chip8)
{
	--chip8.m_stackPointer;
	chip8.m_programCounter = chip8.m_stack[chip8.m_stackPointer];
	chip8.m_programCounter += 2;
}

void CPU::ClearScreen(Chip8 &chip8)
{
	chip8.m_screen = { 0 };
	chip8.m_programCounter += 2;
}

void CPU::JumpToAddress(Chip8 &chip8)
{
	chip8.m_programCounter = chip8.m_opcode & 0x0FFF;
}

void CPU::CallSubroutine(Chip8 &chip8)
{
	chip8.m_stack[chip8.m_stackPointer] = chip8.m_programCounter;
	++chip8.m_stackPointer;
	chip8.m_programCounter = chip8.m_opcode & 0x0FFF;
}

void CPU::SkipInstructionIfVirtualEqualsNN(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] == (chip8.m_opcode & 0x00FF))
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}

void CPU::SkipInstructionIfVirtualEqualsNotNN(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] != (chip8.m_opcode & 0x00FF))
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}

void CPU::SkipInstructionIfVirtualEqualsVirtual(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] == chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4])
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}

void CPU::SetVirtualToNN(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = chip8.m_opcode & 0x00FF;
	chip8.m_programCounter += 2;
}

void CPU::AddNNToVirtual(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] += chip8.m_opcode & 0x00FF;
	chip8.m_programCounter += 2;
}

void CPU::SetVirtualToVirtual(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	chip8.m_programCounter += 2;
}

void CPU::SetVirtualToVirtualOR(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] |= chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	chip8.m_programCounter += 2;
}

void CPU::SetVirtualToVirtualAND(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] &= chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	chip8.m_programCounter += 2;
}

void CPU::SetVirtualToVirtualXOR(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] ^= chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	chip8.m_programCounter += 2;
}

void CPU::AddVirtualToVirtual(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] += chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4] > (0xFF - chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8]))
		chip8.m_virtualRegisters[0xF] = 1;
	else
		chip8.m_virtualRegisters[0xF] = 0;
	chip8.m_programCounter += 2;
}

void CPU::SubVirtualFromVirtual(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4] > chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8])
		chip8.m_virtualRegisters[0xF] = 0;
	else
		chip8.m_virtualRegisters[0xF] = 1;
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] -= chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	chip8.m_programCounter += 2;
}

void CPU::ShiftVirtualRight(Chip8 &chip8)
{
	chip8.m_virtualRegisters[0xF] = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] & 0x1;
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] >>= 1;
	chip8.m_programCounter += 2;
}

void CPU::SetVirtualToVirtualMinus(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] > chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4])
		chip8.m_virtualRegisters[0xF] = 0;
	else
		chip8.m_virtualRegisters[0xF] = 1;
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4] - chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8];
	chip8.m_programCounter += 2;
}

void CPU::ShiftVirtualLeft(Chip8 &chip8)
{
	chip8.m_virtualRegisters[0xF] = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] >> 7;
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] <<= 1;
	chip8.m_programCounter += 2;
}

void CPU::SkipInstructionIfVirtualEqualsNotVirtual(Chip8 &chip8)
{
	if (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] != chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4])
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}

void CPU::SetIndexRegisterToNNN(Chip8 &chip8)
{
	chip8.m_indexRegister = chip8.m_opcode & 0x0FFF;
	chip8.m_programCounter += 2;
}
void CPU::JumpToAddressPlusV0(Chip8 &chip8)
{
	chip8.m_programCounter = (chip8.m_opcode & 0x0FFF) + chip8.m_virtualRegisters[0];
}
void CPU::SetVirtualToRandom(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (chip8.m_opcode & 0x00FF);
	chip8.m_programCounter += 2;
}
void CPU::DrawSprite(Chip8 &chip8)
{
	unsigned short x = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8];
	unsigned short y = chip8.m_virtualRegisters[(chip8.m_opcode & 0x00F0) >> 4];
	unsigned short height = chip8.m_opcode & 0x000F;

	chip8.m_virtualRegisters[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		unsigned short pixel = chip8.m_memory[chip8.m_indexRegister + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			if ((pixel & (0x80 >> xline)) != 0)
			{
				if (chip8.m_screen[(x + xline + ((y + yline) * 64))] == 1)
				{
					chip8.m_virtualRegisters[0xF] = 1;
				}
				chip8.m_screen[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}

	chip8.m_programCounter += 2;
}
void CPU::SkipIfPressed(Chip8 &chip8)
{
	if (chip8.m_keypad[chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8]] != 0)
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}
void CPU::SkipIfNotPressed(Chip8 &chip8)
{
	if (chip8.m_keypad[chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8]] == 0)
		chip8.m_programCounter += 4;
	else
		chip8.m_programCounter += 2;
}
void CPU::SetVirtualToDelayTimer(Chip8 &chip8)
{
	chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = chip8.m_delayTimer;
	chip8.m_programCounter += 2;
}
void CPU::WaitForKeyAndStore(Chip8 &chip8)
{
	bool key_pressed = false;

	for (int i = 0; i < 16; ++i)
	{
		if (chip8.m_keypad[i] != 0)
		{
			chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] = i;
			key_pressed = true;
		}
	}

	if (!key_pressed)
		return;

	chip8.m_programCounter += 2;
}
void CPU::SetDelayTimerToVirtual(Chip8 &chip8)
{
	chip8.m_delayTimer = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8];
	chip8.m_programCounter += 2;
}
void CPU::SetSoundTimerToVirtual(Chip8 &chip8)
{
	chip8.m_soundTimer = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8];
	chip8.m_programCounter += 2;
}
void CPU::AddVirtualToIndex(Chip8 &chip8)
{
	if (chip8.m_indexRegister + chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] > 0xFFF)
		chip8.m_virtualRegisters[0xF] = 1;
	else
		chip8.m_virtualRegisters[0xF] = 0;
	chip8.m_indexRegister += chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8];
	chip8.m_programCounter += 2;
}
void CPU::SetIndexRegisterToVirtualSprite(Chip8 &chip8)
{
	chip8.m_indexRegister = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] * 0x5;
	chip8.m_programCounter += 2;
}
void CPU::Decimals(Chip8 &chip8)
{
	chip8.m_memory[chip8.m_indexRegister] = chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] / 100;
	chip8.m_memory[chip8.m_indexRegister + 1] = (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] / 10) % 10;
	chip8.m_memory[chip8.m_indexRegister + 2] = (chip8.m_virtualRegisters[(chip8.m_opcode & 0x0F00) >> 8] % 100) % 10;
	chip8.m_programCounter += 2;
}
void CPU::StoreVirtualToMemory1(Chip8 &chip8)
{
	for (int i = 0; i <= ((chip8.m_opcode & 0x0F00) >> 8); ++i)
		chip8.m_memory[chip8.m_indexRegister + i] = chip8.m_virtualRegisters[i];

	chip8.m_indexRegister += ((chip8.m_opcode & 0x0F00) >> 8) + 1;
	chip8.m_programCounter += 2;
}
void CPU::StoreVirtualToMemory2(Chip8 &chip8)
{
	for (int i = 0; i <= ((chip8.m_opcode & 0x0F00) >> 8); ++i)
		chip8.m_virtualRegisters[i] = chip8.m_memory[chip8.m_indexRegister + i];

	chip8.m_indexRegister += ((chip8.m_opcode & 0x0F00) >> 8) + 1;
	chip8.m_programCounter += 2;
}