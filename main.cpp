#include <iostream>
#include <pthread.h>
#include "MBDevTable.h"
#include "ModBusClient.h"
#include "MBCtrlTable.h"
#include "thread_excmethod.h"
#include "XmlforModbus.h"

using namespace std;

pthread_mutex_t mutex_read_Ctrltable = PTHREAD_MUTEX_INITIALIZER;

void print(string str)
{
    hash_map<string,ModBus_Node_ptr> ModBusDevTable = MBDevTable::GetInstance()->ModBusDevTable;
    hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable = ModBusDevTable.find(str);
    ModBus_Node_ptr node_ptr = iterMBDevTable->second;
    for(;node_ptr != NULL;node_ptr = node_ptr->next)
    {
        cout << "channel = " << node_ptr->channel << "addr = " << node_ptr->addr << "type = " << node_ptr->type << endl;
    }
}

void printf_control(int addr,enum INFOTYPE type)
{
    Key_Addr_Type key;
    key.addr = addr;
    key.type = type;
    MyHashMap ctrlTable = MBCtrlTable::GetInstance()->CtrlTable;
    IterMyHashMap iterCtrl = ctrlTable.find(key);
    if(iterCtrl == ctrlTable.end())
    {
        printf("not find!\n");
        return;
    }
    printf("channel = %d,data = %x",iterCtrl->second->channel,iterCtrl->second->pre_info[0]);
}

int main()
{
    ModBusClient *modbusclient = new ModBusClient();
    MBDevTable *mbdevtable = MBDevTable::GetInstance();
    MBCtrlTable *mbctrltable = MBCtrlTable::GetInstance();
    mbctrltable->ctx = modbusclient->ctx;
    XmlforModbus *xml = new XmlforModbus();
    mbdevtable->xmlmodbus = xml;
    mbctrltable->xmlmodbus = xml;


    int returnvalue,ret;
    unsigned char data[8];

    pthread_t compare_pthread;
    data[0] = 0x34;
    data[1] = 0x34;
    data[2] = 0x78;
    data[3] = 0x78;
    returnvalue = 1;
   //print("FC0000000022FF0000021E66000D0055");
    mbdevtable->ConfigTable("FC0000000022FF0000021E66000D0055",1,0,AI);
  //  mbdevtable->ConfigTable("FC0000000022FF0000021E66000D0055",2,1,AI);

    modbusclient->SendToMB("FC0000000022FF0000021E66000D0055",4,data);

   // returnvalue = mbctrltable->ConfigTable("FC0000000022FF0000021E66000D0055",1,0,DO);
  //  mbctrltable->ConfigTable("FC0000000022FF0000021E66000D0055",2,1,DO);
  //  printf_control(0,DO);
  //  printf_control(1,DO);
   // printf("value = %d\n",returnvalue);
    pthread_mutex_init(&mutex_read_Ctrltable,NULL);
    if(returnvalue > 0)
    {
        ret = pthread_create(&compare_pthread,NULL,thread_excmethod,NULL);
        if(ret != 0)
            printf("Create pthread error!\n");
    }
    while(1);
    return 0;
}
