//---------------------------------------------------------------------------
#ifndef VIGENERECIPHERH
#define VIGENERECIPHERH

//---------------------------------------------------------------------------
// Vigenere Cipher
// vsovchinnikov
// 2016-05-20
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

class TVigenereCipher {
private:
    AnsiString _Abc;

public:
    TVigenereCipher();
    ~TVigenereCipher();
    void __fastcall SetAbc(const AnsiString &Abc);
	AnsiString __fastcall Encrypt(AnsiString SrcStr, AnsiString KeyStr);
	AnsiString __fastcall Decrypt(AnsiString SrcStr, AnsiString KeyStr);
};

//---------------------------------------------------------------------------
#endif
