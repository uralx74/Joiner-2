/*
 * File: OleXml.h
 * Description: Class for convinient work with OLE-objects MSXml.Application
 * (Decorator Pattern)
 * Created: 08.10.2014
 * Copyright: (C) 2016
 * Author: V.Ovchinnikov
 * Email: utnpsys@gmail.com
 * Changed: 31 aug 2016
 */

#ifndef OLE_XML_H
#define OLE_XML_H

#include "system.hpp"
#include <utilcls.h>
#include "Comobj.hpp"
#include <fstream.h>
#include "taskutils.h"

class OleXml
{

private:

public:
    static const String TRUE_STR_VALUE;
    static const String FALSE_STR_VALUE;
    OleXml(bool validateOnParse = false);
    ~OleXml();
    void __fastcall LoadXMLFile(const AnsiString& XMLFileName);
    void __fastcall LoadXMLText(const AnsiString& XMLText);
    void __fastcall Save(const String& filename) const;

    Variant __fastcall CreateRootNode(const AnsiString& nodeName);
    Variant __fastcall CreateNode(Variant parentNode, const AnsiString& nodeName);
    Variant __fastcall AddChildNode(Variant parentNode, Variant childNode);
    Variant __fastcall CloneNode(Variant node, bool deep = true);

    Variant __fastcall AddAttributeNode(Variant node, const AnsiString& attributeName, const AnsiString& attributeValue);
    void __fastcall SetTextNode(Variant node, const AnsiString& nodeText);
    void __fastcall SetAttributeValue(Variant node, const AnsiString& attributeName, const AnsiString& value);


    Variant __fastcall GetRootNode() const;
    AnsiString __fastcall GetNodeName(Variant node) const;
    Variant __fastcall GetFirstNode(Variant node) const;
    Variant __fastcall GetNextNode(Variant node) const;
    Variant __fastcall SelectSingleNode(const AnsiString& xpath) const;
    Variant __fastcall SelectSingleNode(Variant node, const AnsiString& xpath) const;

    AnsiString __fastcall GetNodeText(Variant Node) const;
    Variant GetAttribute(Variant Node, const AnsiString& attributeName) const;
    AnsiString __fastcall GetAttributeText(Variant node, const AnsiString& attributeName) const;
    AnsiString __fastcall GetAttributeValue(Variant node, int attributeIndex) const;
    AnsiString __fastcall GetAttributeValue(Variant node, const AnsiString& attributeName, const AnsiString& defaultValue = "") const;
    bool __fastcall GetAttributeValue(Variant node, const AnsiString& attributeName, bool defaultValue) const;
    int __fastcall GetAttributeValue(Variant node, const AnsiString& attributeName, int defaultValue) const;
    int __fastcall GetAttributesCount(Variant node) const;

    AnsiString __fastcall GetParseError() const;
    bool __fastcall HasChildNodes(Variant node) const;
    bool __fastcall IsTextElement(Variant node) const;


    //Variant __fastcall FindNode(Variant node, AnsiString nodeName);

private:
    Variant xmlDoc;
};

const String OleXml::TRUE_STR_VALUE = "true";
const String OleXml::FALSE_STR_VALUE = "false";

#endif // OLE_XML_H
