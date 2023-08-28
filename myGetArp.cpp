#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
#include <net/netlink.h>
using namespace std;

#define NUD_VALIDE (NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE|NUD_PROBE|NUD_STALE|NUD_DELAY)
#define GRP_SUBSCRIBE (RTMGRP_IPV6_IFADDR | RTMGRP_NEIGH | RTMGRP_IPV4_IFADDR)

void parseRtAttr(struct nlattr *tb[], int max, struct nlattr *pRta, int len)
{
    memset(tb, 0, sizeof(struct nlattr*) * (max + 1));

    while(RTA_OK(pRta, len))
    {
        if (pRta->nla_type <= max)
        {
            tb[pRta->nla_type] = pRta;
        }
        pRta = RTA_NEXT(pRta, len);
    }
}


int main(){

    uint8_t buf[256 * 1024];

    struct nlmsghdr*    pHeaderAnswer;
    struct sockaddr_nl  local;
    struct iovec        iov {.iov_base = buf, .iov_len = sizeof(buf)};
    struct msghdr       msg;
    struct ndmsg*       pNdmsg;
    struct nlattr*      tb[NDA_MAX + 1];


    struct{
        nlmsghdr header;
        ndmsg    neighMsg;
    }req;


    memset(&local, 0, sizeof(local));
    local.nl_family = AF_NETLINK;
    local.nl_pid = 0;
    local.nl_groups = 0;

    memset(&req, 0, sizeof(req));
    req.header.nlmsg_len = NLMSG_LENGTH(sizeof(req.neighMsg));
    req.header.nlmsg_type = RTM_GETNEIGH;
    req.header.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.neighMsg.ndm_family = AF_INET;


    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if(sock >= 0)
    {
        cout<<"Sock: " << sock << " OK!!!" << endl;
        if(bind(sock, (struct sockaddr*)&local, sizeof(local)) >= 0)
        {
            cout << "Bind: OK!!!" << endl;
            int resSend = send(sock, &req, req.header.nlmsg_len, 0);
            if(resSend > 0)
            {
                cout << "Send: " << resSend << " OK" << endl;

                memset(&msg, 0, sizeof(msg));
                msg.msg_iov = &iov;
                msg.msg_iovlen =1;
                msg.msg_controllen = 0;
                msg.msg_control = NULL;
                msg.msg_flags =0;

                int len = recvmsg(sock, &msg, 0);
                if(len >0)
                {
                    cout << "ResRcv: " << len<< " OK!!!" << endl;

                    memset(tb, 0, sizeof(tb));
                    pHeaderAnswer = (struct nlmsghdr*)buf;
                    while(NLMSG_OK(pHeaderAnswer, len))
                    {
                        pNdmsg = (struct ndmsg*)NLMSG_DATA(pHeaderAnswer);
                        parseRtAttr(tb, NDA_MAX, NDA_HDRLEN(pNdmsg), pHeaderAnswer->nlmsg_len)

                    }
                }
            }
        }
    }

























    return 0;
}
