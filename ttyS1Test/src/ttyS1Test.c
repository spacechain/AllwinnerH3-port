/*********************************************************************************************************
**
**                                    �й������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: SioTest.c
**
** ��   ��   ��: Che.Yang (����)
**
** �ļ���������: 2018 �� 08 �� 15 ��
**
** ��        ��: ���ڷ���ָ������
*********************************************************************************************************/
#define __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"

#define   __UART_DEV        "/dev/ttyS1"
#define   __BUFF_SIZE       (3)

/*********************************************************************************************************
** ��������: main
** ��������: ͨ������ָ���ַ�����ȡ����ֵ
** �䡡��  : NONE
** �䡡��  : ERROR_CODE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT  main (VOID)
{
    INT     iFd0;
    INT     iRet;
    INT     i;
    CHAR    ucBuff[3] ;
    CHAR    redata[256];

    ucBuff[0] = 0xff;
    ucBuff[1] = 0x54;
    ucBuff[2] = 0x0d;
    iFd0 = open(__UART_DEV, O_RDWR,0666);

    if( iFd0 < 0) {
        printf("error open \r\n");
    }

    ioctl(iFd0, FIOBAUDRATE,   SIO_BAUD_9600);
    ioctl(iFd0, FIOSETOPTIONS, ((OPT_CRMOD | OPT_TANDEM | OPT_MON_TRAP | OPT_LINE) & (~OPT_7_BIT)));

    iRet = write(iFd0, ucBuff, 3);

    while (1) {

        iRet = read(iFd0, redata, 256);
        if(iRet <= 0) {
         usleep(100);
        } else {
//               printf("len is %d\r\n", iRet);

           for(i=0;i< iRet;i++) {
               printf("%c", redata[i]);
           }

           close(iFd0);
           return (0);
         }

    }

     close(iFd0);
     return (0);
}

/*********************************************************************************************************
  END
*********************************************************************************************************/

