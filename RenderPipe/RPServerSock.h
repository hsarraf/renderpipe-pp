/* 
RPServerSock.h header file contains the class, RPServerSock,
RPServerSock is abstract and the base class for the server-client connection classes, including TCP and UDP,
RPServerSock also contains main server port, number of pending connection and recieve buffer size.
*/


#ifndef    __RPSERVERSOCK_HEADER__
#define    __RPSERVERSOCK_HEADER__


#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdarg.h>

#include "RPutils.h"
#include "RPStatus.h"


#define SERVERPORT 22222	/* server port number */
#define MAXPENDING 5		/* maximum number of pending socks */
#define RCVBUFSIZE 32		/* size of receive buffer */




class RPServerSock {
public:
	RPServerSock (    unsigned short servPort = 0, 
						void (*connectCallback) (void *) = 0, 
						void (*sendMsgCallback) (void *) = 0, 
						void (*recvMsgCallback) (void *) = 0, 
						void (*closeCallback) (void *) = 0, 
						void (*disconnectCallback) (void *) = 0    );	/* constructor */

	virtual ~RPServerSock();		/* distructor */

	enum ProtoType {		  /* enumeration for transport protocol types */
		pUndetermined = 0,		 /* undetermined protocol */
		pTCP,					 /* TCP protocol */
		pUDP					 /* UDP protocol */
	};		/* end enumeration <ProtoType> */

	ProtoType getProto() {return _pType;}			/* returns transport protocol type, e.g. TCP, UDP, .. */
	void setProto (ProtoType pt) {_pType = pt;}		/* sets transport protocol type, e.g. TCP, UDP, .. */

	virtual int activateServerTCP (RPStatus *stat = 0) = 0;		/* activates server, abstract */



protected:
	virtual int makePassiveSocket (RPStatus *stat = 0) = 0;		/* makes socket passively, abstract method*/
	virtual int makeActiveSocket (RPStatus *stat = 0) = 0;		/* makes socket actively, abstract method */

	/* callback functions */
	static void (*_connectCallback) (void *);		/* called as soon as client connected */
	static void (*_sendMsgCallback) (void *);		/* called as soon as client sends message */
	static void (*_recvMsgCallback) (void *);		/* called as soon as client recienes message */
	static void (*_closeCallback) (void *);			/* called as soon as client closes */
	static void (*_disconnectCallback) (void *);	/* called as soon as client disconnected */

	unsigned short    _serverPort;		/* server port */
	int				  _serverSock;		/* server socket discriptor */

	ProtoType _pType;		/* transport protocol type */

};   /* end class <RPServerSock> */



#endif    /*__RPSERVERSOCK_HEADER__*/