#include "MBDevTable.h"

MBDevTable::MBDevTable()
{
    //ctor
}

MBDevTable::~MBDevTable()
{
    //dtor
}

ModBus_Node_ptr MBDevTable::InitNode()
{
    ModBus_Node_ptr node;
    node = new ModBus_Node();
    node->next = NULL;
    return node;
}

hash_map<string,ModBus_Node_ptr>::iterator MBDevTable::FindNetworkAddr(string networkAddr)
{
    hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable;
    iterMBDevTable = ModBusDevTable.find(networkAddr);
    return iterMBDevTable;
}

ModBus_Node_ptr MBDevTable::FindChannel(int channel,hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable)
{
    ModBus_Node_ptr node_ptr;
    node_ptr = iterMBDevTable->second;
    while(node_ptr != NULL)
    {
        if(node_ptr->channel == channel)
            return node_ptr;
        node_ptr = node_ptr->next;
    }
    return NULL;
}

void MBDevTable::InsertChannelNode(ModBus_Node_ptr node_ptr,ModBus_Node_ptr new_node,hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable)
{
    ModBus_Node_ptr pre_node = NULL;
    while(node_ptr->next != NULL)
    {
        if(new_node->channel > node_ptr->channel && new_node->channel < node_ptr->next->channel)
        {
            new_node->next = node_ptr->next;
            node_ptr->next = new_node;
            return;
        }else{
            pre_node = node_ptr;
            node_ptr = node_ptr->next;
        }
    }
    if(new_node->channel > node_ptr->channel)
        node_ptr->next = new_node;
    else{
        if(pre_node == NULL)
        {
            new_node->next = node_ptr;
            iterMBDevTable->second = new_node;
        }else{
            new_node->next = node_ptr;
            pre_node->next = new_node;
        }
    }
    return;
}

enum CONFIGINFO MBDevTable::ConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable;
    ModBus_Node_ptr new_node,node_ptr;
    new_node = InitNode();
    new_node->networkAddr.assign(networkAddr);
    new_node->channel = channel;
    new_node->addr = addr;
    new_node->type = type;
    xmlmodbus->WriteTableXml(networkAddr,channel,addr,type,xmlmodbus->DevTabledoc,"DevTable.xml");

    iterMBDevTable = FindNetworkAddr(networkAddr);
    if(iterMBDevTable != ModBusDevTable.end())
    {
        node_ptr = FindChannel(channel,iterMBDevTable);
        if(node_ptr)
        {
            node_ptr->addr = addr;
            node_ptr->type = type;
            return SAMECHANNEL;
        }else{
            node_ptr = iterMBDevTable->second;
            InsertChannelNode(node_ptr,new_node,iterMBDevTable);
        }
    }else{
            ModBusDevTable[networkAddr] = new_node;
            return NEWINSERT;
    }
    return SAMENETADDR;
}

enum CONFIGINFO MBDevTable::XmlConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable;
    ModBus_Node_ptr new_node,node_ptr;
    new_node = InitNode();
    new_node->networkAddr.assign(networkAddr);
    new_node->channel = channel;
    new_node->addr = addr;
    new_node->type = type;
    iterMBDevTable = FindNetworkAddr(networkAddr);
    if(iterMBDevTable != ModBusDevTable.end())
    {
        node_ptr = FindChannel(channel,iterMBDevTable);
        if(node_ptr)
        {
            node_ptr->addr = addr;
            node_ptr->type = type;
            return SAMECHANNEL;
        }else{
            node_ptr = iterMBDevTable->second;
            InsertChannelNode(node_ptr,new_node,iterMBDevTable);
        }
    }else{
            ModBusDevTable[networkAddr] = new_node;
            return NEWINSERT;
    }
    return SAMENETADDR;
}
