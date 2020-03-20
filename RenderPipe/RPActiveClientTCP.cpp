#include "RPActiveClientTCP.h"

/* constructor */
RPActiveClientTCP::RPActiveClientTCP(	int clientSock, 
										char *clientIPAddr, 	
										void (*connectCallback) (void *), 
										void (*sendMsgCallback) (void *), 
										void (*recvMsgCallback) (void *), 
										void (*closeCallback) (void *), 
										void (*disconnectCallback) (void *)   ) :
/* member initializer */
_clientSock( clientSock ), 
_clientIPAddr( clientIPAddr )   {

	_connectCallback = connectCallback; 
	_sendMsgCallback = sendMsgCallback; 
	_recvMsgCallback = recvMsgCallback;
	_closeCallback = closeCallback;
	_disconnectCallback = disconnectCallback;

}	/* end constructor */

/* destructor */
RPActiveClientTCP::~RPActiveClientTCP() {}


/* initializes static callback functions */
void (*RPActiveClientTCP::_connectCallback)(void *) = 0;
void (*RPActiveClientTCP::_sendMsgCallback)(void *) = 0;
void (*RPActiveClientTCP::_recvMsgCallback)(void *) = 0;
void (*RPActiveClientTCP::_closeCallback)(void *) = 0;
void (*RPActiveClientTCP::_disconnectCallback)(void *) = 0;


/* activates the connection, this calls setup, handle and finish connection methods */
int RPActiveClientTCP::activateConnection (RPStatus *stat) {
	/* setups active client */
	setupConnection( stat );
	if ( RPStatus::sSuccessful != stat->getStatus() ) {
		stat->setStatus( RPStatus::sConnectionActivationThrdFailed );
		__print( "ERROR: setup active client failed\n" );
		return 1;	/* returns unsuccessfully */
	} /* end if */

	/* handles active client */
	handleConnection( stat );
	if ( RPStatus::sSuccessful != stat->getStatus() ) {
		stat->setStatus( RPStatus::sConnectionActivationThrdFailed );
		__print( "ERROR: handle active client failed\n" );
		return 1;	/* returns unsuccessfully */
	} /* end if */

	/* finishs active client */
	finishConnection( stat );
	if ( RPStatus::sSuccessful != stat->getStatus() ) {
		stat->setStatus( RPStatus::sConnectionActivationThrdFailed );
		__print( "ERROR: finish active client failed\n" );
		return 1;	/* returns unsuccessfully */
	} /* end if */

}  /* end func <activateConnection> */



/* initializes the client connection */
int RPActiveClientTCP::setupConnection(RPStatus *stat) {

	if (_connectCallback)
		_connectCallback( (void *)this );		/* calls connect callback */
	stat->setStatus( RPStatus::sSuccessful );
	return 0;	/* returns successfully */

}	/* end func <setupConnection> */



/* runs the main function of the client connection */
int RPActiveClientTCP::handleConnection(RPStatus *stat) {

    int dataSize;                    /* Size of received message */
	char data[RCVBUFSIZE];
	for (;;) {		/* loop forever */
		recvData( data, &dataSize, stat );		/* recieves data from peer connection */
		if ( RPStatus::sSuccessful == stat->getStatus() ) {		/* checks the state of recieving function */
			/* handle recved data */
			if (_recvMsgCallback)
				_recvMsgCallback( (void *)this );		/* calls recv callback */
/*
			sendData( data, dataSize, stat );
			if ( RPStatus::sSuccessful == stat->getStatus() ) {
				if (_sendMsgCallback)
					_sendMsgCallback( (void *)this );
			}
*/
		}  /* end if */
		else if ( RPStatus::sClientDisconnect == stat->getStatus() ) {
			__print( "INFO: connection disconnected\n" );
			/* handle desconnection */
			if (_disconnectCallback)
				_disconnectCallback( (void *)this );
			break;
		}  /* end else if */

		else if ( RPStatus::sClientRecvFailed == stat->getStatus() ) {
			__print( "INF: recv failed\n" );
			if (_disconnectCallback)
				_disconnectCallback( (void *)this );
			break;
		}  /* end else if */

	}	/* end loop */

	stat->setStatus( RPStatus::sSuccessful );
	return 0;	/* returns successfully */

}	/* end func <handleConnection> */



/* finishes the client connection */
int RPActiveClientTCP::finishConnection(RPStatus *stat) {

    closesocket( _clientSock );    /* ends client socket */
	if (_closeCallback)
		_closeCallback( (void *)this );		/* calls close callback */
	stat->setStatus( RPStatus::sSuccessful );
	return 0;	/* returns successfully */

} /* end func <finishConnection> */




/* recieves data from peer connection */
int RPActiveClientTCP::recvData (char *recvedData, int *recvedSize, RPStatus *stat) {
/*
	char buf[RCVBUFSIZE];
	char *bptr = buf;
	int n;
	int buflen = RCVBUFSIZE;
	while ( (n = recv( _clientSock, bptr, buflen - 1, 0 ) ) > 0 ) {
		bptr += n;
		buflen -= n;
	}
	*(bptr + 1) = '\0';
	recvedData = bptr;
	*recvedSize = RCVBUFSIZE - buflen;
*/
	char buf[RCVBUFSIZE];
	int recvRes;
	/*  recieves data and check the status */
	if ( (recvRes = recv(_clientSock, recvedData, RCVBUFSIZE, 0)) > 0 ) {
		//recvedData = buf;
		recvedData[recvRes] = '\0';
		*recvedSize = recvRes;
		stat->setStatus( RPStatus::sSuccessful );
		return 0;
	}
	else if (recvRes == 0)	/* client disconnected */
		stat->setStatus( RPStatus::sClientDisconnect );
	else		/* client recieved failed */
		stat->setStatus( RPStatus::sClientRecvFailed );

	return 1;	/* returns unsuccessfully */

}  /* end func <recvData> */




/* sends data to peer connection */
int RPActiveClientTCP::sendData (const char *data, int size, RPStatus *stat) {

	/* sends data and checks the status */
	if ( send( _clientSock, data, size, 0 ) != size ) 
		__print( "ERROR: sending\n" );
	if (_sendMsgCallback)
		_sendMsgCallback( (void *)this );		/* calls send callback */
	return 0;

}  /* end func <sendData> */
