#include "KeyNames.h"

QMap<QString, int> KeyNames::codemap;
QMap<int, QString> KeyNames::namemap;

static struct {
	int code;
	char* name;
} stKeyNames[] = {
{Qt::Key_Escape, "Escape"},
{Qt::Key_Tab, "Tab"},
{Qt::Key_Backtab, "Backtab"},
{Qt::Key_BackTab, "BackTab"},
{Qt::Key_Backtab, "Backtab"},
{Qt::Key_Backspace, "Backspace"},
{Qt::Key_BackSpace, "BackSpace"},
{Qt::Key_Backspace, "Backspace"},
{Qt::Key_Return, "Return"},
{Qt::Key_Enter, "Enter"},
{Qt::Key_Insert, "Insert"},
{Qt::Key_Delete, "Delete"},
{Qt::Key_Pause, "Pause"},
{Qt::Key_Print, "Print"},
{Qt::Key_SysReq, "SysReq"},
{Qt::Key_Home, "Home"},
{Qt::Key_End, "End"},
{Qt::Key_Left, "Left"},
{Qt::Key_Up, "Up"},
{Qt::Key_Right, "Right"},
{Qt::Key_Down, "Down"},
{Qt::Key_Prior, "Prior"},
{Qt::Key_PageUp, "PageUp"},
{Qt::Key_Prior, "Prior"},
{Qt::Key_Next, "Next"},
{Qt::Key_PageDown, "PageDown"},
{Qt::Key_Next, "Next"},
{Qt::Key_Shift, "Shift"},
{Qt::Key_Control, "Control"},
{Qt::Key_Meta, "Meta"},
{Qt::Key_Alt, "Alt"},
{Qt::Key_CapsLock, "CapsLock"},
{Qt::Key_NumLock, "NumLock"},
{Qt::Key_ScrollLock, "ScrollLock"},
{Qt::Key_F1, "F1"},
{Qt::Key_F2, "F2"},
{Qt::Key_F3, "F3"},
{Qt::Key_F4, "F4"},
{Qt::Key_F5, "F5"},
{Qt::Key_F6, "F6"},
{Qt::Key_F7, "F7"},
{Qt::Key_F8, "F8"},
{Qt::Key_F9, "F9"},
{Qt::Key_F10, "F10"},
{Qt::Key_F11, "F11"},
{Qt::Key_F12, "F12"},
{Qt::Key_F13, "F13"},
{Qt::Key_F14, "F14"},
{Qt::Key_F15, "F15"},
{Qt::Key_F16, "F16"},
{Qt::Key_F17, "F17"},
{Qt::Key_F18, "F18"},
{Qt::Key_F19, "F19"},
{Qt::Key_F20, "F20"},
{Qt::Key_F21, "F21"},
{Qt::Key_F22, "F22"},
{Qt::Key_F23, "F23"},
{Qt::Key_F24, "F24"},
{Qt::Key_F25, "F25"},
{Qt::Key_F26, "F26"},
{Qt::Key_F27, "F27"},
{Qt::Key_F28, "F28"},
{Qt::Key_F29, "F29"},
{Qt::Key_F30, "F30"},
{Qt::Key_F31, "F31"},
{Qt::Key_F32, "F32"},
{Qt::Key_F33, "F33"},
{Qt::Key_F34, "F34"},
{Qt::Key_F35, "F35"},
{Qt::Key_Super_L, "Super_L"},
{Qt::Key_Super_R, "Super_R"},
{Qt::Key_Menu, "Menu"},
{Qt::Key_Hyper_L, "Hyper_L"},
{Qt::Key_Hyper_R, "Hyper_R"},
{Qt::Key_Help, "Help"},
{Qt::Key_Space, "Space"},
{Qt::Key_Any, "Any"},
{Qt::Key_Space, "Space"},
{Qt::Key_Exclam, "Exclam"},
{Qt::Key_QuoteDbl, "QuoteDbl"},
{Qt::Key_NumberSign, "NumberSign"},
{Qt::Key_Dollar, "Dollar"},
{Qt::Key_Percent, "Percent"},
{Qt::Key_Ampersand, "Ampersand"},
{Qt::Key_Apostrophe, "Apostrophe"},
{Qt::Key_ParenLeft, "ParenLeft"},
{Qt::Key_ParenRight, "ParenRight"},
{Qt::Key_Asterisk, "Asterisk"},
{Qt::Key_Plus, "Plus"},
{Qt::Key_Comma, "Comma"},
{Qt::Key_Minus, "Minus"},
{Qt::Key_Period, "Period"},
{Qt::Key_Slash, "Slash"},
{Qt::Key_0, "0"},
{Qt::Key_1, "1"},
{Qt::Key_2, "2"},
{Qt::Key_3, "3"},
{Qt::Key_4, "4"},
{Qt::Key_5, "5"},
{Qt::Key_6, "6"},
{Qt::Key_7, "7"},
{Qt::Key_8, "8"},
{Qt::Key_9, "9"},
{Qt::Key_Colon, "Colon"},
{Qt::Key_Semicolon, "Semicolon"},
{Qt::Key_Less, "Less"},
{Qt::Key_Equal, "Equal"},
{Qt::Key_Greater, "Greater"},
{Qt::Key_Question, "Question"},
{Qt::Key_At, "At"},
{Qt::Key_A, "A"},
{Qt::Key_B, "B"},
{Qt::Key_C, "C"},
{Qt::Key_D, "D"},
{Qt::Key_E, "E"},
{Qt::Key_F, "F"},
{Qt::Key_G, "G"},
{Qt::Key_H, "H"},
{Qt::Key_I, "I"},
{Qt::Key_J, "J"},
{Qt::Key_K, "K"},
{Qt::Key_L, "L"},
{Qt::Key_M, "M"},
{Qt::Key_N, "N"},
{Qt::Key_O, "O"},
{Qt::Key_P, "P"},
{Qt::Key_Q, "Q"},
{Qt::Key_R, "R"},
{Qt::Key_S, "S"},
{Qt::Key_T, "T"},
{Qt::Key_U, "U"},
{Qt::Key_V, "V"},
{Qt::Key_W, "W"},
{Qt::Key_X, "X"},
{Qt::Key_Y, "Y"},
{Qt::Key_Z, "Z"},
{Qt::Key_BracketLeft, "BracketLeft"},
{Qt::Key_Backslash, "Backslash"},
{Qt::Key_BracketRight, "BracketRight"},
{Qt::Key_AsciiCircum, "AsciiCircum"},
{Qt::Key_Underscore, "Underscore"},
{Qt::Key_QuoteLeft, "QuoteLeft"},
{Qt::Key_BraceLeft, "BraceLeft"},
{Qt::Key_Bar, "Bar"},
{Qt::Key_BraceRight, "BraceRight"},
{Qt::Key_AsciiTilde, "AsciiTilde"},
{Qt::Key_nobreakspace, "nobreakspace"},
{Qt::Key_exclamdown, "exclamdown"},
{Qt::Key_cent, "cent"},
{Qt::Key_sterling, "sterling"},
{Qt::Key_currency, "currency"},
{Qt::Key_yen, "yen"},
{Qt::Key_brokenbar, "brokenbar"},
{Qt::Key_section, "section"},
{Qt::Key_diaeresis, "diaeresis"},
{Qt::Key_copyright, "copyright"},
{Qt::Key_ordfeminine, "ordfeminine"},
{Qt::Key_guillemotleft, "guillemotleft"},
{Qt::Key_notsign, "notsign"},
{Qt::Key_hyphen, "hyphen"},
{Qt::Key_registered, "registered"},
{Qt::Key_macron, "macron"},
{Qt::Key_degree, "degree"},
{Qt::Key_plusminus, "plusminus"},
{Qt::Key_twosuperior, "twosuperior"},
{Qt::Key_threesuperior, "threesuperior"},
{Qt::Key_acute, "acute"},
{Qt::Key_mu, "mu"},
{Qt::Key_paragraph, "paragraph"},
{Qt::Key_periodcentered, "periodcentered"},
{Qt::Key_cedilla, "cedilla"},
{Qt::Key_onesuperior, "onesuperior"},
{Qt::Key_masculine, "masculine"},
{Qt::Key_guillemotright, "guillemotright"},
{Qt::Key_onequarter, "onequarter"},
{Qt::Key_onehalf, "onehalf"},
{Qt::Key_threequarters, "threequarters"},
{Qt::Key_questiondown, "questiondown"},
{Qt::Key_Agrave, "Agrave"},
{Qt::Key_Aacute, "Aacute"},
{Qt::Key_Acircumflex, "Acircumflex"},
{Qt::Key_Atilde, "Atilde"},
{Qt::Key_Adiaeresis, "Adiaeresis"},
{Qt::Key_Aring, "Aring"},
{Qt::Key_AE, "AE"},
{Qt::Key_Ccedilla, "Ccedilla"},
{Qt::Key_Egrave, "Egrave"},
{Qt::Key_Eacute, "Eacute"},
{Qt::Key_Ecircumflex, "Ecircumflex"},
{Qt::Key_Ediaeresis, "Ediaeresis"},
{Qt::Key_Igrave, "Igrave"},
{Qt::Key_Iacute, "Iacute"},
{Qt::Key_Icircumflex, "Icircumflex"},
{Qt::Key_Idiaeresis, "Idiaeresis"},
{Qt::Key_ETH, "ETH"},
{Qt::Key_Ntilde, "Ntilde"},
{Qt::Key_Ograve, "Ograve"},
{Qt::Key_Oacute, "Oacute"},
{Qt::Key_Ocircumflex, "Ocircumflex"},
{Qt::Key_Otilde, "Otilde"},
{Qt::Key_Odiaeresis, "Odiaeresis"},
{Qt::Key_multiply, "multiply"},
{Qt::Key_Ooblique, "Ooblique"},
{Qt::Key_Ugrave, "Ugrave"},
{Qt::Key_Uacute, "Uacute"},
{Qt::Key_Ucircumflex, "Ucircumflex"},
{Qt::Key_Udiaeresis, "Udiaeresis"},
{Qt::Key_Yacute, "Yacute"},
{Qt::Key_THORN, "THORN"},
{Qt::Key_ssharp, "ssharp"},
{Qt::Key_agrave, "agrave"},
{Qt::Key_aacute, "aacute"},
{Qt::Key_acircumflex, "acircumflex"},
{Qt::Key_atilde, "atilde"},
{Qt::Key_adiaeresis, "adiaeresis"},
{Qt::Key_aring, "aring"},
{Qt::Key_ae, "ae"},
{Qt::Key_ccedilla, "ccedilla"},
{Qt::Key_egrave, "egrave"},
{Qt::Key_eacute, "eacute"},
{Qt::Key_ecircumflex, "ecircumflex"},
{Qt::Key_ediaeresis, "ediaeresis"},
{Qt::Key_igrave, "igrave"},
{Qt::Key_iacute, "iacute"},
{Qt::Key_icircumflex, "icircumflex"},
{Qt::Key_idiaeresis, "idiaeresis"},
{Qt::Key_eth, "eth"},
{Qt::Key_ntilde, "ntilde"},
{Qt::Key_ograve, "ograve"},
{Qt::Key_oacute, "oacute"},
{Qt::Key_ocircumflex, "ocircumflex"},
{Qt::Key_otilde, "otilde"},
{Qt::Key_odiaeresis, "odiaeresis"},
{Qt::Key_division, "division"},
{Qt::Key_oslash, "oslash"},
{Qt::Key_ugrave, "ugrave"},
{Qt::Key_uacute, "uacute"},
{Qt::Key_ucircumflex, "ucircumflex"},
{Qt::Key_udiaeresis, "udiaeresis"},
{Qt::Key_yacute, "yacute"},
{Qt::Key_thorn, "thorn"},
{Qt::Key_ydiaeresis, "ydiaeresis"},
{Qt::Key_unknown, "unknown"},
{0,0},
};

void KeyNames::setCodeMap()
{
	int i;

	codemap.clear();
	for(i=0; stKeyNames[i].code != 0; i++){
		codemap.insert(stKeyNames[i].name, stKeyNames[i].code);
	}
}

void KeyNames::setNameMap()
{
	int i;

	namemap.clear();
	for(i=0; stKeyNames[i].code != 0; i++){
		namemap.insert(stKeyNames[i].code, stKeyNames[i].name);
	}
}
