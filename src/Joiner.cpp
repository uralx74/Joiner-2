//---------------------------------------------------------------------------

#ifndef _DEBUG 
#define DEBUG_MESSAGE
#endif  
#ifdef _DEBUG  
#define DEBUG_MESSAGE(msg) OutputDebugString(msg) 
#endif


#include "pch.h"
#pragma hdrstop

#include "CommandLine.h"
#include "Messages.h"
#include "Logger.h"
#include "c:\PROGRS\current\util\appver.h"
#include "TransferModule.h"

USEUNIT("pch.cpp");
USERES("Joiner.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("..\util\MSExcelWorks.cpp");
USEUNIT("..\util\MSXMLWorks.cpp");
USEUNIT("..\util\CommandLine.cpp");
USEUNIT("TransferModule.cpp");
USEUNIT("Storage.cpp");
//USEUNIT("StorageOra.cpp");
USEUNIT("StorageDbf.cpp");
//USEUNIT("StorageText.cpp");
//USEUNIT("StorageExcel.cpp");
USEUNIT("Logger.cpp");
USEUNIT("..\util\VigenereCipher.cpp");
USEUNIT("..\util\TransposCipher.cpp");
USEUNIT("Encoder.cpp");
USEUNIT("TransferThread.cpp");
USEUNIT("XmlParamsLoader.cpp");
USEUNIT("StorageTable.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   //TCommandLine commandline;
    TCommandLine* cl = &TCommandLine::getInstance();
    TLogger* Logger = &TLogger::getInstance();

    // ������������� ���-����, ���� ����� � ����������
    AnsiString sLogFileName = cl->GetValue("-log","-l");
    if (sLogFileName != "") {
        try {
            sLogFileName = ExpandFileName(sLogFileName);
            Logger->AddConsole(sLogFileName, cl->GetFlag("-logrewrite","-lr")); // ���� GetFlag, �� ���-���� ����������������
        } catch (...) {
        }
    }


    Logger->WriteLog("---------------------------------------------------------------------------" );
    Logger->WriteLog("Joiner " + AppFullVersion);
    Logger->WriteLog("AppFileName = " + Application->ExeName);
    //Logger->WriteLog("LogFileName = " + TLogger::GetLogFilename(sLogFileName));
    Logger->WriteLog("LogFileName = " + Logger->GetLogFilename(sLogFileName));

    Logger->WriteLog("��������� ��������." );
    //Logger->WriteLog("������ ���� " + Application->ExeName + " .");
    // �������� ����
    // ��� ����������
    // ��� ������������
    // ������������ ����� ��������� ������
    // ���� � ���-�����



    // ������� ������� � ���������, ���� ���������
    if (cl->GetFlag("-help","-h")) {
        MessageBoxInf(
            "�������������:"
            "\nJoiner [-c=����_�_�����] [-du=my_user1 -dp=my_password1] [-su=my_user2 -sp=my_password2] "
            "[-h]"

            "\n"
            "\n���������:"
            "\n-h | -help\t������� � ��������� (��� ����)"
            "\n-i | -info\t���������� �� ������ ���������"
            "\n"
            "\n-c | -config = <����_�_�����>\t������ ������ ���� � ����� ��������� �������-��������"
            "\n"
            "\n-su | -srcuser\t��� ������������ � �� ���������"
            "\n-sp | -srcpassword\t������ ������� � �� ���������"
            "\n-du | -dstuser\t��� ������������ � �� ����������"
            "\n-dp | -dstpassword\t������ ������� � �� ����������"
            "\n-gp | -getpassword\t���������� ������������� �������� ����� ������������ � ������"
            "\n"
            "\n-ac | -accesscode\t���������� ������������� �������� ����� ������������ � ������"
            "\n-acp | -accesscodepersonal\t���������� ������������� �������� ����� ������������ � ������, � ��������� � ������������"
            "\n-acw | -accesscodeworkstation\t���������� ������������� �������� ����� ������������ � ������, � ��������� � ������� �������"
            "\n"
            "\n-s | -silent\t����� �����"
            "\n-a | -auto\t�������������� ����� ������� (�� ��������� � ������ -s)"
            "\n-ae | -autoexit\t�������������� ����� �� ��������� ����� ���������� (�� ��������� � ������ -s)"
            "\n-l | -log = <����_�_���_�����>\t������ ���� � ���-�����"
            "\n-lr | -logrewrite \t�������������� ���-����"
        );
        return 0;
    }

    // ������� ���������� �� ������, ���� ���������
    if (cl->GetFlag("-info","-i")) {
        MessageBoxInf(
            "��������� ��� ������� ������\nJoiner v." + AppVersion + " (" + AppBuild + ")"
            "\n"
            "\nCopyright � 2014-2016"
            "\n"
            "\n����������� ������ ��� \"���������������\""
            "\n"
            "\n�����: �������-����������� ������ ��"
            "\n������������ ������� ��� \"���������������\""
            "\n�.�.����������"
            "\n"
            "\n e-mail: x74@list.ru"
        );
        return 0;
    }

    // ��������� � ����� ������, ���� ����� ����
    if (cl->GetFlag("-silent","-s")) {
        CoInitialize(NULL);     // COM libruary initialize
        TTransferModule TransferModule;
        TransferModule.Start();
        CoUninitialize();
        return 0;
    }

    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TForm1), &Form1);
         Application->Run();
        Logger->WriteLog("��������� ���������.");
    }
    catch (Exception &exception)
    {
        Logger->WriteLog("��������� ��������� � �������. " + exception.Message);
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
