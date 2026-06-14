#include "X64Diasm.h"
typedef union _MODRM
{
	UINT8 AsUInt8;
	struct
	{
		UINT8 RM : 3;
		UINT8 Reg : 3;
		UINT8 Mod : 2;
	}Fields;
}MODRM,*PMODRM;
typedef union _SIB
{
	UINT8 AsUInt8;
	struct
	{
		UINT8 Base : 3;
		UINT8 Index : 3;
		UINT8 Scale : 2;
	} Fields;
} SIB, * PSIB;
typedef union _REX
{
	UINT8 AsUInt8;
	struct
	{
		UINT8 Base : 1;
		UINT8 Index : 1;
		UINT8 Reg : 1;
		UINT8 Width : 1;
		UINT8 Reserved : 4;
	}Fields;
}REX,*PREX;
UINT8 PrefixGroup[] = { 0x66,0x67,0x2E,0x3E,0x26,0x64,0x65,0x36,0xF0,0xF3,0xF2 };
BOOLEAN IsPrefix(UINT8 CodeByte)
{
	for (UINT32 i = 0; i < sizeof(PrefixGroup); i++)
	{
		if (CodeByte == PrefixGroup[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOLEAN IsRexPrefix(UINT8 CodeByte)
{
	return (CodeByte & 0xF0) == 0x40;
}
UINT8 GetOpCodeLength(PVOID StartAddr, UINT8 RemainingBytes)
{
	if (RemainingBytes == 0) return 0;

	UINT8 first = DEF_PTR(UINT8, StartAddr, 0);
	if (first != 0x0F)
	{
		return 1;
	}
	if (RemainingBytes >= 2)
	{
		UINT8 second = DEF_PTR(UINT8, StartAddr, 1);
		if (second == 0x38 || second == 0x3A)
		{
			return 3;
		}
	}
	return min(RemainingBytes,2);
}
BOOLEAN NeedModRM(PUINT8 StartOpCode,UINT8 OpLengh)
{
	if (OpLengh == 1)
	{
		switch (StartOpCode[0])
		{
		case 0x90:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x06:
		case 0x07:
		case 0x0E:
		case 0x16:
		case 0x17:
		case 0x1F:
		case 0x50: case 0x51: case 0x52: case 0x53:
		case 0x54: case 0x55: case 0x56: case 0x57:
		case 0x58: case 0x59: case 0x5A: case 0x5B:
		case 0x5C: case 0x5D: case 0x5E: case 0x5F:
		{
			return FALSE;
		}
		default:
		{
			return TRUE;
		}
		}
	}
	else if (OpLengh == 2)
	{
		if (StartOpCode[1] >= 0x80 && StartOpCode[1] <= 0x8F)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOLEAN NeedSib(PMODRM ModRm, BOOLEAN HasAddressSizePrefix)
{
	if (HasAddressSizePrefix) return FALSE;
	return (ModRm->Fields.RM == 4) && (ModRm->Fields.Mod != 3);
}
UINT8 GetDisplacementLength(PMODRM ModRmByte, PSIB SibByte, BOOLEAN HasSib)
{
	UINT8 Mod = ModRmByte->Fields.Mod;
	if (Mod == 0x03) return 0;
	if (Mod == 0x01) return 1;
	if (Mod == 0x02) return 4;
	if (HasSib)
	{
		UINT8 Base = SibByte->Fields.Base;
		if (Base == 0x5)
		{
			return 4;
		}
		return 0;
	}
	else
	{
		if (ModRmByte->Fields.RM == 0x5)
		{
			return 4;
		}
		return 0;
	}
}
UINT8 GetImmediateLength(PUINT8 Opcode, UINT8 OpcodeLength, BOOLEAN HasOperandSizePrefix,BOOLEAN RexW,UINT8 ModRMCode)
{
	MODRM ModRM = { 0 };
	ModRM.AsUInt8 = ModRMCode;
	if (OpcodeLength == 1)
	{
		UINT8 op = Opcode[0];
		UINT8 opLow = op & 0x0F;
		UINT8 opHigh = op >> 4;
		if (opHigh == 0xB)
		{
			if (opLow >= 0 && opLow <= 7) return 1;
			if (opLow >= 8 && opLow <= 0x0F) return HasOperandSizePrefix ? 2 : (RexW ? 8 : 4);
			return 0;
		}
		if (opHigh == 0x6)
		{
			if (opLow == 0x8) return HasOperandSizePrefix ? 2 : 4;
			if (opLow == 0x9) return HasOperandSizePrefix ? 2 : 4;
			if (opLow == 0xA) return 1;
			if (opLow == 0xB) return 1;
			return 0;
		}
		if (opHigh == 0x7) return 1;
		if (opHigh >= 0 && opHigh<=3)
		{
			if ((opLow) >= 0 && (opLow) <= 3) return 0;
			if ((opLow) >= 0x08 && (opLow) <= 0x0B) return 0;
			switch (opLow)
			{
			case 0x4:
				return 1;
			case 0x5:
				return HasOperandSizePrefix ? 2 : 4;
			case 0xC:
				return 1;
			case 0xD:
				return HasOperandSizePrefix ? 2 : 4;
			default:
				return 0;
			}
		}
		if (opHigh == 0x8)
		{
			if (opLow == 0 || opLow ==3) return 1;
			else if (opLow == 0x1) return HasOperandSizePrefix ? 2 : 4;
			return 0;
		}
		if (opHigh == 5) return 0;
		if (opHigh == 0xC)
		{
			if (opLow == 0xC) return 0;
			else if (opLow == 0xD) return 1;
			else if (opLow == 0 || opLow == 1 || opLow == 6) return 1;
			else if (opLow == 7) return HasOperandSizePrefix ? 2 : 4;
			return 0;
		}
		if (opHigh == 0xA)
		{
			if (opLow == 0x8) return 1;
			else if (opLow == 0x9) return HasOperandSizePrefix ? 2 : 4;
			return 0;
		}
		if (opHigh == 0xE)
		{
			if (opLow >= 0 && opLow <= 7) return 1;
			else if (opLow >= 8 && opLow <= 0x0F)
			{
				switch (opLow)
				{
				case 0x8:
					return 4;
				case 0x9:
					return 4;
				case 0xB:
					return 1;
				default:
					return 0;
				}
			}
		}
		if (opHigh == 0xF)
		{
			if (ModRM.Fields.Reg == 0)
			{
				if (opLow == 6) return 1;
				if (opLow == 7) return HasOperandSizePrefix ? 2 : (RexW ? 8 : 4);
			}
			return 0;
		}
	}
	else if (OpcodeLength == 2)
	{
		UINT8 op2 = Opcode[1];
		UINT8 op2High = op2 >> 4;
		UINT8 op2Low = op2 & 0x0F;
		if (op2 == 0xA4) return 1;
		if (op2High == 0x7)
		{
			if (op2Low <= 3) return 1;
			return 0;
		}
		if (op2 == 0XBA) return 1;
		if (op2High == 0xC)
		{
			if (op2Low == 2 || op2Low == 4 || op2Low == 5 || op2Low == 6) return 1;
		}
		if (op2High == 8) return 4;
	}
	return 0;
}
UINT8 GetInstructionLength(PVOID CodeAddr, UINT8 MaxLength)
{
	if (!MaxLength) return 0;
	if (!CodeAddr) return 0;
	UINT8 length = 0;
	PUCHAR currentByte = (PUCHAR)CodeAddr;
	BOOLEAN hasOpSizePrefix = FALSE;
	BOOLEAN hasAddrSizePrefix = FALSE;
	BOOLEAN rexW = FALSE;

	while (IsPrefix(*currentByte))
	{
		if (*currentByte == 0x66) hasOpSizePrefix = TRUE;
		if (*currentByte == 0x67) hasAddrSizePrefix = TRUE;
		currentByte++;
		length++;
	}

	if (IsRexPrefix(*currentByte))
	{
		REX rex = { 0 };
		rex.AsUInt8 = *currentByte;
		rexW = rex.Fields.Width;
		currentByte++;
		length++;
	}
	UINT8 opcodeLen = GetOpCodeLength(currentByte, MaxLength - ((UINT8)((UINT64)currentByte - (UINT64)CodeAddr)));
	length += opcodeLen;
	if (NeedModRM(currentByte, opcodeLen))
	{
		MODRM modrm = { 0 };
		modrm.AsUInt8 = DEF_PTR(UINT8, currentByte, opcodeLen);
		length++;
		BOOLEAN hasSib = NeedSib(&modrm, hasAddrSizePrefix);
		if (hasSib)
		{
			SIB sib = { 0 };
			sib.AsUInt8 = DEF_PTR(UINT8, currentByte, opcodeLen + 1);
			length++;
			length += GetDisplacementLength(&modrm, &sib, TRUE);
		}
		else
		{
			length += GetDisplacementLength(&modrm, NULL, FALSE);
		}
		length += GetImmediateLength(currentByte, opcodeLen, hasOpSizePrefix, rexW, modrm.AsUInt8);
	}
	else
	{
		length += GetImmediateLength(currentByte, opcodeLen, hasOpSizePrefix, rexW, 0);
	}
	return length;
}