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

#ifndef ODACUTILS_H
#define ODACUTILS_H

//#include <vector.h>
//#include <classes.hpp>
//#include <MemDS.hpp>
#include "Ora.hpp"


using namespace std;

//------------------------------------------------------------------------------
// 
class TOdacUtilLog
{
private:
    AnsiString s_os_mac_address;
    AnsiString s_os_user_name;
    AnsiString s_task_user_name;
    AnsiString s_task_name;
    AnsiString s_app_ver;
    TOraSession* OraSession;

protected:

public:
    void __fastcall Init(TOraSession* OraSession, AnsiString s_os_mac_address, AnsiString s_task_user_name, AnsiString s_task_name, AnsiString s_app_ver);
    BOOL WriteLog(AnsiString sFuncName, AnsiString descr="");
};

//------------------------------------------------------------------------------
// Инициализация параметров для записи в лог-таблицу
void __fastcall TOdacUtilLog::Init(TOraSession* OraSession, AnsiString s_os_mac_address, AnsiString s_task_user_name, AnsiString s_task_name, AnsiString s_app_ver)
{
    this->OraSession = OraSession;
    this->s_os_mac_address = s_os_mac_address;
    this->s_task_user_name = s_task_user_name;
    this->s_task_name = s_task_name;
    this->s_app_ver = s_app_ver;
}

//------------------------------------------------------------------------------
// Записывает в таблицу БД лог-строку
BOOL TOdacUtilLog::WriteLog(AnsiString sFuncName, AnsiString descr)
{

	TOraQuery *OraQueryLog = new TOraQuery(NULL);
    OraQueryLog->Session = OraSession;
    try {
    	OraQueryLog->SQL->Clear();
 	    OraQueryLog->CreateProcCall("pk_nasel_otdel.p_log_task_write_2", 0);
        OraQueryLog->ParamByName("p_pc_mac")->Value = s_os_mac_address;
        OraQueryLog->ParamByName("p_task_name")->Value = s_task_name;
        OraQueryLog->ParamByName("p_func_name")->Value = sFuncName;
        OraQueryLog->ParamByName("p_descr")->Value = descr;
        OraQueryLog->ParamByName("p_task_user_name")->Value = s_task_user_name;
        OraQueryLog->ParamByName("p_app_ver")->Value = s_app_ver;
        OraQueryLog->ExecSQL();
        OraQueryLog->Close();
    } catch (...) {
        delete OraQueryLog;
    	OraQueryLog = NULL;
        return false;
    }

    delete OraQueryLog;
	OraQueryLog = NULL;

    return true;
}

//------------------------------------------------------------------------------
//  Подсчет количества записей
int GetRecCount(TOraQuery *OraQuery)
{   // Функция для подсчета количества записей в OraQuery

    TOraQuery *OraQueryCount = new TOraQuery(NULL);//OraQuery->Last();
    OraQueryCount->Session = OraQuery->Session;
    OraQuery->SQL->Add( "select count(*) N from (" +OraQuery->FinalSQL + ")" );
    OraQueryCount->Open();
    int RecCount = OraQueryCount->FieldByName("N")->AsInteger;

    OraQueryCount->Close();
    delete OraQueryCount;
    OraQueryCount = NULL;

    return RecCount;
}

//------------------------------------------------------------------------------
// Создание и выполнение OraQuery
TOraQuery* OpenOraQuery(TOraSession* OraSession, AnsiString StrQuery, bool FetchAll = true)
{
    TOraQuery* OraQuery = new TOraQuery(NULL);
    OraQuery->FetchAll = FetchAll;
    OraQuery->Session = OraSession;

    //OraQuery->SQL->Clear();
    OraQuery->SQL->Add(StrQuery);

    try {
        if (OraQuery->Active)
            OraQuery->Refresh();
        else
            OraQuery->Open();
    } catch(Exception &e) {
        delete OraQuery;
        OraQuery = NULL;
        //Application->ShowException(&exception);
        throw Exception(e);   // добавлено 2016-03-22
    }
    return OraQuery;
}

// Аналог nvl Oracle
String ora_nvl(TField* field, String val1)
{
    return field->IsNull ? val1 : field->AsString;
}

// Аналог nvl2 Oracle
String ora_nvl2(TField* field, String val1, String val2)
{
    return field->IsNull ? val2 : val1;
}

#endif ODACUTILS_H
