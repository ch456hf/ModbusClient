#ifndef CONTENT_H_INCLUDED
#define CONTENT_H_INCLUDED

#define SERVERIP "127.0.0.1"
#define PORT     1502

enum INFOTYPE
{
    AI = 0,
    AO,
    DI,
    DO
};

enum CONFIGINFO
{
    NEWINSERT = 1,
    SAMENETADDR,
    SAMECHANNEL,
    SAMEADDRTYPE
};

enum
{
    APP_PROC_LED_OBJ_ID = 1,
    APP_PROC_BUTTON_OBJ_ID
};

enum
{
    MTHD_ON_OFF = 1,
};//the method of per DstObject

typedef unsigned char uint_8;
typedef unsigned short ushort_16;

#endif // CONTENT_H_INCLUDED
