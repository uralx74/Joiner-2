#ifndef TRANSFERTHREAD_H
#define TRANSFERTHREAD_H

#include "TransferModule.h"


//---------------------------------------------------------------------------
class TTransferThread : public TThread
{
public:
    __fastcall TTransferThread(bool CreateSuspended);
    __fastcall ~TTransferThread();

private:
    void __fastcall Execute();
    TTransferModule* pTransferModule;
    void __fastcall OnThreadTerminate(TObject *Sender);
};



#endif
