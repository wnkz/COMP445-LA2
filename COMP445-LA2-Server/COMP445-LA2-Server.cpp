#include "server.h"

int main(void)
{
	UdpServer *ser = new UdpServer();
	ser->run();
	return 0;
}
