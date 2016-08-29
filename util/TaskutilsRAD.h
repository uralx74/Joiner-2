/*******************************************************************************
    Библиотечный модуль taskutil.h
    Содержит вспомогательные функции

    Версия файла от 08.10.2014

    // Разбить и соединить строки
    vector<string> Explode(string& const str, string separator, bool addEmpty = true)
    string Implode(const vector<string> &pieces, const string &glue = "")
    vector<string> ExplodeByBackslash(string str, string separatorstart, string separatorend, vector<bool>& backslash, bool addEmpty = true)

    string& ReplaceAll(string& context, const string& from, const string& to)
    void trim(string& s)

    // Провера строк
    bool IsDate(string str)
    bool IsTime(string str)
    bool IsDataTime(string str)
    bool IsFloat(string str)
    bool IsInt(string str)
*******************************************************************************/

#ifndef TASKUTILS_H
#define TASKUTILS_H

#include <vector>
#include <classes.hpp>
//#include <MemDS.hpp>
#include <System.IOUtils.hpp>


using namespace std;

enum str_pad_type {STR_PAD_LEFT = 0, STR_PAD_RIGHT};

typedef struct { //  Структура для использования в функциях ExplodeByBackslash
    bool fBacksleshed;   // Признак того, что подстрока обрамлена с начала и конца соответствующими подстроками
    UnicodeString text;    // Выделенная подстрока
} EXPLODESTRING;

typedef struct { //  Структура для использования в функциях ExplodeByBackslash
    bool fBacksleshed;   // Признак того, что подстрока обрамлена с начала и конца соответствующими подстроками
    UnicodeString text;    // Выделенная подстрока
    int startpos; // Позиция в строке открывающей подстроки
    int endpos;   // Позиция в строке закрывающей подстроки
    String startsep;
    String endsep;
} EXPLODESTRING2;



//------------------------------------------------------------------------------
//
inline int MessageBoxInf(String msg, String title, unsigned short flags = MB_ICONINFORMATION + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
	return(Application-->MessageBox(msg.c_str(), title.c_str(), flags));
}

//------------------------------------------------------------------------------
//
inline int MessageBoxInf(String msg, unsigned short flags = MB_ICONINFORMATION + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
	return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}

//------------------------------------------------------------------------------
//
inline int MessageBoxQuestion(String msg, unsigned short flags = MB_ICONQUESTION + MB_YESNO + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}

//------------------------------------------------------------------------------
//
inline int MessageBoxStop(String msg, unsigned short flags = MB_ICONSTOP + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}


//------------------------------------------------------------------------------
//
void ExploreDirectory(HWND Handle, UnicodeString Path)
{
	ShellExecute(Handle, L"OPEN", Path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//------------------------------------------------------------------------------
void ExploreFile(HWND Handle, UnicodeString Path)
{
	ShellExecute(Handle, L"OPEN", L"EXPLORER", ("/select, " + Path).c_str(), NULL ,SW_NORMAL);
}

//------------------------------------------------------------------------------
// Создание массива типа varVariant
Variant CreateVariantArray(int RowCount, int ColCount)
{
    int Bounds[4] = {1, RowCount, 1, ColCount};
    return VarArrayCreate(Bounds, 3,  varVariant);
}

//------------------------------------------------------------------------------
// Создание массива типа varVariant
Variant CreateVariantArray(int RowCount)
{
    int Bounds[2] = {1, RowCount};
    return VarArrayCreate(Bounds, 3,  varVariant);
}

TDateTime __fastcall ReplaceDay(const TDateTime& dt, unsigned short day)
{
    unsigned short dd, mm, yyyy;
    dt.DecodeDate(&yyyy, &mm, &dd);
    return EncodeDate(yyyy, mm, day);
}

//------------------------------------------------------------------------------
// Создание массива типа varVariant
void RedimVariantArray(Variant *DataArray, int RowCount, int ColCount)
{

    int nSrcRows = VarArrayHighBound(*DataArray, 1);
    int nSrcCols = VarArrayHighBound(*DataArray, 2);

    Variant ResultArray = CreateVariantArray(RowCount, ColCount);
    if (RowCount > nSrcRows) RowCount = nSrcRows;
    if (ColCount > nSrcCols) ColCount = nSrcCols;
    for (int i = 1; i <= RowCount; i++)
        for (int j = 1; j <= ColCount; j++)
            ResultArray.PutElement(DataArray->GetElement(i, j), i, j);
            //ResultArray[i,j] = DataArray[i,j];

    VarClear(*DataArray);
    *DataArray = ResultArray;
}

/*//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
string Implode(const vector<string> &pieces, const string &glue = "")
{
	string a;
	int leng=pieces.size();
 	for(int i=0; i<leng; i++)
 	{
 		a+= pieces[i];
 		if (  i < (leng-1) )
 			a+= glue;
 	}
 	return a;
}     */

//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
UnicodeString Implode(const vector<UnicodeString> &pieces, const UnicodeString &glue = "")
{
	UnicodeString a;
	int leng=pieces.size();
 	for(int i=0; i<leng; i++)
 	{
 		a+= pieces[i];
 		if (  i < (leng-1) )
 			a+= glue;
 	}
 	return a;
}

//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
UnicodeString Implode(const vector<EXPLODESTRING> &pieces, const UnicodeString &glue = "")
{
	UnicodeString a;
	int leng=pieces.size();
 	for(int i=0; i<leng; i++)
 	{
 		a += pieces[i].text;
 		if (  i < (leng-1) )
 			a += glue;
 	}
 	return a;
}


/*//------------------------------------------------------------------------------
// Заменяет в строке все вхождения подстроки на другую подстроку
UnicodeString& ReplaceAll(UnicodeString& context, const UnicodeString& from, const UnicodeString& to)
{
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = context.find(from, lookHere)) != string::npos)
	{

 		context.replace(foundHere, from.size(), to);
 		lookHere = foundHere + to.size();
 	}
 	return context;
}
    */
/*
//------------------------------------------------------------------------------
// Заменяет в строке все вхождения подстроки на другую подстроку
string& ReplaceAll(string& context, const string& from, const string& to)
{
	size_t lookHere = 0;
	size_t foundHere;
	while((foundHere = context.find(from, lookHere)) != string::npos)
	{
 		context.replace(foundHere, from.size(), to);
 		lookHere = foundHere + to.size();
 	}
 	return context;
}
    */
/**/

//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанную подстроку
std::vector<UnicodeString> Explode(UnicodeString str, UnicodeString separator, bool addEmpty = true)
{   // Разбить строку на подстроки в вектор
    // addEmpty - добавлять в результат пустые строки

//    UnicodeString ss;



	std::vector<UnicodeString> results;
 	unsigned int found1 = 1;
 	unsigned int found2 = 1;
    int nLengthSep = separator.Length();
    //found = str.find_first_of(separator);

    //str = "=1";
 	found2 = PosEx(separator,str, 1);
 	while(found2 > 0) {
 		if(found2 > 0 || addEmpty){
            if (found2 > 0) {
 			    results.push_back(str.SubString(found1, found2-found1));
                 //ss =  str.SubString(found1, found2-found1);
            }
		}
        found1 = found2 + nLengthSep;
        if (found2 !=0)
 		    found2 = PosEx(separator, str, found1);
 	}
 	if(str.Length() > 0 || addEmpty){
        results.push_back(str.SubString(found1, str.Length()-found1+1));
        //ss =  str.SubString(found1, str.Length()-found1+1);

 		// results.push_back(str);   // Закомментированно перед отпуском 30.06.2015! Нужно дополнительно протестировать
	}
	return results;




}
/**/

/*
//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанную подстроку
vector<UnicodeString> Explode(UnicodeString &const str, UnicodeString separator, bool addEmpty = true)
{   // Разбить строку на подстроки в вектор
    // addEmpty - добавлять в результат пустые строки

	vector<UnicodeString> results;
 	unsigned int found;
    //found = str.find_first_of(separator);
 	found = str.Pos(separator);
 	while(found != string::npos){
 		if(found > 0 || addEmpty){
 			results.push_back(str.SubString(1, found));
 		}
 		str = str.SubString(1, found+1);
 		//found = str.find_first_of(separator);
 		found = str.Pos(separator);
 	}
 	if(str.Length() > 0 || addEmpty){
 		results.push_back(str);
	}
	return results;
} */


/*//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанные подстроки-маркеры начала и конца
vector<string> ExplodeByBackslash(string str, string separatorstart, string separatorend, vector<bool>& backslash, bool addEmpty = true)
{
	vector<string> results;
 	unsigned int found_start, found_end;
    //found = str.find_first_of(separator);
    int seplength_start = separatorstart.length();
    int seplength_end = separatorend.length();

 	found_start = str.find(separatorstart);
    found_end = str.find(separatorend, found_start+seplength_start);

    // Если addEmpty добавляем подстроку до первой найденой подстроки
   /*if(addEmpty && found_start > 0){
 		results.push_back(str.substr(0, found_start));
       	backslash.push_back(false);
	}*/

/* 	while(found_start != string::npos && found_end != string::npos && found_start < found_end){
        // Если addEmpty и между закрывающей и новой открывающей есть текст, добавляем этот текст

        if (addEmpty && found_start > 0) {      // Если addEmpty добавляем подстроку до первой найденой подстроки
 			results.push_back(str.substr(/*found_end_old*//*0, found_start));
/*        	backslash.push_back(false);
        }

        // Добавляем строку в скобках
        results.push_back(str.substr(found_start, found_end+seplength_end-found_start));
        backslash.push_back(true);

        // Укорачиваем строку
 		str = str.substr(found_end+seplength_end);

        //found_end_old = found_end+seplength_end;
 		found_start = str.find(separatorstart, 0);
    	found_end = str.find(separatorend, found_start+seplength_start);
 	}

    // Если addEmpty, добавляем окончание строки
 	if(addEmpty && str.length()){
 		results.push_back(str);
       	backslash.push_back(false);
	}
	return results;
}    */



//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанные подстроки-маркеры начала и конца
// Расширенная версия функции ExplodeByBackslash
vector<EXPLODESTRING2> ExplodeByBackslash2(UnicodeString str, UnicodeString separatorstart, UnicodeString separatorend, bool addEmpty = true)
{
    //str = "_date(0,(0),0,0,'mm.yyyy')";

	vector<EXPLODESTRING2> result;
 	unsigned int found_start, found_end;

    int seplength_start = separatorstart.Length();
    int seplength_end = separatorend.Length();

 	found_start = str.Pos(separatorstart);
    found_end = 1;

    EXPLODESTRING2 item;

 	while(found_start != 0 && found_end != 0 /*&& found_start < found_end*/){
        if (addEmpty && found_start > 0) { // Фрагмент за скобками
            item.text = str.SubString(found_end, found_start-found_end);
            item.fBacksleshed = false;
            item.startpos = found_end;    // Надо тестировать!!!!!!!!!!!!!!!!!!
            item.endpos = found_end;      // Надо тестировать!!!!!!!!!!!!!!!!!!
            result.push_back(item);
        }
        found_end = PosEx(separatorend, str, found_start+seplength_start)+seplength_end;

        // Фрагмент в скобках
        item.text = str.SubString(found_start+seplength_start, found_end-found_start-seplength_start-seplength_end);
        item.fBacksleshed = true;
        item.startpos = found_end;    // Надо тестировать!!!!!!!!!!!!!!!!!!
        item.endpos = found_end;      // Надо тестировать!!!!!!!!!!!!!!!!!!
        item.startsep = separatorstart;    // Надо тестировать!!!!!!!!!!!!!!!!!!
        item.endsep = separatorend;      // Надо тестировать!!!!!!!!!!!!!!!!!!

        result.push_back(item);

 		found_start = PosEx(separatorstart, str, found_end);
 	}

    // Фрагмент за скобками в конце строки
 	if(addEmpty && (found_end < str.Length())){
        item.text = str.SubString(found_end, str.Length()-found_end+1);
        item.fBacksleshed = false;
        item.startpos = found_end;    // Надо тестировать!!!!!!!!!!!!!!!!!!
        item.endpos = found_end;      // Надо тестировать!!!!!!!!!!!!!!!!!!
 		result.push_back(item);
	}
	return result;
}


//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанные подстроки-маркеры начала и конца
vector<EXPLODESTRING> ExplodeByBackslash(UnicodeString str, UnicodeString separatorstart, UnicodeString separatorend, bool addEmpty = true)
{
	vector<EXPLODESTRING> result;
 	unsigned int found_start, found_end;

    int seplength_start = separatorstart.Length();
    int seplength_end = separatorend.Length();

 	found_start = str.Pos(separatorstart);
    found_end = 1;

    EXPLODESTRING item;

 	while(found_start != 0 && found_end != 0 /*&& found_start < found_end*/){
        if (addEmpty && found_start > 0) { // Фрагмент за скобками
            item.text = str.SubString(found_end, found_start-found_end);
            item.fBacksleshed = false;
            result.push_back(item);
        }
        found_end = PosEx(separatorend, str, found_start+seplength_start)+seplength_end;

        // Фрагмент в скобках
        item.text = str.SubString(found_start, found_end-found_start);
        item.fBacksleshed = true;

        result.push_back(item);

 		found_start = PosEx(separatorstart, str, found_end);
 	}

    // Фрагмент за скобками в конце строки
 	if(addEmpty && (found_end < str.Length())){
        item.text = str.SubString(found_end, str.Length()-found_end+1);
        item.fBacksleshed = false;
 		result.push_back(item);
	}
	return result;
}

/*//------------------------------------------------------------------------------
// Усекает пробелы в строке слева и справа
void trim(string& s)
{
	size_t p = s.find_first_not_of(" \t");
	s.erase(0, p);
	p = s.find_last_not_of(" \t");
	if (string::npos != p)
	s.erase(p+1);
}  */

/*
//------------------------------------------------------------------------------
// Дополняет строку другой строкой заданной длинны
string str_pad(string input, int pad_length, string pad_string, int pad_type)
{
    int n = input.length();
    int npad = pad_string.length();
    string spad = "";
    while (n < pad_length)
    {
        spad = spad + pad_string;
        n = n + npad;
    }

    return pad_type == STR_PAD_LEFT? spad + input : input + spad ;
} */


//------------------------------------------------------------------------------
// Дополняет строку другой строкой заданной длинны
UnicodeString str_pad(const UnicodeString &input, int pad_length, const UnicodeString &pad_string, int pad_type)
{
    int n = input.Length();
    int npad = pad_string.Length();
    UnicodeString spad = "";
    while (n < pad_length)
    {
        spad = spad + pad_string;
        n = n + npad;
    }

    return pad_type == STR_PAD_LEFT? spad + input : input + spad ;
}


//------------------------------------------------------------------------------
// Дополняет строку другой строкой заданной длинны
UnicodeString str_pad(const UnicodeString &input, const UnicodeString &pad_string, int pad_length, int pad_type)
{
    int n = input.Length();
    int npad = pad_string.Length();
    UnicodeString spad = "";
    while (n < pad_length)
    {
        spad = spad + pad_string;
        n = n + npad;
    }

    return pad_type == STR_PAD_LEFT? spad + input : input + spad ;
}

//------------------------------------------------------------------------------
// Проверяет является ли строка Date dd.mm.yyyy
bool IsDate(String str)
{
    int l = str.Length();

    if ( l <= 0)
        return 0;

    if (l == 10) {
            if (str[3] == str[6] && (str[3] == '.' || str[3] =='/' || str[3] =='-' ) &&
                isdigit(str[1]) && isdigit(str[2]) && isdigit(str[4]) && isdigit(str[5]) && isdigit(str[7]) && isdigit(str[8]) && isdigit(str[9])&& isdigit(str[10]))
            {
                return true;
            }
     }
     return false;
}

//------------------------------------------------------------------------------
// Проверяет является ли строка Time hh:mm:ss
bool IsTime(String str)
{   
    int l = str.Length();

    if ( l <= 0)
        return 0;

    if (l == 8) {
            if (str[3] == str[6] && (str[3] == ':') &&
                isdigit(str[1]) && isdigit(str[2]) && isdigit(str[4]) && isdigit(str[5]) && isdigit(str[7]) && isdigit(str[8]))
            {
                return true;
            }
     }
     return false;
}

//------------------------------------------------------------------------------
//
bool IsDataTime(String str)
{   // Функция проверки является ли строка DataTime dd.mm.yyyy hh:mm:ss
    int l = str.Length();

    if ( l <= 0)
        return 0;

    if (l != 18)
        return false;

    return IsDate(str.SubString(0,10)) && IsTime(str.SubString(10,17));
}


//------------------------------------------------------------------------------
// Проверяет является ли строка числом с плавающей точкой
bool IsFloat(String str)
{   // Функция проверки является ли строка Float 9,9999999
    int l = str.Length();

    if ( l <= 0 )
        return false;

    bool oneSep = false;

    for (int i = 1; i <= l; i++) {
        char a = str[i];
        if (a == '1' || a== '2' || a == '3' || a == '4' || a== '5' ||
            a== '6' || a == '7' || a == '8' || a== '9' || a== '0') {
            continue;
        };

        if (a== '-' || a =='+') {     // если + или - не в начале строки
            if (i > 1)
                return false;
            else
                continue;
        }

        if  (a=='.' || a== ',') {
            if (i == 1 || oneSep)   // если больше одного разделителя дробной части
                return false;
            else {
                oneSep = true;
                continue;
            }
        }
        return false;
    }

    return oneSep;
}

//------------------------------------------------------------------------------
// Проверяет является ли строка целым числом
bool IsInt(String str)
{   // Функция проверки является ли строка Int 9999999
    int l = str.Length();

    if ( l <= 0)
        return 0;

    for (int i = 1; i <= l; i++) {
        char a = str[i];
        if (a == '1' || a== '2' || a == '3' || a == '4' || a== '5' ||
            a== '6' || a == '7' || a == '8' || a== '9' || a== '0') {
            continue;
        };

/*        if (a == '0') {             // если ноль в начале строки
            if (i == 1 && l > 1)
                return false;
            continue;
        } */

        if (a== '-' || a =='+') {     // если + или - не в начале строки
            if (i > 1)
                return false;
            else
                continue;
        }
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Закрывает любой процесс по его PID'у
bool __fastcall KillProcess(DWORD PID)
{
    bool ReturnCode = false;
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, PID);
    if (hProcess != NULL || hProcess != INVALID_HANDLE_VALUE)
    {
        if (TerminateProcess(hProcess, -1))
            ReturnCode = true;
        CloseHandle( hProcess );
    }
    return ReturnCode;
}

//----------------------------------------------------------------------------
// Создает путь к директории назначения
UnicodeString __fastcall CreateWorkDir(UnicodeString work_dir)
{
	UnicodeString tek_kat = ExtractFilePath(Application->ExeName);
    if (! SetCurrentDir(work_dir))
    {
		if (! CreateDir(work_dir))
			Application->MessageBox(L"Ошибка на диске C:\\ !",L"Операция прервана", MB_ICONSTOP + MB_OK);
    }
    SetCurrentDir(tek_kat);
	return (tek_kat);
}

//------------------------------------------------------------------------------
// Возращает полный путь к временному каталогу пользователя Windows
UnicodeString __fastcall GetTempPath()
{
	const unsigned long size = 512;
	//char TempDirectory[size];
	wchar_t TempDirectory[size];
	unsigned long Er = GetTempPath(size, TempDirectory);

    if(Er > size || Er == 0) {
		Er = GetLastError();
		MessageBoxStop("Error: " + IntToStr(int(Er)));
        return NULL;
    } else {
        return (UnicodeString)TempDirectory;
    }

}

//------------------------------------------------------------------------------
//
UnicodeString AddWhere(UnicodeString whereblock, UnicodeString condition, bool addif)
{
    if (!addif)
        return whereblock;
        
    condition = condition.Trim();
    if (condition.Length() == 0)
        return whereblock;

    whereblock = whereblock.Trim();
    int w_length = whereblock.Length();

    if (w_length > 5) {
        whereblock = whereblock + " AND " + condition;
    } else {
        whereblock = "WHERE " + condition;
    }

    return whereblock;
}


/*//---------------------------------------------------------------------------
// Получить hExcelWindow
HWND __fastcall GetExcelPID(Variant appl)
{
    String ExcelCaption = appl.OlePropertyGet("Caption");
    HWND hExcW = FindWindow("XLMAIN", ExcelCaption.c_str());
    return(hExcW);
}*/

/*//----------------------------------------------------------------------------
// Инкримент/декримент даты, назначение даты
TDateTime __fastcall MathDate(TDateTime dt, int dcount, int mcount, int ycount, int dvalue=0, int mvalue=0, int yvalue=0)
{
    unsigned short dd,mm,yy;

    dt = dt - dcount;   // сдвигаем на dcount дней
    DecodeDate(dt,yy, mm,dd);

    if (dvalue != 0)
        dd = dvalue;
    if (mvalue != 0)
        mm = mvalue;
    if (yvalue != 0)
        yy = yvalue;

    int mounth1 = (mm+mcount);

    // Недоделаное
    //mounth1 !=0 ? yy = yy + (unsigned short) (mounth1)/12 : yy = yy - mounth1/12
    //yy = yy+(int)(/12);

    int mounth = (mm+mcount) % 12;
    mounth <= 0 ? mm = 12 + mounth: mm = mounth ;


    //int t = (int)((mm+mcount)/12);
    try {
        return EncodeDate(yy, mm,1);
    } catch (...) {
        return dt;
    }
}*/


//---------------------------------------------------------------------------
// Структура Caption-Value
typedef struct {
    UnicodeString Caption;
    UnicodeString Value;
} TValueListItem;

//---------------------------------------------------------------------------
// Класс для хранения структур Caption-Value
class TValueList {
public:
    __fastcall TValueList();
    __fastcall ~TValueList();
    void Free();
    TValueListItem* GetItem(int ItemIndex);
    //UnicodeString GetValue(int ItemIndex);
    //UnicodeString GetCaption(int ItemIndex);
    void AddItem(UnicodeString Caption, UnicodeString Value);
    int Size();
private:
    //TList
    TList* pItems;
protected:
    int size;

};

//---------------------------------------------------------------------------
// ValueList
__fastcall TValueList::TValueList()
{
    //Items = new TList;
    pItems = new TList;
    size = 0;
}

//---------------------------------------------------------------------------
//
__fastcall TValueList::~TValueList()
{
    //Clear();
    Free();
    delete pItems;
    pItems = NULL;
}

//---------------------------------------------------------------------------
//
int TValueList::Size()
{
    return size;
}

//---------------------------------------------------------------------------
// Получить Item
TValueListItem* TValueList::GetItem(int ItemIndex)
{
    TValueListItem *Item = (TValueListItem*) pItems->Items[ItemIndex];
    return Item;
}
 /*
//---------------------------------------------------------------------------
// Получить Value
UnicodeString ValueList::GetValue(int ItemIndex)
{
    TValueListItem *Item = (ValueListItem*) Items->Items[ItemIndex];
    return Item->Value;
}

//---------------------------------------------------------------------------
// Получить Caption
UnicodeString ValueList::GetCaption(int ItemIndex)
{
    TValueListItem *Item = (ValueListItem*) Items->Items[ItemIndex];
    return Item->Caption;
}
   */
//---------------------------------------------------------------------------
// Добавить в список Caption-Value
void TValueList::AddItem(UnicodeString Caption, UnicodeString Value)
{
    TValueListItem* Item = new TValueListItem;
    Item->Caption = Caption;
    Item->Value = Value;
    pItems->Add(Item);
    size++;
}

//---------------------------------------------------------------------------
// Очистить список объектов
void TValueList::Free()
{
    for (int i = 0; i < size; i++) {
        delete (TValueListItem*)(pItems->Items[i]);
    }
    pItems->Clear();
    size = 0;
}


#endif TASKUTILS_H
