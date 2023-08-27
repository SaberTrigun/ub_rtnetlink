#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
using namespace std;

#define NUD_VALIDE (NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE|NUD_PROBE|NUD_STALE|NUD_DELAY)
#define GRP_SUBSCRIBE (RTMGRP_IPV6_IFADDR | RTMGRP_NEIGH | RTMGRP_IPV4_IFADDR)

void parseRtAttr(struct rtattr *tb[], int max, struct rtattr *pRta, int len)
{
    memset(tb, 0, sizeof(struct rtattr*) * (max + 1));

    while(RTA_OK(pRta, len))
    {
        if (pRta->rta_type <= max)
        {
            tb[pRta->rta_type] = pRta;
        }
        pRta = RTA_NEXT(pRta, len);
    }
}


int main(){

    struct nslmsghdr    *pHeaderAnswer = nullptr;
    struct sockaddr_nl  local;
    struct iovec        iov;
    struct msghdr       msg;


    struct{
        nlmsghdr header;
        ndmsg    neighMsg;
        char     attrBuf[8192];
    }req;

    memset(&local, 0, sizeof(local));
    local.nl_family     = AF_NETLINK;
    local.nl_pid        = 0;
    //local.nl_groups     = RTMGRP_IPV6_IFADDR | RTMGRP_NEIGH | RTMGRP_IPV4_IFADDR;

    memset(&req, 0, sizeof(req));
    req.header.nlmsg_len    = NLMSG_LENGTH(sizeof(struct ndmsg));
    req.header.nlmsg_flags  = NLM_F_REQUEST | NLM_F_DUMP;
    req.header.nlmsg_type   = RTMGRP_IPV6_IFADDR | RTMGRP_NEIGH | RTMGRP_IPV4_IFADDR;
    req.neighMsg.ndm_state  = NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE|NUD_PROBE|NUD_STALE|NUD_DELAY;
    req.neighMsg.ndm_family = AF_INET;
    req.header.nlmsg_pid    = 0;
    req.header.nlmsg_seq    = 1;

    memset(&msg, 0, sizeof(msg));
    iov.iov_base = (void*)&req.header;
    iov.iov_len  = req.header.nlmsg_len;

    msg.msg_iov     = &iov;
    msg.msg_iovlen  = 1;


    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if(sock >= 0)
    {
        cout << "Sock: " << sock << " OK!!!" << endl;
        if(bind(sock, (struct sockaddr*) &local, sizeof(local)) >= 0)
        {
            cout << "Bind: OK!!!" << endl;
            ssize_t sendRes = sendmsg(sock, &msg, 0);
            if(sendRes>= 0)
            {
                cout<< "SendRes: " << sendRes<< " OK!!!" << endl;
                struct iovec    ioAnswer;
                struct msghdr   msgHdr;
                memset(&ioAnswer, 0, sizeof(iovecAnswer));
                memset(&msgHdr, 0, sizeof(msgHdr));

                ioAnswer.iov_base = msgHdr;
                ioAnswer.iov_len  = IFLIST_REPLY_BUFFER;
                msgHdr.msg_iov = &ioAnswer;
                msgHdr.msg_iovlen = 1;
                msgHdr.msg_name = &kernell
                msgHdr.msg_namelen = sizeof(kernel);
                msgHdr.

                ssize_t rcvRes =recvmsg(sock, &msg, MSG_DONTWAIT);
                if(rcvRes >= 0)
                {
                    cout << "RcvRes: " << rcvRes << " OK!!!" << endl;

                    //for()
                    {
                        
                    }
                }
                else
                {
                    cout << "RcvRes: " << rcvRes << " BAD!!!" << endl;
                }
            }
            else
            {
                cout << "SendRes: " << sendRes << " BAD!!!" << endl;
            }
        }
        else
        {
            cout << "Bind: BAD BIND!!" << endl;
        }
    }
    else
    {
        cout << "Sock: BAD SOCK!!!" << endl;
    }





    return 0;
}
