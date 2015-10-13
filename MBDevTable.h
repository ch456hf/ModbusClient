#ifndef MBDEVTABLE_H
#define MBDEVTABLE_H

#include <ext/hash_map>
#include <iostream>
#include <stdio.h>
#include <string>
#include <malloc.h>

#include "content.h"
#include "XmlforModbus.h"

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
    template<> struct hash<const string>
    {
        size_t operator()(const string& s)const
        {
            return hash<const char*>()(s.c_str());
        }
    };
    template<> struct hash<string>
    {
        size_t operator()(const string& s)const
        {
            return hash<const char*>()(s.c_str());
        }
    };
}

typedef struct ModBus_Node
{
    string networkAddr;
    int channel;
    int addr;
    enum INFOTYPE type;
    struct ModBus_Node * next;
}ModBus_Node,*ModBus_Node_ptr;

class MBDevTable
{
    public:
        virtual ~MBDevTable();
        hash_map<string,ModBus_Node_ptr> ModBusDevTable;
        enum CONFIGINFO ConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type);
        enum CONFIGINFO XmlConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type);
        static MBDevTable *GetInstance()
        {
            static MBDevTable _instance;
            return &_instance;
        }
        XmlforModbus *xmlmodbus;

    protected:
    private:
        MBDevTable();
        hash_map<string,ModBus_Node_ptr>::iterator FindNetworkAddr(string networkAddr);
        ModBus_Node_ptr FindChannel(int channel,hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable);
        ModBus_Node_ptr InitNode();
        void InsertChannelNode(ModBus_Node_ptr node_ptr,ModBus_Node_ptr new_node,hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable);

};

#endif // MBDEVTABLE_H
