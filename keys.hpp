#ifndef KEYINFO_H
#define KEYINFO_H

struct Key {
	// Bindable mouse codes
	struct Mouse {
		enum {
			Left               = 0x1,
			Right              = 0x2,
			Middle             = 0x3,
			Back               = 0x4,
			Forwards           = 0x5,
			IncreaseResolution = 0x11,
			DecreaseResolution = 0x12,
			NextResolution     = 0x13,
			G600ModeSwitch     = 0x14,
			SwitchResolution   = 0x15,
			ResetResolution    = 0x16,
			GSwitch            = 0x17,
		};
	};

	// Bindable keyboard codes
	struct Keyboard {
		enum {
			// Modifiers as understood by the G600
			Control      = 0x01,
			Shift        = 0x02,
			Alt          = 0x04,
			Windows      = 0x08,
			RCtrl        = 0x10,
			RShift       = 0x20,
			AltGr        = 0x40,
			RightWindows = 0x80,

			// Reference:
			// "Universal Serial Bus (SUB)""
			// "HID Usage Tables 10/28/2004 Version 1.12"
			// "(c) 1996-2004 USB Implementers' Forum"

			// Num is used for numpad, in this context synonymous to Keypad

			// Following https://en.wikipedia.org/wiki/Bracket:
			//    ( ) = parentheses
			//    [ ] = brackets
			//    { } = braces

			NoEventIndicated  = 0x00,
			ErrorRollOver     = 0x01,
			POSTFail          = 0x02,
			ErrorUndefined    = 0x03,
			A                 = 0x04,
			B                 = 0x05,
			C                 = 0x06,
			D                 = 0x07,
			E                 = 0x08,
			F                 = 0x09,
			G                 = 0x0a,
			H                 = 0x0b,
			I                 = 0x0c,
			J                 = 0x0d,
			K                 = 0x0e,
			L                 = 0x0f,
			M                 = 0x10,
			N                 = 0x11,
			O                 = 0x12,
			P                 = 0x13,
			Q                 = 0x14,
			R                 = 0x15,
			S                 = 0x16,
			T                 = 0x17,
			U                 = 0x18,
			V                 = 0x19,
			W                 = 0x1a,
			X                 = 0x1b,
			Y                 = 0x1c,
			Z                 = 0x1d,
			One               = 0x1e, // and '!'
			Two               = 0x1f, // and '@'
			Three             = 0x20, // and '#'
			Four              = 0x21, // and '$'
			Five              = 0x22, // and '%'
			Six               = 0x23, // and '^'
			Seven             = 0x24, // and '&'
			Eight             = 0x25, // and '*'
			Nine              = 0x26, // and '('
			Zero              = 0x27, // and ')'
			Enter             = 0x28,
			Escape            = 0x29,
			Backspace         = 0x2a,
			Tab               = 0x2b,
			Space             = 0x2c,
			Dash              = 0x2d, // and '_'
			Equal             = 0x2e, // and '+'
			BracketOpen       = 0x2f, // and '{'
			BracketClose      = 0x30, // and '}'
			Backslash         = 0x31, // and '|'
			Hash              = 0x32, // and '~'
			SemiColon         = 0x33, // and ':'
			Quote             = 0x34, // and '"'
			GraveAccent       = 0x35, // and tilde
			Comma             = 0x36, // and '<'
			Period            = 0x37, // and '>'
			Slash             = 0x38, // and '?'
			CapsLock          = 0x39,
			F1                = 0x3a,
			F2                = 0x3b,
			F3                = 0x3c,
			F4                = 0x3d,
			F5                = 0x3e,
			F6                = 0x3f,
			F7                = 0x40,
			F8                = 0x41,
			F9                = 0x42,
			F10               = 0x43,
			F11               = 0x44,
			F12               = 0x45,
			PrintScreen       = 0x46,
			ScrollLock        = 0x47,
			PauseBreak        = 0x48,
			Insert            = 0x49,
			Home              = 0x4a,
			PageUp            = 0x4b,
			Delete            = 0x4c,
			End               = 0x4d,
			PageDown          = 0x4e,
			Right             = 0x4f,
			Left              = 0x50,
			Down              = 0x51,
			Up                = 0x52,
			NumLock           = 0x53, // and clear
			NumDiv            = 0x54,
			NumMult           = 0x55,
			NumMinus          = 0x56,
			NumPlus           = 0x57,
			NumEnter          = 0x58,
			NumOne            = 0x59, // and end
			NumTwo            = 0x5a, // and down
			NumThree          = 0x5b, // and page down
			NumFour           = 0x5c, // and left
			NumFive           = 0x5d,
			NumSix            = 0x5e, // and right
			NumSeven          = 0x5f, // and home
			NumEight          = 0x60, // and up
			NumNine           = 0x61, // and page up
			NumZero           = 0x62, // and insert
			NumDecimalSymbol  = 0x63, // and delete // should be NumDecimal but collision
			NonUSBackslash    = 0x64, // and '|'
			Application       = 0x65,
			Power             = 0x66,
			NumEqual          = 0x67,
			F13               = 0x68,
			F14               = 0x69,
			F15               = 0x6a,
			F16               = 0x6b,
			F17               = 0x6c,
			F18               = 0x6d,
			F19               = 0x6e,
			F20               = 0x6f,
			F21               = 0x70,
			F22               = 0x71,
			F23               = 0x72,
			F24               = 0x73,
			Execute           = 0x74,
			Help              = 0x75,
			Menu              = 0x76,
			Select            = 0x77,
			Stop              = 0x78,
			Again             = 0x79,
			Undo              = 0x7a,
			Cut               = 0x7b,
			Copy              = 0x7c,
			Paste             = 0x7d,
			Find              = 0x7e,
			Mute              = 0x7f,
			VolumeUp          = 0x80,
			VolumeDown        = 0x81,
			LockingCapsLock   = 0x82,
			LockingNumLock    = 0x83,
			LockingScrollLock = 0x84,
			NumComma          = 0x85,
			NumEqualSign      = 0x86,
			International1    = 0x87,
			International2    = 0x88,
			International3    = 0x89,
			International4    = 0x8a,
			International5    = 0x8b,
			International6    = 0x8c,
			International7    = 0x8d,
			International8    = 0x8e,
			International9    = 0x8f,
			Lang1             = 0x90,
			Lang2             = 0x91,
			Lang3             = 0x92,
			Lang4             = 0x93,
			Lang5             = 0x94,
			Lang6             = 0x95,
			Lang7             = 0x96,
			Lang8             = 0x97,
			Lang9             = 0x98,
			AlternateErase    = 0x99,
			SysReq_Attention  = 0x9a,
			Cancel            = 0x9b,
			Clear             = 0x9c,
			Prior             = 0x9d,
			Return            = 0x9e,
			Seperator         = 0x9f,
			Out               = 0xa0,
			Oper              = 0xa1,
			Clear_Again       = 0xa2,
			CrSel_Props       = 0xa3,
			ExSel             = 0xa4,

			// 0xa5 to 0xaf is defined as reserved

			Keypad00           = 0xb0,
			Keypad000          = 0xb1,
			ThousandsSeperator = 0xb2,
			DecimalSeperator   = 0xb3,
			CurrencyUnit       = 0xb4,
			CurrencySubunit    = 0xb5,
			NumOpenParen       = 0xb6,
			NumCloseParen      = 0xb7,
			NumOpenBraces      = 0xb8,
			NumCloseBraces     = 0xb9,
			NumTab             = 0xba,
			NumBackspace       = 0xbb,
			NumA               = 0xbc,
			NumB               = 0xbd,
			NumC               = 0xbe,
			NumD               = 0xbf,
			NumE               = 0xc0,
			NumF               = 0xc1,
			NumXOR             = 0xc2,
			NumCaret           = 0xc3,
			NumPercent         = 0xc4,
			NumLessThan        = 0xc5,
			NumGreaterThan     = 0xc6,
			NumAmpersand       = 0xc7,
			NumDoubleAmpersand = 0xc8,
			NumVertical        = 0xc9,
			NumDoubleVertical  = 0xca,
			NumColon           = 0xcb,
			NumHash            = 0xcc,
			NumSpace           = 0xcd,
			NumAt              = 0xce,
			NumExclamation     = 0xcf,
			NumMemStore        = 0xd0,
			NumMemRecall       = 0xd1,
			NumMemClear        = 0xd2,
			NumMemAdd          = 0xd3,
			NumMemSubtract     = 0xd4,
			NumMemMult         = 0xd5,
			NumMemDiv          = 0xd6,
			NumPlusMinus       = 0xd7,
			NumClear           = 0xd8,
			NumClearEntry      = 0xd9,
			NumBinary          = 0xda,
			NumOctal           = 0xdb,
			NumDecimal         = 0xdc,
			NumHexadecimal     = 0xdd,

			// 0xde - 0xdf is defined as reserved

			// TODO: find out how these behave
			LeftControl  = 0xe0,
			LeftShift    = 0xe1,
			LeftAlt      = 0xe2,
			LeftGUI      = 0xe3,
			RightControl = 0xe4,
			RightShift   = 0xe5,
			RightAlt     = 0xe6,
			RightGUI     = 0xe8,

			// 0xe8 - 0xffff is defined as reserved

			// I am uncertain how they'd send values over 256 since most
			// keyboards and the like probably implemented all this in single
			// bytes
		};
	};
};

#endif
