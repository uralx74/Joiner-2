//---------------------------------------------------------------------------
#ifndef StorageDbfH
#define StorageDbfH

#include <vector>
#include <map>
#include "Ora.hpp"
#include "dbf.hpp"
#include "Dbf_Lang.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Storage.h"


class TStorageDbase;

//---------------------------------------------------------------------------
// Структура для хранения параметров поля (столбца) DBASE
class TDbaseField : public TStorageField {    // Для описания структуры dbf-файла
public:
    TDbaseField(const OleXml& oleXml, Variant node);
    TDbaseField(); // этот конструктор возможно не нужен
    //TDbaseField(TDbaseField* Field);
    //TDbaseField(TStorageField* Field);

    char type;    // Тип fieldtype is a single character [C,D,F,L,M,N]
    int length;         // Длина поля
    int decimals;       // Длина десятичной части

    // Character 	 1-255
    // Date	  8
    // Logical	  1
    // Memo	  10
    // Numeric	1-30
    // Decimals is 0 for non-numeric, number of decimals for numeric.

};

/*
// Структура для хранения параметров
class TDbaseTable: public TStorageTable
{
public:
    AnsiString File;
};*/


//---------------------------------------------------------------------------
// class TStorageDbase: TStorage
// Класс объекта источника/приемника данных
// Используется файл DBase4
//---------------------------------------------------------------------------
class TStorageDbase: public TStorageTable
{
protected:
    virtual void nativeCloseTable();
    
public:
    TStorageDbase(const OleXml& oleXml, Variant node);
    //TStorageDbase(String fileName);
    void nativeOpenTable(bool readOnly = true);
    //Variant getFieldValue(TStorageField* field);
    //void nativeSetFieldValue(TStorageField* field, Variant value);
    Variant nativeGetFieldValue(const TStorageField* const Field);
    void nativeSetFieldValue(TStorageField* field, Variant value);

    //void setFieldValue(Variant Value);
    void nativeCommit();
    void nativeAppend();


    bool eor();     // End Of Records

    //void nextTable();
    virtual void nativeNextRecord();
    virtual bool nativeEor() const;
    virtual int nativeGetRecordCount();


    //void NextField();

    //TDbaseField* addField();
    //TStorageField* addField(TStorageField* Field);
    //void addTable(const TDbaseTable& Table);

    std::vector<TStorageField> getFieldDefs();
    void setFieldDefs(std::vector<TStorageField>);

    AnsiString getTable();

    void loadFieldDefs();


    // Тестирование 2
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);



    virtual AnsiString getTableName() const;

    virtual TStorageField* createField();
    virtual TStorageField* createField(const OleXml& oleXml, Variant node);



private:
    void create();  // Создает файл DBF
    AnsiString _filename;

    //std::vector<TDbaseTable> Tables;    // Список таблиц

    TDbf* pTable;   // Текущее хранилище
};




//---------------------------------------------------------------------------
#endif
