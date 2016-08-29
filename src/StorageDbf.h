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
// ��������� ��� �������� ���������� ���� (�������) DBASE
class TDbaseField : public TStorageField {    // ��� �������� ��������� dbf-�����
public:
    //TDbaseField() {};
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

// ��������� ��� �������� ����������
/*class TDbaseTable: public TStorageTable
{
public:
    AnsiString File;
}; */


//---------------------------------------------------------------------------
// class TStorageDbase: TStorage
// ����� ������� ���������/��������� ������
// ������������ ���� DBase4
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


    // ������������
    //CopyDbaseFields(TDbaseField* field);


    // ������������ 2
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);




    virtual TStorageField* createField();


    AnsiString File;


private:
    void create();  // ������� ���� DBF

    //std::vector<TDbaseTable> Tables;    // ������ ������
    TDbf* pTable;   // ������� ���������
};




//---------------------------------------------------------------------------
#endif
