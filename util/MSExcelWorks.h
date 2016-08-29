//---------------------------------------------------------------------------
#ifndef MSEXCELWORKS
#define MSEXCELWORKS

/*******************************************************************************
	Класс для работ с OLE-обектом Excel.Application
    Версия от 10.11.2014


    Описание технологии работы с компонентом:
    1. Создать объект класса MSExcelWorks и выбрать книгу и лист для вывода данных
        MSExcelWorks msexcel;
        msexcel.OpenApplication();
        workbook = OpenDocument();
        Variant worksheet1 = msexcel.GetSheet(workbook, 1);
    2. Если необходимо изменять формат данных в ячейках, то создать вектор AnsiString:
        std::vector<AnsiString> format_body;
    3. Если необходимо изменять цвет текста, границ, выравнивания в ячейках, то создать переменную Variant
        и объект CELLFORMAT:
        Variant region_body;
        CELLFORMAT cf_body;
        cf_body.BorderStyle = CELLFORMAT::xlContinuous;
        cf_body.FontStyle = cfHead.FontStyle << CELLFORMAT::fsBold;
        cf_body.bSetFontColor = true;
        cf_body.FontColor = clRed;
        cf_body.bWrapText = false;
    4. Создать переменную-массив Variant для данных:
        Variant data_body;
        data_body = CreateVariantArray(1, FieldCount);
    5. Заполнить переменную-массив с данными
        data_body.PutElement("Value", i, j);
    6. Заполнить вектор
        format_body.push_back("@");
        format_body.push_back("0");
        format_body.push_back("ДД.ММ.ГГГГ");
        format_body.push_back("чч:мм:сс");
    7. Вывести данные в лист Excel
        region_body = msexcel.WriteTable(worksheet1, ArrayDataBody, 4 <номер строки>, 1 <номер столбца>, format_body);
    8. Задать формат ячеек:
        msexcel.SetRangeFormat(region_body, cf_body);
    9. Отобразить документ:
        msexcel.SetVisibleExcel(true, true);
    10. Выполнить очистку памяти:
        cf_body.clear();
        VarClear(ArrayDataHead);
        ArrayDataHead = NULL;

    ---
    CopyRange(worksheet, range_body, int Row, int Column, bool flag);
    где Row - отступ от первой строки range_body
    Column - отступ от первого столбца range_body
    flag - признак копировать ли вместе с форматированием диапазона данные

*******************************************************************************/

#include "system.hpp"
#include <utilcls.h>
#include "Comobj.hpp"
#include "sysutils.hpp"
#include "taskutils.h"
#include "OdacVcl.hpp"


class CellFormat {
public:
    __fastcall CellFormat();
    enum TDataAlignment {daDefault = 0, daTop = 2, daBottom = 4, daLeft = 2, daCenter = 3, daRight=4};
    enum TFontStyle {fsDefault = 0, fsNormal, fsBold, fsItalic, fsUnderline, fsStrikeOut};
    enum TBorderStyle {bsDefault = -1, bsNone = 0, xlContinuous = 1, bsBold = 7, bsDash1 = 2, bsDash2 = 3, bsDash3 = 4};
    enum TBorderLine {xlEdgeLeft = 7, xlEdgeTop, xlEdgeBottom, xlEdgeRight, xlInsideHorizontal, xlInsideVertical};
    AnsiString DataFormat;
    TDataAlignment HorizontalAlignment;
    TDataAlignment VerticalAlignment;
    TBorderStyle BorderStyle;
    int FontSize;
    bool bSetFontColor;
    unsigned long FontColor;
    bool bSetBorderColor;
    unsigned long BorderColor;
    bool bSetFillColor;
    unsigned long FillColor;
    Set <char, 0, 9> FontStyle;
    Set <char, 0, 12> BordeLine;
    char ShrinkToFit;
    char bWrapText;

protected:

private:

};

__fastcall CellFormat::CellFormat()
{
    BorderColor = RGB(0,0,0);
    FontColor = BorderColor;
    FontSize = 0;
    DataFormat = "";
    HorizontalAlignment = daDefault;
    VerticalAlignment = daDefault;
    FontStyle.Clear();
    //FontStyle = FontStyle << fsNormal;
    BordeLine = BordeLine << xlEdgeTop << xlEdgeLeft<< xlEdgeBottom << xlEdgeRight << xlInsideHorizontal << xlInsideVertical;
    BorderStyle = bsDefault;
    bSetFontColor = false;
    bSetBorderColor = false;
    bSetFillColor = false;
    ShrinkToFit = -1;
    bWrapText = -1;
}

typedef CellFormat CELLFORMAT;
typedef std::vector<AnsiString> DATAFORMAT;

class MSExcelWorks
{
private:

public:
    enum TExportStatus {ES_ERROR_NOT_ENOUGTH_FIELD = 0, ES_ERROR_RANGE_IS_NOT_SOLID, ES_ERROR_TOO_MUCH_RECORDS};
    //enum TDirection {Down = 1, Up, Left, Right};
    //enum xlBooleane {xlDefault = -1, xlFalse, xlTrue};
    Variant __fastcall OpenApplication();
    Variant __fastcall OpenDocument(AnsiString TemplateName = "");     //Запуск Excel
    //Variant __fastcall OpenWorksheetFromFile(AnsiString& FileName);
	void __fastcall CloseApplication();                     // Функция закрытия Excel
    void __fastcall CloseWorkbook(Variant Workbook, bool fCloseAppIfNoDoc = false);
    void __fastcall SaveDocument(Variant& workbook, const AnsiString& FileName = "");
    Variant __fastcall AddSheet(Variant& Book, AnsiString& SheetName, int SheetIndex = -1);
    void __fastcall SetActiveWorkbook(Variant& Workbook);
    void __fastcall SetActiveWorksheet(Variant& Worksheet);
    void __fastcall SetActiveRange(Variant& Worksheet, int firstRow, int firstCol, int lastRow = 0, int lastCol = 0);
	Variant __fastcall GetActiveSheet();
	Variant __fastcall GetSheet(Variant& Workbook, int SheetIndex = 1);
    Variant __fastcall GetRange(Variant& Worksheet, int firstRow, int firstCol, int countRow = 1, int countCol = 1);
    Variant __fastcall GetRangeByName(Variant& Worksheet, AnsiString& RangeName);
    Variant __fastcall GetRangeFromRange(Variant& range, int firstRow, int firstCol, int countRow = 1, int countCol = 1);
    std::vector<AnsiString> __fastcall GetNamesFromWorkbook(Variant& WorksheetOrWorkbook);
    std::vector<AnsiString> __fastcall GetNamesFromWorksheet(Variant& Worksheet);
    int __fastcall GetRangeRowsCount(Variant& range);
    int __fastcall GetRangeColumnsCount(Variant& range);
    AnsiString __fastcall GetRangeFormat(Variant& range);

    Variant __fastcall WriteTable(Variant& worksheet, const Variant &ArrayData,  int firstRow, int firstCol, std::vector<AnsiString> *DataFormat = NULL);
    Variant __fastcall WriteTable(Variant& worksheet, const Variant &ArrayData, AnsiString CellName, std::vector<AnsiString> *DataFormat = NULL);
    Variant __fastcall WriteTableToRange(Variant& range, const Variant &ArrayData,  int firstRow, int firstCol, std::vector<AnsiString> *DataFormat = NULL);


	Variant __fastcall WriteToRange(const AnsiString& txt, Variant range, AnsiString format = "");
	//Variant __fastcall WriteToRange(const AnsiString& txt, const AnsiString& sRangeName, AnsiString format = "");
	Variant __fastcall WriteToCell(Variant& worksheet, const AnsiString& txt, int Row, int Col, AnsiString format = "");
    Variant __fastcall WriteToCell(Variant& worksheet, const AnsiString& txt, AnsiString CellName, AnsiString format = "");
    Variant __fastcall WriteFormulaToCell(Variant& wst, const AnsiString& txt, int Row, int Col, bool fBold = false);
    Variant __fastcall WriteFormula(Variant& worksheet, const AnsiString& txt, int Row, int Col, int countRow = 1, int countCol = 1,  bool fBold = false);
    Variant __fastcall MergeCells(Variant& worksheet, int firstRow, int firstCol, int lastRow, int lastCol);

    void __fastcall SetRangeFormat(Variant& range, const CellFormat& cf, int firstRow, int firstCol, int countRow = 1, int countCol = 1);
    void __fastcall SetRangeFormat(Variant& range, const CellFormat& cf);
    void __fastcall SetRangeDataFormat(Variant& range, AnsiString& format);
	void __fastcall RangeFormat(Variant& wst, int firstRow, int CountRow, int firstCol, int lastCol, int Size, int Font_Color, int Inter_Color, bool Bold); // Инициализация ячеек
    void __fastcall ClearFormats(Variant& range);
    void __fastcall ClearWorksheet(Variant& Worksheet);
	void __fastcall DrawBorders(Variant& range, bool r7 = true, bool r8 = true, bool r9 = true, bool r10 = true, bool r11 = true, bool r12 = true); // Прорисовываем тонкими линиями решетку вокруг ячеек заданного диапазона
    void __fastcall SetRangeColumnsFormat(Variant& range, const std::vector<AnsiString> &cf);
    void __fastcall CopyRangeFormat(Variant& range_src, Variant& range_dst);
	void __fastcall RangeShtrich(Variant& wst, int firstRow, int CountRow, int firstCol, int lastCol, int Shtrich);
	void __fastcall SetColumnsAutofit(Variant& range);
    void __fastcall SetAutoFilter(Variant& range);
	void __fastcall SetRowsAutofit(Variant& range);

    void __fastcall SetRowHeight(Variant& range, int Height);
	//void __fastcall SetRowHeight(Variant& worksheet, int ColumnIndex, int width);
    void __fastcall SetColumnWidth(Variant& worksheet, int ColumnIndex, int width);
    void __fastcall SetColumnWidth(Variant& range, int Width);

	void __fastcall SetVisible(bool fVisible = true, bool fForeground = true); // Отобразить документ Excel
    void __fastcall SetVisible(Variant Workbook, bool fVisible = true, bool fForeground = true);
	void __fastcall DateTimeCreateDoc(Variant& wst, int Row, int Col);

    Variant __fastcall ReadRange(Variant& worksheet, int firstRow, int firstCol, int lastRow, int lastCol);
    AnsiString __fastcall ReadCell(Variant& worksheet, int Row, int Col); // Чтение данных из яйчеки таблицы Excell
    Variant __fastcall ReadCellFormula(Variant& worksheet, int Row, int Col); // Чтение данных из яйчеки таблицы Excell

    std::vector<AnsiString> __fastcall GetDataFormat(const Variant& ArrayData, int RowIndex);//, std::vector<CELLFORMAT> *formats);
    Variant CreateVariantArray(int RowCount, int ColCount);
    void RedimVariantArray(Variant& ArrayData, int RowCount);
    void __fastcall CopyArray(const Variant& SrcArrayData, Variant* ArrayData,  int srcFirstRow, int srcFirstCol, int srcLastRow, int srcLastCol, int dstFirstRow, int dstFirstCol);

    inline int GetRangeFirstRow(Variant range);
    inline int GetRangeFirstColumn(Variant range);

    void __fastcall FillDown(Variant& worksheet, Variant& range);
    void __fastcall InsertRows(Variant& worksheet, int RowIndex, int RowsCount);
    Variant __fastcall InsertRows(Variant& range);

    Variant __fastcall CopyRangeEx(Variant& worksheet, const Variant& range, int RowIndent = 0, int ColIndent = 0, bool fCopyData = true);
    Variant __fastcall CopyRangeEx(Variant& worksheet, AnsiString sRangeName, int RowIndent = 0, int ColIndent = 0, bool fCopyData = true);
    Variant __fastcall CopyRange(Variant& worksheet, const Variant& range, int Row = 1, int Col = 1, bool fCopyData = true);

    void ExportToExcelFields(TOraQuery* QTable, Variant Worksheet);
    Variant ExportToExcelTable(TOraQuery* QTable, Variant Worksheet, String RangeName, bool fUnbounded = true);

    bool __fastcall IsReadOnly(Variant& workbook);

    void BeforeUpdate();
    void AfterUpdate();


protected:
    Variant ExcelApp;
    Variant WorkBooks;
};

//---------------------------------------------------------------------------
#endif
