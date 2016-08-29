#include "pch.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "XmlParamsLoader.h"





/*
XmlBranch* TXmlLoader::LoadToSingleXmlBranch(MSXMLWorks* msxml, XmlBranch* branch, Variant node)
{
    while (!node.IsEmpty()) {
        String branchName = msxml->GetNodeName(node);
        XmlBranch* newBranch = branch->branch[branchName] = new XmlBranch();


        //LoadToSingleXmlBranch(msxml, newBranch, subnode);


        Variant subnode = msxml->GetFirstNode(node);

        if (!subnode.IsEmpty()) {
            String subBranchName = msxml->GetNodeName(subnode);

            XmlBranch* subBranch = new XmlBranch();
            newBranch->branch[subBranchName] = subBranch;

            LoadToSingleXmlBranch(msxml, subBranch, subnode);
        }

        node = msxml->GetNextNode(node);

        //LoadToSingleXmlBranch();
    }
}    */

/*
XmlBranch* TXmlLoader::LoadToXmlBranch(Variant node)
{
    CoInitialize(NULL);
    MSXMLWorks* msxml = new MSXMLWorks();
    XmlBranch* branch = LoadToSingleXmlBranch(msxml, node);

    

    /*XmlBranch* root = NULL;// = new XmlBranch();
    XmlBranch* branch;
    Variant subnode = node;

    if (!subnode.IsEmpty()) {
        root = new XmlBranch();
        branch = root;
    }*/







    //root->name = msxml->GetNodeName(node);


/*    //Variant subnode = msxml->GetFirstNode(node);
    while (!subnode.IsEmpty()) {
        XmlBranch* branch = new XmlBranch();

        branch->name = msxml->GetNodeName(subnode);

        //msxml->GetAttributeValue(subnode, "file")

    }     */

/*

    delete msxml;

    return branch;
} */

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

AnsiString TXmlLoader::ExpandFileNameCustom(AnsiString FileName, AnsiString FilePath)
{
    AnsiString result = ExpandFileName(FileName);
    if (UpperCase(result) != UpperCase(Trim(FileName))) {
        result = ExtractFilePath(FilePath) + Trim(FileName);
    } else {
        result = FileName;
    }
    return result;

}

//---------------------------------------------------------------------------
// ��������� ��������� �� ��������� ������
// � ����� ������������
bool __fastcall TXmlLoader::LoadParameters()
{

// !!!!!!!!!!!!
// �������� ������ ExpandFilename ����� ������������ ���� �� xml-����� ������������.


    // ��������� ��������� ������
    AnsiString clConfig;    // ���� � ����� ������������
    AnsiString clConfigPath;    // ���� � ����� ������������

    AnsiString clSrcUsername;  // ��� ������������ ���� ������
    AnsiString clSrcPassword;  // ������ � ���� ������
    AnsiString clDstUsername;  // ��� ������������ ���� ������
    AnsiString clDstPassword;  // ������ � ���� ������

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
    MsxmlWorks msxml;
    msxml.LoadXMLFile(clConfig);

    if (msxml.GetParseError() != "") {
        Logger->WriteLog("������: " + msxml.GetParseError());
        return false;
    }



    Variant rootNode = msxml.GetRootNode();
    //Variant node = msxml.GetFirstNode(RootNode);
    Variant storageNode;

    //Variant exportNode;

    AnsiString sss = msxml.GetNodeName(rootNode);

    if (!VarIsEmpty(rootNode)) {
        storageNode =  msxml.SelectSingleNode("//config/import");




        XmlTreeMultimap* xmlTree = new XmlTreeMultimap(msxml.SelectSingleNode("//config/import"));


        BranchIterator branchIt = xmlTree->rootBranch->branch.begin();
        String sss = branchIt->first;
        branchIt++;
        if (branchIt != xmlTree->rootBranch->branch.end()) {
            sss = branchIt->first;
        }

        delete xmlTree;


        if (!VarIsEmpty(storageNode)) {
            Logger->WriteLog("������ ���������� �������", LogLine);


            TStorage* storage = new TStorage();

            storage->registerFactory("dbase4", new TableFactory<TStorageDbase>);


            //storage->registerFactory("dbase4", new DbfFactory());


            //storage->registerFactory("excel", new DbfFactory());
            //storage->registerFactory("oraproc", new DbfFactory());
            //storage->registerFactory("orasql", new DbfFactory());
            //storage->registerFactory("text", new DbfFactory());





            Variant tableNode = msxml.GetFirstNode(storageNode);

            //StorageParameters* storageParameters = new StorageParameters();


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


}            


