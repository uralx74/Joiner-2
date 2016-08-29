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

#include <vector.h>
#include <MemDS.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "Messages.h"


using namespace std;

enum str_pad_type {STR_PAD_LEFT = 0, STR_PAD_RIGHT};

typedef struct { //  Структура для использования в функциях ExplodeByBackslash
    bool fBacksleshed;   // Признак того, что подстрока обрамлена с начала и конца соответствующими подстроками
    AnsiString text;    // Выделенная подстрока
} EXPLODESTRING;

typedef struct { //  Структура для использования в функциях ExplodeByBackslash
    bool fBacksleshed;   // Признак того, что подстрока обрамлена с начала и конца соответствующими подстроками
    AnsiString text;    // Выделенная подстрока
    int startpos; // Позиция в строке открывающей подстроки
    int endpos;   // Позиция в строке закрывающей подстроки
    String startsep;
    String endsep;
} EXPLODESTRING2;


/*
//------------------------------------------------------------------------------
//
inline int MessageBoxInf(String msg, String title, unsigned short flags = MB_ICONINFORMATION + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), title.c_str(), flags));
}

//------------------------------------------------------------------------------
// Сообщение MB_ICONINFORMATION
inline int MessageBoxInf(String msg, unsigned short flags = MB_ICONINFORMATION + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}

//------------------------------------------------------------------------------
// Сообщение MB_ICONQUESTION
inline int MessageBoxQuestion(String msg, unsigned short flags = MB_ICONQUESTION + MB_YESNO + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}

//------------------------------------------------------------------------------
// Сообщение MB_ICONSTOP
inline int MessageBoxStop(String msg, unsigned short flags = MB_ICONSTOP + MB_OK + MB_SYSTEMMODAL + MB_SETFOREGROUND + MB_TOPMOST)
{
    return(Application->MessageBox(msg.c_str(), Application->Title.c_str(), flags));
}*/

//------------------------------------------------------------------------------
// Год из TDate
Word YearOf(const TDate dt)
{
    unsigned short dd, mm, yyyy;
    dt.DecodeDate(&yyyy, &mm, &dd);
    return yyyy;
}

//------------------------------------------------------------------------------
// Месяц из TDate
Word MonthOf(const TDate dt)
{
    unsigned short dd, mm, yyyy;
    dt.DecodeDate(&yyyy, &mm, &dd);
    return mm;
}

//------------------------------------------------------------------------------
// День из TDate
Word DayOf(const TDate dt)
{
    unsigned short dd, mm, yyyy;
    dt.DecodeDate(&yyyy, &mm, &dd);
    return dd;
}

//------------------------------------------------------------------------------
// Количество дней в месяце
Word DaysInAMonth(const TDate dt)
{
    unsigned short dd, mm, yyyy;
    dt.DecodeDate(&yyyy, &mm, &dd);
    int leap = IsLeapYear(yyyy) ? 1 : 0;
    return Sysutils::MonthDays[leap][mm - 1];
}

//---------------------------------------------------------------------------
// Дополнить строку справа
String StrPadR(String Str, int Length, String Symb)
{
    while (Str.Length() < Length)
    {
        Str += Symb;
    }
    return Str;
}

//---------------------------------------------------------------------------
// Дополнить строку слева
String StrPadL(String Str, int Length, String Symb)
{
    while (Str.Length() < Length)
    {
        Str = Symb + Str;
    }
    return Str;
}

//------------------------------------------------------------------------------
//
void ExploreDirectory(HWND Handle, AnsiString Path)
{
	ShellExecute(Handle, "OPEN", Path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void ExploreFile(HWND Handle, AnsiString Path)
{
    ShellExecute(Handle, "OPEN", "EXPLORER", ("/select, " + Path).c_str(), NULL ,SW_NORMAL);
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
AnsiString Implode(const vector<AnsiString> &pieces, const AnsiString &glue = "")
{
	AnsiString a;
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
// Учитываются только элементы не равные пустой строке
AnsiString ImplodeNvl(const vector<AnsiString> &pieces, const AnsiString &glue = "")
{
	AnsiString result;
	int leng=pieces.size();
 	for(int i=0; i<leng; i++)
 	{
        if (pieces[i] == "")
            continue;
        if (result != "")
            result += glue;
 		result+= pieces[i];
 	}
 	return result;
}

//------------------------------------------------------------------------------
// Добавляет в вектор только не пустые значения
void PushBackNvl(vector<AnsiString> &v, AnsiString value)
{
    if (value != "")
        v.push_back(value);
}
//------------------------------------------------------------------------------
// Объединяет вектор подстрок в одну строку используя соединитель
AnsiString Implode(const vector<EXPLODESTRING> &pieces, const AnsiString &glue = "")
{
	AnsiString a;
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
AnsiString& ReplaceAll(AnsiString& context, const AnsiString& from, const AnsiString& to)
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
std::vector<AnsiString> Explode(AnsiString str, AnsiString separator, bool addEmpty = true)
{   // Разбить строку на подстроки в вектор
    // addEmpty - добавлять в результат пустые строки
	std::vector<AnsiString> results;
 	unsigned int found1 = 1;
 	unsigned int found2 = 1;
    int nLengthSep = separator.Length();

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


/*
//------------------------------------------------------------------------------
// Разбивает строку на вектор строк используя для разбиения указанную подстроку
vector<AnsiString> Explode(AnsiString &const str, AnsiString separator, bool addEmpty = true)
{   // Разбить строку на подстроки в вектор
    // addEmpty - добавлять в результат пустые строки

	vector<AnsiString> results;
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
vector<EXPLODESTRING2> ExplodeByBackslash2(AnsiString str, AnsiString separatorstart, AnsiString separatorend, bool addEmpty = true)
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
vector<EXPLODESTRING> ExplodeByBackslash(AnsiString str, AnsiString separatorstart, AnsiString separatorend, bool addEmpty = true)
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
AnsiString str_pad(const AnsiString &input, int pad_length, const AnsiString &pad_string, int pad_type)
{
    int n = input.Length();
    int npad = pad_string.Length();
    AnsiString spad = "";
    while (n < pad_length)
    {
        spad = spad + pad_string;
        n = n + npad;
    }

    return pad_type == STR_PAD_LEFT? spad + input : input + spad ;
}


//------------------------------------------------------------------------------
// Дополняет строку другой строкой заданной длинны
AnsiString str_pad(const AnsiString &input, const AnsiString &pad_string, int pad_length, int pad_type)
{
    int n = input.Length();
    int npad = pad_string.Length();
    AnsiString spad = "";
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
AnsiString __fastcall CreateWorkDir(AnsiString work_dir)
{
    AnsiString tek_kat = ExtractFilePath(Application->ExeName);
    if (! SetCurrentDir(work_dir))
    {
        if (! CreateDir(work_dir))
            Application->MessageBox("Ошибка на диске C:\\ !","Операция прервана",MB_ICONSTOP + MB_OK);
    }
    SetCurrentDir(tek_kat);
    return (tek_kat);
}

//------------------------------------------------------------------------------
// Возращает полный путь к временному каталогу пользователя Windows
AnsiString __fastcall GetTempPath()
{
    const DWORD size = 512;
    char TempDirectory[size];
    DWORD Er = GetTempPath(size, TempDirectory);

    if(Er > size || Er == 0) {
        Er=GetLastError();
		MessageBoxStop("Error: " + IntToStr(Er));
        return NULL;
    } else {
        return (AnsiString)TempDirectory;
    }

}

//------------------------------------------------------------------------------
//
AnsiString AddWhere(AnsiString whereblock, AnsiString condition, bool addif)
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

//---------------------------------------------------------------------------
// Структура Caption-Value
typedef struct {
    AnsiString Caption;
    AnsiString Value;
} TValueListItem;

//---------------------------------------------------------------------------
// Класс для хранения структур Caption-Value
class TValueList {
public:
    __fastcall TValueList();
    __fastcall ~TValueList();
    void Free();
    TValueListItem* GetItem(int ItemIndex);
    void AddItem(AnsiString Caption, AnsiString Value);
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
AnsiString ValueList::GetValue(int ItemIndex)
{
    TValueListItem *Item = (ValueListItem*) Items->Items[ItemIndex];
    return Item->Value;
}

//---------------------------------------------------------------------------
// Получить Caption
AnsiString ValueList::GetCaption(int ItemIndex)
{
    TValueListItem *Item = (ValueListItem*) Items->Items[ItemIndex];
    return Item->Caption;
}
   */
//---------------------------------------------------------------------------
// Добавить в список Caption-Value
void TValueList::AddItem(AnsiString Caption, AnsiString Value)
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

//---------------------------------------------------------------------------
// Проверяет, является ли строка числом 
bool __fastcall IsNumber(String Value, bool bFloat, bool bSign)
{
    int n = Value.Length();
    bool bSignExist = false;
    int iStart = 1;

    if (bSign && n > 0) {   // Знак + или - могут быть только вначале
        bSignExist = Value[1] == '+' || Value[1] == '-';
    }

    if (bSignExist) {   // Если был знак + или -, значит начинаем со второго символа
        iStart = 2;
    }

    bool nDotExist = false;
    for (int i = iStart; i <= n; i++)   // Цикл по символам
    {
        if (!isdigit(Value[i]) ) {
            if (bFloat && Value[i] == DecimalSeparator) {
                if (nDotExist)       // Если ранее уже была .
                    return false;
                else
                    nDotExist = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------
// Возвращает первую строку из параметров, которая != ""
String Nvl(String s1, String s2)
{
    if (Trim(s1) == "")
        return s2;
    else
        return s1;
}

//---------------------------------------------------------------------------
// Возвращает s2, если которая s1 != "", иначе s3
String Nvl2(String s1, String s2, String s3="")
{
    if (Trim(s1) != "")
        return s2;
    else
        return s3;
}

//---------------------------------------------------------------------------
// Возвращает val_true, если val = true, иначе val_false
String Iif(bool val, String val_true, String val_false)
{
    return val ? val_true : val_false;
}


/*
String TimeBetween()
{
    int TotalSec = SecsPerDay * (StopTime - StartTime);
    int dd = TotalSec/SecsPerDay;
    int hh = (TotalSec - dd * 24 * 3600) / 3600;
    int mm = (TotalSec / 60) % 60;
    int ss = TotalSec % 60;
    sTotalTime = s
}*/

#endif TASKUTILS_H
