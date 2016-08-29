//---------------------------------------------------------------------------
#ifndef StorageOraH
#define StorageOraH

#include <vector>
#include <map>

#include "StorageTable.h"

#include "Ora.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Storage.h"


// Структура для хранения параметров полей в Oracle (процедуры/таблицы)
class TOraField : public TStorageField {
};

class TStorageTableOra : public TStorageTable
{
public:
    AnsiString Table;       // Имя таблицы (используется если нужно предварительно очистить)
    AnsiString Server;
    AnsiString Username;
    AnsiString Password;
};



// Структура для хранения параметров
class TOraProcTable : public TStorageTableOra
{
public:
    AnsiString Procedure;   // Имя процедуры
};

// Структура для хранения параметров
class TOraSqlTable : public TStorageTableOra
{
public:
    AnsiString Sql;     // Имя файла с Sql-запросом
};





//---------------------------------------------------------------------------
// class TStorageOra
// base class for Oracle source/destination
//---------------------------------------------------------------------------

class TStorageOra: public TStorage
{
protected:
    void closeTable();

protected:
    ~TStorageOra();

    void openConnection(AnsiString Server, AnsiString Username, AnsiString Password);
    TOraSession* dbSession;
    TOraQuery* dbQuery;
    void truncateTable(TStorageTableOra* Table);
    void prepareQuery();


};

//---------------------------------------------------------------------------
// class TStorageOraProc: TStorage
// Класс объекта приемника данных
// Используется процедура в БД Oracle
//---------------------------------------------------------------------------
class TStorageOraProc: public TStorageOra
{
public:
    TStorageOraProc() {};
    void openTable(bool ReadOnly = true);
    //Variant Get(AnsiString Field);
    void setFieldValue(Variant Value);
    void post();
    void nextField();
    //AnsiString GetSrcField();
    //bool IsActiveField();
    TOraField* addField();
    //void AddField(const TOraField& Field);
    void addTable(const TOraProcTable& Table);
    AnsiString getTable();
private:
    std::vector<TOraProcTable> Tables;    // Список таблиц (процедур)
};



//---------------------------------------------------------------------------
// class TStorageOraSql: TStorage
// Класс объекта источника/приемника данных
// Используется таблица в БД Oracle
//---------------------------------------------------------------------------
class TStorageOraSql: public TStorageOra
{
public:
    TStorageOraSql() {};
    void openTable(bool ReadOnly = true);
    //Variant Get(AnsiString Field);      // Возвращает значение по имени поля
    Variant getFieldValue(TStorageField* Field);
    //void Set(Variant Value);      // Устанавливает значение активного поля
    void setFieldValue(Variant Value);      // Устанавливает значение активного поля
    void append();
    void commit();
    void post();
    bool eor();     // End Of Records
    //void nextTable();
    void nextRecord();
    //AnsiString GetSrcField();
    //bool IsActiveField();
    TOraField* addField();
    void addTable(const TOraSqlTable &Table);
    AnsiString getTable();
    //bool FindField(AnsiString fieldName);

private:
    std::vector<TOraSqlTable> Tables;    // Список полей для экспрта в файл DBF
};







//---------------------------------------------------------------------------
#endif
