/*********************************************************************************
  *Copyright(C),2010-2011,Your Company
  *FileName:  PS.h
  *Author:  yunbin Zhou
  *Version:  1.0
  *Date:  2019.8.31
  *Description:  电源控制系统头文件
  *Others:  
  *Function List:  
 
  *History: 
     1.Date:
       Author:
       Modification:
**********************************************************************************/
#include "psRecord.h"
#include "ellLib.h"
#include "epicsThread.h"
#include "epicsMutex.h"
#include "initHooks.h"
#include "dbScan.h"
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <netdb.h>
#ifndef __IMP_PS__
#define __IMP_PS__
int SocketConnected(int sock);
int open_ps_server_socket(void *p);
int GenerateCMD(struct psRecord *ps);
char * RTN(int i);
int SetCurrent(psRecord *p);
// int GetState(psRecord *p);
int GetState( psRecord *ps, int state_num);
int WriteWave(psRecord *p);
int WriteCurrentWave(psRecord *p);
int WriteAnalogWave(struct psRecord *p);
int WriteCurrentTrigWave(struct psRecord *p);
int WriteExtBumpWave(struct psRecord *p);
int WriteCurrentInjBumpWave(struct psRecord *p);
// Generate a event table for analog PS controllor. type = 0 is general, otherwise is BUMP PS
int generate_event_table(char * event_table, unsigned int delay, int type);
unsigned short convert_to_dac(float real_val, float max_val, float abb_a, float abb_b);
#define WAVE_LENGTH 160000
#define LENGTH 7325
#define S 0
#define ANALOG_PS_HEAD_LEN 32
#define ANALOG_PS_EVENT_OFFSET 0x28

#define DIGITAL_PS_HEAD_LEN 0x43
#define DIGITAL_TRIG_PS_HEAD_LEN 0x3b   // Triangle PS 
// The length of total data.
#define DIGITAL_PS_TOTAL_LEN_OFFSET 0x04
// The version of digital PS.
#define DIGITAL_PS_VER_OFFSET 0x06
// Delay of the event( C0050001 )
#define DIGITAL_PS_DELAY_OFFSET 0x17
#define DIGITAL_PS_DELAY2_OFFSET 0x1F
#define DIGITAL_PS_TOTAL_WAVE_LEN_OFFSET 0x2F
// The number of total wave data.
#define DIGITAL_PS_TOTAL_WAVE_NUM_OFFSET 0x33
// Voltage or current data
#define DIGITAL_PS_DATA_TYPE_OFFSET 0x37
// The  ID of the Virtual Accelerator, default is 0.
#define DIGITAL_PS_VA_NUM_OFFSET 0x3B
// The length of single wave data
#define DIGITAL_PS_SINGLE_WAVE_LEN_OFFSET 0x3F

static const uint8_t analog_ps_head[] =  {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 
        0x1E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 
        0x00, 0x00
        };
static const char ps_q_d_event[] = "2\r\nc05a2001\r\n61a8\r\nc0050001\r\n";
static const char ps_bump_event[] = "2\r\nc05a2001\r\n61a8\r\nc0020001\r\n";
static const char ps_bump3_event[] = "3\r\nc05a2001\r\n61a8\r\nc0080001\r\n100\r\nc0020001\r\n";
static const char pulse_width[] = "4000";

struct InitConfig {
    ELLNODE node;
    epicsMutexId lock;
    IOSCANPVT scan;
    epicsThreadId generator;
    char ipaddress[20]; //ip address of device
    unsigned short protocol; // 0 tcp, !0 udp, udp not works on xt series
    struct sockaddr_in clientService;
    int socket;
    struct psRecord * prec;
    char domain[32];
    char command[32];
    float current_val;
    double c_val;
    unsigned char state[64];
    unsigned long delay;
    int port;
    int length;
    unsigned char * sendcmd;
    unsigned char receive[64];
    int recv_length;
    unsigned short * cmd;
};
static int SendPS(struct InitConfig * config);


#endif
