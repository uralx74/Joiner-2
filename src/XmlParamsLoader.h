//---------------------------------------------------------------------------
#ifndef XML_LOADER_H
#define XML_LOADER_H

#include "..\util\CommandLine.h"
#include "..\util\OleXml.h"
#include "Storage.h"
#include "StorageDbf.h"
#include "StorageOra.h"
//#include "StorageText.h"
//#include "StorageExcel.h"
#include "Logger.h"
#include "encoder.h"


//---------------------------------------------------------------------------
class TXmlLoader
{
private:
    AnsiString ExpandFileNameCustom(AnsiString FileName, AnsiString FilePath);
    TCommandLine* CommandLine;
    TLogger* Logger;


    TStorage* ProceedStorageNode(const OleXml& msxml, const AnsiString& nodeXpath, TStorage* templateStorage = NULL);

    /* Параметры командной строки */
    AnsiString clConfig;    // Путь к файлу конфигурации
    AnsiString clConfigPath;    // Путь к файлу конфигурации
    AnsiString clSrcUsername;  // Имя пользователя базы данных
    AnsiString clSrcPassword;  // Пароль к базе данных
    AnsiString clDstUsername;  // Имя пользователя базы данных
    AnsiString clDstPassword;  // Пароль к базе данных

public:
    TXmlLoader();
    ~TXmlLoader();

    /*
     * Loades to XmlBranch
     */
    bool __fastcall LoadParameters();

    // Источник и приемник
    TStorage* SrcStor;
    TStorage* DstStor;



};





//---------------------------------------------------------------------------
#endif // XML_LOADER_H
