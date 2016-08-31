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
    OleXml(bool validateOnParse = false);
    ~OleXml();
    void __fastcall LoadXMLFile(const AnsiString& XMLFileName);
    void __fastcall LoadXMLText(const AnsiString& XMLText);

    Variant __fastcall GetRootNode() const;
    AnsiString __fastcall GetNodeName(Variant Node) const;
    Variant __fastcall GetFirstNode(Variant Node) const;
    Variant __fastcall GetNextNode(Variant Node) const;
    Variant __fastcall SelectSingleNode(const AnsiString& xpath) const;

    Variant GetAttribute(Variant Node, const AnsiString& AttributeName) const;
    AnsiString __fastcall GetAttributeText(Variant Node, const AnsiString& AttributeName) const;
    AnsiString __fastcall GetAttributeValue(Variant Node, int AttributeIndex) const;
    AnsiString __fastcall GetAttributeValue(Variant Node, const AnsiString& AttributeName, const AnsiString& DefaultValue = "") const;
    bool __fastcall GetAttributeValue(Variant Node, const AnsiString& AttributeName, bool DefaultValue) const;
    int __fastcall GetAttributeValue(Variant Node, const AnsiString& AttributeName, int DefaultValue) const;
    int __fastcall GetAttributesCount(Variant Node) const;

    AnsiString __fastcall GetParseError() const;


    //Variant __fastcall FindNode(Variant node, AnsiString nodeName);

private:
    Variant xmlDoc;
};

#endif // OLE_XML_H
