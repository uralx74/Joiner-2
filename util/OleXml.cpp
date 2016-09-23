#include "OleXml.h"

OleXml::OleXml(bool validateOnParse)
{
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    xmlDoc.OlePropertySet("validateOnParse", validateOnParse);
    xmlDoc.OlePropertySet("resolveExternals", false);
}

OleXml::~OleXml()
{
    xmlDoc = Unassigned;   // эквивалентно VarClear(xmlDoc)
}

/* Создает корневой узел
 */
Variant __fastcall OleXml::CreateRootNode(const AnsiString& nodeName)
{
    Variant processingInstructions = xmlDoc.OleFunction("createProcessingInstruction", "xml", "version='1.0' encoding='windows-1251'");
    xmlDoc.OleFunction("appendChild", processingInstructions);

    Variant node = xmlDoc.OleFunction("CreateElement", nodeName);
    xmlDoc.OlePropertySet("DocumentElement", node);

    return node;
}

/* Создает дочерний узел
 */
Variant __fastcall OleXml::CreateNode(Variant parentNode, const AnsiString& nodeName)
{
    //node.OlePropertySet("DataType", "bin.base64");

    Variant node = xmlDoc.OleFunction("CreateElement", nodeName);
    return node;
}

/* Добавляет дочерний узел
 * необходима проверка, так как не добавляется один и тот же узел несколько раз
 */
Variant __fastcall OleXml::AddChildNode(Variant parentNode, Variant childNode)
{
    Variant node = parentNode.OleFunction("appendChild", childNode);
    return node;
}

/* Клонирует узел
   bool deep - указывает копировать вложенные узлы или нет
 */
Variant __fastcall OleXml::CloneNode(Variant node, bool deep)
{
    return node.OleFunction("cloneNode", deep);
}

/* Добавляет атрибут
 */
Variant __fastcall OleXml::AddAttributeNode(Variant node, const AnsiString& attributeName, const AnsiString& attributeValue)
{
    Variant attribute = xmlDoc.OleFunction("CreateAttribute", attributeName);
    attribute.OlePropertySet("Value", attributeValue);
    node.OleProcedure("setAttributeNode", attribute);

    return attribute;
}

/* Добавляет текст в узел
   После этого Узел считается текстовым и добавлять в него дочерние узлы нельзя
 */
void __fastcall OleXml::SetTextNode(Variant node, const AnsiString& nodeText)
{
    //Variant node = AddNode(parentNode, nodeText);
    node.OlePropertySet("text", nodeText);
}

/* Устанавливает значение атрибута
*/
void __fastcall OleXml::SetAttributeValue(Variant node, const AnsiString& attributeName, const AnsiString& value)
{
    GetAttribute(node, attributeName).OlePropertySet("text", value);
}

/*
   Необходимо протестировать
 */
bool __fastcall OleXml::IsTextElement(Variant node) const
{
    return node.OlePropertyGet("IsTextElement");
}

/* Проверяет, есть ли в указанном узле дочерние узлы
 */
bool __fastcall OleXml::HasChildNodes(Variant node) const
{
    return node.OleFunction("hasChildNodes");
}

/* Сохраняет xml-документ в файл
 */
void __fastcall OleXml::Save(const String& filename) const
{
    xmlDoc.OleProcedure("Save", filename);
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
int __fastcall OleXml::GetAttributesCount(Variant node) const
{
    return node.OlePropertyGet("attributes").OlePropertyGet("length");
}

AnsiString __fastcall OleXml::GetNodeText(Variant node) const
{
    return node.OlePropertyGet("text");
}

/* Возвращает корневой узел
 */
Variant __fastcall OleXml::GetRootNode() const
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

/* Возвращает имя узла
 */
AnsiString __fastcall OleXml::GetNodeName(Variant node) const
{
    return node.OlePropertyGet("NodeName");
}

/* Возврщает первый дочерний узел
 */
Variant __fastcall OleXml::GetFirstNode(Variant node) const
{
    return node.OlePropertyGet("firstChild");
}

/* Возвращает следующий узел от указанного
 */
Variant __fastcall OleXml::GetNextNode(Variant node) const
{
    return node.OlePropertyGet("nextSibling");
}

/* Возвращает узел по пути xpath
   отталкивается от корневого элемета
 */
Variant __fastcall OleXml::SelectSingleNode(const AnsiString& xpath) const
{
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}

/* Возвращает узел по пути xpath
 */
Variant __fastcall OleXml::SelectSingleNode(Variant node, const AnsiString& xpath) const
{
    return node.OleFunction("selectSingleNode", xpath); // selectSingleNode
}


/* Проверяет, существует ли атрибут
 */
Variant OleXml::GetAttribute(Variant node, const AnsiString& attributeName) const
{
    return node.OlePropertyGet("attributes").OleFunction("getNamedItem", attributeName);
    //return attribute.IsEmpty();
}

/* Возвращает атрибут по индексу
*/
AnsiString __fastcall OleXml::GetAttributeValue(Variant node, int attributeIndex) const
{
    return node.OlePropertyGet("attributes").OlePropertyGet("item", attributeIndex).OlePropertyGet("Value");
}

/* Возвращает значение атрибута по имени
*/
AnsiString __fastcall OleXml::GetAttributeText(Variant node, const AnsiString& attributeName) const
{
    Variant attribute = GetAttribute(node, attributeName);
    if (!attribute.IsEmpty())
    {
        return attribute.OlePropertyGet("text");
    }
    else
    {
        throw Exception("Attribute " + attributeName + "is empty.");
    }

    // Второй способ
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
}

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
 */
AnsiString __fastcall OleXml::GetAttributeValue(Variant node, const AnsiString& attributeName, const AnsiString& DefaultValue) const
{
    Variant attribute = GetAttribute(node, attributeName);
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
int __fastcall OleXml::GetAttributeValue(Variant node, const AnsiString& attributeName, int defaultValue) const
{

    Variant attribute = GetAttribute(node, attributeName);
    return (attribute.IsEmpty()) ? defaultValue : attribute.OlePropertyGet("Value");

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
bool __fastcall OleXml::GetAttributeValue(Variant node, const AnsiString& attributeName, bool defaultValue) const
{
    Variant attribute = GetAttribute(node, attributeName);

    if (attribute.IsEmpty()) {
        return defaultValue;
    } else {
        String textValue = attribute.OlePropertyGet("Value");
        if (textValue == TRUE_STR_VALUE)
        {
            return true;
        }
        else if (textValue == FALSE_STR_VALUE)
        {
            return false;
        }
        else
        {
            return defaultValue;
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




