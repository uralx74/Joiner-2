#include "pch.h"
#pragma hdrstop

#include "Storage.h"

#define NDEBUG
#include <assert.h>


void TStorage::registerFactory(String name, TableFactoryBase* factory)
{
    tableFactories[name] = factory;
}


void TStorage::loadTables(StorageParameters& storageParameters)
{
    //storageParameters["file"];


    if (storageParameters["file"] != "") {
        // �������� ������� ��� �������� ������ �� ���������� ����
        TableFactoryBase* tableFactory = tableFactories[storageParameters["type"]];
        if (tableFactory == NULL) {
            throw Exception("Storage type \"" + storageParameters["type"] + "\" doesn't registered.");
        }

        TSearchRec SearchRec;
        FindFirst(storageParameters["file"], faAnyFile, SearchRec);

        if (SearchRec.Name != "") {
            //AnsiString FilePath = ExtractFilePath(Table.File);
            do {
                TStorageTable* table = tableFactory->newTable();

                //table->file = FilePath + SearchRec.Name;
                Tables.push_back(table);
            } while ( FindNext(SearchRec) == 0);
        } else {
            TStorageTable* table = tableFactory->newTable();
            Tables.push_back(table);

        }
        FindClose(SearchRec);
    }

    TableCount = Tables.size();
}


//
/*
void TStorage::loadTables(StorageParameters* storageParameters)
{
    if (storageParameters->file != "") {
        // �������� ������� ��� �������� ������ �� ���������� ����
        TableFactory* tableFactory = tableFactories[storageParameters->type];
        if (tableFactory == NULL) {
            throw Exception("Storage type \"" + storageParameters->type + "\" doesn't registered.");
        }

        TSearchRec SearchRec;
        FindFirst(storageParameters->file, faAnyFile, SearchRec);

        if (SearchRec.Name != "") {
            //AnsiString FilePath = ExtractFilePath(Table.File);
            do {
                TStorageTable* table = tableFactory->newTable();

                //table->file = FilePath + SearchRec.Name;
                Tables.push_back(table);
            } while ( FindNext(SearchRec) == 0);
        } else {
            TStorageTable* table = tableFactory->newTable();
            Tables.push_back(table);

        }
        FindClose(SearchRec);
    }

    TableCount = Tables.size();
}  */








//---------------------------------------------------------------------------
// �������� ���������
void TStorage::closeTable()
{
   /* std::vector<String>::size_type n = Fields.size();
    for (int i = 0; i < n; i++) {   // �������� �����
        delete Fields[i];
    }
    Fields.clear(); */

    (*curTable)->close();

    FieldIndex = 0;
    FieldCount = 0;
    RecordIndex = 0;
    RecordCount = 0;
    Active = false;
}

//---------------------------------------------------------------------------
// ���������� ������ ������
void TStorage::append()
{
    RecordCount++;
    FieldIndex = 0;
}

//---------------------------------------------------------------------------
// �����
void TStorage::rollback()
{
    RecordCount--;
    FieldIndex = 0;
}


//---------------------------------------------------------------------------
// �������� �������� �� ���� ���������������� � ����� � ���������
bool TStorage::isLinkedField()
{
    return (*curTable)->isLinkedField();
}

//---------------------------------------------------------------------------
// ����������� ���� � ������ � ���������
// ���� ���� �������, ������������� ���� linked = true
bool TStorage::linkSource(TStorage* Storage)
{
    (*curTable)->linkSource(*Storage->curTable);
/*    // Storage is Source storage
    int n = Fields.size();
    for (int i = 0; i < n; i++) {
        //String sss = this->Fields[i]->name_src;
        //bool k = Storage->FindField(this->Fields[i]->name_src);
        this->Fields[i]->linked = Storage->findField(Fields[i]->name_src) != NULL;
    }     */
}

//---------------------------------------------------------------------------
// ����� ���� �� �����
TStorageField* TStorage::findField(AnsiString fieldName)
{
//int tt = FieldCount;
    //int n = Fields.size();
    //for (int i = 0; i < n; i++) {
    //    if (Fields[i]->name == fieldName) {
     //       return Fields[i];
    //    }
    //}
    //return NULL;

    return (*curTable)->findField(fieldName);
}

/*//---------------------------------------------------------------------------
//
bool TStorage::FindField(AnsiString fieldName)
{
//int tt = FieldCount;
    int n = Fields.size();
    for (int i = 0; i < n; i++) {
        if (Fields[i]->name == fieldName) {
            return true;
        }
    }
    return false;
} */


//---------------------------------------------------------------------------
// ���������� ��������������� ��������� ���� - ��� ���� � �������-���������
TStorageField* TStorage::getField()
{
    //return Fields[FieldIndex];
    return (*curTable)->getField();
    //return Fields[FieldIndex]->name_src;
}

//---------------------------------------------------------------------------
// ���������� true - ���� ���� ���� �������� ������
bool TStorage::isActiveField()
{
    return (*curTable)->isActiveField();
    //return Fields[FieldIndex]->active && Fields[FieldIndex]->enable;
}


//---------------------------------------------------------------------------
// ������ templateStorage � �������� ��������� ���������
// ������������ ����� ��������� ������������ ��������
TStorage::setTemplate(TStorage* storage, bool deleteAfterUse)
{
    templateStorage = storage;
    delTemplateStorage = deleteAfterUse;
}


//---------------------------------------------------------------------------
// ������������ 2
TStorage::copyFieldsFrom(TStorage* storage)
{
    // �����������
}

//
TStorage::copyFieldsToDbf(TStorage* storage)
{
    // �����������
    Modified = true;
}

//
TStorage::copyFieldsToExcel(TStorage* storage)
{
    // �����������
    bool Modified = true;
}



//---------------------------------------------------------------------------
// ������ ����������� �����
// ���������� ������ ��� ���������� ����������� ����!
void TStorage::fullCopyFields(TStorage* src, TStorage* dst)
{
    /*// �������� �� src � dst
    int n = src->Fields.size();
    for (int i = 0; i < n; i++) {
        dst->addField(src->Fields[i]);
    }*/
}




//---------------------------------------------------------------------------
// TStorage
// The interface class
//---------------------------------------------------------------------------

TStorage::TStorage()
{
    TableIndex = 0;
    TableCount = 0;
    FieldCount = 0;
    FieldIndex = 0;
    RecordCount = 0;
    RecordIndex = 0;
    Active = false;
    ReadOnly = true;
    Modified = false;
    templateStorage = NULL;
    delTemplateStorage = false;
}

//---------------------------------------------------------------------------
// ���������� true ���� ��������� ����� ������ ������
bool TStorage::eot()
{
    return TableIndex >= TableCount;
}

//---------------------------------------------------------------------------
// ���������� true ���� ��������� ����� ������ �����
bool TStorage::eof()
{
    return FieldIndex  >= FieldCount;
}

//---------------------------------------------------------------------------
// ��������� � ��������� �������
void TStorage::nextTable()
{
    ////////////////////// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if (Active) {
        this->closeTable();
    }

    if (!eot()) {
        TableIndex ++;
        FieldIndex = 0;
        RecordIndex = 0;    // 2016-08-08
    } else {
        throw Exception("");
    }

/*    if (!eot()) {
        this->openTable();
    }     */
}

//---------------------------------------------------------------------------
// ��������� � ��������� ������ �������
void TStorage::nextRecord()
{
    if (!eor()) {
        FieldIndex = 0;
        RecordIndex++;
    }
}

//---------------------------------------------------------------------------
// ��������� � ���������� ���� �������
void TStorage::nextField()
{
    if (!eof()) {
        FieldIndex++;
    }
}

//---------------------------------------------------------------------------
//
AnsiString TStorage::getTableStage()
{
    return IntToStr(TableIndex+1) + "/" + IntToStr(TableCount);
}

//---------------------------------------------------------------------------
//
AnsiString TStorage::getRecordStage()
{
    return IntToStr(RecordIndex+1) + "/" + IntToStr(RecordCount);
}





