//---------------------------------------------------------------------------
#ifndef MSWORDWORKS
#define MSWORDWORKS

/*
	Класс для работ с документами MS Word



    Примеры ключей форматирования в шаблоне MergeDocument

    {MERGEFIELD "FieldName" \d} - Сохранить исходное форматирование
    {MERGEFIELD "FieldName" \# "0.00 руб"} - Форматировать в число с разделителями
    {MERGEFIELD "FieldName" \* Caps} - Форматирование строки
    {MERGEFIELD "FieldName" \" "dd.mm.yyyy г."} - Форматирование даты



    Примеры использования

    MSWordWorks msww;

    Variant document = msww.OpenWord("c:\\PROGRS\\help\\_project\\Word\\InsertPicToWord\\tmp\\test2.doc");

    // Вставка изображений
    msww.SetPictureToBookmark(document, "bookmarkname", "c:\\PROGRS\\help\\_project\\Word\\InsertPicToWord\\tmp\\podpis.bmp", 50, 50);
    msww.SetPictureToField(document, "fieldname", "c:\\PROGRS\\help\\_project\\Word\\InsertPicToWord\\tmp\\podpis.bmp", 200, 100);


    // Слияние (Формирование массива данных - экспорт в html - слияние)
    Variant data_table;
    int Bounds[4] = {1, 3, 1, 2};  // Первая строка должна содержать имена полей (Fields) в шаблоне документа
    data_table = VarArrayCreate(Bounds, 3,  varVariant);
    data_table.PutElement("acct_id", 1, 1);
    data_table.PutElement("fio", 1, 2);
    data_table.PutElement("00112233", 2, 1);
    data_table.PutElement("Ivanov Petr", 2, 2);
    data_table.PutElement("999666333", 3, 1);
    data_table.PutElement("Petrov Ivan", 3, 2);

    Variant MergeDocument = msww.MergeDocument(document, array_table);
    //или
    Variant MergeDocument = msww.MergeDocument(document, array_table, i, PagePerDocument); // i - Номер страницы, PagePerDocument


    // Отображение, сохранение
    msww.SetVisible(true);
    msww.SaveAsDocument(MergeDocument, "c:\\tmp\\1.doc");

    msww.CloseDocument(MergeDocument);
    msww.CloseDocument(document);

    VarClear(data_table);



NOTICES:

    SetTextToField(...)
    You must be shure that the Document contains fields is exactly of FormFields type not MergeField or others.
    Also you must set the Bookmark property for Fields inside the template document and use it as FieldName.

*/


#include "system.hpp"
#include <utilcls.h>
#include "Comobj.hpp"
#include "Ora.hpp"

#include <fstream.h>
#include "taskutils.h"


class MergeTable {
private:
public:
    MergeTable ();
    void __fastcall Free();
    void __fastcall AddField(int FieldIndex, const AnsiString &ColumnName);
    void __fastcall PutRecord(const AnsiString &Value, int RecordIndex, int FieldIndex);
    void __fastcall PutRecord(int FieldIndex, const AnsiString &Value);
    void __fastcall PrepareFields(int ColCount);
    void __fastcall PrepareRecords(int RowCount);
    void __fastcall ShrinkRecords(int RecCount = -1);
    void __fastcall Next();
    void __fastcall First();

    Variant head;                       // Массив данных
    Variant data;                       // Массив данных
    int CurrentRecordIndex;             // Индекс текущей строки в массиве данных

    //int StartIndex;
    int EndPage;
    int PagePerDocument;
    int RecCount;
    int FieldsCount;
    AnsiString ResultFileNamePrefix;    // Файл - результат
};

typedef MergeTable MERGETABLE;


//---------------------------------------------------------------------------
//
class MSWordWorks
{
private:
        Variant Documents;

public:
        void __fastcall SetDisplayAlerts(bool flg = true);
	    Variant __fastcall OpenWord();
	    Variant __fastcall OpenWord(const String &DocumentFileName, bool fAsTemplate = true);   // Оставлена для совместимости со старыми версиями программ. Убрать в дальнейшем.
        Variant __fastcall OpenDocument(const String &DocumentFileName, bool fAsTemplate = true);
        void __fastcall SaveAsDocument(Variant Document, String FileName/*, bool fAddToRecentFiles = true*/);
        void __fastcall CloseDocument(Variant Document, bool fCloseAppIfNoDoc = false);
        void __fastcall CloseApplication();
        Variant __fastcall GetDocument(int DocIndex = -1);
   		Variant __fastcall GetPage(Variant Document, int PageIndex = -1);
        void __fastcall SetActiveDocument(Variant Document);
        bool PasteFromClipboard();
        bool SetPictureToBookmark(Variant Document, String BookmarkName, String PictureFileName, int Width = 0, int Height = 0);
        void SetVisible(bool fVisible = true);
        void __fastcall SetTextToBookmark(Variant Document,String BookmarkName, WideString Text);
        void __fastcall SetTextToField(Variant Document, String FieldName, WideString Text);
        void __fastcall SetTextToFieldF(Variant Document, String FieldName, WideString Text);
        Variant __fastcall SetPictureToField(Variant Document, String FieldName, String PictureFileName, int Width = 0, int Height = 0);
        std::vector<String> __fastcall GetFormFields(Variant Document);
        void __fastcall FindTextForReplace(String Text, String ReplaceText, bool fReg = true);
        void __fastcall InsertPicture(Variant Document, String PictureFileName, int Width = 0, int Height = 0);
        Variant CreateTable(Variant Document, int nCols, int nRows);
        void __fastcall InsertText(Variant Document, WideString Text);
        void __fastcall MoveUpCursor(Variant Document);
        void __fastcall GoToBookmark(Variant Document, String BookmarkName);
        void __fastcall GoToText(Variant Document, String Text, bool fReg = true, bool fWord = true);
		int GetPagesCount(Variant Document);
        int GetCurrentPageNumber(Variant Document);
        Variant CopyPage(int PageNumber=0);
        void __fastcall PastePage(Variant Document, int PageNumber = -1);
        void __fastcall InsertFile(Variant Document, AnsiString FileName);

        Variant __fastcall MergeDocumentFromFile(Variant TemplateDocument, AnsiString DatasetFileName, int FirstRecordIndex = 0, int PagePerDocument = 0);
        std::vector<String> __fastcall MergeDocumentToFiles(Variant TemplateDocument, MERGETABLE &md);

        Variant __fastcall MergeDocument(Variant TemplateDocument, MERGETABLE &md, int StartIndex = 0);
        Variant __fastcall MergeDocument(Variant TemplateDocument, const Variant &ArrayData, int FirstRecordIndex = 0, int PagePerDocument = 0, int titleRowIndex = 0);

        std::vector<String> ExportToWordFields(TOraQuery* QTable, Variant Document, String Path, String Prefix, int PagePerDocument);

 	   	Variant WordApp;
        HWND Handle;

        typedef enum WdGoToItem{
            wdGoToStart = 101,
            wdGoToEnd = 102,
            wdGoToBookmark = -1 ,
            wdGoToComment = 6 ,
            wdGoToEndnote = 5 ,
            wdGoToEquation = 10 ,
            wdGoToField = 7 ,
            wdGoToFootnote = 4 ,
            wdGoToGrammaticalError= 14 ,
            wdGoToGraphic = 8 ,
            wdGoToHeading= 11 ,
            wdGoToLine = 3 ,
            wdGoToObject = 9 ,
            wdGoToPage = 1 ,
            wdGoToPercent = 12 ,
            wdGoToProofreadingError = 15 ,
            wdGoToSection = 0 ,
            wdGoToSpellingError = 13 ,
            wdGoToTable = 2
        }WdGoToItem;

        typedef enum WdInformation
        {
            wdActiveEndAdjustedPageNumber = 1,
            wdActiveEndSectionNumber = 2,
            wdActiveEndPageNumber = 3,
            wdNumberOfPagesInDocument = 4,
            wdHorizontalPositionRelativeToPage = 5,
            wdVerticalPositionRelativeToPage = 6,
            wdHorizontalPositionRelativeToTextBoundary = 7,
            wdVerticalPositionRelativeToTextBoundary = 8,
            wdFirstCharacterColumnNumber = 9,
            wdFirstCharacterLineNumber = 10,
            wdFrameIsSelected = 11,
            wdWithInTable = 12,
            wdStartOfRangeRowNumber = 13,
            wdEndOfRangeRowNumber = 14,
            wdMaximumNumberOfRows = 15,
            wdStartOfRangeColumnNumber = 16,
            wdEndOfRangeColumnNumber = 17,
            wdMaximumNumberOfColumns = 18,
            wdZoomPercentage = 19,
            wdSelectionMode = 20,
            wdCapsLock = 21,
            wdNumLock = 22,
            wdOverType = 23,
            wdRevisionMarking = 24,
            wdInFootnoteEndnotePane = 25,
            wdInCommentPane = 26,
            wdInHeaderFooter = 28,
            wdAtEndOfRowMarker = 31,
            wdReferenceOfType = 32,
            wdHeaderFooterType = 33,
            wdInMasterDocument = 34,
            wdInFootnote = 35,
            wdInEndnote = 36,
            wdInWordMail = 37,
            wdInClipboard = 38
        } WdInformation;

        typedef enum WdBuiltInProperty
        {
            wdPropertyTitle = 1,
            wdPropertySubject = 2,
            wdPropertyAuthor = 3,
            wdPropertyKeywords = 4,
            wdPropertyComments = 5,
            wdPropertyTemplate = 6,
            wdPropertyLastAuthor = 7,
            wdPropertyRevision = 8,
            wdPropertyAppName = 9,
            wdPropertyTimeLastPrinted = 10,
            wdPropertyTimeCreated = 11,
            wdPropertyTimeLastSaved = 12,
            wdPropertyVBATotalEdit = 13,
            wdPropertyPages = 14,
            wdPropertyWords = 15,
            wdPropertyCharacters = 16,
            wdPropertySecurity = 17,
            wdPropertyCategory = 18,
            wdPropertyFormat = 19,
            wdPropertyManager = 20,
            wdPropertyCompany = 21,
            wdPropertyBytes = 22,
            wdPropertyLines = 23,
            wdPropertyParas = 24,
            wdPropertySlides = 25,
            wdPropertyNotes = 26,
            wdPropertyHiddenSlides = 27,
            wdPropertyMMClips = 28,
            wdPropertyHyperlinkBase = 29,
            wdPropertyCharsWSpaces = 30
        } WdBuiltInProperty;
            
        typedef enum WdStoryType
        {
            wdMainTextStory = 1,
            wdFootnotesStory = 2,
            wdEndnotesStory = 3,
            wdCommentsStory = 4,
            wdTextFrameStory = 5,
            wdEvenPagesHeaderStory = 6,
            wdPrimaryHeaderStory = 7,
            wdEvenPagesFooterStory = 8,
            wdPrimaryFooterStory = 9,
            wdFirstPageHeaderStory = 10,
            wdFirstPageFooterStory = 11
        } WdStoryType;

        typedef enum WdSaveFormat
        {
            wdFormatDocument = 0,
            wdFormatTemplate = 1,
            wdFormatText = 2,
            wdFormatTextLineBreaks = 3,
            wdFormatDOSText = 4,
            wdFormatDOSTextLineBreaks = 5,
            wdFormatRTF = 6,
            wdFormatUnicodeText = 7,
            wdFormatEncodedText = 7,
            wdFormatHTML = 8
        } WdSaveFormat;

        typedef enum WdOpenFormat
        {
            wdOpenFormatAuto = 0,
            wdOpenFormatDocument = 1,
            wdOpenFormatTemplate = 2,
            wdOpenFormatRTF = 3,
            wdOpenFormatText = 4,
            wdOpenFormatUnicodeText = 5,
            wdOpenFormatEncodedText = 5,
            wdOpenFormatAllWord = 6,
            wdOpenFormatWebPages = 7
        } WdOpenFormat;

        typedef enum WdUnits
        {
            wdCharacter = 1,
            wdWord = 2,
            wdSentence = 3,
            wdParagraph = 4,
            wdLine = 5,
            wdStory = 6,
            wdScreen = 7,
            wdSection = 8,
            wdColumn = 9,
            wdRow = 10,
            wdWindow = 11,
            wdCell = 12,
            wdCharacterFormatting = 13,
            wdParagraphFormatting = 14,
            wdTable = 15,
            wdItem = 16
        } WdUnits;

        typedef enum WdPasteDataType
        {
            wdPasteOLEObject = 0,
            wdPasteRTF = 1,
            wdPasteText = 2,
            wdPasteMetafilePicture = 3,
            wdPasteBitmap = 4,
            wdPasteDeviceIndependentBitmap = 5,
            wdPasteHyperlink = 7,
            wdPasteShape = 8,
            wdPasteEnhancedMetafile = 9,
            wdPasteHTML = 10
        } WdPasteDataType;
};


//---------------------------------------------------------------------------
#endif
