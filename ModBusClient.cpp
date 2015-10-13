#include "ModBusClient.h"

ModBusClient::ModBusClient()
{
    CreateTcp();
}

ModBusClient::~ModBusClient()
{
    //dtor
}

bool ModBusClient::CreateTcp()
{
    ctx = modbus_new_tcp(SERVERIP,PORT);
    if(ctx == NULL){
        printf("Unable to allocate libmodbus context\n");
        return false;
    }
    modbus_set_debug(ctx,FALSE);
    modbus_set_error_recovery(ctx,
                              (modbus_error_recovery_mode)(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
    if(modbus_connect(ctx) == -1) {
        printf("Connected failed: %s\n",modbus_strerror(errno));
        modbus_free(ctx);
        return false;
    }
    return true;
}

int ModBusClient::SendToMB(string networkAddr,int length,uint_8 *data)
{
    int returnValue;
    ModBus_Node_ptr node;
    uint_8 *senddata;
    int data_high,data_low;
    int value;
    hash_map<string,ModBus_Node_ptr>::iterator iterMBDevTable;
    senddata = data;
    iterMBDevTable = MBDevTable::GetInstance()->ModBusDevTable.find(networkAddr);
    if(iterMBDevTable == MBDevTable::GetInstance()->ModBusDevTable.end())
        return -1;
    node = iterMBDevTable->second;
    if(node == NULL){
        printf("No such table!\n");
        return 0;
    }
    while(node != NULL)
    {
        switch(node->type)
        {
            case AI:
                 data_high = *senddata;
                 senddata ++;
                 data_low = *senddata;
                 senddata ++;
                 value = (int)(data_high << 8 | data_low);
                 returnValue = modbus_write_register(ctx,node->addr,value);
              //   printf("value = %x,return = %d\n",value,returnValue);
                 break;
            case DI:
                 value = (int)(*senddata);
                 senddata ++;
                 returnValue = modbus_write_bit(ctx,node->addr,value);
              //   printf("value = %x,return = %d\n",value,returnValue);
                 break;
            case AO:
                 break;
            case DO:
                 break;
        }
        node = node->next;
    }
    return length;
}















