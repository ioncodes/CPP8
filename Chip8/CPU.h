#pragma once
#include "Chip8.h"

class CPU
{
public:
	static void ReturnToSubroutine(Chip8& chip8);
	static void ClearScreen(Chip8& chip8);
	static void JumpToAddress(Chip8& chip8);
	static void CallSubroutine(Chip8& chip8);
	static void SkipInstructionIfVirtualEqualsNN(Chip8& chip8);
	static void SkipInstructionIfVirtualEqualsNotNN(Chip8& chip8);
	static void SkipInstructionIfVirtualEqualsVirtual(Chip8& chip8);
	static void SetVirtualToNN(Chip8& chip8);
	static void AddNNToVirtual(Chip8& chip8);
	static void SetVirtualToVirtual(Chip8& chip8);
	static void SetVirtualToVirtualOR(Chip8& chip8);
	static void SetVirtualToVirtualAND(Chip8& chip8);
	static void SetVirtualToVirtualXOR(Chip8& chip8);
	static void AddVirtualToVirtual(Chip8& chip8);
	static void SubVirtualFromVirtual(Chip8& chip8);
	static void ShiftVirtualRight(Chip8& chip8);
	static void SetVirtualToVirtualMinus(Chip8& chip8);
	static void ShiftVirtualLeft(Chip8& chip8);
	static void SkipInstructionIfVirtualEqualsNotVirtual(Chip8& chip8);
	static void SetIndexRegisterToNNN(Chip8& chip8);
	static void JumpToAddressPlusV0(Chip8& chip8);
	static void SetVirtualToRandom(Chip8& chip8);
	static void DrawSprite(Chip8& chip8);
	static int* to2D(int i);
	static void SkipIfPressed(Chip8& chip8);
	static void SkipIfNotPressed(Chip8& chip8);
	static void SetVirtualToDelayTimer(Chip8& chip8);
	static void WaitForKeyAndStore(Chip8& chip8);
	static void SetDelayTimerToVirtual(Chip8& chip8);
	static void SetSoundTimerToVirtual(Chip8& chip8);
	static void AddVirtualToIndex(Chip8& chip8);
	static void SetIndexRegisterToVirtualSprite(Chip8& chip8);
	static void Decimals(Chip8& chip8);
	static void StoreVirtualToMemory1(Chip8& chip8);
	static void StoreVirtualToMemory2(Chip8& chip8);
};
