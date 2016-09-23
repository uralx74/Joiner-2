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

    AnsiString _table;       // ��� ������� (������������ ���� ����� �������������� ��������)
    AnsiString _server;
    AnsiString _username;
    AnsiString _password;


};



//---------------------------------------------------------------------------
// class TStorageOraProc: TStorage
// ����� ������� ��������� ������
// ������������ ��������� � �� Oracle
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
    AnsiString _procedure;   // ��� ���������

    //std::vector<TOraProcTable> Tables;    // ������ ������ (��������)
};



//---------------------------------------------------------------------------
// class TStorageOraSql: TStorage
// ����� ������� ���������/��������� ������
// ������������ ������� � �� Oracle
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
    //Variant Get(AnsiString Field);      // ���������� �������� �� ����� ����
    Variant getFieldValue(TStorageField* Field);
    //void Set(Variant Value);      // ������������� �������� ��������� ����
    //void setFieldValue(Variant Value);      // ������������� �������� ��������� ����
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
    //std::vector<TOraSqlTable> Tables;    // ������ ����� ��� ������� � ���� DBF
};







//---------------------------------------------------------------------------
#endif
