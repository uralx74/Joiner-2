//---------------------------------------------------------------------------
#ifndef TransferModuleH
#define TransferModuleH

#include "..\util\taskutils.h"
//#include "c:\PROGRS\current\util\appver.h"
#include "..\util\CommandLine.h"
#include <ComCtrls.hpp>
#include "Storage.h"
#include "Logger.h"
#include "encoder.h"
#include "XmlParamsLoader.h"


//---------------------------------------------------------------------------
class TTransferModule
{
private:	// User declarations
    void __fastcall Transfer(TStorage* Src, TStorage* Dst);
    TLogger* Logger;
    bool _IsCancel;

public:		// User declarations
    __fastcall TTransferModule();
    void __fastcall Start();
    void __fastcall Cancel();
};

//---------------------------------------------------------------------------
#endif

