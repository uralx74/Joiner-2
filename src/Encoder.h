/*******************************************************************************
 Encoder.h
 @author: vsovchinnikov
 @date: 2016-05-23
*******************************************************************************/

#ifndef ENCODER_H
#define ENCODER_H

#include <Classes.hpp>
#include <Controls.hpp>
#include <ComCtrls.hpp>

#include "vigenerecipher.h"
#include "transposcipher.h"
#include "taskutils.h"
#include "Userinfo.h"

class TEncoder {

private:
    AnsiString FinishKey;
    AnsiString UsernameKey;
    AnsiString PasswordKey;
    AnsiString CodeInfoKey;
    TVigenereCipher* VigenereCipherPtr;
    TTransposCipher* TransposCipherPtr;
    TUserInfo* UserInfoPtr;
    AnsiString Username_SSID;   // Имя пользователя ОС + SSID
    AnsiString ExtensionWord;   // Для удлинения кода (промежуточного кода) до 144 символов

    static const AnsiString MasterKey;
    static const int RouteRow[];
    static const int RouteCol[];

public:
    TEncoder();
    ~TEncoder();
    AnsiString Encode(const AnsiString &Username, const AnsiString &Password, int ScopeType);
    void Decode(const AnsiString &Code, AnsiString &Username, AnsiString &Password);

};                 
  
//---------------------------------------------------------------------------
#endif

