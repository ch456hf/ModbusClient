#ifndef MBCTRLTABLE_H
#define MBCTRLTABLE_H

#include "ModBusClient.h"
#include "MBDevTable.h"


typedef struct
{
    int addr;
    enum INFOTYPE type;
}Key_Addr_Type;

typedef struct Control_Node
{
    string networkAddr;
    int channel;
    int addr;
    enum INFOTYPE type;
    uint_8 pre_info[2];
}Control_Node,*Control_Node_ptr;

namespace __gnu_cxx{
    struct myhash_func
    {
        size_t operator()(const Key_Addr_Type &Key_Addr_Type)const
        {
            int resault;
            resault = Key_Addr_Type.addr * 10 + Key_Addr_Type.type;
            return (size_t)resault;
        }
    };

    struct compare
    {
        bool operator()(const Key_Addr_Type &key1,const Key_Addr_Type &key2) const
        {
            if(key1.type == key2.type)
                if(key1.addr == key2.addr)
                    return true;
            return false;
        }
    };
}
typedef hash_map<Key_Addr_Type,Control_Node_ptr,myhash_func,compare> MyHashMap;
typedef hash_map<Key_Addr_Type,Control_Node_ptr,myhash_func,compare>::iterator IterMyHashMap;


class MBCtrlTable
{
    public:
        MyHashMap CtrlTable;
        modbus_t *ctx;
        XmlforModbus *xmlmodbus;
        IterMyHashMap FindAddr_Type(Key_Addr_Type node);
        enum CONFIGINFO ConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type);
        enum CONFIGINFO XmlConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type);

        bool ComparePredata(Control_Node_ptr node,uint_8 nowdata[2]);
        void SetData(Control_Node_ptr node,uint_8 data[2]);
        virtual ~MBCtrlTable();
        static MBCtrlTable *GetInstance()
        {
            static MBCtrlTable _instance;
            return &_instance;
        }
    protected:
    private:
        MBCtrlTable();
        int GetDataFromModbus(Control_Node_ptr ctrlnode,uint_8 *data);
        Control_Node_ptr InitNode();
};

#endif // MBCTRLTABLE_H
