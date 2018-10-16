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
 ** 文   件   名: sntp.c
 **
 ** 创   建   人: Lu.ZhenPing (卢振平)
 **
 ** 文件创建日期: 2014 年 05 月 09 日
 **
 ** 描        述: sntp 主程序源文件.
 ********************************************************************************************************/
#include "sntp.h"
/*********************************************************************************************************
**  全局变量定义                                                                                                                                                                                                                                                                                                  *
*********************************************************************************************************/
static  CHAR             cSntpHostName[SNTP_NAME_LEN];                     /*  读取配置文件服务器地址   */
static  UCHAR            ucSntpHandleCmd;                                  /*  包处理命令字             */
static  INT              iSntpInterval;                                    /*  时间更新间隔             */
static  INT              iSntpConnFlag;                                    /*  socket连接标志           */
static  INT              iSntpSockfd;                                      /*  SNTP socket描述符        */
/*********************************************************************************************************
**  调试相关的全局变量                                                                                                                                                                                                                                                                                                  *
*********************************************************************************************************/
static  CHAR             cSntpLogFileName[SNTP_NAME_LEN];                  /*  SNTP log文件名           */
/*********************************************************************************************************
 ** 函数名称:  sntpConfFileParse
 ** 功能描述:  解析SNTP配置文件信息
 ** 输　入  :  pcFileName        配置文件路径
 **            pntcfgFile        配置文件
 ** 输　出  :  SNTP_NO_ERR(成功) OR SNTP_PX_ERR(失败)
 ** 全局变量:  ucSntpHandleCmd   SNTP命令
 **            iSntpInterval     时间更新间隔
 **            cSntpLogFileName  调试log输出文件名
 ** 调用模块:  ini解析库
 ********************************************************************************************************/
INT  sntpConfFileParse (PCHAR  pcFileName, SNTP_CFG_BODY  *pntcfgFile)
{
    dictionary  *ddict;
    PCHAR        pcVal;
    PCHAR        pcDef = LW_NULL;
    INT          iUpdateTSys;
    INT          iNotfound = 0;
    INT          iInterval, iRtcTime, iDebug;

    if (!pntcfgFile) {
        SNTP_ERR("conf struct addr invalid(null).\n");
        return  (SNTP_NULL_POINTER_ERR);
    }
    if (!pcFileName) {
        SNTP_ERR("conf file addr invalid(null).\n");
        return  (SNTP_NO_CFG_FILE_ERR);
    }

    ucSntpHandleCmd &= 0;                                                  /*  命令清零                 */
    bzero(pntcfgFile, sizeof(SNTP_CFG_BODY));

    ddict = iniparser_load(pcFileName);
    if (ddict == LW_NULL) {
        return  (SNTP_NO_CFG_FILE_ERR);
    }

    pcVal = iniparser_getstring(ddict,                                     /*  获得服务器地址           */
                                "server:serverAddr",
                                pcDef);

    SNTP_LOG("server addr: %s\n", pcVal);

    memcpy(pntcfgFile->NTCFG_cHostName, pcVal, strlen(pcVal));

    iUpdateTSys = iniparser_getint(ddict,                                  /*  是否更新系统时间         */
                                   "update:updateTimeSystem",
                                   iNotfound);
    iRtcTime    = iniparser_getint(ddict,                                  /*  是否更新rtc时间          */
                                   "update:writeTimeRtc",
                                   iNotfound);

    SNTP_LOG("update system time ? %s\n", iUpdateTSys ? "YES" : "NO");
    SNTP_LOG("update rtc Time ? %s\n", iRtcTime ? "YES" : "NO");

    if (iUpdateTSys) {
        ucSntpHandleCmd |= SNTP_CMD_UPDATE_TIME;
    }

    if (iRtcTime) {
        ucSntpHandleCmd |= SNTP_CMD_UPDATE_RTC;
    }

    iInterval = iniparser_getint(ddict,
                                 "interval:updateTimeInterval",
                                 iNotfound);                               /*  更新时间的间隔           */

    iSntpInterval = iInterval * SNTP_ONE_MIN;

    SNTP_LOG("update time interval: %d(min)\n", iInterval);
    /*
     *  调试相关
     */
    iDebug = iniparser_getint(ddict,
                              "debug:dumpPktMsg",
                              iNotfound);
    if (iDebug) {
        ucSntpHandleCmd |= SNTP_CMD_DEBUG;
    }

    pcVal = iniparser_getstring(ddict, "debug:logFileName", pcDef);

    if (strlen(pcVal)) {
        memcpy(cSntpLogFileName, pcVal, strlen(pcVal));
    } else {
        memcpy(cSntpLogFileName, SNTP_DEBUG_FILE, strlen(SNTP_DEBUG_FILE));
    }

    if (iDebug) {
        SNTP_LOG("print debug info in %s\n", cSntpLogFileName);
    }

    iniparser_freedict(ddict);

    ddict = LW_NULL;
    pcVal = LW_NULL;

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpReceive
 ** 功能描述:  SNTP接收器主程序
 ** 输　入  :  iSockfd  文件描述符
 **            uiAddr   网络地址
 **            usPort   网络端口号
 ** 输　出  :  SNTP_NO_ERR or SNTP_PX_ERR
 ** 全局变量:
 ** 调用模块:
 ********************************************************************************************************/
static INT  sntpReceive (INT  iSockfd, UINT32  uiAddr, UINT16  usPort)
{
    struct sockaddr_in saRecvr;

    bzero((char *)&saRecvr, sizeof(struct sockaddr_in));

    saRecvr.sin_family      = AF_INET;
    saRecvr.sin_addr.s_addr = htonl(uiAddr);
    saRecvr.sin_port        = htons(usPort);

    if (bind(iSockfd, (struct sockaddr *)&saRecvr,
             sizeof(saRecvr)) == SNTP_PX_ERR) {
        perror("bind");
        return  (SNTP_PX_ERR);
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpConnect
 ** 功能描述:  连接器主程序
 ** 输　入  :  iSockfd          文件描述符
 **            pcHostName       服务器网络地址
 **            pcPort           服务器网络端口号
 ** 输　出  :  连接状态
 ** 全局变量:  ucSntpHandleCmd  SNTP命令
 **            cSntpLogFileName 调试log输出文件名
 ** 调用模块:
 ********************************************************************************************************/
static INT  sntpConnect (INT  iSockfd, PCHAR  pcHostName, PCHAR  pcPort)
{
    struct addrinfo *aiResult;
    struct addrinfo  aiHints;
    INT              iRet;

    if (!pcHostName) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "host name invalid(null).\n");
        }
        return  (SNTP_NULL_POINTER_ERR);
    }
    if (!pcPort) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "port addr invalid(null).\n");
        }
        return  (SNTP_NULL_POINTER_ERR);
    }

    bzero(&aiHints, sizeof(aiHints));
    /*
     *  希望获得服务器的以下类型的信息
     */
    aiHints.ai_socktype = SOCK_DGRAM;
    aiHints.ai_family   = AF_INET;
    aiHints.ai_protocol = IPPROTO_UDP;

    iRet = getaddrinfo(pcHostName,
                       pcPort, &aiHints, &aiResult);                       /*  得到服务器地址信息       */
    if (iRet != 0) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "getaddrinfo error.\n");
        }

        return  (SNTP_PX_ERR);
    }

    if (connect(iSockfd, (struct sockaddr *)aiResult->ai_addr,
                sizeof(struct sockaddr_in)) == SNTP_PX_ERR) {              /*  连接服务器               */
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "connect error.\n");
        }
        return  (SNTP_PX_ERR);
    }

    freeaddrinfo(aiResult);

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpPacketStuff
 ** 功能描述:  填充发送包
 ** 输　入  :  pnttpSntpPacket  SNTP包结构指针
 ** 输　出  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** 全局变量:  ucSntpHandleCmd  SNTP命令
 **            cSntpLogFileName 调试log输出文件名
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpPacketStuff (SNTP_TIME_PKT  *pnttpSntpPacket)
{
    struct timeval tvNow;

    if (!pnttpSntpPacket) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "null packet address.\n");
        }
        return  (SNTP_NULL_POINTER_ERR);
    }

    SNTP_GET_TIME(&tvNow, NULL);                                           /*  获取本地时间             */

    /*
     *  客户端填充本地时间戳和服务器交互的信息头
     */
    pnttpSntpPacket->NTTP_uiPktHdr                       = htonl((LI << 30)      |
                                                                 (VN << 27)      |
                                                                 (MODE << 24)    |
                                                                 (STRATUM << 16) |
                                                                 (POLL << 8)     |
                                                                 (PREC & 0xFF));
    pnttpSntpPacket->NTTP_uiRootDelay                    = htonl(1 << 16);
    pnttpSntpPacket->NTTP_uiRootDispersion               = htonl(1 << 16);
    pnttpSntpPacket->NTTP_nttTransTimestamp.NTT_uiCorase = htonl(tvNow.tv_sec + JAN_1970);
    pnttpSntpPacket->NTTP_nttTransTimestamp.NTT_uiFine   = htonl(NTPFRAC(tvNow.tv_usec));

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpPacketSend
 ** 功能描述:  客户端发送SNTP请求包
 ** 输　入  :  iSockfd          文件描述符
 ** 输　出  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** 全局变量:  ucSntpHandleCmd  SNTP命令
 **            cSntpLogFileName 调试log输出文件名
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpPacketSend (INT  iSockfd)
{
    SNTP_TIME_PKT nttpSntpPacket;
    INT           iRet;

    memset(&nttpSntpPacket, 0, sizeof(nttpSntpPacket));

    sntpPacketStuff(&nttpSntpPacket);                                      /*  填充请求包               */

    iRet = send(iSockfd, (VOID *)&nttpSntpPacket, sizeof(SNTP_TIME_PKT), 0);
    if (iRet < 0) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "send error iret: %d.\n", iRet);
        }
        SNTP_ERR("send packet failed[%d].\n", iRet);
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpSystemTimeUpdate
 ** 功能描述:  更新系统时间主程序
 ** 输　入  :  pnttppIn         时间戳包指针
 ** 输　出  :  SNTP_NO_ERR
 ** 全局变量:  ucSntpHandleCmd  SNTP命令
 **            cSntpLogFileName 输出Log文件名
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpSystemTimeUpdate (SNTP_TIME_PKT  *pnttpIn)
{
    struct timeval tvSet;
    SNTP_TIME      nttTransTime;

    nttTransTime.NTT_uiCorase =
            ntohl(pnttpIn->NTTP_nttTransTimestamp.NTT_uiCorase);          /* 将网络字节序转成主机字节序 */
    nttTransTime.NTT_uiFine   =
            ntohl(pnttpIn->NTTP_nttTransTimestamp.NTT_uiFine);

    tvSet.tv_sec = nttTransTime.NTT_uiCorase - JAN_1970;
    tvSet.tv_usec = USEC(nttTransTime.NTT_uiFine);

    if (SNTP_SET_TIME(&tvSet, LW_NULL) < 0) {                             /* 更新系统时间               */
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "SNTP_SET_TIME error.\n");
        }
    }

    SNTP_LOG("update system time success.\n");

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpSystemTimeUpdate
 ** 功能描述:  更新系统时间主程序
 ** 输　入  :  无
 ** 输　出  :  无
 ** 全局变量:
 ** 调用模块:
 ********************************************************************************************************/
VOID  sntpRtcTimeUpdate (VOID)
{
    SNTP_SYNC_RTC();                                                       /*  将新的时间写到RTC        */
    SNTP_LOG("update rtc time success.\n");
}
/*********************************************************************************************************
 ** 函数名称:  sntpPacketHandle
 ** 功能描述:  处理接收到的服务器时间戳包
 ** 输　入  :  pnttpIn           时间戳包指针
 ** 输　出  :  无
 ** 全局变量:  ucSntpHandleCmd   SNTP命令
 **            cSntpLogFileName  Log输出文件名
 ** 调用模块:
 ********************************************************************************************************/
VOID  sntpPacketHandle (SNTP_TIME_PKT  *pnttpIn)
{
    if ((ucSntpHandleCmd & SNTP_CMD_DEBUG)) {
        sntpDumpPkt(ucSntpHandleCmd, cSntpLogFileName, pnttpIn);
    }

    if ((ucSntpHandleCmd & SNTP_CMD_UPDATE_TIME)) {
        sntpSystemTimeUpdate(pnttpIn);
    }

    if ((ucSntpHandleCmd & SNTP_CMD_UPDATE_RTC)) {
        sntpRtcTimeUpdate();
    }

    if (!(ucSntpHandleCmd & SNTP_CMD_MASK)) {
        SNTP_ERR("Command:%d is not supported.\n", ucSntpHandleCmd);
    }

    ucSntpHandleCmd = SNTP_CMD_INVALID;
}
/*********************************************************************************************************
 ** 函数名称:  sntpProcess
 ** 功能描述:  SNTP算法器主程序
 ** 输　入  :  iSockfd          文件描述符
 ** 输　出  :  SNTP_NO_ERR
 ** 全局变量:  ucSntpHandleCmd  命令
 **            cSntpLogFileName log文件名
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpProcess (INT  iSockfd)
{
    fd_set           fdset;
    INT              iRet, iPackLen;
    SNTP_TIME_PKT    nttpIncoming;
    struct timeval   tvDelay;

    bzero(&nttpIncoming, sizeof(nttpIncoming));
    tvDelay.tv_sec  = 2;
    tvDelay.tv_usec = 0;

    for (;;) {
        FD_ZERO(&fdset);
        FD_SET(iSockfd, &fdset);

        iRet = select(iSockfd + 1, &fdset, LW_NULL, LW_NULL, &tvDelay);     /*  等待isockfd可读         */
        if (iRet != 1) {                                                    /*  select 失败             */
            if (iRet == EINTR) {
                continue;
            }

            if (iRet < 0) {
                perror("select");
            }

            sntpPacketSend(iSockfd);                                        /*  客户端发送请求包        */
            continue;

        } else {                                                            /*  select 成功             */
            iPackLen = recvfrom(iSockfd, &nttpIncoming,                     /*  从服务器接收时间戳      */
                                sizeof(SNTP_TIME_PKT), 0,
                                LW_NULL, LW_NULL);
            if (iPackLen < 0) {
                if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
                    SNTP_LOGHANDLE_F(cSntpLogFileName, "recvfrom error.\n");
                }

            } else if (iPackLen <= sizeof(SNTP_TIME_PKT)) {
                sntpPacketHandle(&nttpIncoming);                            /*  处理从服务器接收到的包  */

            } else {
                if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
                    SNTP_LOGHANDLE_F(cSntpLogFileName, "iPackLen = %d error.\n", iPackLen);
                }
            }
            break;
        }
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpInstallAlgorithm
 ** 功能描述:  SNTP算法器安装
 ** 输　入  :  无
 ** 输　出  :  无
 ** 全局变量:  iSntpSockfd  socket描述符
 ** 调用模块:
 ********************************************************************************************************/
VOID  sntpInstallAlgorithm (VOID)
{
    sntpProcess(iSntpSockfd);
}
/*********************************************************************************************************
 ** 函数名称:  sntpOpenSocket
 ** 功能描述:  打开网络套接字
 ** 输　入  :  无
 ** 输　出  :  网络文件描述符或错误值
 ** 全局变量:
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpOpenSocket (VOID)
{
    INT iSockfd;

    iSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (iSockfd < 0) {
        SNTP_ERR("socket error.\n");
    }

    return  (iSockfd);
}
/*********************************************************************************************************
 ** 函数名称:  sntpInstallReceiver
 ** 功能描述:  安装一个SNTP时间戳接收器
 ** 输　入  :  无
 ** 输　出  :  无
 ** 全局变量:  iSntpSockfd  socket描述符
 ** 调用模块:
 ********************************************************************************************************/
VOID  sntpInstallReceiver (VOID)
{
    sntpReceive(iSntpSockfd, INADDR_ANY, SNTP_PORT);
}
/*********************************************************************************************************
 ** 函数名称:  sntpInstallConnecter
 ** 功能描述:  sntp连接器安装
 ** 输　入  :  ntcfgFile        配置文件结构指针
 ** 输　出  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** 全局变量:  cSntpHostName    服务器地址
 **            iSntpsockfd      socket描述符
 **            ucSntpHandleCmd  SNTP命令
 **            cSntpLogFileName 输出Log文件名
 **            iSntpConnFlag    socket连接标志
 ** 调用模块:
 ********************************************************************************************************/
static INT  sntpInstallConnecter (SNTP_CFG_BODY   *ntcfgFile)
{
    INT  iRet;

    if (!ntcfgFile) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "conf struct addr invalid(null).");
        }
        return  (SNTP_PX_ERR);
    }

    if (memcmp(cSntpHostName, ntcfgFile->NTCFG_cHostName,
               SNTP_NAME_LEN)) {                                           /*  判断是否是已连接的地址   */
        iRet = sntpConnect(iSntpSockfd,
                ntcfgFile->NTCFG_cHostName, "123");                        /*  启动SNTP连接模块         */

        if (iRet == SNTP_NO_ERR) {
            iSntpConnFlag = SNTP_CONN_SUCC;
        } else {
            iSntpConnFlag = SNTP_CONN_FAIILED;
        }

        memcpy(cSntpHostName, ntcfgFile->NTCFG_cHostName,
               SNTP_NAME_LEN);                                             /*  保存服务器地址           */
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** 函数名称:  sntpMain
 ** 功能描述:  sntp时间同步主函数
 ** 输　入  :  无
 ** 输　出  :  SNTP_NO_ERR或其他错误值
 ** 全局变量:  iSntpSockfd   socket描述符
 **            iSntpInterval 更新时间间隔
 ** 调用模块:
 ********************************************************************************************************/
INT  sntpMain (VOID)
{
    INT             iRet;
    SNTP_CFG_BODY   ntcfgFile;

    iSntpSockfd = sntpOpenSocket();                                        /*  打开套接字               */
    if (iSntpSockfd == -1) {
        return  (SNTP_SOCK_ERR);
    }

    sntpInstallReceiver();                                                 /*  安装本地接收模块         */
#if SNTP_DEBUG == 0
    if (daemon(0, 0)) {                                                    /*  开始守护进程             */
        perror("daemon\n");
        return  (SNTP_PX_ERR);
    }
#endif
    for (;;) {

        iRet = sntpConfFileParse(SNTP_CONF_PATH, &ntcfgFile);              /*  解析配置文件信息         */
        if (iRet != SNTP_NO_ERR) {
            return  (SNTP_READ_CONF_ERR);
        }

        sntpInstallConnecter(&ntcfgFile);
        if (iSntpConnFlag == SNTP_CONN_SUCC) {                             /*  已经和服务器做了连接     */
            sntpInstallAlgorithm();                                        /*  启动SNTP算法模块         */
        } else {
            SNTP_ERR("Connect server failed.\n");
        }

        sleep(iSntpInterval);                                              /*  在指定的时间做一次同步   */
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
