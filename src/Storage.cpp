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
}

TStorage::~TStorage()
{
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
 * node - указывает на корень загружаемого элемента - import или export
 * переименовать в AddTable
 */
void TStorage::loadStorage(const OleXml& oleXml, Variant node, bool readOnly)
{
    //AnsiString sss = oleXml.GetNodeName(node);

    if (node.IsEmpty()) {
        return;
    }









 /*   String branchName = oleXml.GetNodeName(node);
    Variant tableNode = oleXml.GetFirstNode(node);

    while (!tableNode.IsEmpty()) {
        String storageType = oleXml.GetNodeName(tableNode);

        // Если тип хранилища зарегистрирован
        if (tableFactories.find(storageType) != tableFactories.end()) {
            AnsiString attrFile = oleXml.GetAttributeValue(tableNode, "file");
            ExpandFileNameCustom(attrFile,);

            if (attrFile != "") {

                TSearchRec searchRec;
                FindFirst(attrFile, faAnyFile, searchRec);

                if (searchRec.Name != "") {
                    //AnsiString FilePath = ExtractFilePath(Table.File);
                    do {
                        //TStorageTable* table = tableFactory->newTable();

                        //table->file = FilePath + SearchRec.Name;
                        //Tables.push_back(table);
                    } while ( FindNext(searchRec) == 0);

                } else {
                    //TStorageTable* table = tableFactory->newTable();
                    //Tables.push_back(table);
                }
            }


            //OleXml* newXml = new OleXml();
            //newXml->CreateRootNode("thisIsRoot");
            //newXml->Save("c:\\test\\nex.xml");
            //oleXml.Save("c:\\test\\old.xml");

            return;



             TableFactoryBase* tableFactory = tableFactories[storageType];
             TStorageTable* table = tableFactory->newTable();

             //TStorageTable* table = tableFactory->newTable(oleXml, tableNode);
             //vector<TStorageTable*> loadedTables = tableFactory->loadTables(oleXml, tableNode);

             Tables.push_back(table);
             //Tables.insert(loadedTables);
        }

        /*XmlBranch* subBranch = new XmlBranch();
        branch->branch.insert(BranchType::value_type(subBranchName, subBranch));
        subBranch = LoadToXmlBranch(subnode);
        subnode = GetNextNode(subnode);*/
    /*}
    //return branch;*/
}


/*
 */
void TStorage::loadTables(StorageParameters& storageParameters)
{
    //storageParameters["file"];

    if (storageParameters["file"] != "") {
        // Выбираем фабрику для создания таблиц по требуемому типу
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
        // Выбираем фабрику для создания таблиц по требуемому типу
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
// Закрытие хранилища
void TStorage::closeTable()
{
   /* std::vector<String>::size_type n = Fields.size();
    for (int i = 0; i < n; i++) {   // Удаление полей
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
// Добавление пустой записи
void TStorage::append()
{
    RecordCount++;
    FieldIndex = 0;
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
// Возвращает true - если поле поле подлежит записи
bool TStorage::isActiveField()
{
    return (*curTable)->isActiveField();
    //return Fields[FieldIndex]->active && Fields[FieldIndex]->enable;
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
    return TableIndex >= TableCount;
}

//---------------------------------------------------------------------------
// Возвращает true если достигнут конец списка полей
bool TStorage::eof()
{
    return FieldIndex  >= FieldCount;
}

//---------------------------------------------------------------------------
// Переходит к следующей таблице
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
// Переходит к следующей записи таблицы
void TStorage::nextRecord()
{
    if (!eor()) {
        FieldIndex = 0;
        RecordIndex++;
    }
}

//---------------------------------------------------------------------------
// Переходит к следующему полю таблицы
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





