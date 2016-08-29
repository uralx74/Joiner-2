#include "pch.h"
#pragma hdrstop

#include "Logger.h"


//------------------------------------------------------------------------------------//
//                                                                                    //
//                                    class TLogConsole                               //
//                                                                                    //
//------------------------------------------------------------------------------------//


//---------------------------------------------------------------------------
// ������� �������
void TLogConsole::AddMsgHistory(const TStringList* History)
{
    for (int i=0; i < History->Count; i++) {
        this->AddMsg(History->Strings[i]);
    }
}


//------------------------------------------------------------------------------------//
//                                                                                    //
//                                    class TLogConsoleFile                           //
//                                                                                    //
//------------------------------------------------------------------------------------//


/*int TLogConsoleRichEdit::AddMsg(const AnsiString& Msg)
{
}*/
//---------------------------------------------------------------------------
TLogConsoleFile::TLogConsoleFile(AnsiString FileName, bool Rewrite)
{

    //FileName = GetLogFilename(FileName);
    LogFilePtr = std::fopen(FileName.c_str(), Rewrite? "wt" : "at");

    //-------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // ��������� ������� �����, ���� �� ���, �� ��������� ����
    if (!LogFilePtr)
        return;


    /*if (Rewrite) {
        FileName = std::fopen(FileName.c_str(), "wt");
    } else {
        FileName = std::fopen(FileName.c_str(), "at");
    } */

}

//---------------------------------------------------------------------------
int TLogConsoleFile::AddMsg(const AnsiString& Msg)
{
    fprintf(LogFilePtr,  "%s\n", Msg);
    return 0;
}


//------------------------------------------------------------------------------------//
//                                                                                    //
//                                    class TLogConsoleRichEdit                       //
//                                                                                    //
//------------------------------------------------------------------------------------//


//---------------------------------------------------------------------------
TLogConsoleRichEdit::TLogConsoleRichEdit(TRichEdit* RichEdit)
{
    LogEditPtr = RichEdit;
}

//---------------------------------------------------------------------------
int TLogConsoleRichEdit::AddMsg(const AnsiString& Msg)
{
    return LogEditPtr->Lines->Add(Msg);
}


//------------------------------------------------------------------------------------//
//                                                                                    //
//                                    class TLogger                                   //
//                                                                                    //
//------------------------------------------------------------------------------------//

TLogger* TLogger::p_instance = 0;
TLoggerDestroyer TLogger::destroyer;

//---------------------------------------------------------------------------
// Singleton
TLoggerDestroyer::~TLoggerDestroyer()
{
    delete p_instance;
}

//---------------------------------------------------------------------------
// Singleton
void TLoggerDestroyer::initialize(TLogger* p)
{
    p_instance = p;
}

//---------------------------------------------------------------------------
// Singleton
TLogger& TLogger::getInstance()
{
    if(!p_instance) {
        p_instance = new TLogger();
        destroyer.initialize(p_instance);
    }
    return *p_instance;
}

//---------------------------------------------------------------------------
//
TLogger::TLogger()
{
    LogEditPtr = NULL;
    LogFilePtr = NULL;
    LogHist = new TStringList();

}

//---------------------------------------------------------------------------
//
TLogger::~TLogger()
{
    fclose(LogFilePtr);
    LogFilePtr = NULL;

    delete LogHist;
    LogHist = NULL;
}

/*int __fastcall AddLogMsgC(AnsiString MessageStr);
{
    LogHist->Add(MessageStr);   // ��������� ������ � �������
}*/

//---------------------------------------------------------------------------
// ����� ���-������
int __fastcall TLogger::WriteLog(AnsiString MessageStr, int LineNumber)
{
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, true, "TLogger::WriteLog");
    if(!hMutex)
        hMutex = CreateMutex(0, 0, "TLogger::WriteLog");

    if (WaitForSingleObject(hMutex, 2000) != WAIT_OBJECT_0) {
        return 0;
    }

    AnsiString sDate = FormatDateTime("yyyy.mm.dd hh:mm:ss", Now());
    MessageStr = "[" + sDate + "] " + MessageStr;

    LogHist->Add(MessageStr);   // ��������� ������ � �������

    /*if (LogEditPtr) {
        if (LineNumber == -1) {
            LineNumber = LogEditPtr->Lines->Add(MessageStr);
        } else {
            // ��������, ����� ������� ������� ��������� ������ ����� �������� windows message !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //LogEditPtr->Lines->BeginUpdate();
            //LogEditPtr->SelLength = 0;
            LogEditPtr->Lines->Strings[LineNumber] = MessageStr;
            //LogEditPtr->Update();
            //LogEditPtr->Lines->EndUpdate();
        }
        LogEditPtr->Update();
        //Application->ProcessMessages();
    }
    if (LogFilePtr) {
        fprintf(LogFilePtr,  "%s\n", MessageStr);
    } */


    for(std::vector<TLogConsole*>::iterator it = Console.begin(); it < Console.end(); it++ ) {
        (*it)->AddMsg(MessageStr);
    }



    // ����������� �������
    ReleaseMutex(hMutex);
	CloseHandle(hMutex);

    return LineNumber;
}

//---------------------------------------------------------------------------
// ������������� �������� ������� ��� ������ ����������
// ���� ����������� �������, �� ������� ��
void __fastcall TLogger::AddConsole(TRichEdit* Edit)
{
    //LogEditPtr = Edit;
    TLogConsoleRichEdit* lc = new TLogConsoleRichEdit(Edit);
    Console.push_back(lc);
    lc->AddMsgHistory(LogHist);
}

//---------------------------------------------------------------------------
// ������������� �������� ���� ��� ������ ����������
// ���� ����������� �������, �� ������� ��
void __fastcall TLogger::AddConsole(AnsiString LogFileName, bool Rewrite)
{
    LogFileName = GetLogFilename(LogFileName);

    TLogConsoleFile* lc = new TLogConsoleFile(LogFileName, Rewrite);
    Console.push_back(lc);
    lc->AddMsgHistory(LogHist);
}

//---------------------------------------------------------------------------
// �������� ���������������� ���� � ����� �����
// %d - ���������� �� ���� � ������� yyyymmdd
// %t - ���������� �� ����� � ������� hhmmss
AnsiString TLogger::GetLogFilename(const AnsiString& LogFileName)
{
    // %d %t
    AnsiString sDate = FormatDateTime("yyyymmdd", Now());
    AnsiString sTime = FormatDateTime("hhmmss", Now());

    TReplaceFlags replaceflags = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;

    AnsiString Result = StringReplace(LogFileName, "%d", sDate, replaceflags);
    Result = StringReplace(Result, "%t", sTime, replaceflags);

    return Result;
}






//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

