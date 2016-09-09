#include "pch.h"
#pragma hdrstop

#include "Storage.h"

#define NDEBUG
#include <assert.h>


//---------------------------------------------------------------------------
// TStorage
// The interface class
//---------------------------------------------------------------------------

TStorage::TStorage() :
    TableIndex(0),
    TableCount(0),
    FieldCount(0),
    FieldIndex(0),
    RecordCount(0),
    RecordIndex(0),
    Active(false),
    ReadOnly(true),
    Modified(false),
    templateStorage(NULL),
    delTemplateStorage(false)
{
    Tables.reserve(10);
    curTable = Tables.begin(); // ��� ��������� ������� Tables ������ ���������
}

TStorage::~TStorage()
{
}

int TStorage::getRecordCount()
{
    //return RecordCount;
    return (*curTable)->nativeGetRecordCount();
}

void TStorage::registerFactory(String name, TableFactoryBase* factory)
{
    tableFactories[name] = factory;
}

bool TStorage::factoryExists(const String& name) const
{
    return tableFactories.find(name) != tableFactories.end();
}

/* ��������� ��������� �� �����-xml
 * node - ��������� �������
 * ������������� � AddTable
 */
void TStorage::loadStorage(const OleXml& oleXml, Variant node, bool readOnly)
{
    //AnsiString nodeName = oleXml.GetNodeName(node);
    if (node.IsEmpty()) {
        return;
    }

    String storageType = oleXml.GetAttributeValue(node, "type");

    // ���� ��� ��������� ���������������
    if (tableFactories.find(storageType) != tableFactories.end()) {
        AnsiString attrFile = oleXml.GetAttributeValue(node, "file");

        TableFactoryBase* tableFactory = tableFactories[storageType];
        TStorageTable* table = tableFactory->newTable(oleXml, node);

        Tables.push_back(table);
    }
}

/* �������� ���������
*/
void TStorage::closeTable()
{
   /* std::vector<String>::size_type n = Fields.size();
    for (int i = 0; i < n; i++) {   // �������� �����
        delete Fields[i];
    }
    Fields.clear(); */

    (*curTable)->closeTable();

    FieldIndex = 0;
    FieldCount = 0;
    RecordIndex = 0;
    RecordCount = 0;
    Active = false;
}

AnsiString TStorage::getTable()
{
    return (*curTable)->getTableName();
}

/*
*/
bool TStorage::eor()
{
    return (*curTable)->eor();
}

//---------------------------------------------------------------------------
// ���������� true ���� ��������� ����� ������ �����
bool TStorage::eof()
{
    return (*curTable)->eof();
}

void TStorage::commit()
{
    (*curTable)->commit();
}

Variant TStorage::getFieldValue(TStorageField* Field) const
{
    return (*curTable)->getFieldValue(Field);
}

void TStorage::setFieldValue(Variant Value)
{
    (*curTable)->setFieldValue(Value);
}

//---------------------------------------------------------------------------
// ���������� ������ ������
void TStorage::append()
{
    //RecordCount++;
    //FieldIndex = 0;
    (*curTable)->append();
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


//---------------------------------------------------------------------------
// ���������� true - ���� ���� ���� �������� ������
bool TStorage::isActiveField()
{
    return (*curTable)->isActiveField();
    //return Fields[FieldIndex]->active && Fields[FieldIndex]->enable;
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
// ���������� true ���� ��������� ����� ������ ������
bool TStorage::eot()
{
    return curTable == Tables.end();
}


void TStorage::openTable(bool ReadOnly)
{
    (*curTable)->open(ReadOnly);
};

//---------------------------------------------------------------------------
// ��������� � ��������� �������
void TStorage::nextTable()
{
    ////////////////////// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    if (Active) {
        this->closeTable();
    }

    if (!eot()) {

        curTable++;
        //TableIndex ++;
        //FieldIndex = 0;
        //RecordIndex = 0;    // 2016-08-08
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
    (*curTable)->nextRecord();
}


//---------------------------------------------------------------------------
// ��������� � ���������� ���� �������
void TStorage::nextField()
{
    (*curTable)->nextField();
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





