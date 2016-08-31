#include "OleXml.h"



OleXml::OleXml(bool validateOnParse)
{
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    xmlDoc.OlePropertySet("validateOnParse", validateOnParse);
}

OleXml::~OleXml()
{
    xmlDoc = Unassigned;   // ������������ VarClear(xmlDoc)
}

/* ��������� xml �� ����� ����
 */
void __fastcall OleXml::LoadXMLFile(const AnsiString& XMLFileName)
{
    xmlDoc.OlePropertyGet("Load", XMLFileName.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}

/* ��������� xml �� ������
 */
void __fastcall OleXml::LoadXMLText(const AnsiString& XMLText)
{
    xmlDoc.OlePropertyGet("LoadXML", XMLText.c_str());
}

/* ���������� ���������� ��������� ����
 */
int __fastcall OleXml::GetAttributesCount(Variant Node) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("length");
}

/* ���������� �������� ����
 */
Variant __fastcall OleXml::GetRootNode() const
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

/* ���������� ��� ����
 */
AnsiString __fastcall OleXml::GetNodeName(Variant Node) const
{
    return Node.OlePropertyGet("NodeName");
}

/* ��������� ������ �������� ����
 */
Variant __fastcall OleXml::GetFirstNode(Variant Node) const
{
    return Node.OlePropertyGet("firstChild");
}

/* ���������� ��������� ���� �� ����������
 */
Variant __fastcall OleXml::GetNextNode(Variant Node) const
{
    return Node.OlePropertyGet("nextSibling");
}

/* ���������� ���� �� ���� xpath
 */
Variant __fastcall OleXml::SelectSingleNode(const AnsiString& xpath) const
{
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}

/* ���������, ���������� �� �������
 */
Variant OleXml::GetAttribute(Variant Node, const AnsiString& AttributeName) const
{
    return Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    //return attribute.IsEmpty();
}

/* ���������� ������� �� �������
*/
AnsiString __fastcall OleXml::GetAttributeValue(Variant Node, int AttributeIndex) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("item", AttributeIndex).OlePropertyGet("Value");
}

/* ���������� �������� �������� �� �����
*/
AnsiString __fastcall OleXml::GetAttributeText(Variant Node, const AnsiString& AttributeName) const
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
AnsiString __fastcall OleXml::GetAttributeValue(Variant Node, const AnsiString& AttributeName, const AnsiString& DefaultValue) const
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
int __fastcall OleXml::GetAttributeValue(Variant Node, const AnsiString& AttributeName, int DefaultValue) const
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
bool __fastcall OleXml::GetAttributeValue(Variant Node, const AnsiString& AttributeName, bool DefaultValue) const
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
AnsiString __fastcall OleXml::GetParseError() const
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

