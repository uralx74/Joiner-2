//---------------------------------------------------------------------------
#ifndef XML_LOADER_H
#define XML_LOADER_H

#include "..\util\CommandLine.h"
#include "..\util\OleXml.h"
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
    bool __fastcall LoadParameters();

    // �������� � ��������
    TStorage* SrcStor;
    TStorage* DstStor;

};





//---------------------------------------------------------------------------
#endif // XML_LOADER_H
