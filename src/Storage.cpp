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
    curTable = Tables.begin(); // при изменении размера Tables менять указатель
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

/* Загружает хранилище из файла-xml
 * node - указывает таблицу
 * переименовать в AddTable
 */
void TStorage::loadStorage(const OleXml& oleXml, Variant node, bool readOnly)
{
    //AnsiString nodeName = oleXml.GetNodeName(node);
    if (node.IsEmpty()) {
        return;
    }

    String storageType = oleXml.GetAttributeValue(node, "type");

    // Если тип хранилища зарегистрирован
    if (tableFactories.find(storageType) != tableFactories.end()) {
        AnsiString attrFile = oleXml.GetAttributeValue(node, "file");

        TableFactoryBase* tableFactory = tableFactories[storageType];
        TStorageTable* table = tableFactory->newTable(oleXml, node);

        Tables.push_back(table);
    }
}

/* Закрытие хранилища
*/
void TStorage::closeTable()
{
   /* std::vector<String>::size_type n = Fields.size();
    for (int i = 0; i < n; i++) {   // Удаление полей
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
// Возвращает true если достигнут конец списка полей
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
// Добавление пустой записи
void TStorage::append()
{
    //RecordCount++;
    //FieldIndex = 0;
    (*curTable)->append();
}

//---------------------------------------------------------------------------
// Откат
void TStorage::rollback()
{
    RecordCount--;
    FieldIndex = 0;
}


//---------------------------------------------------------------------------
// Проверка является ли поле ассоциированными с полем в источнике
bool TStorage::isLinkedField()
{
    return (*curTable)->isLinkedField();
}

//---------------------------------------------------------------------------
// Ассоциирует поля с полями в источнике
// Если поле найдено, устанавливает флаг linked = true
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
// Поиск поля по имени
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
// Возвращает true - если поле поле подлежит записи
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
// Возвращает соответствующее активному полю - имя поля в таблице-источнике
TStorageField* TStorage::getField()
{
    //return Fields[FieldIndex];
    return (*curTable)->getField();
    //return Fields[FieldIndex]->name_src;
}

//---------------------------------------------------------------------------
// Задает templateStorage в качестве источника структуры
// Используется когда Хранилище представляет приемник
TStorage::setTemplate(TStorage* storage, bool deleteAfterUse)
{
    templateStorage = storage;
    delTemplateStorage = deleteAfterUse;
}


//---------------------------------------------------------------------------
// Тестирование 2
TStorage::copyFieldsFrom(TStorage* storage)
{
    // Конвертация
}

//
TStorage::copyFieldsToDbf(TStorage* storage)
{
    // Конвертация
    Modified = true;
}

//
TStorage::copyFieldsToExcel(TStorage* storage)
{
    // Конвертация
    bool Modified = true;
}



//---------------------------------------------------------------------------
// Полное копирование полей
// Вызывается только для источников одинакового типа!
void TStorage::fullCopyFields(TStorage* src, TStorage* dst)
{
    /*// Копируем из src в dst
    int n = src->Fields.size();
    for (int i = 0; i < n; i++) {
        dst->addField(src->Fields[i]);
    }*/
}


//---------------------------------------------------------------------------
// Возвращает true если достигнут конец списка таблиц
bool TStorage::eot()
{
    return curTable == Tables.end();
}


void TStorage::openTable(bool ReadOnly)
{
    (*curTable)->open(ReadOnly);
};

//---------------------------------------------------------------------------
// Переходит к следующей таблице
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
// Переходит к следующей записи таблицы
void TStorage::nextRecord()
{
    (*curTable)->nextRecord();
}


//---------------------------------------------------------------------------
// Переходит к следующему полю таблицы
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





