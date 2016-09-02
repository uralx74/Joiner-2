/*****************************************************************************
     * File: StorageTable.h
     * Description: Table definitions as storage items
     * Created: 05 aug 2016
     * Copyright: (C) 2016
     * Author: V.Ovchinnikov
     * Email: utnpsys@gmail.com
*****************************************************************************/

#ifndef STORAGE_TABLE_H
#define STORAGE_TABLE_H

//#include <exception>
//#include <stdexcept>

class TStorageTable;


/*
 * @class TableFactoryBase
 * @note Abstract class of factory
 */

class TableFactoryBase
{
public:
    virtual TStorageTable* newTable() {};
};

/*
 * @class TableFactory
 * @note Defines concrete factory to create concrete "StorageTable"
 */

template <class T>
class TableFactory : public TableFactoryBase
{
public:
    TStorageTable* newTable();
};

template <class T>
TStorageTable* TableFactory<T>::newTable()
{
    return new T();
}


/**
 * @class TStorageField
 * @note Defines field of table
 */

 class TStorageField {
public:
    bool active;        // Признак необходимости заполненеия поля (если false, поле создается, но не заполняется)
    bool enable;        // Признак того, необходимо ли вообще учитывать это поле (если false, поле не создается)
    String name;    // Имя поля (до 10 символов).
    String name_src;    // Имя поля из копируемой БД

//private:
    bool required;      // Признак обязательности наличия сопоставленного поля в источнике
    bool linked;

    close();

protected:
    //int FieldType;
    //virtual Copy(TStorageField *Field);
    //GetName();
    //GetLength();
    //GetDecimals();
};

typedef std::vector<TStorageField*>::iterator  FieldsIterator;



/**
 * @class TStorageTable
 * @note
 */

//---------------------------------------------------------------------------
class TStorageTable {
public:
    TStorageTable();
    bool truncate;

    void close();
    void open(bool ReadOnly = true);
    void nextRec();


    bool isLinkedField();
    bool isActiveField();
    bool linkSource(TStorageTable* SourceTable);
    TStorageField* findField(AnsiString fieldName);
    TStorageField* getField();
    //TStorageField* addField(TStorageField* Field);
    TStorageField* addField();
    void setValue(Variant Value);

    void appendRecord();




protected:
    std::vector<TStorageField*> Fields1;

    std::vector<TStorageField*> Fields;
    FieldsIterator curField;

    void setRecordCount(unsigned int _recordCount);


    virtual void openTable(bool ReadOnly = true) {};
    virtual void closeTable() {};
    virtual TStorageField* createField() {};
    virtual void setFieldValue(TStorageField* field, Variant Value) {};

    virtual void append() {};
    void nextRecord();


    unsigned int fieldCount;
    unsigned int recordCount;

    bool readOnly;
    bool modified;
    bool active;

   


};









//---------------------------------------------------------------------------
#endif // STORAGE_TABLE_H

