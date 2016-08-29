#include "pch.h"
#pragma hdrstop

#include "StorageText.h"


//---------------------------------------------------------------------------
// class TStorageSqlText: TStorage
// Класс объекта источника/приемника данных
// Используется, хранимая в файле Text
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
/*void TStorageSqlText::Append()
{
    if (hasChangedAfterAppend) {
        hasChangedAfterAppend = false;
        TStorage::Append();
    }
} */
                                       
TStorageSqlText::~TStorageSqlText()
{
}

void TStorageSqlText::open(bool ReadOnly)
{
    this->ReadOnly = ReadOnly;
   
    AnsiString templateFileName = Tables[TableIndex].Template;
    if (templateFileName != "") {
        if (!FileExists(templateFileName)) {
            throw Exception("File not found " + templateFileName + ".");
        }
        TStringList* pStringList;
        pStringList = new TStringList();
        pStringList->Clear();
        pStringList->LoadFromFile(templateFileName);
        SqlText = pStringList->Text;
        pStringList->Free();
    } else {
        throw Exception("File not found.");
    }
}


//Variant Get(AnsiString Field) {};

//---------------------------------------------------------------------------
// Задает значение текущего поля
void TStorageSqlText::setFieldValue(Variant Value)
{
    if (!isActiveField())
        return;

    String dstParam = "/**:" + Fields[FieldIndex]->name + "**/";
    TReplaceFlags replaceflags = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;

    String OldText = SqlText;
    if (!VarIsNull(Value)) {
        SqlText = StringReplace(SqlText, dstParam, Value, replaceflags);
    } else {
        SqlText = StringReplace(SqlText, dstParam, "null", replaceflags);
    }
    if (SqlText != OldText) {
        RecordCount++;
    }
}

//---------------------------------------------------------------------------
// Фиксирует текущее хранилище
void TStorageSqlText::commit()
{
    if (ReadOnly) {
        throw Exception("Can't commit the storage because it is read-only.");
    }

    TStringList* pStringList = new TStringList();

    if (!pStringList) {
        throw Exception("Can't allocate memmory.");
    }

    pStringList->Clear();
    pStringList->Text = SqlText;

    try {
        pStringList->SaveToFile(Tables[TableIndex].File);
        pStringList->Free();
        Modified = true;
        //RecordCount = 1;
        //RecordCount++;
    } catch (...) {
        pStringList->Free();
        throw Exception("Can't commit.");
    }




}

//---------------------------------------------------------------------------
//
/*void TStorageSqlText::closeTable()
{
    Active = false;
    TStorage::closeTable();
}
*/

//---------------------------------------------------------------------------
//
TSqlTextField* TStorageSqlText::addField()
{
    TSqlTextField* Field = new TSqlTextField();
    Fields.push_back(dynamic_cast<TSqlTextField*>(Field));
    Field->required = false;

    FieldCount++;
    return Field;
}

//---------------------------------------------------------------------------
//
void TStorageSqlText::addTable(const TSqlTextTable& Table)
{
    TSearchRec SearchRec;
    FindFirst(Table.Template, faAnyFile, SearchRec);
    if (SearchRec.Name != "") {     //Если файл найден
        Tables.push_back(Table);
        TableCount++;
    } else {
        FindClose(SearchRec);
        throw Exception("Шаблон не найден.");
    }

    /*if (SearchRec.Name != "") {     // Если файл найден, то формируем новый элемент-Таблицу
        AnsiString FilePath = ExtractFilePath(Table.File);
        do {
            TExcelTable NewTable;
            NewTable.File = FilePath + SearchRec.Name;
            NewTable.Truncate = Table.Truncate;
            Tables.push_back(NewTable);
            TableCount++;
        } while ( FindNext(SearchRec) == 0);
    } else {    // Если файл не найден, добавляем таблицу как есть
        Tables.push_back(Table);
        TableCount++;
    }*/
    FindClose(SearchRec);

}


//---------------------------------------------------------------------------
//
AnsiString TStorageSqlText::getTable()
{
    if (!eot()) {
        return Tables[TableIndex].File;
    }
}



