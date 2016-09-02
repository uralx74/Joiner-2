//---------------------------------------------------------------------------
#ifndef StorageH
#define StorageH

#include <vector>

#include "StorageTable.h"
#include "..\util\OleXml.h"

//#include <typeinfo>

typedef std::map<String, String> StorageParameters;


/*
class StorageParameters
{
public:
    //StorageParameters(): file = "" {};
    AnsiString type;
    AnsiString file;
    AnsiString username;
    AnsiString password;
    AnsiString code;
    bool truncate;
};   */




class TStorage;  // ����������� ����������


//---------------------------------------------------------------------------
// ���������
// class TStorage
//---------------------------------------------------------------------------

class TStorage//: public TStorageTable
{
public:
    friend TStorageField;
    TStorage();
    virtual ~TStorage();

    void registerFactory(String name, TableFactoryBase* factory);
    bool factoryExists(const String& name) const;

    void loadTables(StorageParameters& storageParameters);
    void loadStorage(const OleXml& oleXml, Variant node, bool readOnly);
    //void loadTables(std::map<String, String>& storageParameters);




    bool linkSource(TStorage* Storage);
    //bool FindField(AnsiString fieldName);
    TStorageField* findField(AnsiString fieldName);

    void openTable(bool ReadOnly = true) {
        (*curTable)->open();
    };

    Variant getFieldValue(TStorageField* Field) {};
    void setFieldValue(Variant Value) {};
    void commit() {};
    void append();   // ��������� ������ ������ ����� (������������ � DBase)
    void post() {};     // ��������� ������ (������������ � OraProc)
    void rollback();     // ��������� ������ (������������ � OraProc)

    void closeTable();

    bool eot();     // End Of Tables
    bool eor() {};     // End Of Records
    bool eof();     // End Of Fields

    void nextTable();
    void nextRecord();
    void nextField();

    TStorageField* getField();   // ���������� ��� ��������������� ���� � ���������
    bool isActiveField();
    bool isLinkedField();
    bool isActiveTable() { return Active; };
    bool isModified() { return Modified; };
    //virtual void SetReadOnly(bool ReadOnlyFlag){ this->ReadOnly = ReadOnlyFlag; };
    virtual int getRecordCount(){ return RecordCount; };
    virtual int getRecordIndex(){ return RecordIndex; };

    // �������������� �������
    AnsiString getTable() {};     // ���������� ������������ ��������� ��������� ������
    AnsiString getTableStage();     // ���������� ������� ���� ��������� ������
    AnsiString getRecordStage();     // ���������� ������� ���� ��������� ������



    TStorageField* addField(TStorageField* Field) {return NULL;};
    void loadFieldDefs() {};    // ��������� �������� �����



    // ������ ����������� �����
    // ������������ ������� ���������������
    copyFieldsFrom(TStorage* storage);
    copyFieldsToDbf(TStorage* storage);
    copyFieldsToExcel(TStorage* storage);
    //virtual CopyFieldsToOraProc(TStorage* storage);

    virtual setTemplate(TStorage* storage, bool deleteAfterUse = true);

protected:
    void fullCopyFields(TStorage* src, TStorage* dst);
    int TableIndex;
    int TableCount;
    int FieldCount;
    int FieldIndex;
    int RecordCount;
    int RecordIndex;
    bool Active;    // ������� ����, ��� ��������/�������� ������ � ����� � ����������/������ ������
    bool ReadOnly;
    bool Modified;





    // �������
    //std::vector<TStorageField*> Fields;


    std::vector<TStorageTable*> Tables;    // ������ ������
    std::vector<TStorageTable*>::iterator curTable;


    std::map<AnsiString, TableFactoryBase*> tableFactories;

    TStorage* templateStorage;    // ������ (������� ������ ��������� �������)
    bool delTemplateStorage;

};


//---------------------------------------------------------------------------
#endif
