//---------------------------------------------------------------------------
#ifndef XML_LOADER_H
#define XML_LOADER_H

#include "c:\PROGRS\current\util\CommandLine.h"
#include "c:\PROGRS\current\util\MSXMLWorks.h"
#include "Storage.h"
#include "StorageDbf.h"
//#include "StorageOra.h"
//#include "StorageText.h"
//#include "StorageExcel.h"
#include "Logger.h"
#include "encoder.h"


//---------------------------------------------------------------------------
class TXmlLoader
{
private:	// User declarations
    AnsiString ExpandFileNameCustom(AnsiString FileName, AnsiString FilePath);
    TCommandLine* CommandLine;
    TLogger* Logger;

public:
    TXmlLoader();
    ~TXmlLoader();

    /**
    * Loades to XmlBranch
    */
    //XmlBranch* LoadToXmlBranch(String& xpath);

    bool __fastcall LoadParameters();

    // Источник и приемник
    TStorage* SrcStor;
    TStorage* DstStor;

};





//---------------------------------------------------------------------------
#endif // XML_LOADER_H
