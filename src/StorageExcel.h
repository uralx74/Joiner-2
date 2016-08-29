//---------------------------------------------------------------------------
#ifndef StorageExcelH
#define StorageExcelH

#include <vector>
#include "Storage.h"

#include "c:\PROGRS\current\util\MSExcelWorks.h"


class TStorageExcel;


//---------------------------------------------------------------------------
// ��������� ��� �������� ����� � MS Excel
class TExcelField : public TStorageField {    // ��� �������� ��������� dbf-�����
public:
    String format;      // ������ ������ � MS Excel
    //TExcelField(AnsiString Name, int Index);
private:
    friend class TStorageExcel;
    int index;
};

// ��������� ��� �������� ����������
class TExcelTable : public TStorageTable
{
public:
    AnsiString File;
};

//---------------------------------------------------------------------------
// class TStorageExcel: TStorage
// ����� ������� ���������/��������� ������
// ������������ �������, �������� � ����� MS Excel
//---------------------------------------------------------------------------

class TStorageExcel: TStorage
{
protected:
    //friend class TExcelField;
    void closeTable();


public:
    TStorageExcel(){};
    ~TStorageExcel();
    void openTable(bool ReadOnly = true);
    Variant getFieldValue(TStorageField* Field);
    void setFieldValue(Variant Value);
    void commit();
    void append();

    //bool Eot();     // End Of Tables
    bool eor();     // End Of Records
    //bool Eof();     // End Of Fields

    //void NextTable();
    //void nextRecord();
    //void NextField();


    TExcelField* addField();
    void addTable(const TExcelTable& Table);

    AnsiString getTable();

    copyFieldsFrom(TStorage* storage);

private:
    copyFieldsToExcel(TStorage* storage);
    std::vector<TExcelTable> Tables;    // ������ ����� ��� �������
    MSExcelWorks* msexcel;
    Variant Worksheet;
    Variant Workbook;
    Variant Range;
};



//---------------------------------------------------------------------------
#endif
