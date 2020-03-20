/*  
RPActiveClientTCP.h header file contains class, RPActiveClientTCP,
RPActiveClientTCP class is derived from RPServerSock class,
it handles the server-client model of connections on TCP protocol ITERATIVELY.
*/


#ifndef    __RPSERVERSOCKTCP_HEADER__
#define    __RPSERVERSOCKTCP_HEADER__


#include "RPServerSock.h"
#include "RPActiveClientTCP.h"
#include "RPStatus.h"


class RPServerSockTCP : public RPServerSock {
public:
	RPServerSockTCP::RPServerSockTCP (    unsigned short servPort = 0, 
											void (*connectCallback) (void *) = 0, 
											void (*sendMsgCallback) (void *) = 0, 
											void (*recvMsgCallback) (void *) = 0, 
											void (*closeCallback) (void *) = 0, 
											void (*disconnectCallback) (void *) = 0    );  /* constructor */
	virtual ~RPServerSockTCP();		/* distructor */

	virtual int activateServerTCP (RPStatus *stat = 0);		/* activates server */

protected:
	virtual int makePassiveSocket (RPStatus *stat);
	virtual int makeActiveSocket (RPStatus *stat = 0);		/* overrided */
	int runActiveClientTCP (const int clientSock, const struct sockaddr_in *clientAddr, RPStatus *stat);	/* runs an instance of the activated client, not threaded */

	virtual bool isPortOpen (unsigned short port);		/* checks the open/close condition of given port */

};



#endif    /*__RPSERVERSOCKTCP_HEADER__*/