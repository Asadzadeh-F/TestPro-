#ifdef WIN32
#include "EthernetMessenger.h"

/*unsigned char testBuf[] = {0x1F, 0x1B, 0x1F, 0x03, 0x04, 0x05, 0x06, 0x10, 0x34, 0x27, 0x6F, 
	0x42, 0x6B, 0x59, 0x40, 0x79, 0x5B, 0x67, 0x5A, 0x66, 0x5D, 0x44, 0x47, 0x49
};
*/

#pragma comment(lib,"Ws2_32.lib")

EthernetMessenger::EthernetMessenger(DWORD tIp, int tPort):
	ip(tIp),
	port(tPort),
	channelopened(false)
{
	openChannel();
	//sendBuf(testBuf, sizeof(testBuf));
}

EthernetMessenger::~EthernetMessenger(void)
{
	closeChannel();
}

int EthernetMessenger::openChannel()
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
		return -1;
    }

    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==INVALID_SOCKET){
		WSACleanup();
		return -1;
    }

    struct sockaddr_in serverAddress;

    //unsigned long ip_num;
	//if((ip_num = inet_addr(CW2A(ip.GetBuffer()))) == INADDR_NONE){
	//	return -1;
    //}

    memset(&serverAddress,0,sizeof(sockaddr_in));
    serverAddress.sin_family=AF_INET;
    serverAddress.sin_addr.S_un.S_addr = htonl(ip);
	//serverAddress.sin_addr.s_addr = inet_addr( "192.168.1.101" );
    serverAddress.sin_port = htons(port);

	if(connect(sock,(sockaddr*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR){
		WSACleanup();
		return -1;
    }
 
	channelopened = true;

	return 0;
}

int EthernetMessenger::closeChannel()
{
	if(channelopened) {
		closesocket(sock);
		WSACleanup();
	}
	return 0;
}

unsigned int EthernetMessenger::sendBuf(unsigned char *buf, int len)
{
	if(!channelopened) 
		return -1;

	if(send(sock, (const char *)buf, len, 0)==SOCKET_ERROR){
		return -1;
	}

	return 0;
}

#else

#include "EthernetMessenger.h"

EthernetMessenger::EthernetMessenger(unsigned long tIp, int tPort):
	ip(tIp),
	port(tPort),
	channelopened(false)
{
}

EthernetMessenger::~EthernetMessenger(void)
{
}

int EthernetMessenger::openChannel()
{
	return 0;
}

int EthernetMessenger::closeChannel()
{
	return 0;
}

unsigned int EthernetMessenger::sendBuf(unsigned char *buf, int len)
{
	return 0;
}

unsigned int EthernetMessenger::recvBuf(unsigned char *buf, int maxlen)
{
	return 0;
}

unsigned int EthernetMessenger::getStatus(int statusId, int *pStatus)
{
	return 0;
}

unsigned int EthernetMessenger::recoverPrinterStatus(int statusId)
{
	return 0;
}

void EthernetMessenger::setTimeOut(int timeout)
{
}
#endif //WIN32
