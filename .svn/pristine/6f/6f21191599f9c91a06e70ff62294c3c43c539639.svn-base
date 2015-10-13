#include "XmlforModbus.h"
#include "MBDevTable.h"
#include "MBCtrlTable.h"
#include <sys/time.h>

XmlforModbus::XmlforModbus()
{
    //ctor
    xmlKeepBlanksDefault(0);
    ReadDevTableXml();
    ReadCtrlTableXml();
}

XmlforModbus::~XmlforModbus()
{
    //dtor
}

bool XmlforModbus::ReadDevTableXml()
{
    char szFile[512];
    strcpy(szFile,"DevTable.xml");
    DevTabledoc = xmlReadFile(szFile,"UTF-8",XML_PARSE_RECOVER);

    if(DevTabledoc == NULL) {
        DevTabledoc = CreateXml("DevTable.xml");
    }
    if(!ParseXml(DevTabledoc,DEVTABLE)) {
        printf("Parse DevTable failed!\n");
        return false;
    }
    return true;
}

bool XmlforModbus::ReadCtrlTableXml()
{

    char szFile[512];
    strcpy(szFile,"CtrlTable.xml");
    CtrlTabledoc = xmlReadFile(szFile,"UTF-8",XML_PARSE_RECOVER);

    if(CtrlTabledoc == NULL) {
        CtrlTabledoc = CreateXml("CtrlTable.xml");
    }
    if(!ParseXml(CtrlTabledoc,CTRLTABLE)) {
        printf("Parse CtrlTable failed!\n");
        return false;
    }
    return true;
}

/*
  1.用xmlNewDoc 函数创建一个文档指针 doc;
  2.用xmlNewNode 函数创建一个节点指针 root_node;
  3.用xmlDocSetRootElement 将 root_node 设置为 doc 的根节点;
  4.给root_node 添加一系列子节点，并设置字节点的内容和属性;
  5.用xmlSaveFile 保存 xml 到文件;
  6.用xmlFreeDoc 函数关闭文档指针，清除内存。
*/
xmlDocPtr XmlforModbus::CreateXml(string name)
{
    xmlDocPtr doc;
    xmlNodePtr proot_node = NULL;
    doc = xmlNewDoc(BAD_CAST "1.0");
    proot_node = xmlNewNode(NULL,BAD_CAST "root");
    xmlDocSetRootElement(doc,proot_node);
    int nRel = xmlSaveFile(name.c_str(),doc);
    if (nRel != -1)
    {
       cout<<"一个xml文档被创建,写入"<< nRel << "个字节" <<endl;
    }
    return doc;
}

bool XmlforModbus::WriteTableXml(string networkAddr,int channel,int addr,
                                    enum INFOTYPE type,xmlDocPtr doc,string filename)
{
    struct timeval start;
    struct timeval end;
    xmlNodePtr root,pnode;
    char content[50];
    root = xmlDocGetRootElement(doc);
    if (NULL == root) {
        printf("Document not parsed sucessfully!\n");
        xmlFreeDoc(doc);
        return -1;
    }
    pnode = xmlNewNode(NULL,BAD_CAST "Node");
    strcpy(content,networkAddr.c_str());
    xmlNewChild(pnode,NULL,BAD_CAST "networkAddr",BAD_CAST content);
    sprintf(content,"%d",channel);
    xmlNewChild(pnode,NULL,BAD_CAST "channel",BAD_CAST content);
    sprintf(content,"%d",addr);
    xmlNewChild(pnode,NULL,BAD_CAST "addr",BAD_CAST content);
    switch(type) {
        case AI:
            strcpy(content,"AI"); break;
        case AO:
            strcpy(content,"AO"); break;
        case DI:
            strcpy(content,"DI"); break;
        case DO:
            strcpy(content,"DO"); break;
        default:
            break;
    }
    xmlNewChild(pnode,NULL,BAD_CAST "type",BAD_CAST content);
    xmlAddChild(root,pnode);
    gettimeofday(&start,NULL);
    int nRel = xmlSaveFile(filename.c_str(),doc);
    gettimeofday(&end,NULL);
    struct timeval result;
    timersub(&end,&start,&result);
    printf("time use:%d\r\n",result.tv_sec*1000000+result.tv_usec);
    if (nRel != -1)
    {
       //cout<<"一个xml文档被写入"<< nRel << "个字节" <<endl;
    }
    return true;
}

bool XmlforModbus::ParseXml(xmlDocPtr doc,int table)
{
    char *content;
    char *nodename;
    string networkAddr;
    int channel;
    int addr;
    enum INFOTYPE type;
    xmlNodePtr curNode,curChildNode;

    curNode = xmlDocGetRootElement(doc);
    if(curNode == NULL) {
        printf("document not parsed sucessfully!\n");
        xmlFreeDoc(doc);
        return false;
    }

    curNode = curNode->xmlChildrenNode;
    while(curNode != NULL) {
        curChildNode = curNode->xmlChildrenNode;
        while(curChildNode != NULL) {
            content = (char *)xmlNodeGetContent(curChildNode);
            nodename = (char *)curChildNode->name;
           // printf("nodename = %s content = %s\n",nodename,content);
            if(!strcmp(nodename,"networkAddr")) {
                networkAddr.assign(content);
            }else if(!strcmp(nodename,"channel")) {
                channel = atoi(content);
            }else if(!strcmp(nodename,"addr")) {
                addr = atoi(content);
            }else if(!strcmp(nodename,"type")) {
                type = GetType(content);
            }
            curChildNode = curChildNode->next;
        }
        switch(table) {
            case DEVTABLE:
                SetXmlforDevTable(networkAddr,channel,addr,type);
                break;
            case CTRLTABLE:
                SetXmlforCtrlTable(networkAddr,channel,addr,type);
                break;
            default:
                break;
        }
        curNode = curNode->next;
    }
    return true;
}

enum INFOTYPE XmlforModbus::GetType(char *content)
{
    if(!strcmp(content,"AI")) {
        return AI;
    }else if(!strcmp(content,"AO")) {
        return AO;
    }else if(!strcmp(content,"DI")) {
        return DI;
    }else if(!strcmp(content,"DO")) {
        return DO;
    }
}

void XmlforModbus::SetXmlforDevTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    MBDevTable *mbdevtable = MBDevTable::GetInstance();
    mbdevtable->XmlConfigTable(networkAddr,channel,addr,type);
  //  cout << "networkAddr = " << networkAddr << endl;
  //  cout << "channel = " << channel << "addr = " << addr << "type = " << type << endl;
}


void XmlforModbus::SetXmlforCtrlTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    MBCtrlTable *mbctrltable = MBCtrlTable::GetInstance();
    cout << "networkAddr = " << networkAddr << endl;
    cout << "channel = " << channel << "addr = " << addr << "type = " << type << endl;
    mbctrltable->XmlConfigTable(networkAddr,channel,addr,type);
}


