#include "pch.h"
#pragma hdrstop
#pragma package(smart_init)

#include "TransferThread.h"

//---------------------------------------------------------------------------
//
__fastcall TTransferThread::TTransferThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
    //FreeOnTerminate = true;
    Suspended = CreateSuspended;

    pTransferModule = new TTransferModule();
    this->OnTerminate = &OnThreadTerminate;
}

//---------------------------------------------------------------------------
//
__fastcall TTransferThread::~TTransferThread()
{

    delete pTransferModule;
}

//---------------------------------------------------------------------------
//
void __fastcall TTransferThread::Execute()
{
    pTransferModule->Start();
}

//---------------------------------------------------------------------------
//
void __fastcall TTransferThread::OnThreadTerminate(TObject *Sender)
{
    pTransferModule->Cancel();
}



