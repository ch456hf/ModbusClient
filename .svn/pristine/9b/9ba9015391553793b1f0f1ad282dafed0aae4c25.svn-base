#include "MBCtrlTable.h"

MBCtrlTable::MBCtrlTable()
{
    //ctor
}

MBCtrlTable::~MBCtrlTable()
{
    //dtor
}

//Init a node
Control_Node_ptr MBCtrlTable::InitNode()
{
    Control_Node_ptr node;
    node = new Control_Node();
    return node;
}

IterMyHashMap MBCtrlTable::FindAddr_Type(Key_Addr_Type node)
{
    IterMyHashMap iterCtrlTable;
    iterCtrlTable = CtrlTable.find(node);
    return iterCtrlTable;
}

int MBCtrlTable::GetDataFromModbus(Control_Node_ptr ctrlnode,uint_8 *data)
{
    int rc;
    ushort_16 getdata;
    switch(ctrlnode->type)
    {
    case DO:
        rc = modbus_read_bits(ctx,ctrlnode->addr,1,data);
        break;
    case AO:
        rc = modbus_read_registers(ctx,ctrlnode->addr,1,&getdata);
        data[0] = (getdata >> 8) & 0x00FF;
        data[1] = getdata & 0x00FF;
        break;
    default:
        break;
    }
    //printf("data = %x,%x\n",data[0],data[1]);
    return rc;

}

//compare with the predata
bool MBCtrlTable::ComparePredata(Control_Node_ptr node,uint_8 nowdata[2])
{
    int rc;
    rc = GetDataFromModbus(node,nowdata);
    //printf("nowdata = %x,%x\n",nowdata[0],nowdata[1]);
    if(rc != 1){
        printf("Addr = %d,FAILED compare(nb points %d)\n",node->addr,rc);
    }
    switch(node->type)
    {
    case DO:
        if(nowdata[0] == node->pre_info[0])
                return true;
        break;
    case AO:
        if(nowdata[0] == node->pre_info[0] && nowdata[1] == node->pre_info[1])
                return true;
        break;
    }
    return false;
}

void MBCtrlTable::SetData(Control_Node_ptr node,uint_8 data[2])
{
    switch(node->type)
    {
        case DO:
            node->pre_info[0] = data[0];
            break;
        case AO:
            node->pre_info[0] = data[0];
            node->pre_info[1] = data[1];
            break;
    }
}

enum CONFIGINFO MBCtrlTable::ConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    int rc;
    IterMyHashMap iterCtrlTable;
    Control_Node_ptr new_node;
    Key_Addr_Type key;
    new_node = InitNode();
    new_node->networkAddr.assign(networkAddr);
    new_node->channel = channel;
    new_node->addr = addr;
    new_node->type = type;
    key.addr = addr;
    key.type = type;
    xmlmodbus->WriteTableXml(networkAddr,channel,addr,type,xmlmodbus->CtrlTabledoc,"CtrlTable.xml");

    rc = GetDataFromModbus(new_node,new_node->pre_info);
    if(rc != 1){
        printf("Addr = %d,FAILED (nb points %d)\n",addr,rc);
    }
    iterCtrlTable = FindAddr_Type(key);
    if(iterCtrlTable != CtrlTable.end())
    {
        CtrlTable[key] = new_node;
        return SAMEADDRTYPE;
    }
    CtrlTable[key] = new_node;
  //  printf("dataConfig = %x%x\n",new_node->pre_info[0],new_node->pre_info[1]);
    return NEWINSERT;
}

enum CONFIGINFO MBCtrlTable::XmlConfigTable(string networkAddr,int channel,int addr,enum INFOTYPE type)
{
    int rc;
    IterMyHashMap iterCtrlTable;
    Control_Node_ptr new_node;
    Key_Addr_Type key;
    new_node = InitNode();
    new_node->networkAddr.assign(networkAddr);
    new_node->channel = channel;
    new_node->addr = addr;
    new_node->type = type;
    key.addr = addr;
    key.type = type;
    rc = GetDataFromModbus(new_node,new_node->pre_info);
    if(rc != 1){
        printf("Addr = %d,FAILED (nb points %d)\n",addr,rc);
    }
    iterCtrlTable = FindAddr_Type(key);
    if(iterCtrlTable != CtrlTable.end())
    {
        CtrlTable[key] = new_node;
        return SAMEADDRTYPE;
    }
    CtrlTable[key] = new_node;
    printf("dataConfig = %x%x\n",new_node->pre_info[0],new_node->pre_info[1]);
    return NEWINSERT;
}
