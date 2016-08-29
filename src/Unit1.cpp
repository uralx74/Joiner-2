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
// ��������� ��� ������� � ���������� ��� ������������ (���������� � ���-����)
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
            Logger->WriteLog("�������� � ��������� ���������: code=\"" +  XmlUtil::XmlEncode(Code) + "\"");
        }

        if (clDstUsername != "" && clDstPassword != "") {
            AnsiString Code = encoder.Encode(clDstUsername, clDstPassword, ScopeType);
            Logger->WriteLog("�������� � ��������� ���������: code=\"" +  XmlUtil::XmlEncode(Code) + "\"");
        }
    }
}

//---------------------------------------------------------------------------
// ��������� �������� ���������� � ����������� �� ��������� ��������
void TForm1::UpdateInterface()
{
    // ���� �� ������ ���� ������������
    ButtonStart->Enabled = CommandLine->GetFlag("-c", "-config");
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonStartClick(TObject *Sender)
{
    StartTransfer();
}

//---------------------------------------------------------------------------
// ������� ���� ������������
void __fastcall TForm1::OpenConfigButtonClick(TObject *Sender)
{
    // ����� ���� ���������� ���������
    OpenDialog1->Options.Clear();
    OpenDialog1->Options << ofFileMustExist;
    OpenDialog1->Filter = "XML-����� (*.xml)|*.xml|��� ����� (*.*)|*.*";
    OpenDialog1->FilterIndex = 1;
    //OpenDialog1->DefaultExt = "xlsx";

    AnsiString filename;
    if (OpenDialog1->Execute()) {
        TCommandLine* CommandLine = &TCommandLine::getInstance();
        CommandLine->SetValue("-config", "-c", OpenDialog1->FileName);
        Logger->WriteLog("������������� ������ ���� ������������ \"" +  OpenDialog1->FileName + "\"");
        UpdateInterface();
    }

}

//---------------------------------------------------------------------------
// �������� ����������� ���� ���� �� ������� �����
void __fastcall TForm1::LogRichEditContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
    if (LogRichEdit->SelLength == 0)
        Handled = true;
}
//---------------------------------------------------------------------------
// �������� ����� � ������ ������
void __fastcall TForm1::N1Click(TObject *Sender)
{
    LogRichEdit->CopyToClipboard();
}

//---------------------------------------------------------------------------
// ����� �� ���������
void __fastcall TForm1::ExitButtonClick(TObject *Sender)
{
    if (pTransferThread != NULL) {
        if (MessageBoxQuestion("����� ����������� ������ �������. �� �������, ��� ������ �������� � ������� ���������?")) {
            // ��������� �����, ���� ��� ����������
            Logger->WriteLog("������������ �������������� ���������� ������ ����������� ������.");
            pTransferThread->Terminate(); // �� �����������!
            pTransferThread->WaitFor();
            //WaitForSingleObject((HANDLE)pTransferThread->Handle, INFINITE);
        } else {
            return; // ������
        }
    }

    // ��������� ���������
    Close();
}

//---------------------------------------------------------------------------
//
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    LogRichEdit->Lines->Clear();

    Form1->Caption = "Joiner " + AppFullVersion;

    Logger->AddConsole(LogRichEdit);    // ��������� �������, ��� ������ ���-���������

    UpdateInterface();


    // ���� ������������ �������� ������������ ��� ������� ��� ������� � ���������������� ����
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
    // ���� ��� ����������� ���� -a -auto
    if (CommandLine->GetFlag("-a", "-auto") && CommandLine->GetFlag("-c", "-config")) {
        Logger->WriteLog("����������� �������������� ������");
        StartTransfer();
    }
}

//---------------------------------------------------------------------------
// ������������ ������ � ��������� ������
void TForm1::StartTransfer()
{
    OpenConfigButton->Enabled = false;
    ButtonStart->Enabled = false;
    //ExitButton->Enabled = false;
    Timer1->Enabled = true;

    // hEventTermination = CreateEvent ...
    pTransferThread = new TTransferThread(true);    // ������� ���������������� �����
    pTransferThread->Resume();                   // ���������
}

//---------------------------------------------------------------------------
// ������, ������������� ���������� ������
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if (pTransferThread != NULL) {
        if (WaitForSingleObject((HANDLE)pTransferThread->Handle, 0) == WAIT_OBJECT_0) {
            pTransferThread->Free();
            pTransferThread = NULL;

            Timer1->Enabled = false;

            OpenConfigButton->Enabled = true;

// ��������! ���� ��� �������� �������. ��������� �����������!!!!!!!!!!!!!!!
    if (CommandLine->GetFlag("-ae", "-autoexit")) {
        this->Close();
    }
            UpdateInterface();
        }
    }
}
//---------------------------------------------------------------------------

