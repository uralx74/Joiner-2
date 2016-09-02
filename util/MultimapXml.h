/*****************************************************************************
 * File: StorageTable.h
 * Description: Класс для работ с OLE-обектом MSXml.Application
 * Created: 08.10.2014
 * Copyright: (C) 2016
 * Author: V.Ovchinnikov
 * Email: utnpsys@gmail.com
 * Changed: 05 aug 2016
*****************************************************************************/

#ifndef MSXMLWORKS_H
#define MSXMLWORKS_H

/*******************************************************************************




*******************************************************************************/

#include "system.hpp"
#include <utilcls.h>
#include "Comobj.hpp"
#include <fstream.h>
#include <map>
#include "taskutils.h"


class MsxmlWorks
{

private:

public:
    MsxmlWorks(bool validateOnParse = false);
    ~MsxmlWorks();
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


class XmlBranch;

typedef std::multimap<String, XmlBranch*> BranchType;
typedef BranchType::iterator BranchIterator;

class XmlBranch
{
public:
    ~XmlBranch();
    std::map <String, String> param;
    BranchType branch;
private:

};

class XmlTreeMultimap: public MsxmlWorks
{
public:
    ~XmlTreeMultimap();
    XmlTreeMultimap(Variant node);
    XmlBranch* rootBranch;
    //class Iterator;

private:
    MsxmlWorks* xml;
    XmlBranch* LoadToXmlBranch(Variant node);
};

/*class XmlTreeMultimap::Iterator
{

};*/

//---------------------------------------------------------------------------
#endif // MSXMLWORKS_H
