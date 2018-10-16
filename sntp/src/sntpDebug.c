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
 ** ��   ��   ��: sntpDebug.c
 **
 ** ��   ��   ��: Lu.ZhenPing (¬��ƽ)
 **
 ** �ļ���������: 2014 �� 05 �� 15 ��
 **
 ** ��        ��: sntp �����ļ�.
 ********************************************************************************************************/
#include <stdarg.h>
#include "sntp.h"
/*********************************************************************************************************
 ** ��������:  sntpDumpPkt
 ** ��������:  (������)��ӡ���յ��İ�����ϸ��Ϣ
 ** �䡡��  :  iDebug      ���Կ���
 **            pcFileName  ���log�ļ���
 **            pnttpPkt    SNTP���ݰ�
 ** �䡡��  :  ERROR_NONE(�ɹ�) OR PX_ERROR(ʧ��)
 ** ȫ�ֱ���:
 ** ����ģ��:
 ********************************************************************************************************/
INT  sntpDumpPkt (INT  iDebug, PCHAR  pcFileName, SNTP_TIME_PKT  *pnttpPkt)
{
    UINT32           uiLi, uiVn, uiMode, uiStratum, uiPoll, uiPrec;
    UINT32           uiDelay, uiDisp, uiRefid;
    SNTP_TIME        nttReftime, nttOrgtime, nttRectime, nttTranstime;

    uiLi                      = (ntohl(pnttpPkt->NTTP_uiPktHdr) >> 30) & 0x03;
    uiVn                      = (ntohl(pnttpPkt->NTTP_uiPktHdr) >> 27) & 0x07;
    uiMode                    = (ntohl(pnttpPkt->NTTP_uiPktHdr) >> 24) & 0x07;
    uiStratum                 = (ntohl(pnttpPkt->NTTP_uiPktHdr) >> 16) & 0xff;
    uiPoll                    = (ntohl(pnttpPkt->NTTP_uiPktHdr) >> 8)  & 0xff;
    uiPrec                    =  ntohl(pnttpPkt->NTTP_uiPktHdr)         & 0xff;
    uiDelay                   =  ntohl(pnttpPkt->NTTP_uiRootDelay);
    uiDisp                    =  ntohl(pnttpPkt->NTTP_uiRootDispersion);
    uiRefid                   =  ntohl(pnttpPkt->NTTP_uiRefId);

    nttReftime.NTT_uiCorase   = ntohl(pnttpPkt->NTTP_nttRefTimestamp.NTT_uiCorase);
    nttReftime.NTT_uiFine     = ntohl(pnttpPkt->NTTP_nttRefTimestamp.NTT_uiFine);
    nttOrgtime.NTT_uiCorase   = ntohl(pnttpPkt->NTTP_nttOrigTimestamp.NTT_uiCorase);
    nttOrgtime.NTT_uiFine     = ntohl(pnttpPkt->NTTP_nttOrigTimestamp.NTT_uiFine);
    nttRectime.NTT_uiCorase   = ntohl(pnttpPkt->NTTP_nttRecvTimestamp.NTT_uiCorase);
    nttRectime.NTT_uiFine     = ntohl(pnttpPkt->NTTP_nttRecvTimestamp.NTT_uiFine);
    nttTranstime.NTT_uiCorase = ntohl(pnttpPkt->NTTP_nttTransTimestamp.NTT_uiCorase);
    nttTranstime.NTT_uiFine   = ntohl(pnttpPkt->NTTP_nttTransTimestamp.NTT_uiFine);

    if (iDebug & SNTP_CMD_DEBUG) {
        FILE  *fp = SNTP_OPENLOG(pcFileName);
        if (!fp) {
            return  (SNTP_PX_ERR);
        }

        SNTP_LOGHANDLE(fp, "----------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|      |      |         |             |         |              |\n");
        SNTP_LOGHANDLE(fp, "|LI(%d) |VN(%d) | Mode(%d) | Stratum(%d)  | Poll(%d) |Precision(%d)|\n",
                uiLi, uiVn, uiMode, uiStratum, uiPoll, uiPrec);
        SNTP_LOGHANDLE(fp, "|      |      |         |             |         |              |\n");
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                      Root Delay(%.1f)                     |\n",
                SEC2USEC(uiDelay));
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                      Root Dispersion(%.1f)             |\n",
                SEC2USEC(uiDisp));
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                      RefId(%u.%u.%u.%u)                    |\n",
                (uiRefid>>24) & 0xff, (uiRefid>>16) & 0xff, (uiRefid>>8) & 0xff, uiRefid & 0xff);
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                   Reference(%u.%.10u)           |\n",
                nttReftime.NTT_uiCorase, nttReftime.NTT_uiFine);
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                   Originate(%u.%.10u)           |\n",
                nttOrgtime.NTT_uiCorase, nttOrgtime.NTT_uiFine);
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                   Receive(%u.%.10u)             |\n",
                nttRectime.NTT_uiCorase, nttRectime.NTT_uiFine);
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");
        SNTP_LOGHANDLE(fp, "|                   Transmit(%u.%.10u)            |\n",
                nttTranstime.NTT_uiCorase, nttTranstime.NTT_uiFine);
        SNTP_LOGHANDLE(fp, "---------------------------------------------------------------\n");

        SNTP_CLOSELOG(fp);
    }

    return  (SNTP_NO_ERR);
}

/*********************************************************************************************************
  END
*********************************************************************************************************/
