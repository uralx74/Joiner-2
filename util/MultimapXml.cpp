#include "MSXMLWorks.h"



MsxmlWorks::MsxmlWorks(bool validateOnParse)
{
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    xmlDoc.OlePropertySet("validateOnParse", validateOnParse);
}

MsxmlWorks::~MsxmlWorks()
{
    xmlDoc = Unassigned;   // ������������ VarClear(xmlDoc)
}

/* ��������� xml �� ����� ����
 */
void __fastcall MsxmlWorks::LoadXMLFile(const AnsiString& XMLFileName)
{
    xmlDoc.OlePropertyGet("Load", XMLFileName.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}

/* ��������� xml �� ������
 */
void __fastcall MsxmlWorks::LoadXMLText(const AnsiString& XMLText)
{
    xmlDoc.OlePropertyGet("LoadXML", XMLText.c_str());
}

/* ���������� ���������� ��������� ����
 */
int __fastcall MsxmlWorks::GetAttributesCount(Variant Node) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("length");
}

/* ���������� �������� ����
 */
Variant __fastcall MsxmlWorks::GetRootNode() const
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

/* ���������� ��� ����
 */
AnsiString __fastcall MsxmlWorks::GetNodeName(Variant Node) const
{
    return Node.OlePropertyGet("NodeName");
}

/* ��������� ������ �������� ����
 */
Variant __fastcall MsxmlWorks::GetFirstNode(Variant Node) const
{
    return Node.OlePropertyGet("firstChild");
}

/* ���������� ��������� ���� �� ����������
 */
Variant __fastcall MsxmlWorks::GetNextNode(Variant Node) const
{
    return Node.OlePropertyGet("nextSibling");
}

/* ���������� ���� �� ���� xpath
 */
Variant __fastcall MsxmlWorks::SelectSingleNode(const AnsiString& xpath) const
{
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}

/* ���������, ���������� �� �������
 */
Variant MsxmlWorks::GetAttribute(Variant Node, const AnsiString& AttributeName) const
{
    return Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    //return attribute.IsEmpty();
}

/* ���������� ������� �� �������
*/
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, int AttributeIndex) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("item", AttributeIndex).OlePropertyGet("Value");
}

/* ���������� �������� �������� �� �����
*/
AnsiString __fastcall MsxmlWorks::GetAttributeText(Variant Node, const AnsiString& AttributeName) const
{
    Variant attribute = GetAttribute(Node, AttributeName);
    if (!attribute.IsEmpty())
    {
        return attribute.OlePropertyGet("text");
    }
    else
    {
        throw Exception("Attribute " + AttributeName + "is empty.");
    }

    // ������ ������
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
}

/* ���������� �������� ��������,
   ���� ������� �����������, �� ���������� �������� DefaultValue
 */
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, const AnsiString& AttributeName, const AnsiString& DefaultValue) const
{
    Variant attribute = GetAttribute(Node, AttributeName);
    return (attribute.IsEmpty()) ? DefaultValue : VarToStr( attribute.OlePropertyGet("text") );

    /*
    AnsiString attribute = Trim( GetAttributeValue( Node, StringToOleStr(AttributeName) ) );

    if (attribute != "")
    {
        return attribute;
    } else
    {
        return DefaultValue;
    }*/
}

/* ���������� �������� ��������,
   ���� ������� �����������, �� ���������� �������� DefaultValue
 */
int __fastcall MsxmlWorks::GetAttributeValue(Variant Node, const AnsiString& AttributeName, int DefaultValue) const
{

    Variant attribute = GetAttribute(Node, AttributeName);
    return (attribute.IsEmpty()) ? DefaultValue : attribute.OlePropertyGet("Value");

/*    AnsiString attribute = Trim(GetAttributeValue(Node, AttributeName));  // ������ �������
    if (attribute != "")
    {
        try
        {
            return StrToInt(attribute);
        }
        catch (...)
        {
            return DefaultValue;
        }
    }
    else
    {
        return DefaultValue;
    }*/
}

/* ���������� �������� ��������,
   ���� ������� �����������, �� ���������� �������� DefaultValue
 */
bool __fastcall MsxmlWorks::GetAttributeValue(Variant Node, const AnsiString& AttributeName, bool DefaultValue) const
{
    Variant attribute = GetAttribute(Node, AttributeName);

    if (attribute.IsEmpty()) {
        return DefaultValue;
    } else {
        String textValue = attribute.OlePropertyGet("Value");
        if (textValue == "true")
        {
            return true;
        }
        else if (textValue == "false")
        {
            return false;
        }
        else
        {
            return DefaultValue;
        }
    }
}



/* ��������� ������� ������ ������� XML
 */
AnsiString __fastcall MsxmlWorks::GetParseError() const
{
    if( xmlDoc.OlePropertyGet("parseError").OlePropertyGet("errorCode")!=0 )
    {
        return AnsiString(xmlDoc.OlePropertyGet("parseError").OlePropertyGet("reason"));
    }
    else
    {
        return "";
    }
}

/* �������� ��� ��������������� xml-������� � ������
 * � �������� ����� - ������� multimap, � �������� - �������� multimap
 *
 * ���������� ������������
 */
XmlBranch::~XmlBranch()
{
    for (BranchIterator it = this->branch.begin(); it != this->branch.end(); it++) {
        delete it->second;
    }
}

/*
 */
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

/*
 */
XmlTreeMultimap::~XmlTreeMultimap()
{
    delete rootBranch;
}

/*
 */
XmlTreeMultimap::XmlTreeMultimap(Variant node)
{
    rootBranch = LoadToXmlBranch(node);
}


/*
 */
/*Variant __fastcall MSXMLWorks::FindNode(Variant node, AnsiString nodeName)
{
    //return xmlDoc.getElementsByTagName(nodeName);

    node.OleFunction("selectSingleNode", "" + nodeName); // selectSingleNode

    //xmlDoc.SelectSingleNode(nodeName);
    //return node.OlePropertyGet(nodeName);
}*/

/* ���������� �������� �������� �� �����
*/
/*AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, const AnsiString& AttributeName)
{
    Variant attribute = GetAttribute(Node, AttributeName);
    if (!attribute.IsEmpty())
    {
        return attribute.OlePropertyGet("text");
    }
    else
    {
        return "";
    }
    // ������ ������
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
} */
