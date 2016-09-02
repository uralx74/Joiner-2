//---------------------------------------------------------------------------
#ifndef StorageH
#define StorageH

#include <vector>

#include "StorageTable.h"
#include "..\util\OleXml.h"

//#include <typeinfo>

typedef std::map<String, String> StorageParameters;


/*
class StorageParameters
{
public:
    //StorageParameters(): file = "" {};
    AnsiString type;
    AnsiString file;
    AnsiString username;
    AnsiString password;
    AnsiString code;
    bool truncate;
};   */




class TStorage;  // опережающее объявление


//---------------------------------------------------------------------------
// Хранилище
// class TStorage
//---------------------------------------------------------------------------

class TStorage//: public TStorageTable
{
public:
    friend TStorageField;
    TStorage();
    virtual ~TStorage();

    void registerFactory(String name, TableFactoryBase* factory);
    bool factoryExists(const String& name) const;

    void loadTables(StorageParameters& storageParameters);
    void loadStorage(const OleXml& oleXml, Variant node, bool readOnly);
    //void loadTables(std::map<String, String>& storageParameters);




    bool linkSource(TStorage* Storage);
    //bool FindField(AnsiString fieldName);
    TStorageField* findField(AnsiString fieldName);

    void openTable(bool ReadOnly = true) {
        (*curTable)->open();
    };

    Variant getFieldValue(TStorageField* Field) {};
    void setFieldValue(Variant Value) {};
    void commit() {};
    void append();   // Добавляет пустую запись перед (используется в DBase)
    void post() {};     // Фиксирует запись (используется в OraProc)
    void rollback();     // Фиксирует запись (используется в OraProc)

    void closeTable();

    bool eot();     // End Of Tables
    bool eor() {};     // End Of Records
    bool eof();     // End Of Fields

    void nextTable();
    void nextRecord();
    void nextField();

    TStorageField* getField();   // Возвращает имя сопоставленного поля в источнике
    bool isActiveField();
    bool isLinkedField();
    bool isActiveTable() { return Active; };
    bool isModified() { return Modified; };
    //virtual void SetReadOnly(bool ReadOnlyFlag){ this->ReadOnly = ReadOnlyFlag; };
    virtual int getRecordCount(){ return RecordCount; };
    virtual int getRecordIndex(){ return RecordIndex; };

    // Информационные функции
    AnsiString getTable() {};     // Возвращает наименование активного хранилища данных
    AnsiString getTableStage();     // Возвращает текущий этап обработки данных
    AnsiString getRecordStage();     // Возвращает текущий этап обработки данных



    TStorageField* addField(TStorageField* Field) {return NULL;};
    void loadFieldDefs() {};    // Загружает описание полей



    // Методы копирования полей
    // Используется двойная диспетчеризация
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);
    copyFieldsToExcel(TStorage* storage);
    //virtual CopyFieldsToOraProc(TStorage* storage);

    virtual setTemplate(TStorage* storage, bool deleteAfterUse = true);

protected:
    void fullCopyFields(TStorage* src, TStorage* dst);
    int TableIndex;
    int TableCount;
    int FieldCount;
    int FieldIndex;
    int RecordCount;
    int RecordIndex;
    bool Active;    // Признак того, что источник/приемник открыт и готов к считыванию/записи данных
    bool ReadOnly;
    bool Modified;





    // удалить
    //std::vector<TStorageField*> Fields;


    std::vector<TStorageTable*> Tables;    // Список таблиц
    std::vector<TStorageTable*>::iterator curTable;


    std::map<AnsiString, TableFactoryBase*> tableFactories;

    TStorage* templateStorage;    // Шаблон (берется первая доступная таблица)
    bool delTemplateStorage;

};


//---------------------------------------------------------------------------
#endif
