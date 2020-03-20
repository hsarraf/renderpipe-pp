#include "RPServerSock.h"



/* RPServerSock constructor */
RPServerSock::RPServerSock (  unsigned short servPort, 
								void(*connectCallback)(void *), 
								void(*sendMsgCallback)(void *), 
								void(*recvMsgCallback)(void *), 
								void(*closeCallback)(void *), 
								void(*disconnectCallback)(void *)  )    {

	/* sets callback functions */
	_connectCallback = connectCallback;
	_sendMsgCallback = sendMsgCallback;
	_recvMsgCallback = recvMsgCallback;
	_closeCallback = closeCallback;
	_disconnectCallback = disconnectCallback;

	if (servPort)  /* checks if server port is given */
		_serverPort = servPort;
	else		   /* otherwise server port set from SERVERPORT */
		_serverPort = SERVERPORT;

	setProto( pUndetermined );		/* sets protocol type */

}


/* RPServerSock destructor */
RPServerSock::~RPServerSock() {}


/* callback functions initialization */
void (*RPServerSock::_connectCallback)(void *) = 0;
void (*RPServerSock::_sendMsgCallback)(void *) = 0;
void (*RPServerSock::_recvMsgCallback)(void *) = 0;
void (*RPServerSock::_closeCallback)(void *) = 0;
void (*RPServerSock::_disconnectCallback)(void *) = 0;


