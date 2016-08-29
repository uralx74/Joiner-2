#include "pch.h"
#pragma hdrstop

#include "StorageOra.h"


//---------------------------------------------------------------------------
// TStorageOra
//---------------------------------------------------------------------------
// ќписание механизма работы с TOraSession и TOraQuery
//  try {
//      OraSession->StartTransaction();
//      OraQuery->Add("sql text"); или  OraQuery->CreateProcCall("sql text",0);
//      OraQuery->FieldByName("field")->Value = ... или OraQuery->ParamByName("field")->Value = ...
//      OraQuery->ExecSQL();
//      OraSession->Commit();
//  } catch (Exception &e) {
//      OraSession->Rollback();
//      Application->ShowException(&e);
//  }
//
//
//---------------------------------------------------------------------------
//
void TStorageOra::openConnection(AnsiString Server, AnsiString Username, AnsiString Password)
{
    try {
        // ќткрываем приемник
        dbSession = new TOraSession(NULL);
        //ThreadOraSession->OnError = OraSession1Error;
        dbSession->LoginPrompt = false;
        dbSession->Password = Password;
        dbSession->Username = Username;
        dbSession->Server = Server;
        //dbSession->Options = TemplateOraSession->Options;
        //dbSession->HomeName = TemplateOraSession->HomeName;
        dbSession->Options->Direct = true;
        dbSession->ConnectMode = cmNormal;
        dbSession->Pooling = false;
        dbSession->ThreadSafety = true;
        dbSession->AutoCommit = false;
        dbSession->DataTypeMap->Clear();
        dbSession->Connect();
    } catch (Exception& e) {
        delete dbSession;
        dbSession = NULL;
        throw Exception("Can't connect to server \"" + Server + "\". " + e.Message);
    }



 /*   TOraQuery *OraQuery = new TOraQuery(NULL);
    try
    {
        OraQuery->Session = dbSession;
        OraQuery->SQL->Clear();
        OraQuery->SQL->Add("ALTER SESSION SET NLS_LANGUAGE = RUSSIAN");
        OraQuery->Execute();
        OraQuery->SQL->Clear();
        OraQuery->SQL->Add("ALTER SESSION SET NLS_TERRITORY = AMERICA");
        OraQuery->Execute();
        OraQuery->SQL->Clear();
        OraQuery->SQL->Add("ALTER SESSION SET NLS_DATE_FORMAT = 'DD.MM.YYYY'");
        OraQuery->Execute();
        CurrencyString = "р.";
        DecimalSeparator = '.';
        CurrencyDecimals = 2;
        ThousandSeparator = ' ';
        ShortDateFormat = "dd.mm.yyyy";
        DateSeparator = '.';
        OraQuery->SQL->Clear();
        OraQuery->SQL->Add("alter session set nls_numeric_characters='"+ AnsiString(DecimalSeparator)+AnsiString(ThousandSeparator)+"'");
        OraQuery->Execute();
        OraQuery->Close();
        OraQuery->SQL->Clear();
        delete OraQuery;
    } catch (Exception &e) {
        throw e;
    }  */

}

//---------------------------------------------------------------------------
//
void TStorageOra::closeTable()
{
    if (dbQuery != NULL) {
        Active = false;
        dbQuery->Close();
        delete dbQuery;
        dbQuery = NULL;
    }

    if (dbSession != NULL) {
        dbSession->Close();
        delete dbSession;
        dbSession = NULL;
    }

    TStorage::closeTable();
    Active = false;
}

//---------------------------------------------------------------------------
//
TStorageOra::~TStorageOra()
{
    TStorageOra::closeTable();
}

//---------------------------------------------------------------------------
//
void TStorageOra::prepareQuery()
{
    try {
   	    dbQuery = new TOraQuery(NULL);
        dbQuery->Session = dbSession;
        dbQuery->FetchAll = true;
        //dbQuery->CachedUpdates = true;
        dbQuery->SQL->Clear();
    } catch (...) {
        delete dbQuery;
        dbQuery = NULL;
        throw Exception("");
    }
}

//---------------------------------------------------------------------------
// ќчистка таблицы
void TStorageOra::truncateTable(TStorageTableOra* Table)
{
    if (!ReadOnly && Table->Truncate && Table->Table != "") {
        try {
            //dbQuery->SQL->Add("delete from " + Table->Table);
            dbQuery->SQL->Add("truncate table " + Table->Table);
            dbQuery->ExecSQL();
            dbQuery->SQL->Clear();
        } catch (...) {
            delete dbQuery;
            dbQuery = NULL;
            throw Exception("Can't truncate table " + Table->Table + ".");
        }
    } else {
        throw Exception("Can't truncate ReadOnly table .");
    }
}

//---------------------------------------------------------------------------
// —оздание принимающего OraQuery
void TStorageOraProc::openTable(bool ReadOnly)
{
    this->ReadOnly = ReadOnly;

    /*возможно воспользоватьс€ TStorageOra::Open*/

    if (Tables.size()>0) {
        openConnection(Tables[TableIndex].Server, Tables[TableIndex].Username, Tables[TableIndex].Password);
    } else {
        throw Exception("Storage is not specified.");
    }

    prepareQuery();
    if (ReadOnly == false) {
        truncateTable(&Tables[TableIndex]);
    }

    try {
 	    dbQuery->CreateProcCall(Tables[TableIndex].Procedure, 0);
    } catch (...) {
        delete dbQuery;
        dbQuery = NULL;
        throw Exception("Can't create procedure call.");
    }

    Active = true;
}

//---------------------------------------------------------------------------
// ”станавливает значение активного пол€
void TStorageOraProc::setFieldValue(Variant Value)
{
    if (curField->isActiveField())
        dbQuery->ParamByName(curField->name)->Value = Value;
}

//---------------------------------------------------------------------------
//
void TStorageOraProc::post()
{
    dbQuery->ExecSQL();
    FieldIndex = 0;
    RecordCount++;
    Modified = true;
}


//---------------------------------------------------------------------------
//
void TStorageOraProc::nextField()
{
    if (!eof()) {
        FieldIndex++;
    }
}

//---------------------------------------------------------------------------
// ¬озвращает соответствующее активному полю - им€ пол€ в таблице-источнике
/*AnsiString TStorageOraProc::GetSrcField()
{
    return Fields[FieldIndex]->name_src;
}*/

//---------------------------------------------------------------------------
//
/*bool TStorageOraProc::IsActiveField()
{
    return Fields[FieldIndex]->active && Fields[FieldIndex]->enable;
} */

//---------------------------------------------------------------------------
//
TOraField* TStorageOraProc::addField()
{
    TOraField* Field = new TOraField();
    Fields.push_back(dynamic_cast<TStorageField*>(Field));
    FieldCount++;
    return Field;

}

/*
//---------------------------------------------------------------------------
//
void TStorageOraProc::AddField(const TOraField& Field)
{
    Fields.push_back(Field);
    FieldCount++;
} */

//---------------------------------------------------------------------------
//
void TStorageOraProc::addTable(const TOraProcTable& Table)
{
    Tables.push_back(Table);
    TableCount++;
}

//---------------------------------------------------------------------------
// ¬озвращает наименование активного источника/приемника данных
AnsiString TStorageOraProc::getTable()
{
    //return Procedure;
    if (!eot()) {
        if (Tables[TableIndex].Table != "") {   // ≈сли задана таблица, то возвращаем им€ таблицы
            return Tables[TableIndex].Table;
        } else {
            return Tables[TableIndex].Procedure;
        }
    }
}

//---------------------------------------------------------------------------
//  TStorageOraSql
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
void TStorageOraSql::openTable(bool ReadOnly)
{
    this->ReadOnly = ReadOnly;

    if (Tables.size()>0)
        openConnection(Tables[TableIndex].Server, Tables[TableIndex].Username, Tables[TableIndex].Password);
    else
        throw Exception("Storage is not specified.");

    prepareQuery();
    if (ReadOnly == false) {
        truncateTable(&Tables[TableIndex]);
    }

    AnsiString Sql = Tables[TableIndex].Sql;
    AnsiString SqlText;

    if (Sql != "") {
        if (!FileExists(Sql)) {
            throw Exception("File not found " + Sql + ".");
        }
        TStringList* pStringList;
        pStringList = new TStringList();
        pStringList->Clear();
        pStringList->LoadFromFile(Sql);
        SqlText = pStringList->Text;
        pStringList->Free();
    } else {
        SqlText =  "select * from " + Tables[TableIndex].Table + (ReadOnly == true? "": " for update");
    }

    dbQuery->SQL->Clear();
    dbQuery->SQL->Add(SqlText);

    try {
        dbQuery->Open();
        RecordCount = dbQuery->RecordCount;
    } catch (...) {
        throw Exception("Can't to open query.");
    }


    // ¬озможно следует добавить флаг - признак назначени€
    // источник или приемник.
    // ≈сли источник, то присваивать значение FieldsCount = dbQuery->Fields->Count
    //FieldCount = dbQuery->Fields->Count;
    // ¬озможно сделать цикл по OraQuery->Fields и заполн€ть Fields
    // если  Fields.size() == 0
    FieldCount = Fields.size();
    if (FieldCount == 0) {
        FieldCount = dbQuery->Fields->Count;

        TStringList *fieldList = new TStringList();
        dbQuery->Fields->GetFieldNames(fieldList);

        for (int i = 0; i < dbQuery->Fields->Count; i++) {
            TOraField* field = addField();      // ¬озможно заменить на
            field->name = LowerCase(fieldList->Strings[i]);
            field->active = true;
            field->enable = true;
            field->name_src = field->name;
        }
        fieldList->Free();
    }

    //dbSession->StartTransaction();
    Active = true;
}

//---------------------------------------------------------------------------
// ¬озвращает признак наличи€ пол€ с именем
//bool TStorageOraSql::FindField(AnsiString fieldName)
//{
//    return dbQuery->Fields->FindField(fieldName);
//}

//---------------------------------------------------------------------------
// ¬озвращает значение пол€
//Variant TStorageOraSql::Get(AnsiString Field)
Variant TStorageOraSql::getFieldValue(TStorageField* Field)
{
    // ¬озможно ѕеределать?????? Field[FieldIndex].name
    //return dbQuery->FieldByName(Field)->Value;
   /* if (!Field->required) {
        //bool aaa =  dbQuery->Fields->FindField(Field->name_src);
        //bool aaa1 =  dbQuery->Fields->FindField("adfa");
        //bool aaa2 =  dbQuery->Fields->FindField(Field->name_src);
        if (!dbQuery->Fields->FindField(Field->name_src))
            return "";
    }   */
    return dbQuery->FieldByName(Field->name_src)->Value;

}


//---------------------------------------------------------------------------
// ”станавливает значение активного пол€
void TStorageOraSql::setFieldValue(Variant Value)
{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (Fields[FieldIndex]->active && Fields[FieldIndex]->enable) {
        //dbQuery->FieldByName("acct_id")->Value = Value;   // 2016-07-28
        dbQuery->FieldByName(Fields[FieldIndex]->name)->Value = Value;   // 2016-07-28
    }
}

//---------------------------------------------------------------------------
// ƒобавл€ет новую запись в таблицу
void TStorageOraSql::append()
{
    dbQuery->Append();
    TStorage::append();
}

//---------------------------------------------------------------------------
// ‘иксирует изменени€
void TStorageOraSql::commit()
{
    if (ReadOnly) {
        throw Exception("Can't commit the storage because it is read-only.");
       //return;
    }

    if (dbQuery->Modified) {
        try {
            dbQuery->Post();  // 2016-08-01
            //dbQuery->CommitUpdates();
            //dbSession->ApplyUpdates();
            dbSession->Commit();  // 2016-07-28
            Modified = true;
            RecordCount = dbQuery->RecordCount;
        } catch (Exception &e) {
            //dbSession->Rollback();
            throw Exception(e.Message);
        }
    }
    FieldIndex = 0;
}

//---------------------------------------------------------------------------
//
void TStorageOraSql::post()
{
    //dbQuery->ExecSQL();
    FieldIndex = 0;
}

//---------------------------------------------------------------------------
// ¬озвращает true если достигнут конец таблицы
bool TStorageOraSql::eor()
{
    if (dbQuery != NULL) {
        int R = dbQuery->RecordCount;
        int aa = dbQuery->RecNo;
        return dbQuery->Eof;
    } else {
        return true;
    }
}

//---------------------------------------------------------------------------
// ѕереходит к следующей записи таблицы
void TStorageOraSql::nextRecord()
{
    //int k = pTable->RecNo;
    dbQuery->Next();
    TStorage::nextRecord();
}

//---------------------------------------------------------------------------
// ¬озвращает соответствующее активному полю - им€ пол€ в таблице-источнике
/*AnsiString TStorageOraSql::GetSrcField()
{
    return Fields[FieldIndex]->name_src;
}*/

//---------------------------------------------------------------------------
//
/*bool TStorageOraSql::IsActiveField()
{
    return Fields[FieldIndex]->active && Fields[FieldIndex]->enable;
} */

//---------------------------------------------------------------------------
//
TOraField* TStorageOraSql::addField()
{
    TOraField* Field = new TOraField();
    Fields.push_back(Field);
    FieldCount++;
    return Field;

}

//---------------------------------------------------------------------------
//
void TStorageOraSql::addTable(const TOraSqlTable &Table)
{
    Tables.push_back(Table);
    TableCount++;
}

//---------------------------------------------------------------------------
// ¬озвращает наименование активного источника/приемника данных
AnsiString TStorageOraSql::getTable()
{
    if (!eot()) {
        if (Tables[TableIndex].Sql != "") {
            return Tables[TableIndex].Sql;
        } else {
            return Tables[TableIndex].Table;
        }
    } else {
        throw Exception("End of table is reached.");
    }
}
