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

/*class DbfFactory : public TableFactory
{
public:
    TStorageTable* newTable();
};*/



//---------------------------------------------------------------------------
// Структура для хранения параметров поля (столбца) DBASE
class TDbaseField : public TStorageField {    // Для описания структуры dbf-файла
public:
    //TDbaseField() {};
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

// Структура для хранения параметров
/*class TDbaseTable: public TStorageTable
{
public:
    AnsiString File;
}; */


//---------------------------------------------------------------------------
// class TStorageDbase: TStorage
// Класс объекта источника/приемника данных
// Используется файл DBase4
//---------------------------------------------------------------------------
class TStorageDbase: public TStorageTable
{
protected:
    void closeTable();
    
public:
    TStorageDbase();
    TStorageDbase(String fileName);
    void openTable(bool ReadOnly = true);
    Variant getFieldValue(TStorageField* field);

    void setFieldValue(TStorageField* field, Variant value);
    //void setFieldValue(Variant Value);
    void commit();
    void append();


    bool eor();     // End Of Records

    //void nextTable();
    void nextRecord();
    //void NextField();

    //TDbaseField* addField();
    //TStorageField* addField(TStorageField* Field);
    //void addTable(const TDbaseTable& Table);

    std::vector<TStorageField> getFieldDefs();
    void setFieldDefs(std::vector<TStorageField>);

    AnsiString getTable();

    void loadFieldDefs();


    // Тестирование
    //CopyDbaseFields(TDbaseField* field);


    // Тестирование 2
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);




    virtual TStorageField* createField();


    AnsiString File;


private:
    void create();  // Создает файл DBF

    //std::vector<TDbaseTable> Tables;    // Список таблиц
    TDbf* pTable;   // Текущее хранилище
};




//---------------------------------------------------------------------------
#endif
