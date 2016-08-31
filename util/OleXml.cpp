#include "OleXml.h"



OleXml::OleXml(bool validateOnParse)
{
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    xmlDoc.OlePropertySet("validateOnParse", validateOnParse);
}

OleXml::~OleXml()
{
    xmlDoc = Unassigned;   // эквивалентно VarClear(xmlDoc)
}

/* Загружает xml из файла файл
 */
void __fastcall OleXml::LoadXMLFile(const AnsiString& XMLFileName)
{
    xmlDoc.OlePropertyGet("Load", XMLFileName.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}

/* Загружает xml из строки
 */
void __fastcall OleXml::LoadXMLText(const AnsiString& XMLText)
{
    xmlDoc.OlePropertyGet("LoadXML", XMLText.c_str());
}

/* Возвращает количество атрибутов узла
 */
int __fastcall OleXml::GetAttributesCount(Variant Node) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("length");
}

/* Возвращает корневой узел
 */
Variant __fastcall OleXml::GetRootNode() const
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

/* Возвращает имя узла
 */
AnsiString __fastcall OleXml::GetNodeName(Variant Node) const
{
    return Node.OlePropertyGet("NodeName");
}

/* Возврщает первый дочерний узел
 */
Variant __fastcall OleXml::GetFirstNode(Variant Node) const
{
    return Node.OlePropertyGet("firstChild");
}

/* Возвращает следующий узел от указанного
 */
Variant __fastcall OleXml::GetNextNode(Variant Node) const
{
    return Node.OlePropertyGet("nextSibling");
}

/* Возвращает узел по пути xpath
 */
Variant __fastcall OleXml::SelectSingleNode(const AnsiString& xpath) const
{
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}

/* Проверяет, существует ли атрибут
 */
Variant OleXml::GetAttribute(Variant Node, const AnsiString& AttributeName) const
{
    return Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    //return attribute.IsEmpty();
}

/* Возвращает атрибут по индексу
*/
AnsiString __fastcall OleXml::GetAttributeValue(Variant Node, int AttributeIndex) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("item", AttributeIndex).OlePropertyGet("Value");
}

/* Возвращает значение атрибута по имени
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

    // Второй способ
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
}

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
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

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
 */
int __fastcall OleXml::GetAttributeValue(Variant Node, const AnsiString& AttributeName, int DefaultValue) const
{

    Variant attribute = GetAttribute(Node, AttributeName);
    return (attribute.IsEmpty()) ? DefaultValue : attribute.OlePropertyGet("Value");

/*    AnsiString attribute = Trim(GetAttributeValue(Node, AttributeName));  // Ширина столбца
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

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
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

/* Проверяет наличие ошибок разбора XML
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

