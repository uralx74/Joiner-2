#include "pch.h"
#pragma hdrstop

#include "StorageTable.h"

/*
 *
 *
 *
 *
 */
TStorageTable::TStorageTable() :
    _truncate(false),
    _recordCount(0),
    _readOnly(true),
    _modified(false)
{
    Fields.reserve(10);
}

/*
*/
void TStorageTable::open(bool readOnly)
{
    this->nativeOpenTable(readOnly);

    curField = Fields.begin();

    _active = true;
    _readOnly = readOnly;
}

/*
*/
void TStorageTable::commit()
{
    if (_readOnly) {
        throw Exception("Can't commit the storage because it is read-only.");
    } else {
        try {
            this->nativeCommit();
            _modified = true;
        } catch (Exception &e) {
            _modified = false;
        }
    }
}

/*
*/
bool TStorageTable::eof()
{
    return curField == Fields.end();
}

/*
*/
void TStorageTable::setRecordCount(int recordCount)
{
    _recordCount = recordCount;
}

/*
*/
void TStorageTable::append()
{
    curField = Fields.begin();
    this->nativeAppend();
    _recordCount++;
}

/* Закрывает таблицу, удаляет список полей
*/
void TStorageTable::closeTable()
{
    this->nativeCloseTable();
    
    for (FieldsIterator itField = Fields.begin(); itField != Fields.end(); itField++) {
        delete *itField;
    }

    Fields.clear();
    curField = Fields.begin();

    _recordCount = 0;
    _active = false;
    _modified = false;
}

void TStorageTable::nextRecord()
{
    curField = Fields.begin();
    this->nativeNextRecord();

    //if (!eor()) {
    //}
}

bool TStorageTable::eor()
{
    return this->nativeEor();
}


Variant TStorageTable::getFieldValue(TStorageField* Field) const
{
    return this->nativeGetFieldValue(Field);
}

//---------------------------------------------------------------------------
// Устанавливает значение активного поля
void TStorageTable::setFieldValue(Variant Value)
{
    if (isActiveField()) {
        this->nativeSetFieldValue(*curField, Value);
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
    int n = Fields.size();
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

// Этот метод возможно удалить!
/*TStorageField* TStorageTable::addField(TStorageField* Field)
{
    Fields.push_back(Field);// возможно нужно приводить к  static_cast<TDbaseField*>
    FieldCount++;
} */

// Создает и добавляет field
/*TStorageField* TStorageTable::addField()
{
    TStorageField* field = this->createField();
    Fields.push_back(field);    // возможно нужно приводить static_cast<TDbaseField*>
    fieldCount++;
    return field;
}*/

void TStorageTable::nextField()
{
    if (!eof()) {
        curField++;
        //FieldIndex++;
    }
}


// Создает и добавляет field
TStorageField* TStorageTable::addField(TStorageField* field)
{
    //TStorageField* field = this->createField();
    //TStorageField* field = this->createField(oleXml, node);
    Fields.push_back(field);    // возможно нужно приводить static_cast<TDbaseField*>
    return field;
}



/*TStorageField* TStorageTable::newField()
{
    Fields.push_back(Field);// возможно нужно приводить к  static_cast<TDbaseField*>
    FieldCount++;
}*/






