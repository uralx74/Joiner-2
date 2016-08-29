/*******************************************************************************
 Consists methods to work with xml-files
 author: vsovchinnikov
 date: 2016-05-23
*******************************************************************************/

//---------------------------------------------------------------------------
#ifndef XML_UTIL_H
#define XML_UTIL_H


#include "system.hpp"
#include <utilcls.h>
#include "Comobj.hpp"


namespace XmlUtil {

static Variant _xmlDoc;

//---------------------------------------------------------------------------
// Проверяет наличие ошибок разбора XML
AnsiString __fastcall XmlEncode(AnsiString XmlText)
{
    TReplaceFlags replaceflags = TReplaceFlags() << rfReplaceAll << rfIgnoreCase;
    XmlText = StringReplace(XmlText, "&", "&amp;", replaceflags);
    XmlText = StringReplace(XmlText, "<", "&lt;", replaceflags);
    XmlText = StringReplace(XmlText, ">", "&gt;", replaceflags);
    XmlText = StringReplace(XmlText, "'", "&apos;", replaceflags);
    XmlText = StringReplace(XmlText, "\"", "&quot;", replaceflags);
    return XmlText;

    /*if (_xmlDoc.IsEmpty()) {
        _xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
        _xmlDoc.OlePropertyGet("LoadXML", "<t></t>");
    }

    Variant lc = _xmlDoc.OlePropertyGet("LastChild");
    lc.OlePropertySet("Text", XmlText);

    XmlText = lc.OlePropertyGet("Xml");
    return XmlText.SubString(4, XmlText.Length()-7);

    return lc.OlePropertyGet("Xml");*/


}


}

//---------------------------------------------------------------------------
#endif
