/*****************************************************************************
     * File: StorageTable.h
     * Description: Класс для работ с OLE-обектом MSXml.Application
     * Created: 08.10.2014
     * Copyright: (C) 2016
     * Author: V.Ovchinnikov
     * Email: utnpsys@gmail.com
     * Changed: 05 aug 2016
*****************************************************************************/

#ifndef XMLWORKS_H
#define XMLWORKS_H

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
    MsxmlWorks();
    void __fastcall LoadXMLFile(AnsiString XMLFileName);
    void __fastcall LoadXMLText(AnsiString XMLText);

    Variant __fastcall GetRootNode();
    AnsiString __fastcall GetNodeName(Variant Node);
    Variant __fastcall GetFirstNode(Variant Node);
    Variant __fastcall GetNextNode(Variant Node);

    AnsiString __fastcall GetAttributeValue(Variant Node, AnsiString AttributeName, String DefaultValue);
    bool __fastcall GetAttributeValue(Variant Node, AnsiString AttributeName, bool DefaultValue);
    int __fastcall GetAttributeValue(Variant Node, AnsiString AttributeName, int DefaultValue);

    AnsiString __fastcall GetAttributeValue(Variant Node, int AttributeIndex);
    AnsiString __fastcall GetAttributeValue(Variant Node, AnsiString AttributeName);
    Variant GetAttribute(Variant Node, AnsiString AttributeName);

    //AnsiString __fastcall GetValueAttribute(Variant Attribute);
    int __fastcall GetAttributesCount(Variant Node);

    AnsiString __fastcall GetParseError();

    //Variant __fastcall FindNode(Variant node, AnsiString nodeName);
    Variant __fastcall SelectSingleNode(AnsiString xpath);


    Variant xmlDoc;
    Variant rootNode;



private:
    //XmlBranch* LoadToSingleXmlBranch(Variant node);
//    XmlBranch* LoadToSingleXmlBranch(MSXMLWorks* msxml, Variant node);

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
#endif // XMLWORKS_H
