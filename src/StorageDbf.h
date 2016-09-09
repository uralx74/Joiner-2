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
// ��������� ��� �������� ���������� ���� (�������) DBASE
class TDbaseField : public TStorageField {    // ��� �������� ��������� dbf-�����
public:
    TDbaseField(const OleXml& oleXml, Variant node);
    TDbaseField(); // ���� ����������� �������� �� �����
    //TDbaseField(TDbaseField* Field);
    //TDbaseField(TStorageField* Field);

    char type;    // ��� fieldtype is a single character [C,D,F,L,M,N]
    int length;         // ����� ����
    int decimals;       // ����� ���������� �����

    // Character 	 1-255
    // Date	  8
    // Logical	  1
    // Memo	  10
    // Numeric	1-30
    // Decimals is 0 for non-numeric, number of decimals for numeric.

};

/*
// ��������� ��� �������� ����������
class TDbaseTable: public TStorageTable
{
public:
    AnsiString File;
};*/


//---------------------------------------------------------------------------
// class TStorageDbase: TStorage
// ����� ������� ���������/��������� ������
// ������������ ���� DBase4
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


    // ������������ 2
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);



    virtual AnsiString getTableName() const;

    virtual TStorageField* createField();
    virtual TStorageField* createField(const OleXml& oleXml, Variant node);



private:
    void create();  // ������� ���� DBF
    AnsiString _filename;

    //std::vector<TDbaseTable> Tables;    // ������ ������

    TDbf* pTable;   // ������� ���������
};




//---------------------------------------------------------------------------
#endif
