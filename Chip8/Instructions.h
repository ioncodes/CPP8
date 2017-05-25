#pragma once
#include <map>

class Instructions
{
public:
	enum Instruction
	{
		Clear,										// 0x0000
		ReturnToSubroutine,							// 0x000E
		JumpToAddress,								// 0x1000
		CallSubroutine,								// 0x2000
		SkipInstructionIfVirtualEqualsNN,			// 0x3000
		SkipInstructionIfVirtualEqualsNotNN,		// 0x4000
		SkipInstructionIfVirtualEqualsVirtual,		// 0x5000
		SetVirtualToNN,								// 0x6000
		AddNNToVirtual,								// 0x7000
		Virtuals,									// 0x8000
		SkipInstructionIfVirtualEqualsNotVirtual,	// 0x9000
		SetIndexRegisterToNNN,						// 0xA000
		JumpToAddressPlusV0,						// 0xB000
		SetVirtualToRandom,							// 0xC000
		DrawSprite,									// 0xD000
		Keys,										// 0xE000
		FXS,										// 0xF000
		InstructionUnknown							// 0xFFFFFFFF
	};
	enum Virtual
	{
		SetVirtualToVirtual,						// 0x0000
		SetVirtualToVirtualOR,						// 0x0001
		SetVirtualToVirtualAND,						// 0x0002
		SetVirtualToVirtualXOR,						// 0x0003
		AddVirtualToVirtual,						// 0x0004
		SubVirtualFromVirtual,						// 0x0005
		ShiftVirtualRight,							// 0x0006
		SetVirtualToVirtualMinus,					// 0x0007
		ShiftVirtualLeft,							// 0x000E
		VirtualUnknown								// 0xFFFFFFFF
	};
	enum Key
	{
		SkipIfPressed,								// 0x009E
		SkipIfNotPressed,							// 0x00A1
		KeyUnknown									// 0xFFFFFFFF
	};
	enum FX
	{
		SetVirtualToDelayTimer,						// 0x0007
		WaitForKeyAndStore,							// 0x000A
		SetDelayTimerToVirtual,						// 0x0015
		SetSoundTimerToVirtual,						// 0x0018
		AddVirtualToIndex,							// 0x001E
		SetIndexRegisterToVirtualSprite,			// 0x0029
		Decimals,									// 0x0033
		StoreVirtualToMemory1,						// 0x0055
		StoreVirtualToMemory2,						// 0x0065
		UnknownFX									// 0xFFFFFFFF
	};
	const std::map<unsigned int, Instruction> instructions
	{
		{ 0x0000, Clear },
		{ 0x000E, ReturnToSubroutine},
		{ 0x1000, JumpToAddress},
		{ 0x2000, CallSubroutine},
		{ 0x3000, SkipInstructionIfVirtualEqualsNN },
		{ 0x4000, SkipInstructionIfVirtualEqualsNotNN},
		{ 0x5000, SkipInstructionIfVirtualEqualsVirtual},
		{ 0x6000, SetVirtualToNN},
		{ 0x7000, AddNNToVirtual},
		{ 0x8000, Virtuals},
		{ 0x9000, SkipInstructionIfVirtualEqualsNotVirtual},
		{ 0xA000, SetIndexRegisterToNNN },
		{ 0xB000, JumpToAddressPlusV0},
		{ 0xC000, SetVirtualToNN},
		{ 0xD000, DrawSprite},
		{ 0xE000, Keys },
		{ 0xF000, FXS},
		{ 0xFFFFFFFF, InstructionUnknown}
	};
	const std::map<unsigned int, Virtual> virtuals
	{
		{ 0x0000, SetVirtualToVirtual },
		{ 0x0001, SetVirtualToVirtualOR },
		{ 0x0002, SetVirtualToVirtualAND },
		{ 0x0003, SetVirtualToVirtualXOR },
		{ 0x0004, AddVirtualToVirtual },
		{ 0x0005, SubVirtualFromVirtual },
		{ 0x0006, ShiftVirtualRight },
		{ 0x0007, SetVirtualToVirtualMinus },
		{ 0x000E, ShiftVirtualLeft },
		{ 0xFFFFFFFF, VirtualUnknown }
	};
	const std::map<unsigned int, Key> keys
	{
		{ 0x009E, SkipIfPressed },
		{ 0x00A1, SkipIfNotPressed },
		{ 0xFFFFFFFF, KeyUnknown }
	};
	const std::map<unsigned int, FX> fxs
	{
		{ 0x0007,SetVirtualToDelayTimer },
		{ 0x000A,WaitForKeyAndStore },
		{ 0x0015,SetDelayTimerToVirtual },
		{ 0x0018,SetSoundTimerToVirtual },
		{ 0x001E,AddVirtualToIndex },
		{ 0x0029,SetIndexRegisterToVirtualSprite },
		{ 0x0033,Decimals },
		{ 0x0055,StoreVirtualToMemory1 },
		{ 0x0065,StoreVirtualToMemory2 },
		{ 0xFFFFFFFF,UnknownFX }
	};
	Instruction getInstruction(unsigned int opcode) const;
	Virtual getVirtual(unsigned int opcode) const;
	Key getKey(unsigned int opcode) const;
	FX getFX(unsigned int opcode) const;
};
