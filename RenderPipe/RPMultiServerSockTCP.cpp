#include "RPMultiServerSockTCP.h"


/* RPMultiServerSockTCP constructor */
RPMultiServerSockTCP::RPMultiServerSockTCP ( unsigned short servPort, 
												void (*connectCallback) (void *), 
												void (*sendMsgCallback) (void *), 
												void (*recvMsgCallback) (void *), 
												void (*closeCallback) (void *), 
												void (*disconnectCallback) (void *)   )  :
/* member initializer */
RPServerSockTCP( servPort, 
			 connectCallback, 
			 sendMsgCallback, 
			 recvMsgCallback, 
			 closeCallback, 
			 disconnectCallback )    {
}


/* RPMultiServerSockTCP distructor */
RPMultiServerSockTCP::~RPMultiServerSockTCP() {
}


/* initializes the list, _allActiveClientsList */
list<RPActiveClientTCP *>  RPMultiServerSockTCP::_allActiveClientsList = list<RPActiveClientTCP *>();


/* initializes the list, _activeClientThreadsList */
list<thread *> RPMultiServerSockTCP::_activeClientThreadsList = list <thread *>();


/* initializes _mutx */
mutex RPMultiServerSockTCP::_mutx = mutex();



int RPMultiServerSockTCP::makeActiveSocket (RPStatus *stat) {

	int clntSock;	/* Socket descriptor for client */
	int clntLen;	/* Socket descriptor length for client */
	struct sockaddr_in clntAddr;	/* address struct of client */

	for (;;) /* Runs forever */ {
        /* Sets the size of the in-out parameter */
        clntLen = sizeof(clntAddr);

        /* Waits for a client to connect */
		if ((clntSock = accept(_serverSock, (struct sockaddr *)&clntAddr, &clntLen)) < 0) {
			__print( "ERROR: Socket accept failes\n" );
			stat->setStatus( RPStatus::sSockAcceptFailed );
			continue;
		}
		/*  connection established */

		/*  makes active client arguments struct */
		activeClientArgs acArgs;
		acArgs.clntSock = clntSock;
		acArgs.clntAddr = &clntAddr;

		/* makes the new active client thread and pushes it to list of threads */
		_activeClientThreadsList.push_back( new thread( runActiveClientThreadTCP, (void *)&acArgs ) );

    }  /*  end loop */

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/* here is where we are out of accept loop, so all the made threads must be joined and deleted */
	/////////////////////////////////////////////////////////////////////////////////////////////////

	list<thread *>::iterator it;  /* threads list iterator */
	/* loop through the threads */
	for(it = _activeClientThreadsList.begin() ; it != _activeClientThreadsList.end() ; ++it) {
      thread *t = *it;
      t->join();	/* joins this thread */
      delete t;		/* delete this thread objects */
    }

	stat->setStatus( RPStatus::sSuccessful );
	return 0;  /* successful return */

}  /* end func <makeActiveSocket> */


/* runs an instance of the activated client, threaded */
void RPMultiServerSockTCP::runActiveClientThreadTCP (void *args) {

	/* gets all the data and prepare to make the thread */
	struct activeClientArgs *acArgs = (activeClientArgs *)args;
	int clientSock = acArgs->clntSock;
	struct sockaddr_in *clientAddr = acArgs->clntAddr;
	unsigned short    port = ntohs( clientAddr->sin_port );				/* port is the same as   _serverPort */
	char       *clientIPAddr = inet_ntoa( clientAddr->sin_addr );			/* accpeted client IP address in string dotted form */

	/* prepares the active clietn object */
	RPActiveClientTCP  activeClient( clientSock, clientIPAddr, 
										_connectCallback, 
										_sendMsgCallback, 
										_recvMsgCallback, 
										_closeCallback, 
										_disconnectCallback );

	add_allActiveClientsList( &activeClient );		/*  adds the acltiva client object to the list */

	RPStatus stat;
	activeClient.activateConnection( &stat );		/* activate the connection and fall into the main loop -> handleConnection() */

	/////////////////////////////////////////////////
	/* here is where we are out of the handle loop */
	/////////////////////////////////////////////////

	if ( RPStatus::sSuccessful != stat.getStatus() )
		__print( "ERROR on thread, %d\n", this_thread::get_id() );

	rem_allActiveClientsList( &activeClient );		/* removes the active ciient from the list */

}  /* end func <runActiveClientThreadTCP> */
