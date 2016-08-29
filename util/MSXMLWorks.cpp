#include "MSXMLWorks.h"

//---------------------------------------------------------------------------
//
MsxmlWorks::MsxmlWorks()
{
    //Variant xmlObj = CreateOleObject("Microsoft.XMLDOM");
    //Variant xmlDoc = CreateOleObject("MSXML.DOMDocument");
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    //xmlDoc.OlePropertySet("validateOnParse", false);
}

//---------------------------------------------------------------------------
//
void __fastcall MsxmlWorks::LoadXMLFile(AnsiString XMLFileName)
{
    xmlDoc.OlePropertyGet("Load", XMLFileName.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}

//---------------------------------------------------------------------------
//
void __fastcall MsxmlWorks::LoadXMLText(AnsiString XMLText)
{
    //StringToOleStr(XMLText);
    xmlDoc.OlePropertyGet("LoadXML", XMLText.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}


//---------------------------------------------------------------------------
// Проверяет, существует ли атрибут
Variant MsxmlWorks::GetAttribute(Variant Node, AnsiString AttributeName)
{
    return Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    //return attribute.IsEmpty();
}

//---------------------------------------------------------------------------
// Возвращает количество атрибутов узла
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, int AttributeIndex)
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("item",AttributeIndex).OlePropertyGet("Value");
}

//---------------------------------------------------------------------------
// Возвращает значение атрибута по имени
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, AnsiString AttributeName)
{
    Variant attribute = Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    if (!attribute.IsEmpty())
        return attribute.OlePropertyGet("text");
    else
        return "";

    // Второй способ
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
}


//---------------------------------------------------------------------------
// Возвращает значение атрибута,
// если атрибут отсутствует, то возвращает значение DefaultValue
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, AnsiString AttributeName, String DefaultValue)
{
    AnsiString attribute = Trim(GetAttributeValue(Node, AttributeName));
    if (attribute != "") {
        return attribute;
    } else {
        return DefaultValue;
    }
}

//---------------------------------------------------------------------------
// Возвращает значение атрибута,
// если атрибут отсутствует, то возвращает значение DefaultValue
int __fastcall MsxmlWorks::GetAttributeValue(Variant Node, AnsiString AttributeName, int DefaultValue)
{
    AnsiString attribute = Trim(GetAttributeValue(Node, AttributeName));  // Ширина столбца
    if (attribute != "") {
        try {
            return StrToInt(attribute);
        } catch (...) {
            return DefaultValue;
        }
    } else
        return DefaultValue;
}

//---------------------------------------------------------------------------
// Возвращает значение атрибута,
// если атрибут отсутствует, то возвращает значение DefaultValue
bool __fastcall MsxmlWorks::GetAttributeValue(Variant Node, AnsiString AttributeName, bool DefaultValue)
{
    AnsiString attribute = LowerCase(Trim(GetAttributeValue(Node, AttributeName)));  // Ширина столбца

    if (attribute == "true")
        return true;
    else if (attribute == "false")
        return false;
    else
        return DefaultValue;
}

/*//---------------------------------------------------------------------------
// Возвращает значение атрибута
AnsiString MSXMLWorks::GetValueAttribute(Variant Attribute)
{
    return Attribute.OlePropertyGet("Value");
}  */

//---------------------------------------------------------------------------
// Возвращает количество атрибутов узла
int __fastcall MsxmlWorks::GetAttributesCount(Variant Node)
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("length");
}

//---------------------------------------------------------------------------
//
Variant __fastcall MsxmlWorks::GetRootNode()
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

//---------------------------------------------------------------------------
//
/*Variant __fastcall MSXMLWorks::FindNode(Variant node, AnsiString nodeName)
{
    //return xmlDoc.getElementsByTagName(nodeName);

    node.OleFunction("selectSingleNode", "" + nodeName); // selectSingleNode

    //xmlDoc.SelectSingleNode(nodeName);
    //return node.OlePropertyGet(nodeName);
}  */

Variant __fastcall MsxmlWorks::SelectSingleNode(AnsiString xpath)
{
    //msxml.xmlDoc.OlePropertySet("SelectionLanguage", "XPath");
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}


//---------------------------------------------------------------------------
//
AnsiString __fastcall MsxmlWorks::GetNodeName(Variant Node)
{
    return Node.OlePropertyGet("NodeName");
}


//---------------------------------------------------------------------------
// Возврщает первый дочерний узел
Variant __fastcall MsxmlWorks::GetFirstNode(Variant Node)
{
    return Node.OlePropertyGet("firstChild");
}

//---------------------------------------------------------------------------
// Возвращает следующий узел от указанного
Variant __fastcall MsxmlWorks::GetNextNode(Variant Node)
{
    return Node.OlePropertyGet("nextSibling");
}

//---------------------------------------------------------------------------
// Проверяет наличие ошибок разбора XML
AnsiString __fastcall MsxmlWorks::GetParseError()
{
    if( xmlDoc.OlePropertyGet("parseError").OlePropertyGet("errorCode")!=0 )
    {
        return AnsiString(xmlDoc.OlePropertyGet("parseError").OlePropertyGet("reason"));
    } else {
        return "";
    }
}


/*
XmlBranch* MsxmlWorks::LoadToXmlBranch(Variant node)
{
    if (!node.IsEmpty()) {
        XmlBranch* branch = new XmlBranch();
        String branchName = GetNodeName(node);
        //parentBranch->branch[branchName] = branch;

        Variant subnode = GetFirstNode(node);
        while (!subnode.IsEmpty()) {
            String subBranchName = GetNodeName(subnode);
            XmlBranch* subBranch = new XmlBranch();
            branch->branch.insert(BranchType::value_type(subBranchName, subBranch));
            subBranch = LoadToXmlBranch(subnode);
            subnode = GetNextNode(subnode);
        }
        return branch;
    }
    return NULL;
}*/

// Необходимо тестирование
XmlBranch::~XmlBranch()
{
    //BranchIterator it = this->branch.begin();
    /*for (BranchIterator it = this->branch.begin(); it != this->branch.end(); it++) {
        delete it->second->branch;
    } */

    for (BranchIterator it = this->branch.begin(); it != this->branch.end(); it++) {
        delete it->second;
    }
}

XmlBranch* XmlTreeMultimap::LoadToXmlBranch(Variant node)
{
    if (!node.IsEmpty()) {
        XmlBranch* branch = new XmlBranch();
        String branchName = GetNodeName(node);
        //parentBranch->branch[branchName] = branch;

        Variant subnode = GetFirstNode(node);
        while (!subnode.IsEmpty()) {
            String subBranchName = GetNodeName(subnode);
            XmlBranch* subBranch = new XmlBranch();
            branch->branch.insert(BranchType::value_type(subBranchName, subBranch));
            subBranch = LoadToXmlBranch(subnode);
            subnode = GetNextNode(subnode);
        }
        return branch;
    }
    return NULL;
}


XmlTreeMultimap::~XmlTreeMultimap()
{
    delete rootBranch;
}

XmlTreeMultimap::XmlTreeMultimap(Variant node)
{
    rootBranch = LoadToXmlBranch(node);
}
