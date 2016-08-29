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


// ��������� ��� �������� ���������� ����� � Oracle (���������/�������)
class TOraField : public TStorageField {
};

class TStorageTableOra : public TStorageTable
{
public:
    AnsiString Table;       // ��� ������� (������������ ���� ����� �������������� ��������)
    AnsiString Server;
    AnsiString Username;
    AnsiString Password;
};



// ��������� ��� �������� ����������
class TOraProcTable : public TStorageTableOra
{
public:
    AnsiString Procedure;   // ��� ���������
};

// ��������� ��� �������� ����������
class TOraSqlTable : public TStorageTableOra
{
public:
    AnsiString Sql;     // ��� ����� � Sql-��������
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
// ����� ������� ��������� ������
// ������������ ��������� � �� Oracle
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
    std::vector<TOraProcTable> Tables;    // ������ ������ (��������)
};



//---------------------------------------------------------------------------
// class TStorageOraSql: TStorage
// ����� ������� ���������/��������� ������
// ������������ ������� � �� Oracle
//---------------------------------------------------------------------------
class TStorageOraSql: public TStorageOra
{
public:
    TStorageOraSql() {};
    void openTable(bool ReadOnly = true);
    //Variant Get(AnsiString Field);      // ���������� �������� �� ����� ����
    Variant getFieldValue(TStorageField* Field);
    //void Set(Variant Value);      // ������������� �������� ��������� ����
    void setFieldValue(Variant Value);      // ������������� �������� ��������� ����
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
    std::vector<TOraSqlTable> Tables;    // ������ ����� ��� ������� � ���� DBF
};







//---------------------------------------------------------------------------
#endif
