/*******************************************************************************
    Библиотечный модуль taskutil.h
    Содержит вспомогательные функции

    Версия файла от 08.10.2014

    // Разбить и соединить строки

*******************************************************************************/

#ifndef KEYBOARDUTIL_H
#define KEYBOARDUTIL_H

#include <classes.hpp>


class TKeyboardUtil {
private:
    enum {ENGLISH = 409, RUSSIAN = 419};

public:
    AnsiString GetLayout();
    void SetNextLayout();
//    bool GetKeyboardState();

};


//------------------------------------------------------------------------------
//
AnsiString TKeyboardUtil::GetLayout()
{
    char sLayout[KL_NAMELENGTH];
    GetKeyboardLayoutName(sLayout);

    //HKL hklLayout = GetKeyboardLayout(0);

    switch (atoi(sLayout)) {
    case ENGLISH:
	    return "EN";
    case RUSSIAN:
	    return "RU";
    default:
        return "NA";
    }
}

//------------------------------------------------------------------------------
//
void TKeyboardUtil::SetNextLayout()
{
    ActivateKeyboardLayout(0, 0);
}

/*
//------------------------------------------------------------------------------
//
bool TKeyboardUtil::KeyboardState()
{
    TKeyboardState KeyboardState;
    GetKeyboardState(KeyboardState);
    return KeyboardState[VK_CAPITAL];
}  */



#endif KEYBOARDUTIL_H
