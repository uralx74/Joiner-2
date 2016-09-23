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

/*class TStorageTableOra : public TStorageTable
{
public:
}; */



//---------------------------------------------------------------------------
// class TStorageOra
// base class for Oracle source/destination
//---------------------------------------------------------------------------

class TStorageOra: public TStorageTable
{
protected:
    virtual void nativeCloseTable();

    //TStorageOra(const OleXml& oleXml, Variant node);
    virtual ~TStorageOra();

    virtual int nativeGetRecordCount() const;


    void openConnection(AnsiString Server, AnsiString Username, AnsiString Password);
    TOraSession* dbSession;
    TOraQuery* dbQuery;
    void truncateTable(const String& tableName);
    void prepareQuery();

    AnsiString _table;       // Имя таблицы (используется если нужно предварительно очистить)
    AnsiString _server;
    AnsiString _username;
    AnsiString _password;


};



//---------------------------------------------------------------------------
// class TStorageOraProc: TStorage
// Класс объекта приемника данных
// Используется процедура в БД Oracle
//---------------------------------------------------------------------------
class TStorageOraProc: public TStorageOra
{
public:
    TStorageOraProc(const OleXml& oleXml, Variant node);


    virtual void nativeOpenTable(bool readOnly = true);
    virtual TStorageField* createField(const OleXml& oleXml, Variant node) {};
    virtual void nativeSetFieldValue(TStorageField* field, Variant Value);
    virtual Variant nativeGetFieldValue(const TStorageField* const field) {};

    virtual void nativeNextRecord() {};
    virtual void nativeAppend() {};
    virtual void nativeCommit() {};
    virtual bool nativeEor() const {};
    virtual void nativePost();
    virtual AnsiString getTableName() const;




    //void openTable(bool ReadOnly = true);

    //Variant Get(AnsiString Field);
    //void setFieldValue(Variant Value);
    //AnsiString GetSrcField();
    //bool IsActiveField();
    //TOraField* addField();
    //void AddField(const TOraField& Field);
    //void addTable(const TOraProcTable& Table);
private:
    //TOraProcTable table
    AnsiString _procedure;   // Имя процедуры

    //std::vector<TOraProcTable> Tables;    // Список таблиц (процедур)
};



//---------------------------------------------------------------------------
// class TStorageOraSql: TStorage
// Класс объекта источника/приемника данных
// Используется таблица в БД Oracle
//---------------------------------------------------------------------------
class TStorageOraSql: public TStorageOra
{
public:
    TStorageOraSql(const OleXml& oleXml, Variant node);


    virtual void nativeOpenTable(bool readOnly = true);
    virtual TStorageField* createField(const OleXml& oleXml, Variant node) {};
    virtual void nativeSetFieldValue(TStorageField* field, Variant Value);
    virtual Variant nativeGetFieldValue(const TStorageField* const field) {};

    virtual void nativeNextRecord();
    virtual void nativeAppend();
    virtual void nativePost() {};
    virtual void nativeCommit();
    virtual bool nativeEor() const {};

    virtual AnsiString getTableName() const;





    void openTable(bool ReadOnly = true);
    //Variant Get(AnsiString Field);      // Возвращает значение по имени поля
    Variant getFieldValue(TStorageField* Field);
    //void Set(Variant Value);      // Устанавливает значение активного поля
    //void setFieldValue(Variant Value);      // Устанавливает значение активного поля
    //void commit();
    //void post();
    //bool eor();     // End Of Records
    //void nextTable();
    //void nextRecord();
    //AnsiString GetSrcField();
    //bool IsActiveField();
    //TOraField* addField();
    //void addTable(const TOraSqlTable &Table);
    //bool FindField(AnsiString fieldName);

private:
    AnsiString _sql;
    //std::vector<TOraSqlTable> Tables;    // Список полей для экспрта в файл DBF
};







//---------------------------------------------------------------------------
#endif
