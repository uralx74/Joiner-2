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
// Отмена запуска потока
void __fastcall TTransferModule::Cancel()
{
    //_IsCancel = true;
}

/* Запускает процесс загрузки параметров и обработки (копирования) данных
*/
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

/* Копирует данные из хранилища - источника в приемник
   В цикле перебирает по порядку таблцы, поля, записи и копирует поячеечно
   данные в приемник.
*/
void __fastcall TTransferModule::Transfer(TStorage* Src, TStorage* Dst)
{
    if (!Src || !Dst) {
        Logger->WriteLog("Процедура копирования неможет быть запущена, так как " +
            ((String)(!Src? "источник" : "приемник")) + " не задан.");
        return;
    }

    Logger->WriteLog("Процедура копирования данных запущена.");
    TDateTime StartTime = Now();


    int log_n;
    log_n = Logger->WriteLog("Открытие приемника >");
    try {
        Dst->openTable(false);   // Открываем приемник
        Logger->WriteLog("Приемник открыт успешно.", log_n);
    } catch (Exception &e) {
        Logger->WriteLog("Не удалось открыть приемник \"" + Dst->getTable() + "\". " + e.Message, log_n);
        Dst->closeTable();
        Logger->WriteLog("Процедура копирования данных завершена.");
        return;
    }

   int DstRecordCountFirst = Dst->getRecordCount();



   Dst->eot();
    //!!!!!!!!!!!!!!!!!!!!!!!!!!
    /*Sleep(1000);
    if (_IsCancel) {
        log_n = Logger->WriteLog("ПРЕРЫВАНИЕ");
        throw Exception("Start()");
    }*/

    while(!Src->eot()) { // Цикл по таблицам
        try {
            log_n = Logger->WriteLog("Открытие источника >");

            //int k = Src->getTableCount();
            Src->openTable(true);   // Открываем источник
            Logger->WriteLog("Источник открыт успешно.", log_n);
        } catch (Exception &e) {    // если ошибка, переходим на следующую таблицу
            Logger->WriteLog("Не удалось открыть источник \"" + Src->getTable()+ "\" (" + Src->getTableStage() + "). " + e.Message, log_n);
            Src->nextTable();
            continue;
        }

        // Если источник открыт успешно
        log_n = Logger->WriteLog("Загружается > \"" + Src->getTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage()+")");

        try {
            // Здесь сделать подготовку приемника.
            // Чтото вроде ;
            // В приемнике необходимо отметить те поля, которым есть сопоставленные поля в источнике
            // Вывести список полей, которые необходимы (required), но которых нет в источнике
            // В зависимости от ключей командной строки, если такие поля обнаружатся, то останавливать цикл или продолжать

            Dst->linkSource(Src);

            int DstRecordCount = Dst->getRecordCount();
            while(!Src->eor()) {    // Цикл по строкам
                //String sss = Src->getRecordStage();
                //if (Src->GetRecordIndex() % 100 == 0) {
                    //Logger->WriteLog("Загружается > \"" + Src->GetTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage() + ")",log_n);
                    //Logger->WriteLog("Загружается > \"" + Src->GetTable() + "\" (" + Src->getTableStage() + "; " + Src->getRecordStage() + ")");
                //}
                //Logger->WriteLog("Загружается > " + Src->GetTable() + " " + Src->GetStage(), log_n);

                Dst->append();

                while(!Dst->eof()) {    // Цикл по столбцам в приемнике
                    bool isLinkedField = Dst->isLinkedField();
                    bool isActiveField = Dst->isActiveField();
                    if (isLinkedField && isActiveField) {
                        Variant Value;

                        try {
                            TStorageField *field = Dst->getField();
                            Value = Src->getFieldValue(field);
                        } catch (Exception &e) {
                            Logger->WriteLog("Ошибка в источнике \"" + Src->getTable() + "\" (" + Src->getTableStage() + ")" + ". " + e.Message /*+ Dst->GetSrcField()*/, log_n);
                            throw Exception("");
                        }
                        try {
                            Dst->setFieldValue(Value);

                        } catch (Exception &e) {
                            Logger->WriteLog("Ошибка в приемнике \"" + Dst->getTable() + "\" (" + Dst->getTableStage() + ")" + ". " + e.Message /*+ Dst->GetSrcField()*/, log_n);
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

                Dst->post();  // Фиксирует строку
                Src->nextRecord();
            }

            Dst->commit();

            // Проверять, был ли хоть один Commit в приемнике
            // может быть использовать RecordIndex или RecordCount
            int LoadedRecordsCount = Dst->getRecordCount() - DstRecordCount;

            Logger->WriteLog("Загружено " + IntToStr(LoadedRecordsCount) + " записей из \"" + Src->getTable() + "\"", log_n);
            DstRecordCount = Dst->getRecordCount();

        } catch (Exception &e) {
            if (e.Message != "")
                Logger->WriteLog("Непредвиденая ошибка. Источник: \"" + Src->getTable() + "\" (" + Src->getTableStage() + "), приемник \"" + Dst->getTable()+ "\". " + e.Message /*+ Dst->GetSrcField()*/);
        }
        Src->nextTable();
    }

    // Формируем строку - секундомер
    TDateTime StopTime = Now() ;
    int TotalSec = SecsPerDay * (StopTime - StartTime);
    int hh = (TotalSec ) / 3600;
    int mm = (TotalSec - hh * 3600) / 60;
    int ss = TotalSec % 60;
    AnsiString sTotalTime = IntToStr(ss) + " сек";
    if (mm > 0)
        sTotalTime = IntToStr(mm) + " мин " + sTotalTime;
    if (hh > 0)
        sTotalTime = IntToStr(hh) + " час " + sTotalTime;


    int DstRecordCountTotal = Dst->getRecordCount() - DstRecordCountFirst;

    // Проверять, был ли хоть один Commit в приемнике
    // может быть использовать RecordIndex или RecordCount
    if (Dst->isModified()) {
        Logger->WriteLog("Всего загружено " + IntToStr(DstRecordCountTotal) + " записей за " + sTotalTime+ ".");
        Logger->WriteLog("Результат сохранен в \"" + Dst->getTable() + "\".");
    } else {
        Logger->WriteLog("Приемник не был изменен. Всего затрачено времени на процесс: " + sTotalTime+ ".");
    }

    // Закрытие источника и приемника
    // Временно - закрытие последних открытых таблиц
    // в дальнейшем изменить на closeStorage();
    Src->closeTable();
    Dst->closeTable();

    Logger->WriteLog("Процедура копирования данных завершена.");
}


