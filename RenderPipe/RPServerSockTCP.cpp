#include "RPServerSockTCP.h"


/* RPServerSockTCP construcor */
RPServerSockTCP::RPServerSockTCP (    unsigned short servPort,
										void (*connectCallback) (void *), 
										void (*sendMsgCallback) (void *), 
										void (*recvMsgCallback) (void *), 
										void (*closeCallback) (void *), 
										void (*disconnectCallback) (void *)   ) : 
/* member initializer */
RPServerSock( servPort, 
			 connectCallback, 
			 sendMsgCallback, 
			 recvMsgCallback, 
			 closeCallback, 
			 disconnectCallback )    {

    /* sets the transport protocol type to TCP*/
	setProto( RPServerSock::pTCP );
	/* check port open condition */
	if ( !isPortOpen( _serverPort ) ) {
		__print( "the port, %d is already in use", _serverPort );
		return;		/* exit constructor */
	}
	/* makes the socket passive */
	RPStatus pstat;
	makePassiveSocket( &pstat );
	if ( RPStatus::sSuccessful != pstat.getStatus() ) {
		/*handle error*/
		return;		/* exit constructor */
	}

}   /* end construcor */



/* RPServerSockTCP disstrucor */
RPServerSockTCP::~RPServerSockTCP() {}



/* passivly activates socket */
int RPServerSockTCP::makePassiveSocket (RPStatus *stat) {

    int servSock;                    /* Socket descriptor for server */
    struct sockaddr_in servAddr;	 /* local address */
    WSADATA wsaData;                 /* structure for WinSock setup communication */


	/* loads Winsock 2.2 DLL */
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { 
		__print( "ERROR: WSAStartup failed\n" );
		stat->setStatus( RPStatus::sSockStartupFailed );
		WSACleanup();	/* unloads Winsock */
		return 1;    /* returns unsuccessfully */
	} /* end if */

    /* creates socket for incoming connections */
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		__print( "ERROR: Socket initialization failed\n" );
		stat->setStatus( RPStatus::sSockFDFailed );
		WSACleanup();	/* unloads Winsock */
		return 1;    /* returns unsuccessfully */
	} /* end if */

    /* Constructs local address structure */
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(_serverPort);      /* Local port */


    /* Binds to the local address */
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		__print( "ERROR: Socket bind failed\n" );
		stat->setStatus( RPStatus::sSockBindFailed );
		WSACleanup();	/* unloads Winsock */
		return 1;    /* returns unsuccessfully */
	} /* end if */


    /* Marks the socket so it will listen for incoming connections */
	if (listen(servSock, MAXPENDING) < 0) {
		__print( "ERROR: Socket listen failed\n" );
		stat->setStatus( RPStatus::sSockListenFailed );
		WSACleanup();	/* unloads Winsock */
		return 1;    /* returns unsuccessfully */
	} /* end if */

	/* passive socket made*/

	_serverSock = servSock;   	/* sets _serverSock data member */

	return 0;		/* returns successfully */

}	/* end func <makePassiveSocket> */



/* main activation procedure, it is called externally by the main server */
int RPServerSockTCP::activateServerTCP (RPStatus *stat) {

	if (stat)	/* checks if stat is set, if so, activates socket */
		return makeActiveSocket( stat );	
	else {		/* otherwise sets stat and activates socket */
		RPStatus stat;
		return makeActiveSocket( &stat );
	}	/* end else */

}  /* end func <activateServerTCP> */


/* actively activates socket */
int RPServerSockTCP::makeActiveSocket (RPStatus *stat) {

	int clntSock;	/* socket descriptor for client */
	int clntLen;	/* socket descriptor length for client */
	struct sockaddr_in clntAddr;	/* address struct of client */

	for (;;) {     /* runs forever */
		
		clntLen = sizeof(clntAddr);     /* Sets the size of the in-out parameter */

        /* waits for a client to connect, iteratively */
		if ((clntSock = accept(_serverSock, (struct sockaddr *) &clntAddr, &clntLen)) < 0) {
			__print( "ERROR: Socket accept failes\n" );
			stat->setStatus( RPStatus::sSockAcceptFailed );
			continue;
		} /* end if */
		/*  connection established */

		/* makes active client object */
		runActiveClientTCP( clntSock, &clntAddr, stat );
		if ( RPStatus::sClientActivationFailed == stat->getStatus() ) {
			__print( "ERROR: client activation failed!!" );
			continue;
		} /* end if */

    }	/* end loop */

	stat->setStatus( RPStatus::sSuccessful );
	return 0;   /* returns successfully */

}  /* end func <makeActiveSocket> */



/* acivates the incoming client */
int RPServerSockTCP::runActiveClientTCP (const int clientSock, const struct sockaddr_in *clientAddr, RPStatus *stat) {

	//unsigned short    port = ntohs( clientAddr->sin_port );				/* port is the same as   _serverPort */
	char       *clientIPAddr = inet_ntoa( clientAddr->sin_addr );			/* accpeted client IP address in string dotted form */

	/* makes the active clietn object */
	RPActiveClientTCP ( clientSock, clientIPAddr, 
						_connectCallback, 
						_sendMsgCallback, 
						_recvMsgCallback, 
						_closeCallback, 
						_disconnectCallback );

	stat->setStatus( RPStatus::sSuccessful );
	return 0;   /* returns successfully */

}  /* end func <runActiveClientTCP> */



/* checks if the given port is open */ 
bool RPServerSockTCP::isPortOpen(unsigned short port) {

	return true;

}  /*end func <isPortOpen> */

