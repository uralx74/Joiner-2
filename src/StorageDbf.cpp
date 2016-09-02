#include "pch.h"
#pragma hdrstop

#include "StorageDbf.h"


/*TStorageTable* DbfFactory::newTable()
{
    return new TStorageDbase();
    //return new TStorageTable();
} */


//---------------------------------------------------------------------------
// ��������� ����� ���� � ������ ����� ()
// �������� ���������� �� AddField(TDbaseField* Field)
/*TStorageField* TStorageDbase::addField(TStorageField* Field)
{
    TStorageField* newField = new TDbaseField();

    //String test2 = (static_cast<TDbaseField*>(newField))->type;

    // ����������? ���������� ����� ����� ����������� ������ ���������
    *(static_cast<TDbaseField*>(newField)) = *(static_cast<TDbaseField*>(Field));

    this->Fields.push_back(newField);// �������� ����� ��������� �  static_cast<TDbaseField*>
    FieldCount++;
    return newField;
}  */

//---------------------------------------------------------------------------
void TStorageDbase::loadFieldDefs()
{
    if (!pTable) {
        return;
    }

    int n = pTable->DbfFieldDefs->Count;

    for(int i=0; i < pTable->DbfFieldDefs->Count; i++) {
        TDbfFieldDef* fieldDef = pTable->DbfFieldDefs->Items[i];
        TDbaseField* dbaseField = static_cast<TDbaseField*>(this->createField());

        dbaseField->type = fieldDef->NativeFieldType;
        dbaseField->name = fieldDef->FieldName;
        dbaseField->length = fieldDef->Size;
        dbaseField->decimals = fieldDef->Precision;
        dbaseField->active = true;
        dbaseField->enable = true;
        dbaseField->name_src = fieldDef->FieldName;
    }
}

//---------------------------------------------------------------------------
// ����������� ����� �� ���������
// ������������ ������� ���������������
TStorageDbase::copyFieldsFrom(TStorage* storage)
{
    // � ����������� �� ���� storage (������������ �� TStorage), ���������� ����� CopyFieldsToDbf
    //storage->copyFieldsToDbf(this);
}


//---------------------------------------------------------------------------
// ������ ����������� ����� �� DBF � DBF
TStorageDbase::copyFieldsToDbf(TStorage* storage)
{
    //TStorage::fullCopyFields(this, storage);
}


//---------------------------------------------------------------------------
//  TStorageDbase
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
TStorageDbase::TStorageDbase() :
    pTable(NULL)
{
    //pTable = NULL;
}

/*TStorageDbase::TStorageDbase(String fileName)
{
    //TStorageDbase* storageDbaseStructure = new TStorageDbase();
    TDbaseTable table;
    table.File = fileName;
    //table.File = ExpandFileName(fileName);
    this->addTable(table);
} */



TStorageField* TStorageDbase::createField()
{
    return new TDbaseField();
}




//---------------------------------------------------------------------------
// ��������� ����� ���� � ������ �����
//TDbaseField* TStorageDbase::addField()
//{
    /*TStorageField* Field = new TDbaseField();
    Fields.push_back(Field);
    FieldCount++;
    return Field;*/

//    TStorageField* Field = new TDbaseField();
    //Fields.push_back(dynamic_cast<TStorageField*>(Field));
//    Fields.push_back((TStorageField*)(Field));

    //TStorageField* pField = (TStorageField*)Field;
    //TStorageField* tttt = new TDbaseField();

    //String name1 = typeid(*tttt).name();
    //String name2 = typeid(*pField).name();


//    FieldCount++;
//    return (TDbaseField*)Field;
//}

/*
//---------------------------------------------------------------------------
// ��������� ����� ���� � ������ �����
void TStorageDbase::AddField(const TDbaseField* Field)
{
    Fields.push_back((TStorageField*)Field);
    FieldCount++;
}*/

//---------------------------------------------------------------------------
// ��������� ������� � ������ (��� �����, ����� ���� ������ ������)
/*void TStorageDbase::addTable(const TDbaseTable& Table)
{
    //TDbaseTable* Table = new TDbaseTable();

    TSearchRec SearchRec;
    FindFirst(Table.File, faAnyFile, SearchRec);

    if (SearchRec.Name != "") {
        AnsiString FilePath = ExtractFilePath(Table.File);
        do {
            TDbaseTable NewTable;
            NewTable.File = FilePath + SearchRec.Name;
            Tables.push_back(NewTable);
            TableCount++;
        } while ( FindNext(SearchRec) == 0);
    } else {
        Tables.push_back(Table);
        TableCount++;
    }
    FindClose(SearchRec);


    //Tables.push_back(Table);
    //TableCount++;
}*/

//---------------------------------------------------------------------------
// �������� �������
void TStorageDbase::openTable(bool ReadOnly)
{
    //this->ReadOnly = ReadOnly;
    //if (Tables.size() <= TableIndex) {
    //    throw Exception("The table with index \"" + IntToStr(TableIndex) + "\" doesn't exists.");
    //}

    if (ReadOnly) {
        pTable = new TDbf(NULL);

        pTable->TableName = File;
        pTable->Exclusive = true;

        if (FileExists(File)) {
            try {
                pTable->Open();
                //RecordCount = pTable->RecordCount;
            } catch (...) {
                throw Exception("Can't to open file  " + File + ".");
            }
        } else {
            throw Exception("File not found " + File + ".");
        }
    } else {
        create();
    }

    //return true;
}

//---------------------------------------------------------------------------
//
void TStorageDbase::create()
{
    pTable = new TDbf(NULL);

    //pTableDst->TableLevel = 7; // required for AutoInc field
    pTable->TableLevel = 4;
    pTable->LanguageID = DbfLangId_RUS_866;

    pTable->TableName = ExtractFileName(File);
    pTable->FilePathFull = ExtractFilePath(File);

    // ������� ����������� ����� ������� �� ����������
    TDbfFieldDefs* TempFieldDefs;
    TempFieldDefs = new TDbfFieldDefs(NULL);

    if (TempFieldDefs == NULL) {
        throw Exception("Can't create storage.");
    }

 /*
    // ����������� ����� �� �������, ���� �� �����
    if (templateStorage) {
        templateStorage->openTable();
        templateStorage->loadFieldDefs();

        this->copyFieldsFrom(templateStorage);

        templateStorage->closeTable();
        if (delTemplateStorage) {
            delete templateStorage;
        }
    }




    for(std::vector<TStorageField*>::iterator it = Fields.begin(); it < Fields.end(); it++ ) {

        TDbaseField* Field = (TDbaseField*)*it;

        if (!Field->enable)
            continue;
        TDbfFieldDef* TempFieldDef = TempFieldDefs->AddFieldDef();
        TempFieldDef->FieldName = Field->name;
        //TempFieldDef->Required = true;
        //TempFieldDef->FieldType = Field->type;    // Use FieldType if Field->Type is TFieldType else use NativeFieldType
        TempFieldDef->NativeFieldType = Field->type;
        TempFieldDef->Size = Field->length;
        TempFieldDef->Precision = Field->decimals;
    }

    if (TempFieldDefs->Count == 0) {
        delete pTable;
        return;
    }

    pTable->CreateTableEx(TempFieldDefs);
    pTable->Exclusive = true;
    try {
        pTable->Open();
    } catch (...) {
    }

    FieldCount = Fields.size();   */
}


//---------------------------------------------------------------------------
//
std::vector<TStorageField> TStorageDbase::getFieldDefs()
{
    std::vector<TStorageField> Result;
}

//---------------------------------------------------------------------------
//
void TStorageDbase::setFieldDefs(std::vector<TStorageField>)
{
}

//---------------------------------------------------------------------------
// ���������� �������� ����
Variant TStorageDbase::getFieldValue(TStorageField* Field)
{
    // �������� ����������?????? Field[FieldIndex].name
    //if ()
    try {
        return pTable->FieldByName(Field->name_src)->Value;
    } catch (...) {
        throw Exception("Field \"" + Field->name_src + "\" not found.");
    }
}

//---------------------------------------------------------------------------
// ������������� �������� ��������� ����
void TStorageDbase::setFieldValue(TStorageField* field, Variant value)
{
    if (/*!VarIsEmpty(Value) &&*/ !VarIsNull(value) && (AnsiString)value != "") {
        //if (/*!VarIsEmpty(Value) && !VarIsNull(Value) &&*/ (AnsiString)Value != "")
                // (AnsiString)Value != "" - ��� �������� ��������,
                // ��� ��� � DBF ��������� �������� NULL
            pTable->FieldByName(field->name)->Value = value;
    }
}

//---------------------------------------------------------------------------
// ��������� ���������
void TStorageDbase::commit()
{
    /*if (ReadOnly) {
        throw Exception("Can't commit the storage because it is read-only.");
    }*/

    if (pTable->Modified) {
        try {
            pTable->Post();
            modified = true;
            //recordCount = pTable->RecordCount;
        } catch (...) {
            throw;
        }
    }
    //pTable->Post();
}

//---------------------------------------------------------------------------
// ��������� ����� ������ � �������
void TStorageDbase::append()
{
    pTable->Append();
}

//---------------------------------------------------------------------------
// ��������� �������
void TStorageDbase::closeTable()
{
    if (pTable != NULL) {
        pTable->Close();
        delete pTable;
        pTable = NULL;
    }
}

//---------------------------------------------------------------------------
// ���������� true ���� ��������� ����� �������
bool TStorageDbase::eor()
{
    return pTable->Eof;
}

//---------------------------------------------------------------------------
// ���������� true ���� ���������� ��������� ������
//bool TStorageDbase::Eof()
//{
//    return FieldIndex + 1 == Fields.size();
//}

//---------------------------------------------------------------------------
// ��������� � ��������� �������
/*void TStorageDbase::nextTable()
{
    TStorage::nextTable();

    if (pTable != NULL) {
        closeTable();
    }

    if (!eot()) {
        openTable();
    }
}*/

//---------------------------------------------------------------------------
// ��������� � ��������� ������ �������
void TStorageDbase::nextRecord()
{
    try {
        pTable->Next();
    } catch (...) {
    }
}

//---------------------------------------------------------------------------
// ���������� ������������ ��������� ���������/��������� ������
AnsiString TStorageDbase::getTable()
{
    return File;
}

