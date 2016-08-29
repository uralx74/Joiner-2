//---------------------------------------------------------------------------
#ifndef COMMANDLINE
#define COMMANDLINE

/*******************************************************************************
	Класс для работы с параметрами командной строки
    Версия от 18.06.2015


*******************************************************************************/

#include "system.hpp"
#include <utilcls.h>
#include <map>
#include "Comobj.hpp"


class TCommandLine; // Опережающее объявление


class TCommandLineDestroyer
{
private:
    TCommandLine* p_instance;
public:
    ~TCommandLineDestroyer();
    void initialize( TCommandLine* p );
};


class TCommandLine
{

private:
	__fastcall TCommandLine();
	__fastcall ~TCommandLine();
    static TCommandLine* p_instance;            // Singleton
    static TCommandLineDestroyer destroyer;     // Singleton

    void __fastcall Parse();


protected:
    //TCommandLine( const TCommandLine& );
    //TCommandLine& operator=( TCommandLine& );

    friend class TCommandLineDestroyer;


public:
    static TCommandLine& getInstance();

    String __fastcall GetValue(AnsiString Name, AnsiString AltName, AnsiString DefaultValue = "");
    bool __fastcall GetFlag(AnsiString Name, AnsiString AltName, bool DefaultValue = false);
    void __fastcall SetValue(AnsiString Name, AnsiString AltName, AnsiString Value);

 	std::map <String,String> startparams;
};


//---------------------------------------------------------------------------
#endif
