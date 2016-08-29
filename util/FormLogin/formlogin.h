//---------------------------------------------------------------------------

#ifndef FormLoginH
#define FormLoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include "DBAccess.hpp"
#include "Ora.hpp"
#include <Db.hpp>
#include "Registry.hpp"
#include "c:\PROGRS\current\util\taskutils.h"
#include "c:\PROGRS\current\util\keyboardutil.h"
#include <ActnList.hpp>

//---------------------------------------------------------------------------
class TLoginForm : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *LoginBtn;
    TBitBtn *CancelBtn;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TMaskEdit *PasswordMaskEdit;
    TEdit *UsernameEdit;
    TImage *Image1;
    TActionList *ActionList1;
    TAction *Login;
    TAction *Cancel;
    TTimer *Timer1;
    TPanel *KBLayoutPanel;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall LoginExecute(TObject *Sender);
    void __fastcall CancelExecute(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall KBLayoutPanelClick(TObject *Sender);
private:	// User declarations
    AnsiString AppName;
    TKeyboardUtil KeyboardUtil;

public:		// User declarations
    __fastcall TLoginForm(TComponent* Owner);
    int RetryCount;
    bool __fastcall Execute(TOraSession* Session, String Username="", String Password="");
    bool __fastcall CheckRole(AnsiString Role);
    std::vector<AnsiString>* __fastcall GetUserPriveleges();

    TOraSession *Session;
};
//---------------------------------------------------------------------------
extern PACKAGE TLoginForm *LoginForm;
//---------------------------------------------------------------------------
#endif
