#include "MSWordWorks.h"


//---------------------------------------------------------------------------
//
MergeTable::MergeTable()
{
    FieldsCount = 0;
    CurrentRecordIndex = 1;
    RecCount = 0;
    PagePerDocument = 500;
}

//---------------------------------------------------------------------------
// Усечение (на самом деле лишь меняет значение RecCount)
void __fastcall MergeTable::ShrinkRecords(int RecCount)
{
    if (RecCount <0) {
        this->RecCount = CurrentRecordIndex - 1;
    } else {
        this->RecCount = RecCount;
    }
}

//---------------------------------------------------------------------------
// Подготовка (создание) массива данных
void __fastcall MergeTable::PrepareFields(int ColCount)
{
    VariantClear(head);
    FieldsCount = ColCount;
    head = CreateVariantArray(1, ColCount);
}

//---------------------------------------------------------------------------
//
void __fastcall MergeTable::PrepareRecords(int RowCount)
{
    VariantClear(data);
    RecCount = RowCount;
    data = CreateVariantArray(RowCount, FieldsCount);
}

//---------------------------------------------------------------------------
//
void __fastcall MergeTable::AddField(int FieldIndex, const AnsiString &FieldName)
{
     head.PutElement(FieldName, 1, FieldIndex);
}

//---------------------------------------------------------------------------
//
void __fastcall MergeTable::PutRecord(const AnsiString &Value, int RecordIndex, int FieldIndex)
{
    data.PutElement(Value, RecordIndex, FieldIndex);

    if (RecCount < CurrentRecordIndex)
        RecCount = CurrentRecordIndex;  // Возможно стоит сделать иначе для использования готовых массивов данных

}

//---------------------------------------------------------------------------
// Вставка значения в текущую строку
void __fastcall MergeTable::PutRecord(int FieldIndex, const AnsiString &Value)
{
    data.PutElement(Value, CurrentRecordIndex, FieldIndex);

}

//---------------------------------------------------------------------------
// Очистка массива
void __fastcall MergeTable::Free()
{
    VariantClear(data);
    VariantClear(head);

}

//---------------------------------------------------------------------------
//
void __fastcall MergeTable::Next()
{
    CurrentRecordIndex++;
    // Для предотвращения возможности выхода за пределы массива
    //if (CurrentRecordIndex > VarArrayHighBound(data,1) {
    //      RedimVariantArray(data, RecCount, fields.size()+100);
    //}
}

//---------------------------------------------------------------------------
//
void __fastcall MergeTable::First()
{
    CurrentRecordIndex = 1;
}

//---------------------------------------------------------------------------
//
void __fastcall MSWordWorks::SetDisplayAlerts(bool flg)
{
    WordApp.OlePropertySet("DisplayAlerts", flg);
}


//---------------------------------------------------------------------------
// Отрыть документ Word
Variant __fastcall MSWordWorks::OpenWord()
{
    Variant Document;
	try {
        // Создание объекта Word.Application
        WordApp = CreateOleObject("Word.Application.8");

        randomize();
        AnsiString OldTitle = WordApp.OlePropertyGet("Caption");
        AnsiString TempTitle = "Temp - " + IntToStr(random(1000000));
        WordApp.OlePropertySet("Caption", TempTitle);
        Handle = FindWindow("OpusApp", TempTitle.c_str());
        WordApp.OlePropertySet("Caption", OldTitle);

        // Отключить режим показа предупреждений.
        WordApp.OlePropertySet("DisplayAlerts", false);

        // Отключить проверку грамматики для ускорения работы
		WordApp.OlePropertyGet("Options").OlePropertySet("CheckSpellingAsYouType", false);
        WordApp.OlePropertyGet("Options").OlePropertySet("CheckGrammarAsYouType", false);
        WordApp.OlePropertyGet("Options").OlePropertySet("CheckGrammarWithSpelling", false);

        // Отображение Word.Application
        WordApp.OlePropertySet("Visible", false);

        // Инициализация ссылки на документы
	  	Documents = WordApp.OlePropertyGet("Documents");
    } catch (Exception &e) {
        try {
           CloseApplication();
        } catch(...) {}
        throw Exception(e);
    }
}

//---------------------------------------------------------------------------
// Отрыть документ Word - оставлена для совместимости со старыми версиями
Variant __fastcall MSWordWorks::OpenWord(const String &DocumentFileName, bool fAsTemplate)
{
    Variant Document;
	try {
        // Создание объекта Word.Application
        WordApp = CreateOleObject("Word.Application.8");

        randomize();
        AnsiString OldTitle = WordApp.OlePropertyGet("Caption");
        AnsiString TempTitle = "Temp - " + IntToStr(random(1000000));
        WordApp.OlePropertySet("Caption", TempTitle);
        Handle = FindWindow("OpusApp", TempTitle.c_str());
        WordApp.OlePropertySet("Caption", OldTitle);

        // Отключить режим показа предупреждений.
        WordApp.OlePropertySet("DisplayAlerts", false);

        // Отключить проверку грамматики для ускорения работы
		WordApp.OlePropertyGet("Options").OlePropertySet("CheckSpellingAsYouType", false);
        WordApp.OlePropertyGet("Options").OlePropertySet("CheckGrammarAsYouType", false);
        WordApp.OlePropertyGet("Options").OlePropertySet("CheckGrammarWithSpelling", false);

        // Отображение Word.Application
        WordApp.OlePropertySet("Visible", false);

        // Инициализация ссылки на документы
	  	Documents = WordApp.OlePropertyGet("Documents");

        // Выбор документа из файла
        if (fAsTemplate) {
        	// Каждый вновь создаваемый документ получает индекс Item = 1
        	Documents.OleProcedure("Add", DocumentFileName, false,0);
        	Document = Documents.OleFunction("Item",1); // Доступ к документу
        } else {
        	Document = Documents.OleFunction("Open", DocumentFileName);
        }
        return Document;
    } catch (Exception &e) {
        try {
            if (!VarIsEmpty(Document))     // Если шаблон открыт
                CloseDocument(Document);
        } catch(...) {}
        try {
           CloseApplication();
        } catch(...) {}
        throw Exception(e);
    }
}

//---------------------------------------------------------------------------
// Отрыть документ Word из файла
Variant __fastcall MSWordWorks::OpenDocument(const String &DocumentFileName, bool fAsTemplate)
{
    // Выбор документа из файла
    // У Ole-процедуры Open существует множество дополнительных параметров
    Variant Document;
    if (fAsTemplate) {
        // Открывается существующий документ
    	//Document = Documents.OleFunction("Open", DocumentFileName);
    	Document = Documents.OleFunction("Open", DocumentFileName);
    } else {
        // Создается новый документ, если открываемый документ - шаблон
    	// Каждый вновь создаваемый документ получает индекс Item = 1
    	Documents.OleProcedure("Add", DocumentFileName, false,0);
    	Document = Documents.OleFunction("Item",1); // Доступ к документу
    }
    return Document;
}

//---------------------------------------------------------------------------
// Получение документа по индексу
Variant __fastcall MSWordWorks::GetDocument(int DocIndex)
{
	try {
        if (DocIndex >= 0) {
            //int k = WordApp.OlePropertyGet("Documents").OlePropertyGet("Count");
	        return WordApp.OlePropertyGet("Documents").OleFunction("Item", DocIndex);
        } else
	        return WordApp.OlePropertyGet("ActiveDocument");
    } catch (...) {
    	return NULL;
    }
}

//---------------------------------------------------------------------------
//
Variant __fastcall MSWordWorks::GetPage(Variant Document, int PageIndex)
{
   	return Document.OlePropertyGet("Item", wdPropertyPages).OlePropertyGet("Value");
   	//return Document.OlePropertyGet("BuiltInDocumentProperties").OlePropertyGet("Item", wdPropertyPages).OlePropertyGet("Value");
}

//---------------------------------------------------------------------------
//
int MSWordWorks::GetCurrentPageNumber(Variant Document)
{
    int page = Document.OlePropertyGet("Selection").OlePropertyGet("Information", wdActiveEndPageNumber);
    return page;
}

//---------------------------------------------------------------------------
// Скрыть или показать документ
void MSWordWorks::SetVisible(bool fVisible)
{
	// Отображение Word.Application
	WordApp.OlePropertySet("Visible", fVisible);
}

//---------------------------------------------------------------------------
// Вставка текста в закладку (закладка заменяется изображением)
void __fastcall MSWordWorks::SetTextToBookmark(Variant Document, String BookmarkName, WideString Text)
{
    Variant Bookmark = Document.OlePropertyGet("Bookmarks").OleFunction("Item", (OleVariant)BookmarkName);
	Bookmark.OlePropertyGet("Range").OlePropertySet("Text", Text);
}

//---------------------------------------------------------------------------
// Вставка изображения в закладку (текст заменяется изображением)
bool MSWordWorks::SetPictureToBookmark(Variant Document, String BookmarkName, String PictureFileName, int Width, int Height)
{
    Variant Bookmarks=Document.OlePropertyGet("Bookmarks");
    Variant Bookmark = Bookmarks.OleFunction("Item", (OleVariant)BookmarkName);
    Bookmark.OlePropertyGet("Range").OlePropertyGet("InlineShapes").OleProcedure("AddPicture", PictureFileName, false, true);

    // Не проверено!
    Bookmark.OleProcedure("Delete");


    //vBookmark.OlePropertyGet("Range").OlePropertySet("Text","12");
    /*// Выбор Bookmark по имени
    Variant Bookmark = Bookmarks.OleFunction("Item", (OleVariant)BookmarkName);
    // Вставить изображение из файла
    Bookmark.OlePropertyGet("Range").OlePropertyGet("InlineShapes").OleProcedure("AddPicture", PictureFileName, false, true);
	//Второй вариант
	// Определение выбранного участка в документе
    //Variant Selection = WordApp.OlePropertyGet("Selection");
    //Variant Selection = Document.OlePropertyGet("Selection");

	// возможно нужно удалить!!!!
    /*Variant InlineShape, PictureShape;
    InlineShape =  Document.OlePropertyGet("InlineShapes").OleFunction("Item", 1);
    //InlineShape = Field.OlePropertyGet("Range").OlePropertyGet("InlineShapes").OleFunction("Item", i);

    // Конвертируем в Shape
    PictureShape = InlineShape.OleFunction("ConvertToShape");
    // Расположение изображения перед текстом
    PictureShape.OleFunction("ZOrder", 5);
    // Устанавливаем размер изображения
    if (Width != 0 && Height != 0) {
    	PictureShape.OlePropertySet("Width", Width);
    	PictureShape.OlePropertySet("Height", Height);
    } */
}

//---------------------------------------------------------------------------
// Возвращает вектор с именами полей FormFields
std::vector<String> __fastcall MSWordWorks::GetFormFields(Variant Document)
{
    // Рабочий вариант, но без проверки на существования поля с именем FieldName
    //Variant Field = Document.OlePropertyGet("FormFields").OleFunction("Item", (OleVariant)FieldName);
    //Field.OlePropertyGet("Range").OlePropertySet("Text", Text);

    Variant FormFields = Document.OlePropertyGet("FormFields");
    Variant Field = Unassigned;
    int n = FormFields.OlePropertyGet("Count");

    std::vector<String> vFormFields;
    vFormFields.reserve(n);


    for (int i = 1; i <= n; i++) {  // Цикл по каждому полю. Проверяем наличие поля FieldName
        String Name = UpperCase(FormFields.OleFunction("Item", i).OlePropertyGet("Result"));
        vFormFields.push_back(Name);
        /*if (FieldName == Name) {
            Field = Document.OlePropertyGet("FormFields").OleFunction("Item", (OleVariant)FieldName);
        }*/
    }

    return vFormFields;
}

//---------------------------------------------------------------------------
// Вставка текста в поле (поле заменяется текстом) - быстрый вариант, без проверки существования поля
void __fastcall MSWordWorks::SetTextToFieldF(Variant Document, String FieldName, WideString Text)
{
    // Рабочий вариант, но без проверки на существования поля с именем FieldName
    Variant Field = Document.OlePropertyGet("FormFields").OleFunction("Item", (OleVariant)FieldName);
    Field.OlePropertyGet("Range").OlePropertySet("Text", Text);
}

//---------------------------------------------------------------------------
// Вставка текста в поле (поле заменяется текстом)
void __fastcall MSWordWorks::SetTextToField(Variant Document, String FieldName, WideString Text)
{
    Variant FormFields = Document.OlePropertyGet("FormFields");
    Variant Field = Unassigned;
    FieldName = UpperCase(FieldName);
    int n = FormFields.OlePropertyGet("Count");
    for (int i = 1; i <= n; i++) {  // Цикл по каждому полю. Проверяем наличие поля FieldName
        String Name = UpperCase(FormFields.OleFunction("Item", i).OlePropertyGet("Result"));
        if (FieldName == Name) {
            Field = Document.OlePropertyGet("FormFields").OleFunction("Item", (OleVariant)FieldName);
        }
    }

    if (!Field.IsEmpty())
        Field.OlePropertyGet("Range").OlePropertySet("Text", Text);




/*  Variant Fields = Document.OlePropertyGet("MailMerge").OlePropertyGet("Fields");
    int FieldsCount = Fields.OlePropertyGet("Count");
    Variant Field = Fields.OleFunction("Item", 1);
    Variant Code = Field.OlePropertyGet("Code");
    String Text = code.OlePropertyGet("Text");
    String Type = Field.OlePropertyGet("Type");
*/

}

//---------------------------------------------------------------------------
// Вставка рисунка в поле (поле остается)
Variant __fastcall MSWordWorks::SetPictureToField(Variant Document, String FieldName, String PictureFileName, int Width, int Height)
{
    try {
        Variant Field = Document.OlePropertyGet("FormFields").OleFunction("Item", (OleVariant)FieldName);
        Variant InlineShapes = Field.OlePropertyGet("Range").OlePropertyGet("InlineShapes");

        InlineShapes.OleProcedure("AddPicture", PictureFileName, false, true);
        Variant InlineShape = InlineShapes.OleFunction("Item", 1);

        // Не проверено!
        Field.OleProcedure("Delete");

        if (Width != 0 || Height != 0) {
            InlineShape.OlePropertySet("Width", Width);
            InlineShape.OlePropertySet("Height", Height);
        }

        return InlineShape;
    } catch (Exception &e) {
        throw(Exception("Exception has occurred.\nFile \"" + PictureFileName + "\" not found."));
    }

    // Получаем номер вставленного изображения
    //int index = Range.OlePropertyGet("InlineShapes").OlePropertyGet("Count");

    // Устанавливаем расположение на листе
    //PictureShape.OleFunction("IncrementLeft",30);
    //PictureShape.OleFunction("IncrementTop",40);
}

//---------------------------------------------------------------------------
// Поиск текста с заменой
void __fastcall MSWordWorks::FindTextForReplace(String Text, String ReplaceText, bool fReg)
{
        // Определение выбранного участка в документе
        //Variant Selection = Document.OlePropertyGet("Selection");
        Variant Selection = WordApp.OlePropertyGet("Selection");

        // Поиск текста по всему документу
		Variant Find = Selection.OlePropertyGet("Find");
        Find.OleProcedure("Execute", Text/*Текст, который будем менять*/, fReg/*учитывать регистрe*/,
        	false/*Только полное слово*/,false/*Учитывать универсальные символы*/,false/*Флажок Произносится как*/,
        	false/*Флажок Все словоформы*/,true/*Искать вперед*/,1/*Активация кнопки Найти далее*/,
        	false/* Задание формата */, ReplaceText/*На что заменить*/,2/*Заменить все*/);   // Этот вариант работает

        /*
        Find.OleProcedure("ClearFormatting");                                         // Этот вариант НЕ работает, надо разбираться
        Find.OlePropertyGet("Replacement").OleProcedure("ClearFormatting");
        Find.OlePropertySet("Text",Text);
        Find.OlePropertyGet("Replacement").OlePropertySet(Text,ReplaceText);
        Find.OlePropertySet("Forward",True);
        Find.OlePropertySet("Wrap",1);
        Find.OlePropertySet("Format",False);
        Find.OlePropertySet("MatchCase",False);
        Find.OlePropertySet("MatchWholeWord",False);
        Find.OlePropertySet("MatchWildcards",False);
        Find.OlePropertySet("MatchSoundsLike",False);
        Find.OlePropertySet("MatchAllWordForms",False);
        Find.OleProcedure("Execute",2);   /**/
}

//---------------------------------------------------------------------------
// (недоделано!) копирование страницы по номеру   (недоделано!)(недоделано!)(недоделано!)(недоделано!)(недоделано!)
Variant MSWordWorks::CopyPage(int PageNumber)
{
	//Variant Selection = Document.OlePropertyGet("Selection");
	// Переместить курсор
    WordApp.OlePropertyGet("Selection").OleFunction("WholeStory");
    //CurrentSelection.OleProcedure("MoveUp", wdLine, 1, wdExtend);
    //Selection.MoveUp Unit:=wdLine, Count:=1, Extend:=wdExtend
    return WordApp.OlePropertyGet("Selection").OleFunction("Copy");
}

//---------------------------------------------------------------------------
//  Вставка страницы по номеру(недоделано!)(недоделано!)(недоделано!)(недоделано!)(недоделано!)(недоделано!)(недоделано!)
void __fastcall MSWordWorks::PastePage(Variant Document, int PageNumber)
{
    Variant Selection = WordApp.OlePropertyGet("Selection");
    //Variant Selection = Document.OlePropertyGet("Selection");
	// Переместить курсор в конец документа
    Selection.OleProcedure("EndKey", wdStory);
	//CurrentSelection.OleProcedure("InsertNewPage");
    Selection.OleFunction("Paste");
    //Selection.OleFunction("PasteAndFormat", 0);
    //Selection.OleFunction("PasteAndFormat", Page);
}

//---------------------------------------------------------------------------
//
void __fastcall MSWordWorks::InsertFile(Variant Document, AnsiString FileName)
{
    // Переместить курсор в конец документа
    Variant Selection = Document.OlePropertyGet("Selection");
    Selection.OleProcedure("EndKey", wdStory);
    Selection.OleProcedure("InsertFile", FileName.c_str(), "", false, false);
}

//---------------------------------------------------------------------------
// Переместить курсор в начало документа (недоделано!)
void __fastcall  MSWordWorks::MoveUpCursor(Variant Document)
{
	//Variant Selection = Document.OlePropertyGet("Selection");
	// Переместить курсор
    Variant Selection = Document.OlePropertyGet("Selection");
	Selection.OleProcedure("MoveUp", 7, 1);
}

//---------------------------------------------------------------------------
// Сохранить в файл
void __fastcall MSWordWorks::SaveAsDocument(Variant Document, String FileName/*, bool fAddToRecentFiles*/)
{
    // Сохранение документа в файл
    // У данной Ole-процедуры множество дополнительных параметров
    // Также существует аналог этой процедуры - SaveAs2
    Document.OleProcedure("SaveAs", FileName);
}

//---------------------------------------------------------------------------
//
void __fastcall MSWordWorks::SetActiveDocument(Variant Document)
{
	// Активация документа
    Document.OleProcedure("Activate");
}

//---------------------------------------------------------------------------
// Создать таблицу
Variant MSWordWorks::CreateTable(Variant Document, int nCols, int nRows)
{
    Variant range = Document.OleFunction("Range");
	// Создане таблицы в области Range
    Document.OlePropertyGet("Tables").OleProcedure("Add", range, (OleVariant) nCols, (OleVariant) nRows);

	// Выбор существующей таблицы
	//Table = Tables.OleFunction("Item", 1);
	//RowCount = Table.OlePropertyGet("Rows").OlePropertyGet("Count");
	//ColCount = Table.OlePropertyGet("Columns").OlePropertyGet("Count");
}

//---------------------------------------------------------------------------
// Перейти к закладке
void __fastcall MSWordWorks::GoToBookmark(Variant Document, String BookmarkName)
{
    Document.OleFunction("Range").OleProcedure("GoTo", wdGoToBookmark, 0, 0, WideString(BookmarkName));
//    Document.OlePropertyGet("Selection").OleProcedure("GoTo",(int)-1, 0, 0, WideString(BookmarkName));
}

//---------------------------------------------------------------------------
// Перейти к тексту
void __fastcall MSWordWorks::GoToText(Variant Document, String Text, bool fReg, bool fWord)
{
    // Определение выбранного участка в документе
    //Variant Selection = WordApp.OlePropertyGet("Selection");
    //CurrentSelection = WordApp.OlePropertyGet("Selection");

    Variant Selection = Document.OleFunction("Range");

    // Поиск текста по всему документу
	Variant Find = Selection.OlePropertyGet("Find");
    Find.OleProcedure("Execute", Text/*Текст, который будем менять*/, fReg/*учитывать регистрe*/,
        fWord/*Только полное слово*/,false/*Учитывать универсальные символы*/,false/*Флажок Произносится как*/,
        false/*Флажок Все словоформы*/,true/*Искать вперед*/,1/*Активация кнопки Найти далее*/,
        false/* Задание формата */, 0/*На что заменить*/,0/*Заменить все*/);   // Этот вариант работает

    //СurrentSelection.OleProcedure("GoTo",(int)-1, 0, 0, WideString(BookmarkName));
}

//---------------------------------------------------------------------------
// Вставить изображение
void __fastcall MSWordWorks::InsertPicture(Variant Document, String PictureFileName, int Width, int Height)
{
     // Вставить изображение из файла в позицию CurrentSelection
     Variant Selection = Document.OleFunction("Range");
     Selection.OlePropertyGet("InlineShapes").OleProcedure("AddPicture", "C:\\_project\\InsertPicToWord\\tmp\\podpis.bmp", false, true);
}

//---------------------------------------------------------------------------
// Вставить текст
void __fastcall MSWordWorks::InsertText(Variant Document, WideString Text)
{
     // Вставить текст в позицию CurrentSelection
     Variant Selection = Document.OleFunction("Range");
     Selection.OleProcedure("TypeText", Text);
}

//---------------------------------------------------------------------------
// Вставить из Clipboard (необходимо следить за содержимым Clipboard)
bool MSWordWorks::PasteFromClipboard()
{
     // Вставить из буффера
	WordApp.OlePropertyGet("Selection").OleFunction("Paste");
    return true;
}

//---------------------------------------------------------------------------
//  Закрытие приложения Word с закрытием всех открытых документов
void __fastcall MSWordWorks::CloseApplication()
{
	// Закрытие приложения Word (с запосом на сохранение документа)
    if (!WordApp.IsEmpty()) {
        Variant document;
        while (Documents.OlePropertyGet("Count") > 0) {
            document = WordApp.OlePropertyGet("ActiveDocument");
            document.OleFunction("Close", false);
        }
	    WordApp.OleProcedure("Quit");
    }
}

//---------------------------------------------------------------------------
// Закрытие документа
void __fastcall MSWordWorks::CloseDocument(Variant Document, bool fCloseAppIfNoDoc)
{
	Document.OleFunction("Close", false);

    if (fCloseAppIfNoDoc && Documents.OlePropertyGet("Count") == 0) {
        CloseApplication();
    }
}

//---------------------------------------------------------------------------
// Определение количества страниц
int MSWordWorks::GetPagesCount(Variant Document)
{
	return Document.OlePropertyGet("BuiltInDocumentProperties").OlePropertyGet("Item", wdPropertyPages).OlePropertyGet("Value");
}

//---------------------------------------------------------------------------
// Слияние с объектом MERGETABLE в файл
std::vector<String> __fastcall MSWordWorks::MergeDocumentToFiles(Variant TemplateDocument, MERGETABLE &md)
{
    //md.TemplateDocument = OpenWord(md.TemplateFileName, true);

    int nFiles;
    if (md.PagePerDocument <= 0) {          // Расчитываем кол-во результирующих файлов
        md.PagePerDocument = md.RecCount;
        nFiles = 1;
    } else {
        nFiles = ceil((double)md.RecCount/(double)md.PagePerDocument);
    }

    int nPad = IntToStr(nFiles).Length();  // Кол-во знаков в индексе в имени файлов

    std::vector<String> vFiles;
    vFiles.reserve(nFiles);

    int FileIndex = 0;
    for (int i = 1; i <= md.RecCount; i = i + md.PagePerDocument) {
        FileIndex++;
        AnsiString filename = md.ResultFileNamePrefix + str_pad(IntToStr(FileIndex), nPad, "0", STR_PAD_LEFT) + ".doc";
        Variant ResultDocument = MergeDocument(TemplateDocument, md, i);

        // Сохраняем документ, без помещения его в список последних файлов (AddToRecentFiles = false - 6й параметр)
        try {
            ResultDocument.OleProcedure("SaveAs", filename, 0, false, "", false);
        } catch (Exception &e) {
            throw Exception("Ошибка при сохранении в файл\n" + filename);

        }
        CloseDocument(ResultDocument);

        vFiles.push_back(filename);
    }
    //CloseDocument(TemplateDocument);

    return vFiles;
    //return FileIndex;
}

//---------------------------------------------------------------------------
// Слияние с объектом MERGETABLE в объект Word - Document
Variant __fastcall MSWordWorks::MergeDocument(Variant TemplateDocument, MERGETABLE &md, int StartIndex)
{
    randomize();
    AnsiString TmpFileName = "ds" + IntToStr(random(100000000)) + ".html";

    TmpFileName = GetTempPath() + TmpFileName;

    int ArrayRowsCount = md.RecCount;

    //int ArrayRowsCount = VarArrayHighBound(ArrayData.data, 1) - VarArrayLowBound(ArrayData.data, 1)+1;
    //int ArrayColsCount = VarArrayHighBound(md.data, 2) - VarArrayLowBound(md.data, 2)+1;

    //int ArrayRowsCount = ArrayData.data2[0].size();
    //int ArrayColsCount1 = md.FieldsCount;

    //int startCol = VarArrayLowBound(ArrayData.data, 2);
    int startCol = 1;
    int LastRecordIndex;

    if (StartIndex <= 0 )
        StartIndex = 1;
    int PagesCount = StartIndex + md.PagePerDocument-1;
    if (PagesCount > md.RecCount)
        PagesCount = md.RecCount;


    ofstream out(TmpFileName.c_str());

    // Заголовок HTML
    out<<"<html>\n";
    out<<"<head><META http-equiv=""content-type"" content=""text/html; charset=windows-1251""></head>\n";
    out<<"<body>\n<table>";

    // Заголовки
    out<<"<tr>";
    for (int j = startCol; j <= md.FieldsCount; j++)
    {
        AnsiString s ="<td>"  + md.head.GetElement(1, j) + "</td>";
        out<< s.c_str();
    }
    out<<"</tr>";
    out<<"\n";

    // Тело
    for (int i = StartIndex; i <= PagesCount; i++)
    {
        out<<"<tr>";
        for (int j = startCol; j <= md.FieldsCount; j++)
        {
            //AnsiString s ="<td>#@#sep_"  + md.data.GetElement(i, j) + "</td>";  // Закомментировано 2016-07-21
            AnsiString s ="<td>"  + md.data.GetElement(i, j) + "</td>";
            out<< s.c_str();
        }
        out<<"</tr>";
        out<<"\n";
    }
    out<<"</table></body></html>";

    out.close();

    Variant Document = MergeDocumentFromFile(TemplateDocument, TmpFileName, 1, PagesCount);

    //FindTextForReplace("#@#sep_", ""); // Закомментировано 2016-07-21
    for (int i = 0; i < 5; i ++) {      // Delete temporary file
        if (remove(TmpFileName.c_str()) == 0)
            break;
        Sleep(500);
    }  

    return Document;
}

/*
//---------------------------------------------------------------------------
// Слияние с объектом MERGETABLE в объект Word - Document
Variant __fastcall MSWordWorks::MergeDocument(Variant TemplateDocument, MERGETABLE &md, int FirstRecordIndex)
{
    randomize();
    AnsiString TmpFileName = "ds" + IntToStr(random(100000000)) + ".html";
    TmpFileName = GetTempPath() + TmpFileName;

    int ArrayRowsCount = md.RecCount;

    //int ArrayRowsCount = VarArrayHighBound(ArrayData.data, 1) - VarArrayLowBound(ArrayData.data, 1)+1;
    int ArrayColsCount = VarArrayHighBound(md.data, 2) - VarArrayLowBound(md.data, 2)+1;

    //int ArrayRowsCount = ArrayData.data2[0].size();
    int ArrayColsCount1 = md.fields.size();

    //int ArrayRowsCount = VarArrayHighBound(ArrayData.data, 1) - VarArrayLowBound(ArrayData.data, 1)+1;
    //int ArrayColsCount = VarArrayHighBound(ArrayData.data, 2) - VarArrayLowBound(ArrayData.data, 2)+1;

    //int startCol = VarArrayLowBound(ArrayData.data, 2);
    int startCol = 1;
    int LastRecordIndex;

    if (FirstRecordIndex <= 0 )
        FirstRecordIndex = 1;

    //int delta = ArrayRowsCount - FirstRecordIndex + 1;
    int PagesCount;

    if (md.PagePerDocument <= 0)
    {
        //PagePerDocument = ArrayRowsCount - FirstRecordIndex + 1;
        PagesCount = md.RecCount;
    } else if (md.PagePerDocument >= delta) {
        //PagePerDocument = delta;
        PagesCount = md.PagePerDocument;
    }
    LastRecordIndex = FirstRecordIndex + PagePerDocument - 1;

    int FieldsCount_Document = TemplateDocument.OlePropertyGet("Fields").OlePropertyGet("Count");

    ofstream out(TmpFileName.c_str());

    out<<"<html>\n";
    out<<"<head><META http-equiv=""content-type"" content=""text/html; charset=windows-1251""></head>\n";
    out<<"<body>\n<table>";


    // Заголовки
    out<<"<tr>";
    for (std::map<AnsiString, int>::iterator field = ArrayData.fields.begin(); field != ArrayData.fields.end(); ++field)
    {
        AnsiString s ="<td>"  + field->first + "</td>";
        out<< s.c_str();
    }
    out<<"</tr>";
    out<<"\n";

    // Тело
    for (int i = FirstRecordIndex; i <= LastRecordIndex; i++)
    {
        out<<"<tr>";
        for (int j = startCol; j <= ArrayColsCount; j++)
        {

            //AnsiString s ="<td>"  + ArrayData.data2[i][j] + "</td>";
            AnsiString s ="<td>"  + ArrayData.data.GetElement(i, j) + "</td>";
            out<< s.c_str();
        }
        out<<"</tr>";
        out<<"\n";
    }
    out<<"</table></body></html>";

    out.close();


    Variant Document = MergeDocumentFromFile(TemplateDocument, TmpFileName, 1, PagePerDocument);

    for (int i = 0; i < 5; i ++) {      // Delete temporary file
        if (remove(TmpFileName.c_str()) == 0)
            break;
        Sleep(500);
    }

    return Document;
}  */

//---------------------------------------------------------------------------
//
Variant __fastcall MSWordWorks::MergeDocument(Variant TemplateDocument, const Variant &ArrayData, int FirstRecordIndex, int PagePerDocument, int titleRowIndex)
{
    randomize();
    AnsiString TmpFileName = "ds" + IntToStr(random(100000000)) + ".html";
    TmpFileName = GetTempPath() + TmpFileName;


    int ArrayRowsCount = VarArrayHighBound(ArrayData, 1) - VarArrayLowBound(ArrayData, 1)+1;
    int ArrayColsCount = VarArrayHighBound(ArrayData, 2) - VarArrayLowBound(ArrayData, 2)+1;

    //int startRow = VarArrayLowBound(*ArrayData, 1);
    int startCol = VarArrayLowBound(ArrayData, 2);
    int LastRecordIndex;


    if (titleRowIndex <= 0)
        titleRowIndex = VarArrayLowBound(ArrayData, 1);

    if (FirstRecordIndex <= 0 )
        FirstRecordIndex = titleRowIndex + 1;

    int delta = ArrayRowsCount - FirstRecordIndex + 1;
    if (PagePerDocument <= 0)
    {
        PagePerDocument = ArrayRowsCount - FirstRecordIndex + 1;
    } else if (PagePerDocument >= delta) {
        PagePerDocument = delta;
    }

    LastRecordIndex = FirstRecordIndex + PagePerDocument - 1;

/*    if (RecordCount <= 0)
    {
        LastRecordIndex = VarArrayHighBound(ArrayData, 1);
    }
    else
    {
        LastRecordIndex = FirstRecordIndex + RecordCount;
        if (LastRecordIndex > ArrayRowsCount) {
            LastRecordIndex = ArrayRowsCount;
            RecordCount =
        }
    }     */

    //int FieldsCount_Document = TemplateDocument.OlePropertyGet("Fields").OlePropertyGet("Count");

    //int FieldsCount = FieldsCount_Document < ArrayColsCount? FieldsCount_Document : ArrayColsCount;

    ofstream out(TmpFileName.c_str());

    out<<"<html>\n";
    out<<"<head><META http-equiv=""content-type"" content=""text/html; charset=windows-1251""></head>\n";
    out<<"<body>\n<table>";

    // Заголовки
    out<<"<tr>";
    for (int j = startCol; j <= ArrayColsCount; j++)
    {
        AnsiString s ="<td>"  + ArrayData.GetElement(titleRowIndex, j) + "</td>";
        out<< s.c_str();
    }
    out<<"</tr>";
    out<<"\n";


    for (int i = FirstRecordIndex; i <= LastRecordIndex; i++)
    {
        out<<"<tr>";
        for (int j = startCol; j <= ArrayColsCount; j++)
        {
            // AnsiString s ="<td>#@#sep_"  + ArrayData.GetElement(i, j) + "</td>";   // Закомментированно 2016-07-21
            AnsiString s ="<td>#@#sep_"  + ArrayData.GetElement(i, j) + "</td>";
            out<< s.c_str();
        }
        out<<"</tr>";
        out<<"\n";
    }
    out<<"</table></body></html>";

    out.close();

    Variant Document = MergeDocumentFromFile(TemplateDocument, TmpFileName, 1, PagePerDocument);
    //FindTextForReplace("#@#sep_", "");  // Закомментировано 2016-07-21

    for (int i = 0; i < 5; i ++) {      // Delete temporary file
        if (remove(TmpFileName.c_str()) == 0)
            break;
        Sleep(500);
    }

    return Document;
}

//---------------------------------------------------------------------------
// Слияние из готового файла с данными (html)
Variant __fastcall MSWordWorks::MergeDocumentFromFile(Variant TemplateDocument, AnsiString DatasetFileName, int FirstRecordIndex, int PagePerDocument)
{
    Variant ResultDocument;
    Variant MailMerge;
    Variant PasswordDocument, PasswordTemplate, WritePasswordDocument, WritePasswordTemplate, SQLStatement, SQLStatement1;
    PasswordDocument = "";
    PasswordTemplate = "";
    WritePasswordDocument = "";
    WritePasswordTemplate = "";
    SQLStatement = "";
    //SQLStatement = "SELECT * FROM [Лист1$]";
    SQLStatement = "SELECT * FROM `Table`";
    SQLStatement1 = "SELECT * FROM `Table`";
    String chemin, texte;
    texte = "test";

    MailMerge = TemplateDocument.OlePropertyGet("MailMerge");
    MailMerge.OlePropertySet("MainDocumentType", 0);    // wdFormLetters = 0
    //MailMerge.OleProcedure("OpenDataSource", chemin.c_str(), 1, true, true, false, false, PasswordDocument, PasswordTemplate, false, WritePasswordDocument, WritePasswordTemplate, texte.c_str(), SQLStatement, SQLStatement1, false);
    MailMerge.OleProcedure("OpenDataSource", DatasetFileName.c_str(), 0, false, false, true, false, PasswordDocument, PasswordTemplate, false, WritePasswordDocument, WritePasswordTemplate, texte.c_str(), SQLStatement, SQLStatement1, false);
    MailMerge.OlePropertySet("Destination", 0);
    MailMerge.OlePropertySet("SuppressBlankLines", 0);

    int LastRecordIndex;
    if (FirstRecordIndex <= 0)
        FirstRecordIndex = 1;           //wdDefaultFirstRecord = 1

    if (PagePerDocument  <= 0)
        LastRecordIndex = 0xFFFFFFF0;   //wdDefaultLastRecord = 0xFFFFFFF0
    else
        LastRecordIndex = FirstRecordIndex + PagePerDocument - 1;

    MailMerge.OlePropertyGet("DataSource").OlePropertySet("FirstRecord", FirstRecordIndex);
    MailMerge.OlePropertyGet("DataSource").OlePropertySet("LastRecord", LastRecordIndex);

    MailMerge.OleProcedure("Execute", false);

    // Free datasource
    MailMerge.OlePropertySet("MainDocumentType", 0xFFFFFFFF);    // wdNotAMergeDocument = 0xFFFFFFFF



    // Return the new document
    WordApp = MailMerge.OlePropertyGet("Application");
    return WordApp.OlePropertyGet("Documents").OleFunction("Item", 1);


    //return GetDocument(1);

    /*MailMerge.ExecFunction("OpenDataSource") <<XLSFileName               // Name
                                    <<0                         // Format
                                    <<false                     // ConfirmConversions
                                    <<false                     // ReadOnly
                                    <<true                      // LinkToSource
                                    <<false                     // AddToRecentFiles
                                    <<EmptStr                   // PasswordDocument
                                    <<EmptStr                   // PasswordTemplate
                                    <<false                     // Revert
                                    <<EmptStr                   // WritePasswordDocument
                                    <<EmptStr                   // WritePasswordTemplate
                                    <<"Entire Spreadsheet"      // Connection
                                    <<EmptStr                   // SQLStatement
                                    <<EmptStr                   // SQLStatement1
                                    <<false                     // OpenExclusive
                                    <<8                         // SubType
         );*/

}

//---------------------------------------------------------------------------
// Слияние из готового файла с данными (html)
std::vector<String> MSWordWorks::ExportToWordFields(TOraQuery* QTable, Variant Document, String Path, String Prefix, int PagePerDocument)
{
    try {
        Variant vFields = Document.OlePropertyGet("MailMerge").OlePropertyGet("Fields");
        int FieldCount = vFields.OlePropertyGet("Count");

        //String Type = Field.OlePropertyGet("Type");
        int RecordCount = QTable->RecordCount;

        // Создание объекта-таблицы для слияния
        MERGETABLE mergetable;
        mergetable.ResultFileNamePrefix = Path+Prefix;
        mergetable.PrepareFields(FieldCount);
        mergetable.PrepareRecords(RecordCount);
        mergetable.PagePerDocument = PagePerDocument;
        QTable->First();


        // Цикл по элементам MergeField
        // Заполняем шапку таблицы именами полей (должны быть = именам полей в Query)
        for (int i = 1; i <= FieldCount; i++) {
            Variant vField = vFields.OleFunction("Item", i);
            Variant vCode = vField.OlePropertyGet("Code");

            // Возможно следует доработать, так как ниже используется костыль
            // Получаем код поля и выделяем из него Имя поля
            // Mergefield имя опции
            String Text = vCode.OlePropertyGet("Text");
            Text = Text.Trim();
            AnsiString sepMergeField = "MERGEFIELD ";
            int SepPos = Text.Pos(sepMergeField) + sepMergeField.Length()-1;
            String FieldName = Text.SubString(SepPos+1, Text.Length()-SepPos).Trim();
            SepPos = FieldName.Pos(" ");
            if (SepPos == 0)
                SepPos = FieldName.Length();
            FieldName = FieldName.SubString(1,SepPos).Trim();

            // Удаляем поля MergeField из документа, если аналогичного поля нет в источнике
            if (QTable->FindField(FieldName) == NULL) {
                //vCode.OlePropertySet("Text", "проверка");     // Работает, но удаляет поле
                //vField.OleProcedure("Delete");                // Работает, но удаляет поле

                // Заменяем ненайденные в источнике поля на строки вида NA_имя_поля
                // возможно работает не оптимально, так как доступ к полю
                // осуществляется через WordApp.OlePropertyGet("Selection")
                // Возможны исключения если в данном экземпляре приложения Word
                // будут происходить паралельные процессы.
                vField.OleFunction("Select");
                Variant vSelection = WordApp.OlePropertyGet("Selection");
                Variant vrange = vSelection.OlePropertyGet("Range");
                vrange.OlePropertySet("Text", "NA_"+  FieldName);

                FieldCount--;
                i--;
            } else {
                mergetable.AddField(i, FieldName);
            }
        }

        // Заполнение массива данных
        for (int i = 1; i <= RecordCount; i++) {
            for (int j = 1; j <= FieldCount; j++) {
                String FieldName = mergetable.head.GetElement(1, j);

                mergetable.PutRecord(j, QTable->FieldByName(FieldName)->AsString);

                // Старый вариант
                // Заменяем ненайденные в источнике поля на строки вида NA_имя_поля
                // Работает не оптимально, так как производит эту процедуру для каждой строки
                // из источника данных. Логичнее убирать поля слияния ранее.
                //TField* Field = QTable->FindField(FieldName);
                //if (Field) {
                //    mergetable.PutRecord(j, QTable->FieldByName(FieldName)->AsString);
                //} else {    // Field does not exist in souce Query
                //    mergetable.PutRecord(j, "NA_" + FieldName);
                //}

            }

            QTable->Next();
            mergetable.Next();

        }

        MSWordWorks msword;
        return msword.MergeDocumentToFiles(Document, mergetable);
    } catch (Exception &e) {
        throw Exception(e); // Добавлено 2016-03-25. Проверить!
    }

    //return std::vector<String> ();
}

/*
SaveAs2 аналог SaveAs. Между ними сущетсвует небольшое отличие (см. параметр CompatibilityMode)

ActiveDocument.SaveAs2 FileName
        FileFormat:=wdFormatXMLDocument, LockComments:=False, Password:="", AddToRecentFiles _
        :=True, WritePassword:="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts _
        :=False, SaveNativePictureFormat:=False, SaveFormsData:=False, _
        SaveAsAOCELetter:=False, CompatibilityMode:=14
        


wdFormatDocument                    =  0
wdFormatDocument97                  =  0
wdFormatDocumentDefault             = 16
wdFormatDOSText                     =  4
wdFormatDOSTextLineBreaks           =  5
wdFormatEncodedText                 =  7
wdFormatFilteredHTML                = 10
wdFormatFlatXML                     = 19
wdFormatFlatXMLMacroEnabled         = 20
wdFormatFlatXMLTemplate             = 21
wdFormatFlatXMLTemplateMacroEnabled = 22
wdFormatHTML                        =  8
wdFormatPDF                         = 17
wdFormatRTF                         =  6
wdFormatTemplate                    =  1
wdFormatTemplate97                  =  1
wdFormatText                        =  2
wdFormatTextLineBreaks              =  3
wdFormatUnicodeText                 =  7
wdFormatWebArchive                  =  9
wdFormatXML                         = 11
wdFormatXMLDocument                 = 12
wdFormatXMLDocumentMacroEnabled     = 13
wdFormatXMLTemplate                 = 14
wdFormatXMLTemplateMacroEnabled     = 15
wdFormatXPS                         = 18
wdFormatOfficeDocumentTemplate      = 23
wdFormatMediaWiki                   = 24



//Documents.OleFunction("Open", FileName, ConfirmConversions, ReadOnly, AddToRecentFiles,
//    PasswordDocument, PasswordTemplate, Revert,
//    WritePasswordDocument, WritePasswordTemplate, Format   )

*/

