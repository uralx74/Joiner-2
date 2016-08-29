#include "pch.h"
#pragma hdrstop

#include "StorageExcel.h"


//---------------------------------------------------------------------------
//  TStorageExcel
//---------------------------------------------------------------------------
TStorageExcel::~TStorageExcel()
{
    closeTable();
}

//---------------------------------------------------------------------------
// ��������� ������� � ������ (��� �����, ����� ���� ������ ������)
void TStorageExcel::addTable(const TExcelTable& Table)
{
    TSearchRec SearchRec;
    FindFirst(Table.File, faAnyFile, SearchRec);

    if (SearchRec.Name != "") {     // ���� ���� ������, �� ��������� ����� �������-�������
        AnsiString FilePath = ExtractFilePath(Table.File);
        do {
            TExcelTable NewTable;
            NewTable.File = FilePath + SearchRec.Name;
            NewTable.Truncate = Table.Truncate;
            Tables.push_back(NewTable);
            TableCount++;
        } while ( FindNext(SearchRec) == 0);
    } else {    // ���� ���� �� ������, ��������� ������� ��� ����
        Tables.push_back(Table);
        TableCount++;
    }
    FindClose(SearchRec);
}

//---------------------------------------------------------------------------
// �������� �������
void TStorageExcel::openTable(bool ReadOnly)
{
    this->ReadOnly = ReadOnly;

    // ��������� ������ �������� - �������� ������� �����!!!!
    msexcel = new MSExcelWorks();
    if (ReadOnly) {
        if (FileExists(Tables[TableIndex].File)) {
            try {
                msexcel->OpenApplication();
                Workbook = msexcel->OpenDocument(Tables[TableIndex].File);
                Worksheet = msexcel->GetSheet(Workbook, 1);
            } catch (Exception &e) {
                throw Exception("Can't to open file  \"" + Tables[TableIndex].File + "\".\n" + e.Message);
            }
        } else {
            throw Exception("File not found \"" + Tables[TableIndex].File + "\".");
        }
    } else {  // ���� ��������� ��� ��������, �� � ����� ������ ������� ����� ����
        if (FileExists(Tables[TableIndex].File)) {
            try {
                msexcel->OpenApplication();
                Workbook = msexcel->OpenDocument(Tables[TableIndex].File);
            } catch (Exception &e) {
                closeTable();
                throw Exception("Can't to open file  \"" + Tables[TableIndex].File + "\".\n" + e.Message);
            }
            if (msexcel->IsReadOnly(Workbook)) {
                closeTable();
                throw Exception("Can't to open file  \"" + Tables[TableIndex].File + "\" for writing.");
            }
            Worksheet = msexcel->GetSheet(Workbook, 1);

            for(int i = 0; i < Fields.size(); i++) {
                AnsiString sCellValue = msexcel->ReadCell(Worksheet, 1, i+1);
                if (Fields[i]->name != sCellValue) {
                    closeTable();
                    throw Exception("Field names \"" + Fields[i]->name + "\" and \"" + sCellValue +"\" do not match.");
                }
            }
            if (Tables[TableIndex].Truncate) {
                //msexcel->SetVisibleExcel();
                msexcel->ClearWorksheet(Worksheet);
                // ������� "���������" ������� (�����)
                for(int i = 0; i < Fields.size(); i++) {
                    msexcel->WriteToCell(Worksheet, Fields[i]->name, 1, i+1, "@");
                }
            }
        } else {
            try {
                msexcel->OpenApplication();
                Workbook = msexcel->OpenDocument();
                Worksheet = msexcel->GetSheet(Workbook, 1);

                // ������� "���������" ������� (�����)
                for(int i = 0; i < Fields.size(); i++) {
                    msexcel->WriteToCell(Worksheet, Fields[i]->name, 1, i+1, "@");
                }
                msexcel->SaveDocument(Workbook, Tables[TableIndex].File);
                //Modified = true;
            } catch (Exception &e) {
                throw Exception("Can't to create file  \"" + Tables[TableIndex].File + "\".\n" + e.Message);
            }
        }
    }

    // ������� ��������
    if (ReadOnly) {
        int j = 1;
        String sCellValue;
        while ( (sCellValue = msexcel->ReadCell(Worksheet, 1, j)) != "") {
            TExcelField* field = this->addField();
            if (field != NULL) {
                field->name = LowerCase(sCellValue);
                field->index = j;
                j++;
            }
            //field->active = true;
            //field->enable = true;
            //Fields.push_back(field);
            //FieldsList[LowerCase(sCellValue)] = j;
            //j++;
        }
    }

    FieldCount = Fields.size();

    // ������� �����
    int i = 2;
    String sCellValue;
    while ( (sCellValue = msexcel->ReadCell(Worksheet, i, 1)) != "") {
        i++;
    }
    RecordCount = i - 2;

    Active = true;
}

//TExcelField::TExcelField(AnsiString Name, int Index)
//{
 //   TExcelField* field = new TExcelField();
//}

//---------------------------------------------------------------------------
// ��������� �������
void TStorageExcel::closeTable()
{
    if (msexcel != NULL) {
        Active = false;
        msexcel->CloseApplication();
        delete msexcel;
        msexcel = NULL;

        // � ����������� �������� ��� ����������
        Workbook = Unassigned;   // ������������ VarClear(WorkBooks)
        Worksheet = Unassigned;   // ������������ VarClear(WorkBooks)
    }
    TStorage::closeTable();
}

//---------------------------------------------------------------------------
//
bool TStorageExcel::eor()
{
    return RecordIndex >= RecordCount;
}

//---------------------------------------------------------------------------
// ���������� ������������ ��������� ���������/��������� ������
AnsiString TStorageExcel::getTable()
{
    if (!eot()) {
        return Tables[TableIndex].File;
    }
}

//---------------------------------------------------------------------------
// ���������� �������� ����
Variant TStorageExcel::getFieldValue(TStorageField* Field)
{
    AnsiString fieldName = LowerCase(Field->name_src);

    //TStorageField* field = FindField(Field->name_src);

    TExcelField* field = static_cast<TExcelField*>(findField(Field->name_src));

    if (field != NULL) {
        return msexcel->ReadCell(Worksheet, RecordIndex+2, field->index);
    } else {
        throw Exception("Field not found " + fieldName + ".");
    }

    /*if (FieldsList.find(fieldName) != FieldsList.end() ) {
        return msexcel->ReadCell(Worksheet, RecordIndex+2, FieldsList[fieldName]);
    } else {
        throw Exception("Field not found " + fieldName + ".");
    }*/
}

//---------------------------------------------------------------------------
// ��������� ����� ������ � �������
void TStorageExcel::append()
{
    RecordIndex = RecordCount;
    TStorage::append();
}

//---------------------------------------------------------------------------
// ������������� �������� ��������� ����
void TStorageExcel::setFieldValue(Variant Value)
{
    if (Fields[FieldIndex]->active && Fields[FieldIndex]->enable) {
        msexcel->WriteToCell(Worksheet, Value, RecordIndex+2, FieldIndex+1, ((TExcelField*)Fields[FieldIndex])->format);
    }
}

//---------------------------------------------------------------------------
// ��������� ����� ���� � ������ �����
TExcelField* TStorageExcel::addField()
{
    TExcelField* Field = new TExcelField();
    if (Field) {
        Fields.push_back(dynamic_cast<TStorageField*>(Field));
        FieldCount++;
    }
    return Field;
}

//---------------------------------------------------------------------------
// ��������� ��������� (��������� ����)
void TStorageExcel::commit()
{
    //����� ������� 
    if (ReadOnly) {
        throw Exception("Can't commit the storage because it is read-only.");
        //return;
    }

    msexcel->SaveDocument(Workbook);
    Modified = true;
}

//---------------------------------------------------------------------------
// ����������� ����� �� ���������
// ������������ ������� ���������������
TStorageExcel::copyFieldsFrom(TStorage* storage)
{
    storage->copyFieldsToExcel(this);
}

//---------------------------------------------------------------------------
// ������ ����������� ����� �� Excel � Excel
TStorageExcel::copyFieldsToExcel(TStorage* storage)
{
    TStorage::fullCopyFields(this, storage);
}



