#include "MSXMLWorks.h"



MsxmlWorks::MsxmlWorks(bool validateOnParse)
{
    xmlDoc = CreateOleObject("Msxml2.DOMDocument.3.0");
    xmlDoc.OlePropertySet("Async", false);
    xmlDoc.OlePropertySet("validateOnParse", validateOnParse);
}

MsxmlWorks::~MsxmlWorks()
{
    xmlDoc = Unassigned;   // эквивалентно VarClear(xmlDoc)
}

/* Загружает xml из файла файл
 */
void __fastcall MsxmlWorks::LoadXMLFile(const AnsiString& XMLFileName)
{
    xmlDoc.OlePropertyGet("Load", XMLFileName.c_str());
    //rootNode = XmlDoc.OlePropertyGet("documentElement");
}

/* Загружает xml из строки
 */
void __fastcall MsxmlWorks::LoadXMLText(const AnsiString& XMLText)
{
    xmlDoc.OlePropertyGet("LoadXML", XMLText.c_str());
}

/* Возвращает количество атрибутов узла
 */
int __fastcall MsxmlWorks::GetAttributesCount(Variant Node) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("length");
}

/* Возвращает корневой узел
 */
Variant __fastcall MsxmlWorks::GetRootNode() const
{
    return xmlDoc.OlePropertyGet("DocumentElement");
}

/* Возвращает имя узла
 */
AnsiString __fastcall MsxmlWorks::GetNodeName(Variant Node) const
{
    return Node.OlePropertyGet("NodeName");
}

/* Возврщает первый дочерний узел
 */
Variant __fastcall MsxmlWorks::GetFirstNode(Variant Node) const
{
    return Node.OlePropertyGet("firstChild");
}

/* Возвращает следующий узел от указанного
 */
Variant __fastcall MsxmlWorks::GetNextNode(Variant Node) const
{
    return Node.OlePropertyGet("nextSibling");
}

/* Возвращает узел по пути xpath
 */
Variant __fastcall MsxmlWorks::SelectSingleNode(const AnsiString& xpath) const
{
    return xmlDoc.OleFunction("selectSingleNode", xpath); // selectSingleNode
}

/* Проверяет, существует ли атрибут
 */
Variant MsxmlWorks::GetAttribute(Variant Node, const AnsiString& AttributeName) const
{
    return Node.OlePropertyGet("attributes").OleFunction("getNamedItem", AttributeName);
    //return attribute.IsEmpty();
}

/* Возвращает атрибут по индексу
*/
AnsiString __fastcall MsxmlWorks::GetAttributeValue(Variant Node, int AttributeIndex) const
{
    return Node.OlePropertyGet("attributes").OlePropertyGet("item", AttributeIndex).OlePropertyGet("Value");
}

/* Возвращает значение атрибута по имени
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

    // Второй способ
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
}

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
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

/* Возвращает значение атрибута,
   если атрибут отсутствует, то возвращает значение DefaultValue
 */
int __fastcall MsxmlWorks::GetAttributeValue(Variant Node, const AnsiString& AttributeName, int DefaultValue) const
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



/* Проверяет наличие ошибок разбора XML
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

/* Струтура для конвертирования xml-объекта в дерево
 * В качестве узлов - объекты multimap, а атрибуты - элементы multimap
 *
 * Необходимо тестирование
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

/* Возвращает значение атрибута по имени
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
    // Второй способ
    //return Node.OleFunction("GetAttribute", StringToOleStr(AttributeName));
} */
