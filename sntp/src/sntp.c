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
 ** ��   ��   ��: sntp.c
 **
 ** ��   ��   ��: Lu.ZhenPing (¬��ƽ)
 **
 ** �ļ���������: 2014 �� 05 �� 09 ��
 **
 ** ��        ��: sntp ������Դ�ļ�.
 ********************************************************************************************************/
#include "sntp.h"
/*********************************************************************************************************
**  ȫ�ֱ�������                                                                                                                                                                                                                                                                                                  *
*********************************************************************************************************/
static  CHAR             cSntpHostName[SNTP_NAME_LEN];                     /*  ��ȡ�����ļ���������ַ   */
static  UCHAR            ucSntpHandleCmd;                                  /*  ������������             */
static  INT              iSntpInterval;                                    /*  ʱ����¼��             */
static  INT              iSntpConnFlag;                                    /*  socket���ӱ�־           */
static  INT              iSntpSockfd;                                      /*  SNTP socket������        */
/*********************************************************************************************************
**  ������ص�ȫ�ֱ���                                                                                                                                                                                                                                                                                                  *
*********************************************************************************************************/
static  CHAR             cSntpLogFileName[SNTP_NAME_LEN];                  /*  SNTP log�ļ���           */
/*********************************************************************************************************
 ** ��������:  sntpConfFileParse
 ** ��������:  ����SNTP�����ļ���Ϣ
 ** �䡡��  :  pcFileName        �����ļ�·��
 **            pntcfgFile        �����ļ�
 ** �䡡��  :  SNTP_NO_ERR(�ɹ�) OR SNTP_PX_ERR(ʧ��)
 ** ȫ�ֱ���:  ucSntpHandleCmd   SNTP����
 **            iSntpInterval     ʱ����¼��
 **            cSntpLogFileName  ����log����ļ���
 ** ����ģ��:  ini������
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

    ucSntpHandleCmd &= 0;                                                  /*  ��������                 */
    bzero(pntcfgFile, sizeof(SNTP_CFG_BODY));

    ddict = iniparser_load(pcFileName);
    if (ddict == LW_NULL) {
        return  (SNTP_NO_CFG_FILE_ERR);
    }

    pcVal = iniparser_getstring(ddict,                                     /*  ��÷�������ַ           */
                                "server:serverAddr",
                                pcDef);

    SNTP_LOG("server addr: %s\n", pcVal);

    memcpy(pntcfgFile->NTCFG_cHostName, pcVal, strlen(pcVal));

    iUpdateTSys = iniparser_getint(ddict,                                  /*  �Ƿ����ϵͳʱ��         */
                                   "update:updateTimeSystem",
                                   iNotfound);
    iRtcTime    = iniparser_getint(ddict,                                  /*  �Ƿ����rtcʱ��          */
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
                                 iNotfound);                               /*  ����ʱ��ļ��           */

    iSntpInterval = iInterval * SNTP_ONE_MIN;

    SNTP_LOG("update time interval: %d(min)\n", iInterval);
    /*
     *  �������
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
 ** ��������:  sntpReceive
 ** ��������:  SNTP������������
 ** �䡡��  :  iSockfd  �ļ�������
 **            uiAddr   �����ַ
 **            usPort   ����˿ں�
 ** �䡡��  :  SNTP_NO_ERR or SNTP_PX_ERR
 ** ȫ�ֱ���:
 ** ����ģ��:
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
 ** ��������:  sntpConnect
 ** ��������:  ������������
 ** �䡡��  :  iSockfd          �ļ�������
 **            pcHostName       �����������ַ
 **            pcPort           ����������˿ں�
 ** �䡡��  :  ����״̬
 ** ȫ�ֱ���:  ucSntpHandleCmd  SNTP����
 **            cSntpLogFileName ����log����ļ���
 ** ����ģ��:
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
     *  ϣ����÷��������������͵���Ϣ
     */
    aiHints.ai_socktype = SOCK_DGRAM;
    aiHints.ai_family   = AF_INET;
    aiHints.ai_protocol = IPPROTO_UDP;

    iRet = getaddrinfo(pcHostName,
                       pcPort, &aiHints, &aiResult);                       /*  �õ���������ַ��Ϣ       */
    if (iRet != 0) {
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "getaddrinfo error.\n");
        }

        return  (SNTP_PX_ERR);
    }

    if (connect(iSockfd, (struct sockaddr *)aiResult->ai_addr,
                sizeof(struct sockaddr_in)) == SNTP_PX_ERR) {              /*  ���ӷ�����               */
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "connect error.\n");
        }
        return  (SNTP_PX_ERR);
    }

    freeaddrinfo(aiResult);

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** ��������:  sntpPacketStuff
 ** ��������:  ��䷢�Ͱ�
 ** �䡡��  :  pnttpSntpPacket  SNTP���ṹָ��
 ** �䡡��  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** ȫ�ֱ���:  ucSntpHandleCmd  SNTP����
 **            cSntpLogFileName ����log����ļ���
 ** ����ģ��:
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

    SNTP_GET_TIME(&tvNow, NULL);                                           /*  ��ȡ����ʱ��             */

    /*
     *  �ͻ�����䱾��ʱ����ͷ�������������Ϣͷ
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
 ** ��������:  sntpPacketSend
 ** ��������:  �ͻ��˷���SNTP�����
 ** �䡡��  :  iSockfd          �ļ�������
 ** �䡡��  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** ȫ�ֱ���:  ucSntpHandleCmd  SNTP����
 **            cSntpLogFileName ����log����ļ���
 ** ����ģ��:
 ********************************************************************************************************/
INT  sntpPacketSend (INT  iSockfd)
{
    SNTP_TIME_PKT nttpSntpPacket;
    INT           iRet;

    memset(&nttpSntpPacket, 0, sizeof(nttpSntpPacket));

    sntpPacketStuff(&nttpSntpPacket);                                      /*  ��������               */

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
 ** ��������:  sntpSystemTimeUpdate
 ** ��������:  ����ϵͳʱ��������
 ** �䡡��  :  pnttppIn         ʱ�����ָ��
 ** �䡡��  :  SNTP_NO_ERR
 ** ȫ�ֱ���:  ucSntpHandleCmd  SNTP����
 **            cSntpLogFileName ���Log�ļ���
 ** ����ģ��:
 ********************************************************************************************************/
INT  sntpSystemTimeUpdate (SNTP_TIME_PKT  *pnttpIn)
{
    struct timeval tvSet;
    SNTP_TIME      nttTransTime;

    nttTransTime.NTT_uiCorase =
            ntohl(pnttpIn->NTTP_nttTransTimestamp.NTT_uiCorase);          /* �������ֽ���ת�������ֽ��� */
    nttTransTime.NTT_uiFine   =
            ntohl(pnttpIn->NTTP_nttTransTimestamp.NTT_uiFine);

    tvSet.tv_sec = nttTransTime.NTT_uiCorase - JAN_1970;
    tvSet.tv_usec = USEC(nttTransTime.NTT_uiFine);

    if (SNTP_SET_TIME(&tvSet, LW_NULL) < 0) {                             /* ����ϵͳʱ��               */
        if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
            SNTP_LOGHANDLE_F(cSntpLogFileName, "SNTP_SET_TIME error.\n");
        }
    }

    SNTP_LOG("update system time success.\n");

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** ��������:  sntpSystemTimeUpdate
 ** ��������:  ����ϵͳʱ��������
 ** �䡡��  :  ��
 ** �䡡��  :  ��
 ** ȫ�ֱ���:
 ** ����ģ��:
 ********************************************************************************************************/
VOID  sntpRtcTimeUpdate (VOID)
{
    SNTP_SYNC_RTC();                                                       /*  ���µ�ʱ��д��RTC        */
    SNTP_LOG("update rtc time success.\n");
}
/*********************************************************************************************************
 ** ��������:  sntpPacketHandle
 ** ��������:  ������յ��ķ�����ʱ�����
 ** �䡡��  :  pnttpIn           ʱ�����ָ��
 ** �䡡��  :  ��
 ** ȫ�ֱ���:  ucSntpHandleCmd   SNTP����
 **            cSntpLogFileName  Log����ļ���
 ** ����ģ��:
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
 ** ��������:  sntpProcess
 ** ��������:  SNTP�㷨��������
 ** �䡡��  :  iSockfd          �ļ�������
 ** �䡡��  :  SNTP_NO_ERR
 ** ȫ�ֱ���:  ucSntpHandleCmd  ����
 **            cSntpLogFileName log�ļ���
 ** ����ģ��:
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

        iRet = select(iSockfd + 1, &fdset, LW_NULL, LW_NULL, &tvDelay);     /*  �ȴ�isockfd�ɶ�         */
        if (iRet != 1) {                                                    /*  select ʧ��             */
            if (iRet == EINTR) {
                continue;
            }

            if (iRet < 0) {
                perror("select");
            }

            sntpPacketSend(iSockfd);                                        /*  �ͻ��˷��������        */
            continue;

        } else {                                                            /*  select �ɹ�             */
            iPackLen = recvfrom(iSockfd, &nttpIncoming,                     /*  �ӷ���������ʱ���      */
                                sizeof(SNTP_TIME_PKT), 0,
                                LW_NULL, LW_NULL);
            if (iPackLen < 0) {
                if (SNTP_CONTAIN(ucSntpHandleCmd, SNTP_CMD_DEBUG)) {
                    SNTP_LOGHANDLE_F(cSntpLogFileName, "recvfrom error.\n");
                }

            } else if (iPackLen <= sizeof(SNTP_TIME_PKT)) {
                sntpPacketHandle(&nttpIncoming);                            /*  ����ӷ��������յ��İ�  */

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
 ** ��������:  sntpInstallAlgorithm
 ** ��������:  SNTP�㷨����װ
 ** �䡡��  :  ��
 ** �䡡��  :  ��
 ** ȫ�ֱ���:  iSntpSockfd  socket������
 ** ����ģ��:
 ********************************************************************************************************/
VOID  sntpInstallAlgorithm (VOID)
{
    sntpProcess(iSntpSockfd);
}
/*********************************************************************************************************
 ** ��������:  sntpOpenSocket
 ** ��������:  �������׽���
 ** �䡡��  :  ��
 ** �䡡��  :  �����ļ������������ֵ
 ** ȫ�ֱ���:
 ** ����ģ��:
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
 ** ��������:  sntpInstallReceiver
 ** ��������:  ��װһ��SNTPʱ���������
 ** �䡡��  :  ��
 ** �䡡��  :  ��
 ** ȫ�ֱ���:  iSntpSockfd  socket������
 ** ����ģ��:
 ********************************************************************************************************/
VOID  sntpInstallReceiver (VOID)
{
    sntpReceive(iSntpSockfd, INADDR_ANY, SNTP_PORT);
}
/*********************************************************************************************************
 ** ��������:  sntpInstallConnecter
 ** ��������:  sntp��������װ
 ** �䡡��  :  ntcfgFile        �����ļ��ṹָ��
 ** �䡡��  :  SNTP_NO_ERR or   SNTP_PX_ERR
 ** ȫ�ֱ���:  cSntpHostName    ��������ַ
 **            iSntpsockfd      socket������
 **            ucSntpHandleCmd  SNTP����
 **            cSntpLogFileName ���Log�ļ���
 **            iSntpConnFlag    socket���ӱ�־
 ** ����ģ��:
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
               SNTP_NAME_LEN)) {                                           /*  �ж��Ƿ��������ӵĵ�ַ   */
        iRet = sntpConnect(iSntpSockfd,
                ntcfgFile->NTCFG_cHostName, "123");                        /*  ����SNTP����ģ��         */

        if (iRet == SNTP_NO_ERR) {
            iSntpConnFlag = SNTP_CONN_SUCC;
        } else {
            iSntpConnFlag = SNTP_CONN_FAIILED;
        }

        memcpy(cSntpHostName, ntcfgFile->NTCFG_cHostName,
               SNTP_NAME_LEN);                                             /*  �����������ַ           */
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
 ** ��������:  sntpMain
 ** ��������:  sntpʱ��ͬ��������
 ** �䡡��  :  ��
 ** �䡡��  :  SNTP_NO_ERR����������ֵ
 ** ȫ�ֱ���:  iSntpSockfd   socket������
 **            iSntpInterval ����ʱ����
 ** ����ģ��:
 ********************************************************************************************************/
INT  sntpMain (VOID)
{
    INT             iRet;
    SNTP_CFG_BODY   ntcfgFile;

    iSntpSockfd = sntpOpenSocket();                                        /*  ���׽���               */
    if (iSntpSockfd == -1) {
        return  (SNTP_SOCK_ERR);
    }

    sntpInstallReceiver();                                                 /*  ��װ���ؽ���ģ��         */
#if SNTP_DEBUG == 0
    if (daemon(0, 0)) {                                                    /*  ��ʼ�ػ�����             */
        perror("daemon\n");
        return  (SNTP_PX_ERR);
    }
#endif
    for (;;) {

        iRet = sntpConfFileParse(SNTP_CONF_PATH, &ntcfgFile);              /*  ���������ļ���Ϣ         */
        if (iRet != SNTP_NO_ERR) {
            return  (SNTP_READ_CONF_ERR);
        }

        sntpInstallConnecter(&ntcfgFile);
        if (iSntpConnFlag == SNTP_CONN_SUCC) {                             /*  �Ѿ��ͷ�������������     */
            sntpInstallAlgorithm();                                        /*  ����SNTP�㷨ģ��         */
        } else {
            SNTP_ERR("Connect server failed.\n");
        }

        sleep(iSntpInterval);                                              /*  ��ָ����ʱ����һ��ͬ��   */
    }

    return  (SNTP_NO_ERR);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
