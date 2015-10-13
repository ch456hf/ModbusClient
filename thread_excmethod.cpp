#include <stdio.h>
#include <pthread.h>
#include <sys/wait.h>
#include "MBCtrlTable.h"
//#include "Monitor/ClientServerSvc.h"

extern pthread_mutex_t mutex_read_Ctrltable;
//extern CClientServerServices *ClientServerManager;

void ExecuteMethod(Control_Node_ptr node,uint_8 data[2])
{
    string networkAddr = node->networkAddr;
    uint_8 senddata[3];
    switch(node->type)
    {
        case DO:
            senddata[0] = 0x01;
            senddata[1] = data[0];
            //ClientServerManager->createExecReq(networkAddr,APP_PROC_LED_OBJ_ID,
                                     //         MTHD_ON_OFF,2,senddata);
            break;
        case AO:
            break;
    }
}

void *thread_excmethod(void *arg)
{
    /*
      1.compare with the control table
      2.different data different method
      3.remember add mutex
    */
    MBCtrlTable *thread_Ctrltable;
    MyHashMap Ctrltable;
    IterMyHashMap iterCtrltable;
    Control_Node_ptr node;
    uint_8 now_data[2];
    while(1)
    {
        thread_Ctrltable = MBCtrlTable::GetInstance();
        pthread_mutex_lock(&mutex_read_Ctrltable);
        Ctrltable = thread_Ctrltable->CtrlTable;
        pthread_mutex_unlock(&mutex_read_Ctrltable);
        for(iterCtrltable = Ctrltable.begin();iterCtrltable != Ctrltable.end();iterCtrltable ++)
        {
            node = iterCtrltable->second;
            if(!thread_Ctrltable->ComparePredata(node,now_data)){
                printf("The data has been changed!\n");
                thread_Ctrltable->SetData(node,now_data);
                ExecuteMethod(node,now_data);//Execute some method
            }
        }
        usleep(500000);
    }
}
