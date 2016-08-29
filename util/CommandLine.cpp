#include "CommandLine.h"

TCommandLine* TCommandLine::p_instance = NULL;
TCommandLineDestroyer TCommandLine::destroyer;

//---------------------------------------------------------------------------
//
TCommandLineDestroyer::~TCommandLineDestroyer()
{
    delete p_instance;
}

//---------------------------------------------------------------------------
//
void TCommandLineDestroyer::initialize(TCommandLine* p)
{
    p_instance = p;
}

//---------------------------------------------------------------------------
//
TCommandLine& TCommandLine::getInstance()
{
    if(!p_instance) {
        p_instance = new TCommandLine();
        destroyer.initialize(p_instance);     
    }
    return *p_instance;
}


//---------------------------------------------------------------------------
//
__fastcall TCommandLine::TCommandLine()
{
    Parse();
}

//---------------------------------------------------------------------------
//
__fastcall TCommandLine::~TCommandLine()
{
    startparams.clear();
}

//---------------------------------------------------------------------------
// –азбор параметров и сохранение в std::map
void __fastcall TCommandLine::Parse()
{
    int n = ParamCount();

    AnsiString paramname = "";
    AnsiString paramvalue = "";

    for (int i = 1; i <= n; i++) {
        AnsiString sParamStr = Trim(ParamStr(i));

        if (sParamStr[1] == '-') {

            // »щем раделитель параметр = значение
            // ≈сли нашли, то это параметр со значением
            // иначе - параметр-переключатель
            int eqPos = sParamStr.Pos("=");
            if (eqPos > 0) {
                paramname = sParamStr.SubString(1, eqPos-1);
                paramvalue = sParamStr.SubString(eqPos+1, sParamStr.Length()-eqPos);
            } else {
                paramname = sParamStr;
                paramvalue = "true";
            }
            startparams[paramname] = paramvalue;

        } else { // если параметр без ключа
        }
    }
}

//---------------------------------------------------------------------------
// ¬озвращает значение параметра по длинному или короткому имени
String __fastcall TCommandLine::GetValue(AnsiString Name, AnsiString AltName, AnsiString DefaultValue)
{
    if (Name != "" && startparams[Name] != "")
        return startparams[Name];
    else if (AltName != "" && startparams[AltName] != "")
        return startparams[AltName];

    return DefaultValue;
}

//---------------------------------------------------------------------------
// ”станавливает значение параметра
void __fastcall TCommandLine::SetValue(AnsiString Name, AnsiString AltName, AnsiString Value)
{
    if (Name != "")
        startparams[Name] = Value;

    if (AltName != "")
        startparams[AltName] = Value;
}


//---------------------------------------------------------------------------
// ¬озвращает значение параметра по длинному или короткому имени
bool __fastcall TCommandLine::GetFlag(AnsiString Name, AnsiString AltName, bool DefaultValue)
{
    if (Name != "" && startparams[Name] != "")
        return true;
    else if (AltName != "" && startparams[AltName] != "")
        return true;

    return DefaultValue;
}

