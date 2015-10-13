#ifndef MODBUSCLIENT_H
#define MODBUSCLIENT_H

#include <stdio.h>
#include <string>
#include <errno.h>
#include <stdlib.h>
#include <modbus.h>

#include "MBDevTable.h"

using namespace std;

class ModBusClient
{
    public:
        ModBusClient();
        virtual ~ModBusClient();
        modbus_t *ctx;
        int SendToMB(string networkAddr,int length,uint_8 *data);
    protected:
    private:
        bool CreateTcp();
};

#endif // MODBUSCLIENT_H
