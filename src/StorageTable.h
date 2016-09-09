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

#include "..\util\OleXml.h"

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
    virtual TStorageTable* newTable(const OleXml& oleXml, Variant node) {};
};

/*
 * @class TableFactory
 * @note Defines concrete factory to create concrete "StorageTable"
 */

template <class T>
class TableFactory : public TableFactoryBase
{
public:
    TStorageTable* newTable(const OleXml& oleXml, Variant node);
};

template <class T>
TStorageTable* TableFactory<T>::newTable(const OleXml& oleXml, Variant node)
{
    return new T(oleXml, node);
}


/*
 * @class TStorageField
 * @note Defines field of table
 */

class TStorageField {
public:
    bool active;        // ������� ������������� ����������� ���� (���� false, ���� ���������, �� �� �����������)
    bool enable;        // ������� ����, ���������� �� ������ ��������� ��� ���� (���� false, ���� �� ���������)
    String name;    // ��� ���� (�� 10 ��������).
    String name_src;    // ��� ���� �� ���������� ��

//private:
    bool required;      // ������� �������������� ������� ��������������� ���� � ���������
    bool linked;

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
    virtual ~TStorageTable() {};

    void nextRecord();

    void closeTable();
    void open(bool ReadOnly = true);
    void nextRec();
    void nextField();


    bool isActiveTable() const { return _active; };
    bool isLinkedField();
    bool isActiveField();
    bool linkSource(TStorageTable* SourceTable);
    TStorageField* findField(AnsiString fieldName);
    TStorageField* getField();

    //TStorageField* addField();
    TStorageField* addField(TStorageField* field);
    //TStorageField* addField(const OleXml& oleXml, Variant node);

    Variant getFieldValue(TStorageField* Field) const;
    void setFieldValue(Variant Value);

    virtual AnsiString getTableName() const = 0;

    void append();
    bool eof();
    bool eor();
    void commit();
    virtual int nativeGetRecordCount() = 0;

protected:
    void setRecordCount(int recordCount);

    virtual void nativeOpenTable(bool readOnly = true) = 0;
    virtual void nativeCloseTable() = 0;
    virtual TStorageField* createField(const OleXml& oleXml, Variant node) = 0;
    virtual void nativeSetFieldValue(TStorageField* field, Variant Value) = 0;
    virtual Variant nativeGetFieldValue(const TStorageField* const field) = 0;

    virtual void nativeNextRecord() {};
    virtual void nativeAppend() {};
    virtual void nativeCommit() = 0;
    virtual bool nativeEor() const = 0;

    std::vector<TStorageField*> Fields;
    FieldsIterator curField;
    int _recordCount;
    bool _truncate;
    bool _readOnly;
    bool _modified;
    bool _active;



};









//---------------------------------------------------------------------------
#endif // STORAGE_TABLE_H

