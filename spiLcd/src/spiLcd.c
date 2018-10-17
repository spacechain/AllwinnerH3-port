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
** 文   件   名: spiLcd.c
**
** 创   建   人: Che.Yang (车扬)
**
** 文件创建日期: 2018 年 07 月 20 日
**
** 描        述: spi st7789s 主控芯片驱动程序
**
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "pthread.h"
#include "stdlib.h"
#include<sys/gpiofd.h>
#include "sys/mman.h"
#include <sys/time.h>
#include <malloc.h>
#include "system/device/graph/gmemDev.h"
/*********************************************************************************************************
  定义
*********************************************************************************************************/
#define __MCSPI_TEST_BUF_LEN               (240*320*3)
#define __MCSPI_TEST_DMA_BUF_ALIGN         8

#define  LCD_RESET                         (91)
#define  LED_EN                            (0)
#define  LCD_D_C                           (1)

#define FRAME_WIDTH                        (240)
#define FRAME_HEIGHT                       (320)


LW_GM_SCRINFO                              scrInfo;
LW_GM_VARINFO                              varInfo;

INT                                        iFbFd =-1;

LW_SPI_MESSAGE                             spiMsg;
PLW_SPI_DEVICE                             pSpiDevice;

UCHAR                                     *pucSrcBuffer;
UCHAR                                     *pucDestBuffer;

INT                                        resetIO;
INT                                        cmdDataIO;
INT                                        led;
int num;

/*********************************************************************************************************
** 函数名称: openFb
** 功能描述: 打开  /dev/fb
** 输　入  :
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
UCHAR* fbOpen (VOID) {
    INT                     iError;
    UCHAR                  *pvFrameBuffer;

    iFbFd = open("/dev/fb0", O_RDWR, 0666);
    if (iFbFd < 0) {
        printf("failed to open /dev/fb0\n");
        return  (-1);
    } else {
        printf("success open /dev/fb0 \r\n");
    }

    iError = ioctl(iFbFd, LW_GM_GET_SCRINFO, &scrInfo);
    if (iError < 0) {
        printf("failed to get /dev/fb0 screen info\n");
        close(iFbFd);
        return  (-1);
    }

    iError = ioctl(iFbFd, LW_GM_GET_VARINFO, &varInfo);
    if (iError < 0) {
        printf("failed to get /dev/fb0 var info\n");
        close(iFbFd);
        return  (-1);
    }
    pvFrameBuffer = mmap(LW_NULL, scrInfo.GMSI_stMemSize, PROT_READ | PROT_WRITE, MAP_SHARED, iFbFd, 0);
         if (pvFrameBuffer == MAP_FAILED) {
             printf("failed to mmap /dev/fb0\n");
             close(iFbFd);
             return (0);
         }
    return (pvFrameBuffer);
}
/*********************************************************************************************************
** 函数名称: gpioOpen
** 功能描述: 打开  gpio
** 输　入  : num   io口号
** 输　出  :
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  gpioOpen (INT num) {
    INT            fd;
    fd=gpiofd(num,O_WRONLY, GPIO_FLAG_OUT_INIT_HIGH);                /*  IO口打开         */
    if(fd < 0) {
       fprintf(stderr,"failed to open gpio%d\n",num);
       return (-1);
    }
    return (fd);
}


INT st7789init (VOID) {

     pSpiDevice = API_SpiDeviceCreate("/bus/spi/0", "/dev/null");
     if (pSpiDevice == LW_NULL) {
         printf("failed to create spi device!\n");
         API_SpiDeviceDelete(pSpiDevice);
         return  (-1);
     }


     resetIO = gpioOpen(LCD_RESET);
     gpiofd_write(resetIO, 0);
     cmdDataIO = gpioOpen(LCD_D_C);

//     led = gpioOpen(LED_EN);
//     gpiofd_write(led,1);

     return (0);
}

INT st7789reset (VOID) {

   gpiofd_write(resetIO, 1);
   usleep(50000);
   gpiofd_write(resetIO, 0);
   usleep(100000);
   gpiofd_write(resetIO, 1);
   usleep(100000);
   return (0);
}

VOID send_ctrl_cmd (UCHAR cmd) {


    gpiofd_write(cmdDataIO, 0);

    lib_bzero(&spiMsg, sizeof(spiMsg));
    spiMsg.SPIMSG_pucRdBuffer =pucDestBuffer;
    spiMsg.SPIMSG_pucWrBuffer =  &cmd;
    spiMsg.SPIMSG_uiLen       = 1;
    if (API_SpiDeviceTransfer(pSpiDevice, &spiMsg, 1) != 1) {
        printf("failed to spi transfer!\n");
        API_SpiDeviceDelete(pSpiDevice);
        API_VmmDmaFree(pucSrcBuffer);
        return;
    }
    gpiofd_write(cmdDataIO, 1);


}

VOID send_data_cmd (UCHAR data) {

    gpiofd_write(cmdDataIO, 1);

    lib_bzero(&spiMsg, sizeof(spiMsg));
    spiMsg.SPIMSG_pucRdBuffer = pucDestBuffer;
    spiMsg.SPIMSG_pucWrBuffer = &data;
    spiMsg.SPIMSG_uiLen       = 1;

    if (API_SpiDeviceTransfer(pSpiDevice, &spiMsg, 1) != 1) {
        printf("failed to spi transfer!\n");
        API_SpiDeviceDelete(pSpiDevice);
        API_VmmDmaFree(pucSrcBuffer);
        return;
    }

    gpiofd_write(cmdDataIO, 0);

}

static void sw_clear_panel(unsigned int color)
{
   unsigned short x0, y0, x1, y1, x, y;
   unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

   x0 = (unsigned short)0;
   y0 = (unsigned short)0;
   x1 = (unsigned short)FRAME_WIDTH-1;
   y1 = (unsigned short)FRAME_HEIGHT-1;

   h_X_start=((x0&0xFF00)>>8);
   l_X_start=(x0&0x00FF);
   h_X_end=((x1&0xFF00)>>8);
   l_X_end=(x1&0x00FF);

   h_Y_start=((y0&0xFF00)>>8);
   l_Y_start=(y0&0x00FF);
   h_Y_end=((y1&0xFF00)>>8);
   l_Y_end=(y1&0x00FF);

   send_ctrl_cmd(0x2A);
   send_data_cmd(h_X_start);
   send_data_cmd(l_X_start);
   send_data_cmd(h_X_end);
   send_data_cmd(l_X_end);

   send_ctrl_cmd(0x2B);
   send_data_cmd(h_Y_start);
   send_data_cmd(l_Y_start);
   send_data_cmd(h_Y_end);
   send_data_cmd(l_Y_end);

   send_ctrl_cmd(0x29);
   send_ctrl_cmd(0x2C);

   for (y = y0; y <= y1; ++ y) {
      for (x = x0; x <= x1; ++ x) {
         send_data_cmd(0xff);
         send_data_cmd(0);
         send_data_cmd(0);

      }
   }

}

INT spisend (UCHAR   *pvFrameBuffer) {

    unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;
    unsigned short x0, y0, x1, y1;
    int            i =0;

    st7789init();
    st7789reset();

    send_ctrl_cmd(0x11);
    usleep(120000);

    send_ctrl_cmd(0x36);
    send_data_cmd(0x00);//40

    send_ctrl_cmd(0x3a);
    send_data_cmd(0x06);//06

    send_ctrl_cmd(0xb0);
    send_data_cmd(0xa0);//06

    send_ctrl_cmd(0xb2);
    send_data_cmd(0x28);
    send_data_cmd(0x28);
    send_data_cmd(0x05);
    send_data_cmd(0x33);
    send_data_cmd(0x33);



    send_ctrl_cmd(0xb7);
    send_data_cmd(0x35);

    send_ctrl_cmd(0xbb);
    send_data_cmd(0x3c);//23

    send_ctrl_cmd(0xb1);
    send_data_cmd(0x80);
    send_data_cmd(0x10);


    send_ctrl_cmd(0xc0);
    send_data_cmd(0x2c);

    send_ctrl_cmd(0xc2);
    send_data_cmd(0x01);

    send_ctrl_cmd(0xc3);
    send_data_cmd(0x05);//14

    send_ctrl_cmd(0xc4);
    send_data_cmd(0x20);

    send_ctrl_cmd(0xc6);
    send_data_cmd(0x14); // 14

    send_ctrl_cmd(0xd0);
    send_data_cmd(0xa4);
    send_data_cmd(0xa1);

    send_ctrl_cmd(0xe0);
    send_data_cmd(0xd0);
    send_data_cmd(0x00);
    send_data_cmd(0x02);
    send_data_cmd(0x07);
    send_data_cmd(0x07);
    send_data_cmd(0x19);
    send_data_cmd(0x2e);
    send_data_cmd(0x54);
    send_data_cmd(0x41);
    send_data_cmd(0x2d);
    send_data_cmd(0x17);
    send_data_cmd(0x18);
    send_data_cmd(0x14);
    send_data_cmd(0x18);

    send_ctrl_cmd(0xe1);
    send_data_cmd(0xd0);
    send_data_cmd(0x00);
    send_data_cmd(0x02);
    send_data_cmd(0x07);
    send_data_cmd(0x04);
    send_data_cmd(0x24);
    send_data_cmd(0x2c);
    send_data_cmd(0x44);
    send_data_cmd(0x42);
    send_data_cmd(0x1c);
    send_data_cmd(0x1a);
    send_data_cmd(0x17);
    send_data_cmd(0x15);
    send_data_cmd(0x18);
    send_ctrl_cmd(0x35);
    send_data_cmd(0x00);//40

    send_ctrl_cmd(0x44);
    send_data_cmd(0x19);
    send_ctrl_cmd(0x29);

    fbOpen();
    /*
     * 初始化屏幕坐标
     */
    x0 = (unsigned short)0;
    y0 = (unsigned short)0;
    x1 = (unsigned short)FRAME_WIDTH-1;
    y1 = (unsigned short)FRAME_HEIGHT-1;

    h_X_start=((x0&0xFF00)>>8);
    l_X_start=(x0&0x00FF);
    h_X_end=((x1&0xFF00)>>8);
    l_X_end=(x1&0x00FF);

    h_Y_start=((y0&0xFF00)>>8);
    l_Y_start=(y0&0x00FF);
    h_Y_end=((y1&0xFF00)>>8);
    l_Y_end=(y1&0x00FF);

    send_ctrl_cmd(0x2A);
    send_data_cmd(h_X_start);
    send_data_cmd(l_X_start);
    send_data_cmd(h_X_end);
    send_data_cmd(l_X_end);

    send_ctrl_cmd(0x2B);
    send_data_cmd(h_Y_start);
    send_data_cmd(l_Y_start);
    send_data_cmd(h_Y_end);
    send_data_cmd(l_Y_end);

    send_ctrl_cmd(0x29);
    send_ctrl_cmd(0x2C);

//    sw_clear_panel(0x35);

    gpiofd_write(cmdDataIO, 1);
    /* 定义两个结构体 */
    struct timeval start;
    struct timeval end;
    unsigned long timer;
    /* 程序开始之前计时start */
    gettimeofday(&start, NULL);

  while(1)   {

     lib_bzero(&spiMsg, sizeof(spiMsg));
     spiMsg.SPIMSG_pucRdBuffer = NULL;
     spiMsg.SPIMSG_uiLen       = scrInfo.GMSI_stMemSize;
     spiMsg.SPIMSG_pucWrBuffer = pvFrameBuffer;

    if (API_SpiDeviceTransfer(pSpiDevice, &spiMsg, 1) != 1) {
        printf("failed to spi transfer!\n");
        API_SpiDeviceDelete(pSpiDevice);

        API_VmmDmaFree(pucSrcBuffer);
        return (0);
        }

    usleep(65000);

    }
    gettimeofday(&end, NULL);
    /* 统计程序段运行时间(unit is usec)*/
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("timer = %ld us\n", timer);

    gpiofd_write(cmdDataIO, 0);
    close(resetIO);
    close(cmdDataIO);

    API_SpiDeviceDelete(pSpiDevice);

    free(pvFrameBuffer);
    close(iFbFd);
    close(led);


     return (0);
}
/*********************************************************************************************************
** 函数名称: spiTestStart
** 功能描述: McSPI 测试
** 输　入  : NONE
** 输　出  : ERROR_CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  main (VOID)
{
    UCHAR *framebuffer = fbOpen();
    spisend(framebuffer);
    return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
