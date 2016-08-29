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

    // Устанавливаем Лог-файл, если задан в параметрах
    AnsiString sLogFileName = cl->GetValue("-log","-l");
    if (sLogFileName != "") {
        try {
            sLogFileName = ExpandFileName(sLogFileName);
            Logger->AddConsole(sLogFileName, cl->GetFlag("-logrewrite","-lr")); // Если GetFlag, то лог-файл перезаписывается
        } catch (...) {
        }
    }


    Logger->WriteLog("---------------------------------------------------------------------------" );
    Logger->WriteLog("Joiner " + AppFullVersion);
    Logger->WriteLog("AppFileName = " + Application->ExeName);
    //Logger->WriteLog("LogFileName = " + TLogger::GetLogFilename(sLogFileName));
    Logger->WriteLog("LogFileName = " + Logger->GetLogFilename(sLogFileName));

    Logger->WriteLog("Программа запущена." );
    //Logger->WriteLog("Полный путь " + Application->ExeName + " .");
    // Выводить путь
    // имя компьютера
    // имя пользователя
    // Используемые ключи командной строки
    // путь к Лог-файлу



    // Выводим справку о программе, если требуется
    if (cl->GetFlag("-help","-h")) {
        MessageBoxInf(
            "Использование:"
            "\nJoiner [-c=путь_к_файлу] [-du=my_user1 -dp=my_password1] [-su=my_user2 -sp=my_password2] "
            "[-h]"

            "\n"
            "\nПараметры:"
            "\n-h | -help\tСправка о программе (это окно)"
            "\n-i | -info\tИнформация об авторе программы"
            "\n"
            "\n-c | -config = <путь_к_файлу>\tЗадает полный путь к файлу настройки импорта-экспорта"
            "\n"
            "\n-su | -srcuser\tИмя пользователя в БД источника"
            "\n-sp | -srcpassword\tПароль доступа к БД источника"
            "\n-du | -dstuser\tИмя пользователя в БД назначения"
            "\n-dp | -dstpassword\tПароль доступа к БД назначения"
            "\n-gp | -getpassword\tОтображает зашифрованные значения имени пользователя и пароля"
            "\n"
            "\n-ac | -accesscode\tОтображает зашифрованные значения имени пользователя и пароля"
            "\n-acp | -accesscodepersonal\tОтображает зашифрованные значения имени пользователя и пароля, с привязкой к пользователю"
            "\n-acw | -accesscodeworkstation\tОтображает зашифрованные значения имени пользователя и пароля, с привязкой к рабочей станции"
            "\n"
            "\n-s | -silent\tТихий режим"
            "\n-a | -auto\tАвтоматический режим запуска (не требуется с ключем -s)"
            "\n-ae | -autoexit\tАвтоматический выход из программы после завершения (не требуется с ключем -s)"
            "\n-l | -log = <путь_к_лог_файлу>\tЗадает путь к лог-файлу"
            "\n-lr | -logrewrite \tПерезаписывать лог-файл"
        );
        return 0;
    }

    // Выводим информацию об авторе, если требуется
    if (cl->GetFlag("-info","-i")) {
        MessageBoxInf(
            "Программа для слияния файлов\nJoiner v." + AppVersion + " (" + AppBuild + ")"
            "\n"
            "\nCopyright © 2014-2016"
            "\n"
            "\nЦентральный филиал ОАО \"Челябэнергосбыт\""
            "\n"
            "\nАвтор: Инженер-программист Отдела ИТ"
            "\nЦентрального филиала ОАО \"Челябэнергосбыт\""
            "\nВ.С.Овчинников"
            "\n"
            "\n e-mail: x74@list.ru"
        );
        return 0;
    }

    // Выполняем в тихом режиме, если задан ключ
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
        Logger->WriteLog("Программа завершена.");
    }
    catch (Exception &exception)
    {
        Logger->WriteLog("Программа завершена с ошибкой. " + exception.Message);
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
