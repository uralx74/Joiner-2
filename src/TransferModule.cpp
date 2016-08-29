// -c="c:\7-test sqltext\nasel_ccb_ft\change_cal_period.xml" -l="log_nasel_ccb_ft.log" -a -aeq
// -c="c:\6\kavabunga\kavabunga.xml" -l="%dlog.log" -a -accce
//

#include "pch.h"
#pragma hdrstop
#include "TransferModule.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TTransferModule::TTransferModule()
{
    //Logger = Singleton::getInstance();
    //_IsCancel = false;
}

//---------------------------------------------------------------------------
// ������ ������� ������
void __fastcall TTransferModule::Cancel()
{
    //_IsCancel = true;
}

//---------------------------------------------------------------------------
// ������
void __fastcall TTransferModule::Start()
{
    Logger = &TLogger::getInstance();

    //pParentThread = Thread;
    TXmlLoader* pXmlLoader = new TXmlLoader();

    if (pXmlLoader->LoadParameters()) {
        Transfer(pXmlLoader->SrcStor, pXmlLoader->DstStor);
    }

    delete pXmlLoader;
}

//---------------------------------------------------------------------------
//
void __fastcall TTransferModule::Transfer(TStorage* Src, TStorage* Dst)
{
    if (!Src || !Dst) {
        Logger->WriteLog("��������� ����������� ������� ���� ��������, ��� ��� " +
            ((String)(!Src? "��������" : "��������")) + " �� �����.");
        return;
    }

    Logger->WriteLog("��������� ����������� ������ ��������.");
    TDateTime StartTime = Now();


    int log_n;
    log_n = Logger->WriteLog("�������� ��������� >");
    try {
        Dst->openTable(false);   // ��������� ��������
        Logger->WriteLog("�������� ������ �������.", log_n);
    } catch (Exception &e) {
        Logger->WriteLog("�� ������� ������� �������� \"" + Dst->getTable() + "\". " + e.Message, log_n);
        Dst->closeTable();
        Logger->WriteLog("��������� ����������� ������ ���������.");
        return;
    }

   int DstRecordCountFirst = Dst->getRecordCount();



    //!!!!!!!!!!!!!!!!!!!!!!!!!!
    /*Sleep(1000);
    if (_IsCancel) {
        log_n = Logger->WriteLog("����������");
        throw Exception("Start()");
    }*/

    while(!Src->eot()) { // ���� �� ��������
        try {
            log_n = Logger->WriteLog("�������� ��������� >");
            Src->openTable(true);   // ��������� ��������
            Logger->WriteLog("�������� ������ �������.", log_n);
        } catch (Exception &e) {    // ���� ������, ��������� �� ��������� �������
            Logger->WriteLog("�� ������� ������� �������� \"" + Src->getTable()+ "\" (" + Src->getTableStage() + "). " + e.Message, log_n);
            Src->nextTable();
            continue;
        }

        // ���� �������� ������ �������
        log_n = Logger->WriteLog("����������� > \"" + Src->getTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage()+")");

        try {
            // ����� ������� ���������� ���������.
            // ����� ����� ;
            // � ��������� ���������� �������� �� ����, ������� ���� �������������� ���� � ���������
            // ������� ������ �����, ������� ���������� (required), �� ������� ��� � ���������
            // � ����������� �� ������ ��������� ������, ���� ����� ���� �����������, �� ������������� ���� ��� ����������

            Dst->linkSource(Src);

            int DstRecordCount = Dst->getRecordCount();
            while(!Src->eor()) {    // ���� �� �������
                //String sss = Src->getRecordStage();
                //if (Src->GetRecordIndex() % 100 == 0) {
                    //Logger->WriteLog("����������� > \"" + Src->GetTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage() + ")",log_n);
                    //Logger->WriteLog("����������� > \"" + Src->GetTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage() + ")");
                //}
                //Logger->WriteLog("����������� > " + Src->GetTable() + " " + Src->GetStage(), log_n);

                Dst->append();

                while(!Dst->eof()) {    // ���� �� �������� � ���������
                    bool isLinkedField = Dst->isLinkedField();
                    bool isActiveField = Dst->isActiveField();
                    if (isLinkedField && isActiveField) {
                        Variant Value;

                        try {
                            TStorageField *field = Dst->getField();
                            Value = Src->getFieldValue(field);
                        } catch (Exception &e) {
                            Logger->WriteLog("������ � ��������� \"" + Src->getTable() + "\" (" + Src->getTableStage() + ")" + ". " + e.Message /*+ Dst->GetSrcField()*/, log_n);
                            throw Exception("");
                        }
                        try {
                            Dst->setFieldValue(Value);
                        } catch (Exception &e) {
                            Logger->WriteLog("������ � ��������� \"" + Dst->getTable() + "\" (" + Dst->getTableStage() + ")" + ". " + e.Message /*+ Dst->GetSrcField()*/, log_n);
                            throw Exception("");
                        }
                    }
                    Dst->nextField();
                }

                //if (Dst->IsModified()) {
                //    Dst->Post();
                //} else {
                //    Dst->Rollback();
                //}

                Dst->post();  // ��������� ������
                Src->nextRecord();
            }

            Dst->commit();

            // ���������, ��� �� ���� ���� Commit � ���������
            // ����� ���� ������������ RecordIndex ��� RecordCount
            int LoadedRecordsCount = Dst->getRecordCount() - DstRecordCount;

            Logger->WriteLog("��������� " + IntToStr(LoadedRecordsCount) + " ������� �� \"" + Src->getTable() + "\"", log_n);
            DstRecordCount = Dst->getRecordCount();

        } catch (Exception &e) {
            if (e.Message != "")
                Logger->WriteLog("������������� ������. ��������: \"" + Src->getTable() + "\" (" + Src->getTableStage() + "), �������� \"" + Dst->getTable()+ "\". " + e.Message /*+ Dst->GetSrcField()*/);
        }
        Src->nextTable();
    }

    // ��������� ������ - ����������
    TDateTime StopTime = Now() ;
    int TotalSec = SecsPerDay * (StopTime - StartTime);
    int hh = (TotalSec ) / 3600;
    int mm = (TotalSec - hh * 3600) / 60;
    int ss = TotalSec % 60;
    AnsiString sTotalTime = IntToStr(ss) + " ���";
    if (mm > 0)
        sTotalTime = IntToStr(mm) + " ��� " + sTotalTime;
    if (hh > 0)
        sTotalTime = IntToStr(hh) + " ��� " + sTotalTime;


    int DstRecordCountTotal = Dst->getRecordCount() - DstRecordCountFirst;

    // ���������, ��� �� ���� ���� Commit � ���������
    // ����� ���� ������������ RecordIndex ��� RecordCount
    if (Dst->isModified()) {
        Logger->WriteLog("����� ��������� " + IntToStr(DstRecordCountTotal) + " ������� �� " + sTotalTime+ ".");
        Logger->WriteLog("��������� �������� � \"" + Dst->getTable() + "\".");
    } else {
        Logger->WriteLog("�������� �� ��� �������. ����� ��������� ������� �� �������: " + sTotalTime+ ".");
    }

    // �������� ��������� � ���������
    // �������� - �������� ��������� �������� ������
    // � ���������� �������� �� closeStorage();
    Src->closeTable();
    Dst->closeTable();

    Logger->WriteLog("��������� ����������� ������ ���������.");
}


