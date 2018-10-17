/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: SioTest.c
**
** 创   建   人: Che.Yang (车扬)
**
** 文件创建日期: 2018 年 08 月 15 日
**
** 描        述: 串口发送指定命令
*********************************************************************************************************/
#define __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"

#define   __UART_DEV        "/dev/ttyS1"
#define   __BUFF_SIZE       (3)

/*********************************************************************************************************
** 函数名称: main
** 功能描述: 通过发送指定字符并读取返回值
** 输　入  : NONE
** 输　出  : ERROR_CODE
** 全局变量:
** 调用模块:
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

