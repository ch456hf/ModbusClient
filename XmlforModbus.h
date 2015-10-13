#ifndef XMLFORMODBUS_H
#define XMLFORMODBUS_H

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include "content.h"

using namespace std;

#define DEVTABLE   1
#define CTRLTABLE  2

class XmlforModbus
{
    public:
        XmlforModbus();
        bool ReadDevTableXml();
        bool ReadCtrlTableXml();
        bool WriteTableXml(string networkAddr,int channel,int addr,
                              enum INFOTYPE type,xmlDocPtr doc,string filename);
        virtual ~XmlforModbus();
    public:
        xmlDocPtr DevTabledoc;
        xmlDocPtr CtrlTabledoc;
    protected:
    private:
        bool ParseXml(xmlDocPtr doc,int table);
        void SetXmlforDevTable(string networkAddr,int channel,int addr,enum INFOTYPE type);
        void SetXmlforCtrlTable(string networkAddr,int channel,int addr,enum INFOTYPE type);
        enum INFOTYPE GetType(char *content);
        xmlDocPtr CreateXml(string name);
};

#endif // XMLFORMODBUS_H
