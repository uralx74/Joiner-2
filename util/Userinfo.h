//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

#ifndef USERINFO_H
#define USERINFO_H

#include <Sddl.h>
#include <Lmcons.h>

class TUserInfo {
public:
    TUserInfo();
	AnsiString __fastcall GetSSID();
	AnsiString __fastcall GetUsername();

};

TUserInfo::TUserInfo()
{
}

//---------------------------------------------------------------------------
// Returns the current user SSID
AnsiString __fastcall TUserInfo::GetSSID()
{
    //
    HANDLE hToken;

    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);

    DWORD len = 0;
    GetTokenInformation(hToken, TokenUser, 0, 0, &len);

    TOKEN_USER* pUser = (TOKEN_USER*)malloc(len);

    GetTokenInformation(hToken, TokenUser, pUser, len, &len);

    TCHAR* sid = 0;
    ConvertSidToStringSid(pUser->User.Sid, &sid);

    

    AnsiString Result = AnsiString(sid);

    CloseHandle(hToken);
    LocalFree(sid);
    free(pUser);


    return Result;

}

//---------------------------------------------------------------------------
// Returns the current user name
AnsiString __fastcall TUserInfo::GetUsername()
{
    char username[UNLEN+1];
    DWORD username_len = UNLEN+1;
    ::GetUserName(username, &username_len);

    return AnsiString(username);
}

#endif
