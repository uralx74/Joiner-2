object Form1: TForm1
  Left = 195
  Top = 123
  Width = 614
  Height = 540
  Caption = 'Joiner'
  Color = clBtnFace
  Constraints.MinHeight = 540
  Constraints.MinWidth = 614
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonStart: TButton
    Left = 168
    Top = 464
    Width = 153
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Выполнить'
    TabOrder = 0
    OnClick = ButtonStartClick
  end
  object OpenConfigButton: TButton
    Left = 8
    Top = 464
    Width = 153
    Height = 25
    Hint = 'Открыть файл конфигурации...'
    Anchors = [akLeft, akBottom]
    Caption = 'Открыть конфигурацию...'
    TabOrder = 1
    OnClick = OpenConfigButtonClick
  end
  object LogRichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 598
    Height = 457
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    HideScrollBars = False
    Lines.Strings = (
      'RichEdit1')
    ParentFont = False
    PlainText = True
    PopupMenu = PopupMenu1
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 2
    OnContextPopup = LogRichEditContextPopup
  end
  object ExitButton: TButton
    Left = 440
    Top = 464
    Width = 153
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Выход'
    TabOrder = 3
    OnClick = ExitButtonClick
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'xml'
    Options = [ofPathMustExist, ofEnableSizing]
    Left = 160
    Top = 440
  end
  object PopupMenu1: TPopupMenu
    Left = 128
    Top = 440
    object N1: TMenuItem
      Caption = 'Копировать'
      ShortCut = 16451
      OnClick = N1Click
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 352
    Top = 464
  end
end
