/*
RPMultiServerSockTCP.h header file contains class, RPMultiServerSockTCP,
it is derived from RPServerSockTCP class,
it handles the server-client model of connections on TCP protocol CONCURRENTLY.
*/


#ifndef    __RPMULTISERVERSOCKTCP_HEADER__
#define    __RPMULTISERVERSOCKTCP_HEADER__


#include "RPServerSockTCP.h"
#include "RPActiveClientTCP.h"
#include <list>
using std::list;

#include "tinythread.h"
using namespace tthread;



/* struct holding the active client arguments, casted to (void *) for thread function */
struct activeClientArgs {
	int clntSock;		/* holds the client socket discriptor */
	struct sockaddr_in *clntAddr;		/* client address struct */ 
};  /* end struct <activeClientArgs> */


class RPMultiServerSockTCP : public RPServerSockTCP {
public:
	RPMultiServerSockTCP( unsigned short servPort = 0, 
								void (*connectCallback) (void *) = 0, 
								void (*sendMsgCallback) (void *) = 0, 
								void (*recvMsgCallback) (void *) = 0, 
								void (*closeCallback) (void *) = 0, 
								void (*disconnectCallback) (void *) = 0    );		/* constructor */
	virtual ~RPMultiServerSockTCP();		/* distructor */

	inline static list<RPActiveClientTCP *>    get_allActiveClientsList() {return RPMultiServerSockTCP::_allActiveClientsList;}		/* gets all the activated clients */ 
	inline static void    add_allActiveClientsList (RPActiveClientTCP *ac) {_allActiveClientsList.push_back( ac );}			/*  add given activated client */
	inline static void    rem_allActiveClientsList (RPActiveClientTCP *ac) {_allActiveClientsList.remove( ac );}			/* remove activated given client */


protected:
	virtual int makeActiveSocket (RPStatus *stat = 0);			/* make the socket active, overrided */
	static void runActiveClientThreadTCP (void *args);			/* runs an instance of the activated client, threaded */

	static   list<thread *> _activeClientThreadsList;	/* list of all the threads of activated clients */
	static mutex _mutx;			/*  mutex for locking the threads of activated clients */

	static list<RPActiveClientTCP *>    _allActiveClientsList;		/* list of all the activated clients */

};  /* end class <RPMultiServerSockTCP> */



#endif    /*__RPMULTISERVERSOCKTCP_HEADER__*/