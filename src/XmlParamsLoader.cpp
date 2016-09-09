#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "XmlParamsLoader.h"


namespace utilfiles {
/* ��������� ���� � ����� FileName �� �������������� � �����������,
 * ��������� ������ ���� FilePath,
 * �� ������ ���� ���� FileName �������� �������������
 * ����������: � ������� ������� ��������� � ��������� ����������
 */
AnsiString ExpandFileNameCustom(AnsiString FileName, AnsiString FilePath)
{
    AnsiString result = ExpandFileName(FileName);
    if (UpperCase(result) != UpperCase(Trim(FileName))) {
        result = ExtractFilePath(FilePath) + Trim(FileName);
    } else {
        result = FileName;
    }
    return result;
}
}

//---------------------------------------------------------------------------
//
TXmlLoader::TXmlLoader()
{
    Logger = &TLogger::getInstance();
    CommandLine = &TCommandLine::getInstance();

    SrcStor = NULL;
    DstStor = NULL;

}

//---------------------------------------------------------------------------
//
TXmlLoader::~TXmlLoader()
{
    if (SrcStor != NULL)
        delete SrcStor;
    if (DstStor != NULL)
        delete DstStor;
}



//---------------------------------------------------------------------------
// ��������� ��������� �� ��������� ������
// � ����� ������������
bool __fastcall TXmlLoader::LoadParameters()
{

// !!!!!!!!!!!!
// �������� ������ ExpandFilename ����� ������������ ���� �� xml-����� ������������.



    clConfig = CommandLine->GetValue("-config","-c");
    clDstUsername = CommandLine->GetValue("-dstuser","-du");
    clDstPassword = CommandLine->GetValue("-dstpassword","-dp");
    clSrcUsername = CommandLine->GetValue("-srcuser","-su");
    clSrcPassword = CommandLine->GetValue("-srcpassword","-sp");
    //clLogfile = CommandLine->GetValue("-logfile","-l");
    //clSilent = CommandLine->GetValue("-silent","-s");

    // ��������������� ������������� ���� � ����� � ���������� ����
    clConfig = ExpandFileName(clConfig);
    clConfigPath = ExtractFilePath(clConfig);

    int LogLine = Logger->WriteLog("�������� ����� ������������ > " + clConfig);

    if (!FileExists(clConfig)) {
        Logger->WriteLog("������. ���� ������������ �� ������ " + clConfig);
        return false;
    }

    CoInitialize(NULL);

    OleXml msxml;
    msxml.LoadXMLFile(clConfig);

    //OleXml newXml;
    //Variant importNode = newXml.CreateRootNode("import");


    if (msxml.GetParseError() != "") {
        Logger->WriteLog("������: " + msxml.GetParseError());
        return false;
    }

    Variant rootNode = msxml.GetRootNode();
    Variant storageNode;

    if (VarIsEmpty(rootNode)) {
        return false;
    }

    Logger->WriteLog("�������� ���������� ��������� ������...", LogLine);
    SrcStor = ProceedStorageNode(msxml, "//config/import");
    Logger->WriteLog("�������� ���������� ��������� ������...", LogLine);
    DstStor = ProceedStorageNode(msxml, "//config/export");
}

/*
 */
TStorage* TXmlLoader::ProceedStorageNode(const OleXml& msxml, const AnsiString& nodeXpath)
{
    Variant storageNode = msxml.SelectSingleNode(nodeXpath);
    bool isImport = (LowerCase(msxml.GetNodeName(storageNode)) == "import");
    if (!VarIsEmpty(storageNode)) {
        int LogLine = 0;

        TStorage* storage = new TStorage();
        storage->registerFactory("dbase4", new TableFactory<TStorageDbase>);
        //storage->registerFactory("excel", new DbfFactory());
        //storage->registerFactory("oraproc", new DbfFactory());
        //storage->registerFactory("orasql", new DbfFactory());
        //storage->registerFactory("text", new DbfFactory());
        //storage->loadStorage(msxml, storageNode, true);

        Variant tableNode = msxml.SelectSingleNode(nodeXpath + "/table");


        for ( true ;!tableNode.IsEmpty(); tableNode = msxml.GetNextNode(tableNode) ) {

            String storageType = msxml.GetAttributeValue(tableNode, "type");
            if (!storage->factoryExists(storageType)) {
                // ���� ��� ������� �� ���������������, �� ������� ���������
                // � �� ��������� � �������������� ������ ������.
                Logger->WriteLog("������. ��� ������� type=\"" + storageType + "\" �� ���������������. ��� ������� �������������� �� �����.");
                continue;
            }


            AnsiString filename = msxml.GetAttributeValue(tableNode, "file");
            if (filename != "") {
                AnsiString fullFilename = utilfiles::ExpandFileNameCustom(filename, clConfigPath);

                TSearchRec searchRec;
                FindFirst(fullFilename, faAnyFile, searchRec);
                AnsiString filePath = ExtractFilePath(fullFilename);

                // ������� ����� ����
                // ���� ��������, �� ������������ ����� �� �����, � ����������� ������ �� ������� �����,
                // ����� ��������� ���������� ���� ��� ������ � �������� �� �������������
                Variant singleTableNode = msxml.CloneNode(tableNode, true);
                if (isImport) {
                    if (searchRec.Name != "") {
                        do {
                            msxml.SetAttributeValue(singleTableNode, "file", filePath + searchRec.Name);
                            //msxml.AddAttributeNode(singleTableNode, "readonly", "true");
                            storage->loadStorage(msxml, singleTableNode, false);
                        } while ( FindNext(searchRec) == 0 );
                    }
                } else {
                    msxml.SetAttributeValue(singleTableNode, "file", fullFilename);
                    //msxml.AddAttributeNode(singleTableNode, "readonly", "false");

                    bool xmlSourceAsTemplate = msxml.GetAttributeValue(singleTableNode, "source_as_template", false);
                    //StorDbase->AddTemplate(xmlTemplate);

                    /*if (xmlTemplate != "") {    // ���� ����� ���� � �������
                        try {
                            StorDbase->setTemplate(new TStorageDbase(ExpandFileNameCustom(xmlTemplate, clConfigPath)), true);
                        } catch (Exception &e) {
                            Logger->WriteLog("������. �� ������� ��������� ������ �� ����� " + xmlTemplate + ".");
                            throw Exception("");
                        }
                    } else if (xmlSourceAsTemplate) {
                        // ����� �� ��������� ����� �������������?
                        // ��� �������� ������ ����� �� ����� ��������� ��������� �� ������, �������, ��������?
                    StorDbase->setTemplate(SrcStor);  */



                    storage->loadStorage(msxml, singleTableNode, false);
                }
            } else {
                storage->loadStorage(msxml, tableNode, isImport);
            }
        }
        return storage;
    }
}

            //String storageType = msxml.GetNodeName(tableNode);
            //tableNode = msxml.GetNextNode(tableNode);
            //Variant tableNode = msxml.GetFirstNode(storageNode);
            //String storageType2 = msxml.GetAttributeValue(tableNode, "type");
   /*
            while (!tableNode.IsEmpty()) {
                String storageType = oleXml.GetNodeName(tableNode);
            }

        // ���� ��� ��������� ���������������
        if (factoryExists()) {
            AnsiString attrFile = oleXml.GetAttributeValue(tableNode, "file");
            ExpandFileNameCustom(attrFile,);

            if (attrFile != "") {

                TSearchRec searchRec;
                FindFirst(attrFile, faAnyFile, searchRec);

                if (searchRec.Name != "") {
                    //AnsiString FilePath = ExtractFilePath(Table.File);
                    do {
                        //TStorageTable* table = tableFactory->newTable();

                        //table->file = FilePath + SearchRec.Name;
                        //Tables.push_back(table);
                    } while ( FindNext(searchRec) == 0);

                } else {
                    //TStorageTable* table = tableFactory->newTable();
                    //Tables.push_back(table);
                }
            }


            //OleXml* newXml = new OleXml();
            //newXml->CreateRootNode("thisIsRoot");
            //newXml->Save("c:\\test\\nex.xml");
            //oleXml.Save("c:\\test\\old.xml");

            return;



             TableFactoryBase* tableFactory = tableFactories[storageType];
             TStorageTable* table = tableFactory->newTable();

             //TStorageTable* table = tableFactory->newTable(oleXml, tableNode);
             //vector<TStorageTable*> loadedTables = tableFactory->loadTables(oleXml, tableNode);

             Tables.push_back(table);
             //Tables.insert(loadedTables);
        }

        /*XmlBranch* subBranch = new XmlBranch();
        branch->branch.insert(BranchType::value_type(subBranchName, subBranch));
        subBranch = LoadToXmlBranch(subnode);
        subnode = GetNextNode(subnode);*/
    









 /*
        if (!VarIsEmpty(storageNode)) {
            Logger->WriteLog("������ ���������� �������", LogLine);

            TStorage* storage = new TStorage();
            storage->registerFactory("dbase4", new TableFactory<TStorageDbase>);

            //TableFactoryBase* tableFactory = tableFactories[storageParameters["type"]];


            //storage->registerFactory("dbase4", new DbfFactory());


            Variant tableNode = msxml.GetFirstNode(storageNode);

            //StorageParameters* storageParameters = new StorageParameters();


            String k = msxml.GetAttributeValue(tableNode, "file");

            StorageParameters params;
            params["type"] = msxml.GetNodeName(tableNode);
            params["file"] = ExpandFileNameCustom(msxml.GetAttributeValue(tableNode, "file"), clConfigPath);

            //storageParameters->type = msxml.GetNodeName(tableNode);
            //storageParameters->file = ExpandFileNameCustom(msxml.GetAttributeValue(tableNode, "file"), clConfigPath);
            storage->loadTables(params);
        }





        //exportNode = msxml.FindNode(rootNode, "export");
        //Logger->WriteLog("������ ���������� ��������", LogLine);
    }


    Logger->WriteLog("���� ������������ �������� " + clConfig, LogLine);
    return true;






/*    while (!node.IsEmpty())
    {
        String sNodeName = msxml.GetNodeName(node);
        if (sNodeName == "import")  {
            Variant subnode = msxml.GetFirstNode(node);
            AnsiString sImportType = msxml.GetNodeName(subnode);

            if (sImportType == "dbase4") {
                TStorageDbase* StorDbase = new TStorageDbase();
                SrcStor = (TStorage*) StorDbase;

                while (!subnode.IsEmpty()) {
                    if (msxml.GetNodeName(subnode) == "dbase4") {
                        TDbaseTable Table;
                        Table.File = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "file"), clConfigPath);  // ��� ����� ����� ���� ������ ������
                        StorDbase->addTable(Table);
                    }
                    subnode = msxml.GetNextNode(subnode);
                }
            } else if (sImportType == "orasql" || sImportType == "oratable") {
                TStorageOraSql* StorOraSql = new TStorageOraSql();
                SrcStor = (TStorage*) StorOraSql;

                while (!subnode.IsEmpty()) {
                    if (msxml.GetNodeName(subnode) == "orasql" || sImportType == "oratable") {
                        TOraSqlTable Table;
                        Table.Server = msxml.GetAttributeValue(subnode, "server");

                        // � ���������� ����������. 1. ��������� ������ ������������; 2. ����������-������������
                        // ���� ����� � OraProc � � ��������

                        // ���� ������ ��� ������������ � ������ � ��������� ������
                        if (clSrcUsername != "" || clSrcPassword != "") {
                            Table.Username = clSrcUsername;
                            Table.Password = clSrcPassword;
                        } else {
                            AnsiString code = msxml.GetAttributeValue(subnode, "code");
                            if (code != "") {
                                TEncoder encoder;
                                try {
                                    encoder.Decode(code, Table.Username, Table.Password );
                                } catch (...) {
                                    Logger->WriteLog("������. �� ������� ������������ ������ � ��������� \"" + Table.Server + "\"");
                                    return false;
                                }
                            } else {    // ���� ������ �� ������������� ��������
                                Table.Username = msxml.GetAttributeValue(subnode, "username");
                                Table.Password = msxml.GetAttributeValue(subnode, "password", clSrcPassword);
                            }
                        }


                        if (sImportType == "orasql") {
                            //Table.Sql = ExpandFileName(msxml.GetAttributeValue(subnode, "sql"));
                            Table.Sql = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "sql"), clConfigPath);

                        }
                        Table.Table = msxml.GetAttributeValue(subnode, "table");
                        StorOraSql->addTable(Table);
                    }
                    subnode = msxml.GetNextNode(subnode);
                }
            } else if (sImportType == "excel") {
                TStorageExcel* StorageExcel = new TStorageExcel();
                SrcStor = (TStorage*) StorageExcel;

                while (!subnode.IsEmpty()) {
                    if (msxml.GetNodeName(subnode) == "excel") {
                        TExcelTable Table;
                        Table.File = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "file"), clConfigPath);  // ��� ����� ����� ���� ������ ������
                        StorageExcel->addTable(Table);
                    }
                    subnode = msxml.GetNextNode(subnode);
                }
            }
        } else if (sNodeName == "export") {

            Variant subnode = msxml.GetFirstNode(node);
            AnsiString sExportType = msxml.GetNodeName(subnode);
            if (sExportType == "dbase4") {

                TStorageDbase* StorDbase = new TStorageDbase();
                DstStor = (TStorage*) StorDbase;
                //DstStor->SetReadOnly(false);

                TDbaseTable Table;
                Table.File = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "file"), clConfigPath);
                Table.Truncate = msxml.GetAttributeValue(subnode, "truncate", false);
                StorDbase->addTable(Table);

                String xmlTemplate = msxml.GetAttributeValue(subnode, "template", "");
                bool xmlSourceAsTemplate = msxml.GetAttributeValue(subnode, "source_as_template", false);

                //StorDbase->AddTemplate(xmlTemplate);

                if (xmlTemplate != "") {    // ���� ����� ���� � �������
                    try {
                        StorDbase->setTemplate(new TStorageDbase(ExpandFileNameCustom(xmlTemplate, clConfigPath)), true);
                    } catch (Exception &e) {
                        Logger->WriteLog("������. �� ������� ��������� ������ �� ����� " + xmlTemplate + ".");
                        throw Exception("");
                    }
                } else if (xmlSourceAsTemplate) {
                    // ����� �� ��������� ����� �������������?
                    // ��� �������� ������ ����� �� ����� ��������� ��������� �� ������, �������, ��������?
                    StorDbase->setTemplate(SrcStor);

                } else {

                    // ����� �������� ���� ��������� TStorage � ��������� � ���� ���� ?
                    Variant node_fields = msxml.GetFirstNode(subnode);

                    while (!node_fields.IsEmpty()) {
                        if (msxml.GetNodeName(node_fields) == "field") {
                            // �������� ���������� �� ����������� ������� ��� ��.
                            // � ����� ������������ �� �������� �������
                            TDbaseField* dbaseField = StorDbase->addField();
                            if (dbaseField != NULL) {
                                // ������� ������� ��� ��������� ��������
                                dbaseField->type = LowerCase(msxml.GetAttributeValue(node_fields, "type", "C"))[1];
                                dbaseField->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                                dbaseField->length = msxml.GetAttributeValue(node_fields, "length", 0);
                                dbaseField->decimals = msxml.GetAttributeValue(node_fields, "decimals", 0);
                                dbaseField->active = msxml.GetAttributeValue(node_fields, "active", true);
                                dbaseField->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                                dbaseField->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", dbaseField->name));
                            }
                            //if (dbaseField->name_src == "" )
                            //    dbaseField->name_src = dbaseField->name;
                        }
                        node_fields = msxml.GetNextNode(node_fields);
                    }
                }

           } else if (sExportType == "oraproc") {   //
                TStorageOraProc* StorOraProc = new TStorageOraProc();
                DstStor = (TStorage*) StorOraProc;

                TOraProcTable Table;
                Table.Server = msxml.GetAttributeValue(subnode, "server");

                //!!! �������� ����������� - ��������� �����������!
                // ���� ������ ��� ������������ � ������ � ��������� ������
                if (clSrcUsername != "" || clSrcPassword != "") {
                    Table.Username = clSrcUsername;
                    Table.Password = clSrcPassword;
                } else {
                    AnsiString code = msxml.GetAttributeValue(subnode, "code");
                    if (code != "") {
                        TEncoder encoder;
                        try {
                            encoder.Decode(code, Table.Username, Table.Password );
                        } catch (...) {
                            Logger->WriteLog("������. �� ������� ������������ ������ � ��������� \"" + Table.Server + "\"");
                            return false;
                        }
                    } else {    // ���� ������ �� ������������� ��������
                        Table.Username = msxml.GetAttributeValue(subnode, "username");
                        Table.Password = msxml.GetAttributeValue(subnode, "password", clSrcPassword);
                    }
                }

                Table.Procedure = msxml.GetAttributeValue(subnode, "procedure");
                Table.Table = msxml.GetAttributeValue(subnode, "table");
                Table.Truncate = msxml.GetAttributeValue(subnode, "truncate", false);
                StorOraProc->addTable(Table);


                Variant node_fields = msxml.GetFirstNode(subnode);
                while (!node_fields.IsEmpty()) {
                    if (msxml.GetNodeName(node_fields) == "field") {
                        TOraField* field = StorOraProc->addField();   // AddField

                        if (field != NULL) {
                            field->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                            field->active = msxml.GetAttributeValue(node_fields, "active", true);
                            field->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                            field->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", field->name));
                            //field->name_src = field->name_src == "" ? "" : field->name;
                            //field->name_src = field->name;
                        }
                    }
                    node_fields = msxml.GetNextNode(node_fields);
                }

            } else if (sExportType == "oratable") {   // �������� - ������� � �� Oracle

                TStorageOraSql* StorOraTable = new TStorageOraSql();
                DstStor = (TStorage*) StorOraTable;
                //DstStor->SetReadOnly(false);

                TOraSqlTable Table;
                Table.Server = msxml.GetAttributeValue(subnode, "server");


                // ���� ������ ��� ������������ � ������ � ��������� ������
                if (clSrcUsername != "" || clSrcPassword != "") {
                    Table.Username = clSrcUsername;
                    Table.Password = clSrcPassword;
                } else {
                    AnsiString code = msxml.GetAttributeValue(subnode, "code");
                    if (code != "") {
                        TEncoder encoder;
                        try {
                            encoder.Decode(code, Table.Username, Table.Password );
                        } catch (...) {
                            Logger->WriteLog("������. �� ������� ������������ ������ � ��������� \"" + Table.Server + "\"");
                            return false;
                        }
                    } else {    // ���� ������ �� ������������� ��������
                        Table.Username = msxml.GetAttributeValue(subnode, "username");
                        Table.Password = msxml.GetAttributeValue(subnode, "password", clSrcPassword);
                    }
                }

                Table.Sql = ""; // �����!
                Table.Table = msxml.GetAttributeValue(subnode, "table");
                Table.Truncate = msxml.GetAttributeValue(subnode, "truncate", false);
                StorOraTable->addTable(Table);

                Variant node_fields = msxml.GetFirstNode(subnode);
                while (!node_fields.IsEmpty()) {
                    if (msxml.GetNodeName(node_fields) == "field") {
                        TOraField* field = StorOraTable->addField();      // �������� �������� ��
                        if (field != NULL) {
                            field->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                            field->active = msxml.GetAttributeValue(node_fields, "active", true);
                            field->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                            field->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", field->name));
                        }
                    }
                    node_fields = msxml.GetNextNode(node_fields);
                }
            } else if (sExportType == "excel") {

                TStorageExcel* StorageExcel = new TStorageExcel();
                DstStor = (TStorage*) StorageExcel;
                //DstStor->SetReadOnly(false);

                TExcelTable Table;
                Table.File = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "file"), clConfigPath);
                Table.Truncate = msxml.GetAttributeValue(subnode, "truncate", false);
                StorageExcel->addTable(Table);

                Variant node_fields = msxml.GetFirstNode(subnode);
                while (!node_fields.IsEmpty()) {
                    if (msxml.GetNodeName(node_fields) == "field") {
                        TExcelField* field = StorageExcel->addField();
                        if (field) {
                            field->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                            field->active = msxml.GetAttributeValue(node_fields, "active", true);
                            field->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                            field->format = msxml.GetAttributeValue(node_fields, "format", "");
                            field->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", field->name));
                            if (field->name_src == "" )
                                field->name_src = field->name;
                            }
                    }
                    node_fields = msxml.GetNextNode(node_fields);
                }
            } else if (sExportType == "sqltext") {

                TStorageSqlText* StorageSqlText = new TStorageSqlText();
                DstStor = (TStorage*) StorageSqlText;
                //DstStor->SetReadOnly(false);

                TSqlTextTable Table;
                Table.File = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "file"), clConfigPath);
                Table.Template = ExpandFileNameCustom(msxml.GetAttributeValue(subnode, "template"), clConfigPath);
                //Table.Truncate = msxml.GetAttributeValue(subnode, "truncate", false);
                StorageSqlText->addTable(Table);

                Variant node_fields = msxml.GetFirstNode(subnode);
                while (!node_fields.IsEmpty()) {
                    if (msxml.GetNodeName(node_fields) == "field") {
                        TSqlTextField* field = StorageSqlText->addField();
                        if (field) {
                            field->name = LowerCase(msxml.GetAttributeValue(node_fields, "name"));
                            field->active = msxml.GetAttributeValue(node_fields, "active", true);
                            field->enable = msxml.GetAttributeValue(node_fields, "enable", true);
                            //field->format = msxml.GetAttributeValue(node_fields, "format", "");
                            field->name_src = LowerCase(msxml.GetAttributeValue(node_fields, "name_src", field->name));
                            if (field->name_src == "" )
                                field->name_src = field->name;
                        }
                    }
                    node_fields = msxml.GetNextNode(node_fields);
                }
            }
        }
        node = msxml.GetNextNode(node);
    }

    */


            


