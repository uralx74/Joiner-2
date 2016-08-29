//---------------------------------------------------------------------------
#ifndef TRANSPOSCIPHER_H
#define TRANSPOSCIPHER_H

//---------------------------------------------------------------------------
// Transposition Cipher
// Шифрование двойной перестновкой в квадратной матрице
// @author: vsovchinnikov
// @date: 2016-05-20
//---------------------------------------------------------------------------


#include <Classes.hpp>
#include <math.h>

class TTransposCipher {
private:
    AnsiString _Abc;
    int* _RouteRow;  // Маршрут перстановки длинный

public:
    TTransposCipher();
    ~TTransposCipher();
	AnsiString __fastcall Encrypt(AnsiString SrcStr, const int* _pRouteRow, const int* _pRouteCol, AnsiString Extension = "");
    AnsiString __fastcall Decrypt(AnsiString SrcStr, const int* _pRouteRow, const int* _pRouteCol);

    void __fastcall SetRoute(int* RouteRow1);
};

//---------------------------------------------------------------------------
#endif
