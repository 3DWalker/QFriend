#include "protocol.h"

//初始化协议
PDU *mkPDU(uint usMsgLen)
{
    uint usPDULen = sizeof(PDU) + usMsgLen;
    PDU *pdu = (PDU *)malloc(usPDULen);
    if (NULL == pdu)
    {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0, usPDULen);
    pdu->usPDULen = usPDULen;
    pdu->usMsgLen = usMsgLen;

    return pdu;
}
