#include "pch.h"
#pragma hdrstop

#include "StorageOra.h"


//---------------------------------------------------------------------------
// TStorageOra
//---------------------------------------------------------------------------
// �������� ��������� ������ � TOraSession � TOraQuery
//  try {
//      OraSession->StartTransaction();
//      OraQuery->Add("sql text"); ���  OraQuery->CreateProcCall("sql text",0);
//      OraQuery->FieldByName("field")->Value = ... ��� OraQuery->ParamByName("field")->Value = ...
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
        // ��������� ��������
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
        CurrencyString = "�.";
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
// ������� �������
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
// �������� ������������ OraQuery
void TStorageOraProc::openTable(bool ReadOnly)
{
    this->ReadOnly = ReadOnly;

    /*�������� ��������������� TStorageOra::Open*/

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
// ������������� �������� ��������� ����
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
// ���������� ��������������� ��������� ���� - ��� ���� � �������-���������
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
// ���������� ������������ ��������� ���������/��������� ������
AnsiString TStorageOraProc::getTable()
{
    //return Procedure;
    if (!eot()) {
        if (Tables[TableIndex].Table != "") {   // ���� ������ �������, �� ���������� ��� �������
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


    // �������� ������� �������� ���� - ������� ����������
    // �������� ��� ��������.
    // ���� ��������, �� ����������� �������� FieldsCount = dbQuery->Fields->Count
    //FieldCount = dbQuery->Fields->Count;
    // �������� ������� ���� �� OraQuery->Fields � ��������� Fields
    // ����  Fields.size() == 0
    FieldCount = Fields.size();
    if (FieldCount == 0) {
        FieldCount = dbQuery->Fields->Count;

        TStringList *fieldList = new TStringList();
        dbQuery->Fields->GetFieldNames(fieldList);

        for (int i = 0; i < dbQuery->Fields->Count; i++) {
            TOraField* field = addField();      // �������� �������� ��
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
// ���������� ������� ������� ���� � ������
//bool TStorageOraSql::FindField(AnsiString fieldName)
//{
//    return dbQuery->Fields->FindField(fieldName);
//}

//---------------------------------------------------------------------------
// ���������� �������� ����
//Variant TStorageOraSql::Get(AnsiString Field)
Variant TStorageOraSql::getFieldValue(TStorageField* Field)
{
    // �������� ����������?????? Field[FieldIndex].name
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
// ������������� �������� ��������� ����
void TStorageOraSql::setFieldValue(Variant Value)
{
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (Fields[FieldIndex]->active && Fields[FieldIndex]->enable) {
        //dbQuery->FieldByName("acct_id")->Value = Value;   // 2016-07-28
        dbQuery->FieldByName(Fields[FieldIndex]->name)->Value = Value;   // 2016-07-28
    }
}

//---------------------------------------------------------------------------
// ��������� ����� ������ � �������
void TStorageOraSql::append()
{
    dbQuery->Append();
    TStorage::append();
}

//---------------------------------------------------------------------------
// ��������� ���������
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
// ���������� true ���� ��������� ����� �������
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
// ��������� � ��������� ������ �������
void TStorageOraSql::nextRecord()
{
    //int k = pTable->RecNo;
    dbQuery->Next();
    TStorage::nextRecord();
}

//---------------------------------------------------------------------------
// ���������� ��������������� ��������� ���� - ��� ���� � �������-���������
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
// ���������� ������������ ��������� ���������/��������� ������
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
