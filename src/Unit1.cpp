#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------


#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dbf"
#pragma link "dbf"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    Logger = &TLogger::getInstance();
    CommandLine = &TCommandLine::getInstance();
    pTransferThread = NULL;
}

//---------------------------------------------------------------------------
// Формирует код доступа и отображает его пользователю (дописывает в лог-файл)
void TForm1::ShowCode(int ScopeType)
{
    if (ScopeType > -1) {
        AnsiString clSrcUsername = CommandLine->GetValue("-srcuser","-su");
        AnsiString clSrcPassword = CommandLine->GetValue("-srcpassword","-sp");
        AnsiString clDstUsername = CommandLine->GetValue("-dstuser","-du");
        AnsiString clDstPassword = CommandLine->GetValue("-dstpassword","-dp");

        TEncoder encoder;

        if (clSrcUsername != "" && clSrcPassword != "") {
            AnsiString Code = encoder.Encode(clSrcUsername, clSrcPassword, ScopeType);
            Logger->WriteLog("Вставьте в настройки источника: code=\"" +  XmlUtil::XmlEncode(Code) + "\"");
        }

        if (clDstUsername != "" && clDstPassword != "") {
            AnsiString Code = encoder.Encode(clDstUsername, clDstPassword, ScopeType);
            Logger->WriteLog("Вставьте в настройки приемника: code=\"" +  XmlUtil::XmlEncode(Code) + "\"");
        }
    }
}

//---------------------------------------------------------------------------
// Обновляет элементы управления в зависимости от состояния обьектов
void TForm1::UpdateInterface()
{
    // Если не выбран файл конфигурации
    ButtonStart->Enabled = CommandLine->GetFlag("-c", "-config");
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonStartClick(TObject *Sender)
{
    StartTransfer();
}

//---------------------------------------------------------------------------
// Открыть файл конфигурации
void __fastcall TForm1::OpenConfigButtonClick(TObject *Sender)
{
    // Опции окна сохранения результов
    OpenDialog1->Options.Clear();
    OpenDialog1->Options << ofFileMustExist;
    OpenDialog1->Filter = "XML-файлы (*.xml)|*.xml|Все файлы (*.*)|*.*";
    OpenDialog1->FilterIndex = 1;
    //OpenDialog1->DefaultExt = "xlsx";

    AnsiString filename;
    if (OpenDialog1->Execute()) {
        TCommandLine* CommandLine = &TCommandLine::getInstance();
        CommandLine->SetValue("-config", "-c", OpenDialog1->FileName);
        Logger->WriteLog("Пользователем выбран файл конфигурации \"" +  OpenDialog1->FileName + "\"");
        UpdateInterface();
    }

}

//---------------------------------------------------------------------------
// Скрывает контекстное меню если не выделен текст
void __fastcall TForm1::LogRichEditContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
    if (LogRichEdit->SelLength == 0)
        Handled = true;
}
//---------------------------------------------------------------------------
// Копирует текст в буффер обмена
void __fastcall TForm1::N1Click(TObject *Sender)
{
    LogRichEdit->CopyToClipboard();
}

//---------------------------------------------------------------------------
// Выход из программы
void __fastcall TForm1::ExitButtonClick(TObject *Sender)
{
    if (pTransferThread != NULL) {
        if (MessageBoxQuestion("Поток копирования данных активен. Вы уверены, что хотите прервать и закрыть программу?")) {
            // Прерываем поток, ждем его завершения
            Logger->WriteLog("Активировано принудительное завершение потока копирования данных.");
            pTransferThread->Terminate(); // Не реализовано!
            pTransferThread->WaitFor();
            //WaitForSingleObject((HANDLE)pTransferThread->Handle, INFINITE);
        } else {
            return; // Отмена
        }
    }

    // Закрываем программу
    Close();
}

//---------------------------------------------------------------------------
//
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    LogRichEdit->Lines->Clear();

    Form1->Caption = "Joiner " + AppFullVersion;

    Logger->AddConsole(LogRichEdit);    // Добавляем консоль, для вывода лог-сообщений

    UpdateInterface();


    // Если пользователь запросил сформировать код доступа для вставки в конфигурационный файл
    int ScopeType = CommandLine->GetFlag("-ac", "-accesscode")? 0 : -1;
    ScopeType = ScopeType == -1 && CommandLine->GetFlag("-acp", "-accesscodepersonal")? 1 : ScopeType;
    ScopeType = ScopeType == -1 && CommandLine->GetFlag("-acw", "--accesscodeworkstation")? 2 : ScopeType;

    if (ScopeType > -1) {
        ShowCode(ScopeType);
    }
}

//---------------------------------------------------------------------------
//
void __fastcall TForm1::FormActivate(TObject *Sender)
{
    // Если был использован ключ -a -auto
    if (CommandLine->GetFlag("-a", "-auto") && CommandLine->GetFlag("-c", "-config")) {
        Logger->WriteLog("Активирован автоматический запуск");
        StartTransfer();
    }
}

//---------------------------------------------------------------------------
// Организовать запуск в отдельном потоке
void TForm1::StartTransfer()
{
    OpenConfigButton->Enabled = false;
    ButtonStart->Enabled = false;
    //ExitButton->Enabled = false;
    Timer1->Enabled = true;

    // hEventTermination = CreateEvent ...
    pTransferThread = new TTransferThread(true);    // Создаем приостановленный поток
    pTransferThread->Resume();                   // Запускаем
}

//---------------------------------------------------------------------------
// Таймер, отслеживающий завершение потока
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if (pTransferThread != NULL) {
        if (WaitForSingleObject((HANDLE)pTransferThread->Handle, 0) == WAIT_OBJECT_0) {
            pTransferThread->Free();
            pTransferThread = NULL;

            Timer1->Enabled = false;

            OpenConfigButton->Enabled = true;

// ВНИМАНИЕ! ЭТОТ КОД ДОБАВЛЕН ВСПЕШКЕ. ТРЕБУЕТСЯ РЕФАКТОРИНГ!!!!!!!!!!!!!!!
    if (CommandLine->GetFlag("-ae", "-autoexit")) {
        this->Close();
    }
            UpdateInterface();
        }
    }
}
//---------------------------------------------------------------------------

