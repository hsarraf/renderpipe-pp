/* 101101101 */

#include "RPJob.h"
#include "RPActiveClientTCP.h"
#include "RPServerSock.h"
#include "RPServerSockTCP.h"
#include "RPMultiServerSockTCP.h"
#include "RPStatus.h"

#include "tinythread.h"
using namespace tthread;

void connectf (void *args) {
	RPActiveClientTCP *client = (RPActiveClientTCP *)args;
	printf( "\n--\n" );
	printf( "%s is connected\n", client->get_clientIPAddr() );
	printf( "--\n" );
}

void sendf (void *args) {
	RPActiveClientTCP *client = (RPActiveClientTCP *)args;
	printf( "\n--\n" );
	printf( "%s is sent\n", client->get_clientIPAddr() );
	printf( "--\n" );
}

void recvf (void *args) {
	RPActiveClientTCP *client = (RPActiveClientTCP *)args;
	printf( "\n--\n" );
	printf( "%s is recved\n", client->get_clientIPAddr() );
	printf( "--\n" );
}

void closef (void *args) {
	RPActiveClientTCP *client = (RPActiveClientTCP *)args;
	printf( "\n--\n" );
	printf( "%s is closed\n", client->get_clientIPAddr() );
	printf( "--\n" );
}

void disconnectf (void *args) {
	RPActiveClientTCP *client = (RPActiveClientTCP *)args;
	printf( "\n--\n" );
	printf( "%s is disconnected\n", client->get_clientIPAddr() );
	printf( "--\n" );
}

int main() {

//	RPServerSock *server = new( RPMultiServerSockTCP )( 22222, connectf );
	RPServerSock *server = new RPMultiServerSockTCP( 6000, connectf, sendf, recvf, closef, disconnectf );
	server->activateServerTCP();
	
	return 0;

}