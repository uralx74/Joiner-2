#include "pch.h"
#pragma hdrstop

#include "StorageTable.h"


void TStorageTable::open(bool ReadOnly)
{
    this->openTable(ReadOnly);
    fieldCount = Fields.size();

    active = true;
}

//---------------------------------------------------------------------------
TStorageTable::TStorageTable()
{
    truncate = false;

    fieldCount = 0;
    recordCount = 0;
    readOnly = false;
    modified = false;
}

void TStorageTable::setRecordCount(unsigned int _recordCount)
{
    recordCount = _recordCount;
}

void TStorageTable::appendRecord()
{
    append();
    recordCount++;
}


void TStorageTable::close()
{
    //std::vector<String>::size_type n = Fields.size();
    //for (int i = 0; i < n; i++) {   // �������� �����
    //    delete Fields[i];
    //}

    this->closeTable();
    
    for (FieldsIterator itField = Fields.begin(); itField != Fields.end(); itField++) {
        delete *itField;
    }

    Fields.clear();

    fieldCount = 0;
    recordCount = 0;

    active = false;
}

void TStorageTable::nextRec()
{
    //if (!eor()) {
        curField = Fields.begin();
        nextRecord();
    //}

}


//---------------------------------------------------------------------------
// ������������� �������� ��������� ����
void TStorageTable::setValue(Variant Value)
{
    if (isActiveField()) {
        this->setFieldValue(*curField, Value);
        //pTable->FieldByName(curField->name)->Value = Value;
    }
}


bool TStorageTable::isLinkedField()
{
    return (*curField)->linked;
}

bool TStorageTable::isActiveField()
{
    return (*curField)->active && (*curField)->enable;
}

bool TStorageTable::linkSource(TStorageTable* SourceTable)
{
    for (FieldsIterator itField = Fields.begin(); itField != Fields.end(); itField++) {
        (*itField)->linked = SourceTable->findField((*itField)->name_src) != NULL;
    }
}

TStorageField* TStorageTable::findField(AnsiString fieldName)
{
    //int n = Fields.size();
    for (FieldsIterator itField = Fields.begin(); itField != Fields.end(); itField++) {
        if ((*itField)->name == fieldName) {
            return *itField;
        }
    }
    return NULL;
}

TStorageField* TStorageTable::getField()
{
    return *curField;
}

// ���� ����� �������� �������!
/*TStorageField* TStorageTable::addField(TStorageField* Field)
{
    Fields.push_back(Field);// �������� ����� ��������� �  static_cast<TDbaseField*>
    FieldCount++;
} */

// ������� � ��������� field
TStorageField* TStorageTable::addField()
{
    TStorageField* field = this->createField();
    Fields.push_back(field);    // �������� ����� ��������� static_cast<TDbaseField*>
    fieldCount++;
    return field;
}


/*TStorageField* TStorageTable::newField()
{
    Fields.push_back(Field);// �������� ����� ��������� �  static_cast<TDbaseField*>
    FieldCount++;
}*/






