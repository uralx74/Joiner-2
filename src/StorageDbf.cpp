#include "pch.h"
#pragma hdrstop

#include "StorageDbf.h"


/*этот конструктор возможно ненужен*/
TDbaseField::TDbaseField()
{
}

TDbaseField::TDbaseField(const OleXml& oleXml, Variant node)
{
    name = LowerCase(oleXml.GetAttributeValue(node, "name"));
    name_src = LowerCase(oleXml.GetAttributeValue(node, "name_src", name));
    active = oleXml.GetAttributeValue(node, "active", true);
    enable = oleXml.GetAttributeValue(node, "enable", true);

    type = LowerCase(oleXml.GetAttributeValue(node, "type", "C"))[1];                 // Тип fieldtype is a single character [C,D,F,L,M,N]
    length = oleXml.GetAttributeValue(node, "length", 0);   // Длина поля
    decimals = oleXml.GetAttributeValue(node, "decimals", 0); // Длина десятичной части
}

/*
 *
 *
 *
 *
 *
 */

/* Возвращает имя таблицы
 */
AnsiString TStorageDbase::getTableName() const
{
    return _filename;
    //return pTable->FilePathFull + pTable->TableName;
}

/*
 */
TStorageDbase::TStorageDbase(const OleXml& oleXml, Variant node) :
    pTable(NULL)
{
    _filename = oleXml.GetAttributeValue(node, "file");
    _truncate = oleXml.GetAttributeValue(node, "truncate", false);
    _readOnly = oleXml.GetAttributeValue(node, "readonly", true);



    // Здесь возможно тоже создавать TStorage и добавлять в него поля ?
    Variant nodeField = oleXml.SelectSingleNode(node, "field");
    /*if (!VarIsEmpty(nodeField)) {

        AnsiString name = oleXml.GetAttributeValue(nodeField, "name", "");


    }*/


    while (!VarIsEmpty(nodeField) && oleXml.GetNodeName(nodeField) == "field") {
        // Возможно переделать на абстрактную фабрику или др.
        // а также позаботиться об удалении обьекта

        TStorageField* field = this->createField(oleXml, nodeField);
        this->addField(field);
//        TStorageField* dbaseField = this->addField(oleXml, nodeField);

        /*TDbaseField* dbaseField = (TStorageField*)this->addField();
        if (dbaseField != NULL) {
            // Создать функцию для установки значений
            dbaseField->type = LowerCase(msxml.GetAttributeValue(node_fields, "type", "C"))[1];
            dbaseField->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
            dbaseField->length = msxml.GetAttributeValue(node_fields, "length", 0);
            dbaseField->decimals = msxml.GetAttributeValue(node_fields, "decimals", 0);
            dbaseField->active = msxml.GetAttributeValue(node_fields, "active", true);
            dbaseField->enable = msxml.GetAttributeValue(node_fields, "enable", true);
            dbaseField->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", dbaseField->name));
        }  */
        //if (dbaseField->name_src == "" )
        //    dbaseField->name_src = dbaseField->name;
        nodeField = oleXml.GetNextNode(nodeField);
    }






/*                    while (!node_fields.IsEmpty()) {
                        if (msxml.GetNodeName(node_fields) == "field") {
                            // Возможно переделать на абстрактную фабрику или др.
                            // а также позаботиться об удалении обьекта
                            TDbaseField* dbaseField = StorDbase->addField();
                            if (dbaseField != NULL) {
                                // Создать функцию для установки значений
                                dbaseField->type = LowerCase(msxml.GetAttributeValue(node_fields, "type", "C"))[1];
                                dbaseField->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                                dbaseField->length = msxml.GetAttributeValue(node_fields, "length", 0);
                                dbaseField->decimals = msxml.GetAttributeValue(node_fields, "decimals", 0);
                                dbaseField->active = msxml.GetAttributeValue(node_fields, "active", true);
                                dbaseField->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                                dbaseField->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", dbaseField->name));
                            }
                            //if (dbaseField->name_src == "" )
                            //    dbaseField->name_src = dbaseField->name;
                        }
                        node_fields = msxml.GetNextNode(node_fields);
                    }

*/











    //String xmlTemplate = msxml.GetAttributeValue(subnode, "template", "");
    //bool xmlSourceAsTemplate = msxml.GetAttributeValue(subnode, "source_as_template", false);
}

/* Добавляет новое поле в список полей ()
 * возможно переделать на AddField(TDbaseField* Field)
 */
/*TStorageField* TStorageDbase::addField(TStorageField* Field)
{
    TStorageField* newField = new TDbaseField();

    //String test2 = (static_cast<TDbaseField*>(newField))->type;

    // Необходимо? приведение типов чтобы скопировать обьект правильно
    *(static_cast<TDbaseField*>(newField)) = *(static_cast<TDbaseField*>(Field));

    this->Fields.push_back(newField);// возможно нужно приводить к  static_cast<TDbaseField*>
    FieldCount++;
    return newField;
}*/

/*
 */
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
        dbaseField->name = LowerCase(fieldDef->FieldName);
        dbaseField->length = fieldDef->Size;
        dbaseField->decimals = fieldDef->Precision;
        dbaseField->active = true;
        dbaseField->enable = true;
        dbaseField->name_src = dbaseField->name;

        addField(dbaseField);   // 2016-09-08
    }
}

/* Копирование полей из источника
 * Используется двойная диспетчеризация
 */
TStorageDbase::copyFieldsFrom(TStorage* storage)
{
    // В зависимости от типа storage (производного от TStorage), вызывается метод CopyFieldsToDbf
    //storage->copyFieldsToDbf(this);
}


/* Полное копирование полей из DBF в DBF
 */
TStorageDbase::copyFieldsToDbf(TStorage* storage)
{
    //TStorage::fullCopyFields(this, storage);
}



/*TStorageDbase::TStorageDbase(String fileName)
{
    //TStorageDbase* storageDbaseStructure = new TStorageDbase();
    TDbaseTable table;
    table.File = fileName;
    //table.File = ExpandFileName(fileName);
    this->addTable(table);
} */



/* проверить необходимость создания поля без загрузки из xml*/
TStorageField* TStorageDbase::createField()
{
    return new TDbaseField();
}

TStorageField* TStorageDbase::createField(const OleXml& oleXml, Variant node)
{
    return new TDbaseField(oleXml, node);
}



//---------------------------------------------------------------------------
// Добавляет новое поле в список полей
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
// Добавляет новое поле в список полей
void TStorageDbase::AddField(const TDbaseField* Field)
{
    Fields.push_back((TStorageField*)Field);
    FieldCount++;
}*/

//---------------------------------------------------------------------------
// Добавляет таблицу в список (имя файла, может быть задано маской)
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

/* Открытие таблицы
 */
void TStorageDbase::nativeOpenTable(bool readOnly)
{
    //this->ReadOnly = ReadOnly;
    //if (Tables.size() <= TableIndex) {
    //    throw Exception("The table with index \"" + IntToStr(TableIndex) + "\" doesn't exists.");
    //}

    if (readOnly) {
        pTable = new TDbf(NULL);

        pTable->TableName = _filename;
        pTable->Exclusive = true;

        if (FileExists(_filename)) {
            try {
                pTable->Open();
                //RecordCount = pTable->RecordCount;
            } catch (...) {
                throw Exception("Can't to open file  " + _filename + ".");
            }
        } else {
            throw Exception("File not found " + _filename + ".");
        }
        loadFieldDefs();
    } else {
        create();
    }
}

//---------------------------------------------------------------------------
//
void TStorageDbase::create()
{
    int sss = Fields.size();
    pTable = new TDbf(NULL);

    //pTableDst->TableLevel = 7; // required for AutoInc field
    pTable->TableLevel = 4;
    pTable->LanguageID = DbfLangId_RUS_866;

    pTable->TableName = ExtractFileName(_filename);
    pTable->FilePathFull = ExtractFilePath(_filename);

    // Создаем определение полей таблицы из параметров
    TDbfFieldDefs* TempFieldDefs;
    TempFieldDefs = new TDbfFieldDefs(NULL);

    if (TempFieldDefs == NULL) {
        throw Exception("Can't create storage.");
    }


/*    // Копирование полей из шаблона, если он задан
    if (templateStorage) {
        templateStorage->openTable();
        templateStorage->loadFieldDefs();

        this->copyFieldsFrom(templateStorage);

        templateStorage->closeTable();
        if (delTemplateStorage) {
            delete templateStorage;
        }
    } */



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
        //pTable->Append();
        //pTable->FieldByName("filename")->Value = "value";
        //pTable->Post();
    } catch (...) {
    }

    //FieldCount = Fields.size();   /**/
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

/* Возвращает значение поля
*/
Variant TStorageDbase::nativeGetFieldValue(const TStorageField* const Field)
{
    // Возможно Переделать?????? Field[FieldIndex].name
    //if ()
    try {
        return pTable->FieldByName(Field->name_src)->Value;
    } catch (...) {
        throw Exception("Field \"" + Field->name_src + "\" not found.");
    }
}

/* Устанавливает значение активного поля
*/
void TStorageDbase::nativeSetFieldValue(TStorageField* field, Variant value)
{
    //bool s = VarIsNull(value);
    //AnsiString ss = (AnsiString)value;

    if (/*!VarIsEmpty(Value) &&*/ !VarIsNull(value) && (AnsiString)value != "") {
                // (AnsiString)Value != "" - Эта проверка критична,
                // так как в DBF исключено значение NULL
            pTable->FieldByName(field->name)->Value = value;
    }
}

/* Фиксирует изменения
*/
void TStorageDbase::nativeCommit()
{
    if (pTable->Modified) {
        pTable->Post();
    }
}

/* Возвращает количество записей в таблице
*/
int TStorageDbase::nativeGetRecordCount()
{
    return pTable->RecordCount;
}

/* Добавляет новую запись в таблицу
*/
void TStorageDbase::nativeAppend()
{
    pTable->Append();
}

/* Закрывает таблицу
*/
void TStorageDbase::nativeCloseTable()
{
    if (pTable != NULL) {
        pTable->Close();
        delete pTable;
        pTable = NULL;
    }
}

/* Возвращает true если достигнут конец таблицы
*/
bool TStorageDbase::eor()
{
    return pTable->Eof;
}

/* Возвращает true если достигнута последняя запись
*/
//bool TStorageDbase::Eof()
//{
//    return FieldIndex + 1 == Fields.size();
//}

//---------------------------------------------------------------------------
// Переходит к следующей таблице
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
// Переходит к следующей записи таблицы
void TStorageDbase::nativeNextRecord()
{
    //try {
        pTable->Next();
    //} catch (...) {
    //}
}


bool TStorageDbase::nativeEor() const
{
    return pTable->Eof;
}


