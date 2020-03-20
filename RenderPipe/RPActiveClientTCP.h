/*
RPActiveClientTCP.h header file contains the class, RPActiveClientTCP, 
it handles the activated client after attempting to activate client,
the object made from this class runs as a thread when the server object is RPMultiServerSockTCP
*/


#ifndef    __RPACTIVECLIENTTCP_HEADER__
#define    __RPACTIVECLIENTTCP_HEADER__


#include <list>
using   std::list;

#include "RPServerSock.h"
#include "RPStatus.h"


class RPActiveClientTCP {
public:
	RPActiveClientTCP(	int clientSock, 
						char *clinetIPAddr, 
						void (*connectCallback) (void *) = 0, 
						void (*sendMsgCallback) (void *) = 0, 
						void (*recvMsgCallback) (void *) = 0, 
						void (*closeCallback) (void *) = 0, 
						void (*disconnectCallback) (void *) = 0    );	/* constructor */
	virtual ~RPActiveClientTCP();		/* distructor */

	inline int get_clientSock() {return _clientSock;}		/* returns the client socket descriptor */
	inline char *get_clientIPAddr() {return _clientIPAddr;}	/* returns the client IP address in dotted string form */

	int activateConnection (RPStatus *stat);	/* activates the connection, this is calles externally to run the core function of the client */

private:

	int recvData (char *recvedData, int *recvedSize, RPStatus *stat = 0 );		/* recieves data from peer connection */
	int sendData (const char *data, int size, RPStatus *stat = 0 );		/* sends data to peer connection */

	int setupConnection (RPStatus *stat);		/* initializes the client connection */
	int handleConnection (RPStatus *stat);		/* runs the main function of the client connection */
	int finishConnection (RPStatus *stat);		/* finishes the client connection */

	int _clientSock;		/* client socket descriptor */
	char *_clientIPAddr;		/* client IP address in dotted string form */
	static void (*_connectCallback) (void *);		/* connection callback */
	static void (*_sendMsgCallback) (void *);		/* recieve message  callback */
	static void (*_recvMsgCallback) (void *);		/* send message callback */
	static void (*_closeCallback) (void *);			/* close connection callback */
	static void (*_disconnectCallback) (void *);	/* disconnect callback */
	
};	/* end class <RPActiveClientTCP> */


#endif    /* __RPACTIVECLIENTTCP_HEADER__ */

