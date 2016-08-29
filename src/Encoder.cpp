#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "Encoder.h"


const AnsiString TEncoder::MasterKey = "p65i76+l6OUU$5kVIy149WrW5&?qt+tB2!+Xrg1O?FZk_&2e?6|F1)M9zU.}Zog{";

const int TEncoder::RouteRow[] = {1, 0, 2, 5, 6, 4, 9, 7, 3, 8};  // Маршрут перстановки длинный
const int TEncoder::RouteCol[] = {6, 9, 7, 0, 4, 1, 5, 2, 8, 3};  // Маршрут перстановки длинный


//---------------------------------------------------------------------------
//
TEncoder::TEncoder()
{

    VigenereCipherPtr = new TVigenereCipher();
    VigenereCipherPtr->SetAbc("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ1234567890~@#$%^&*_+|!\"№;:?()/'.,{}[]<>\\ ");

    TransposCipherPtr = new TTransposCipher();

    UserInfoPtr = new TUserInfo();

    UsernameKey = VigenereCipherPtr->Encrypt("A&Eeg9y4)Y52fJ4W;|5m147_{Vmwh3W9W$#6N!q:T&%c7s1-K8qGbcG^P4tGd$R^", MasterKey);
    PasswordKey = VigenereCipherPtr->Encrypt("@}T8hYto7d%WmaOEyf84c*h=[3!AY6?8x9MIjR2P&M{4#!Xchv?1SPx20№5P1!%4", MasterKey);
    FinishKey = VigenereCipherPtr->Encrypt("gGuRrY4rx66R$?sQj|Xl=(54S№H3g23fC#b[TTf:3_C~v_3l+74HY^w2[E&8x6T8", MasterKey);
    CodeInfoKey = VigenereCipherPtr->Encrypt("q976L155X:3&L^6OfOlEJi$kZ3K2zl0Ev[gS0zsoZB&.,_(7E07@Ia)|@a=jj6$Z", MasterKey);

    ExtensionWord = "1JRo#Ed0FG5j511xv:8zj7r2S5rhe!3xvSFH1PCSn]gr-VsDP+R9}#gL5pfmQPU900xn9X=I[3№#I]j]N]R3]_kX(2=.3q;x]4GNMu(2[3@(8nk)m^?(1M2]z71{A8B4";

    Username_SSID = UserInfoPtr->GetUsername() + UserInfoPtr->GetSSID();
}

//---------------------------------------------------------------------------
//
TEncoder::~TEncoder()
{
    delete VigenereCipherPtr;
    delete TransposCipherPtr;
    delete UserInfoPtr;
}

//---------------------------------------------------------------------------
//
AnsiString TEncoder::Encode(const AnsiString &Username, const AnsiString &Password, int ScopeType)
{
    AnsiString UsernameLength = StrPadL(IntToStr(Username.Length()), 2, "0");
    AnsiString PasswordLength = StrPadL(IntToStr(Password.Length()), 2, "0");

    AnsiString UsernameEnc;
    AnsiString PasswordEnc;

    // Здесь выбор от  ScopeType
    switch(ScopeType) {
        case 0: { // Key
            UsernameEnc = VigenereCipherPtr->Encrypt(Username, UsernameKey);
            PasswordEnc = VigenereCipherPtr->Encrypt(Password, PasswordKey);
            break;
        }
        case 1: { // Username + SSID + Key
            UsernameEnc = VigenereCipherPtr->Encrypt(Username, Username_SSID + UsernameKey);
            PasswordEnc = VigenereCipherPtr->Encrypt(Password, Username_SSID + PasswordKey);
            break;
        }
        case 2: { // Registry
            //UsernameEnc = CipherPtr->Encrypt(Username, Username + SSID + UsernameKey);
            //PasswordEnc = CipherPtr->Encrypt(Password, Username + SSID + PasswordKey);
            break;
        }
    }

    // Шифруем финишную строку Виженером
    AnsiString CodeInfo = VigenereCipherPtr->Encrypt(IntToStr(ScopeType) + UsernameLength + PasswordLength, CodeInfoKey); // Кодируем цифры
    AnsiString Code = CodeInfo + UsernameEnc + PasswordEnc;

    // Шифруем перестановкой
    Code = TransposCipherPtr->Encrypt(Code, RouteRow, RouteCol);

    // Шифруем Виженером
    return VigenereCipherPtr->Encrypt(Code, FinishKey);     // Копируем всю строку целиком
}


//---------------------------------------------------------------------------
// Дешифрует кодовую строку, результаты сохраняет в Username и Password
void TEncoder::Decode(const AnsiString &Code, AnsiString &Username, AnsiString &Password)
{
    // 0                         00              00               Username     Password
    // код области действия      длина имени     длина пароля     имя          пароль
    // Код области действия
    //   - 0 - универсальный
    //   - 1 - пользователь ОС (имя пользователя, SSID)
    //   - 2 - на рабочую станцию (требуется запись в реестре)

    // Расшифровка виженера
    AnsiString DecriptFinish = VigenereCipherPtr->Decrypt(Code, FinishKey);

    // Расшифровка перестановки
    try {
        DecriptFinish = TransposCipherPtr->Decrypt(DecriptFinish, RouteRow, RouteCol);
    } catch (...) { // может вернуть ошибку если корень от длины строки не целое число
        throw;
    }

    // Расшифровка виженера
    AnsiString CodeInfo = VigenereCipherPtr->Decrypt(DecriptFinish.SubString(1,5), CodeInfoKey);

    int ScopeType = StrToInt(CodeInfo.SubString(1,1));
    int UsernameLength = StrToInt(CodeInfo.SubString(2,2));
    int PasswordLength = StrToInt(CodeInfo.SubString(4,2));

    AnsiString UsernameEnc = DecriptFinish.SubString(6, UsernameLength);
    AnsiString PasswordEnc = DecriptFinish.SubString(6 + UsernameLength, PasswordLength);

    // Здесь выбор от  ScopeType
    switch(ScopeType) {
        case 0: {
            Username = VigenereCipherPtr->Decrypt(UsernameEnc, UsernameKey);
            Password = VigenereCipherPtr->Decrypt(PasswordEnc, PasswordKey);
            break;
        }
        case 1: {
            Username = VigenereCipherPtr->Decrypt(UsernameEnc, Username_SSID + UsernameKey);
            Password = VigenereCipherPtr->Decrypt(PasswordEnc, Username_SSID + PasswordKey);
            break;
        }
    }
}


