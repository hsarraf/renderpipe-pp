/*
RPStatus.h header file contains the class, RPStatus,
to check the status of steps of procedures,
including server-client connections, job activities, ...
*/


#ifndef    __RPSTATUS_HEADER__
#define    __RPSTATUS_HEADER__


#include <string.h>
#include <errno.h>

extern int      errno;


/* status class */
class RPStatus {
public:
	/* status enumeration */
	enum State {
		sSuccessful = 0,					/* successful */
		sSockStartupFailed,					/* socket start up failed */
		sSockFDFailed,						/* socket file descriptor failed */
		sSockBindFailed,					/* socket bind failed */
		sSockListenFailed,					/* socket listen failed */
		sSockAcceptFailed,					/* socket accept failed */
		sClientRecvFailed,					/* client recieve failed */
		sServerSendFailed,					/* server send failed */
		sClientActivationFailed,			/* client activation failed */
		sConnectionActivationThrdFailed,	/* connection activation failed */
		sSetupConnectionFailed,				/* setup connection failed */
		sHandleConnectionFailed,			/* handle connection failed */
		sFinishConnectionFailed,			/* finish connection failed */
		sClientDisconnect,					/* client disconnected */
		sFailurewithoutReturn,				/* failure state without return */
		sFailureWithReturn,					/* failure state with return */
		sFailureWithExit					/* failure state with exit */
	};		/* end enum <RPStatus> */

	RPStatus() {init();};
	virtual ~RPStatus() {};

	bool hasError() {
		if (errno)		return true;
		else		    return false;
	}
	inline int getErrNo() {return errno;}
	inline char *getErrStr() {return strerror( errno );}
	inline State getStatus() {return _state;}
	inline void setStatus (State s) {_state = s;}
	inline char *getStatStr() {return _statStr;}
	inline void setStatStr (char *s) {_statStr = s;}
	inline RPStatus *init() {errno = 0 ; _state = sSuccessful ; _statStr = 0; return this;};

private:
	State _state;
	char *_statStr;

};	/* end class <RPStatus> */



#endif    /*__RPSTATUS_HEADER__*/