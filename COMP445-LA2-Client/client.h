#pragma once
#pragma comment(lib,"wsock32.lib")
#include <winsock.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <time.h>
#include "client.h"

#define STIMER 0
#define UTIMER 300000

#define CLIENT_PORT 5000
#define REMOTE_PORT 7000

#define TRACE 1

#define MAX_RETRIES 10

#define INPUT_LENGTH    40
#define HOSTNAME_LENGTH 40
#define USERNAME_LENGTH 40
#define FILENAME_LENGTH 40
#define MAX_FRAME_SIZE 256
#define MAX_RANDOM 256;
#define SEQUENCE_WIDTH 1

typedef enum { GET=1, PUT } Direction;
typedef enum { TIMEOUT=1, INCOMING_PACKET, RECEIVE_ERROR} ReceiveResult;
typedef enum { CLIENT_REQ=1, ACK_CLIENT_NUM, ACK_SERVER_NUM, FILE_NOT_EXIST, INVALID } HandshakeType;
typedef enum { INITIAL_DATA=1, DATA, FINAL_DATA } MessageFrameHeader;
typedef enum { HANDSHAKE=1, FRAME, FRAME_ACK } PacketType;

typedef struct { 
	PacketType packet_type;
	int number;
} Acknowledgment;

typedef struct {
	PacketType packet_type;
	MessageFrameHeader header;
	unsigned int snwseq:SEQUENCE_WIDTH;
	int buffer_length;
	char buffer[MAX_FRAME_SIZE];
} MessageFrame;

typedef struct {
	PacketType packet_type;
	HandshakeType type;
	Direction direction;
	int client_number;
	int server_number;
	char hostname[HOSTNAME_LENGTH];
	char username[USERNAME_LENGTH];
	char filename[FILENAME_LENGTH];
} ThreeWayHandshake;

class UdpClient
{
    int sock;
	struct sockaddr_in sa;
	struct sockaddr_in sa_in;
	int sa_in_size;
	struct timeval timeouts;
	ThreeWayHandshake handshake;
	int random;
	WSADATA wsadata;

private:
	std::ofstream fout;

public:
	UdpClient(char *fn="client_log.txt");
	~UdpClient();
    void run();	

	bool SendFile(int, char *, char *, int);
	int SendRequest(int, ThreeWayHandshake *, struct sockaddr_in *);
	int SendFrame(int, MessageFrame *);
	int SendFileAck(int, Acknowledgment *);

	bool ReceiveFile(int, char *, char *, int);
	ReceiveResult ReceiveResponse(int, ThreeWayHandshake *);
	ReceiveResult ReceiveFrame(int, MessageFrame *);
	ReceiveResult ReceiveFileAck(int, Acknowledgment *);

	unsigned long ResolveName(char name[]);
    void err_sys(char * fmt,...);
};
