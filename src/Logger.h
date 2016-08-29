//---------------------------------------------------------------------------
#ifndef LoggerH
#define LoggerH

class TLogger;  // ����������� ����������


class TLoggerDestroyer
{
private:
    TLogger* p_instance;
public:    
    ~TLoggerDestroyer();
    void initialize( TLogger* p );
};


class TLogConsole
{
public:
    //TLogConsole();
    virtual int AddMsg(const AnsiString& Msg) = 0;
    //virtual void SetMsg(const AnsiString& Msg, int Line);
    void AddMsgHistory(const TStringList* History);
};

class TLogConsoleRichEdit: public TLogConsole
{
public:
    TLogConsoleRichEdit(TRichEdit* RichEdit);
    int AddMsg(const AnsiString& Msg);

protected:

    TRichEdit* LogEditPtr;  // ��������� �� ���-RichEdit
};


class TLogConsoleFile: public TLogConsole
{
public:
    TLogConsoleFile(AnsiString FileName, bool Rewrite = false);

protected:
    int AddMsg(const AnsiString& Msg);
    std::FILE* LogFilePtr;  // ��������� �� ���-����
};


class TLogger
{
private:
    static TLogger* p_instance;
    static TLoggerDestroyer destroyer;

    TRichEdit* LogEditPtr;  // ��������� �� ���-RichEdit
    std::FILE* LogFilePtr;  // ��������� �� ���-����
    TStringList* LogHist;      // ������� ���-���������


    std::vector<TLogConsole*> Console;

protected:
    TLogger();
    ~TLogger();
    friend class TLoggerDestroyer;      // for access to p_instance

public:
    AnsiString GetLogFilename(const AnsiString& LogFileName);
    static TLogger& getInstance();

    int __fastcall WriteLog(AnsiString MessageStr, int LineNumber = -1);
    //int __fastcall WriteLogC(AnsiString MessageStr, int LineNumber = -1);
    //int __fastcall RefreshConsole();



    void __fastcall AddConsole(TRichEdit* Edit);
    void __fastcall AddConsole(AnsiString LogFileName, bool Rewrite = false);
};













    /*

// Singleton.h
class Singleton
{
private: 
    Singleton() {}
    Singleton( const Singleton&);  
    Singleton& operator=( Singleton& );
public:
    static Singleton& getInstance() {
        static Singleton  instance;
        return instance;
    }    
};    */


   /*
class TLogger
{
public:
    int __fastcall WriteLog(AnsiString MessageStr, int LineNumber = -1);

    static const TLogger& Instance()
    {
        static TLogger theSingleInstance;
        return theSingleInstance;
    }

    //TRichEdit* LogEdit;
private:
    TLogger(){}
    TLogger(const TLogger& root);
    TLogger& operator=(const TLogger&);
};
       */

//---------------------------------------------------------------------------
#endif

