#include "client.h"

int main(int argc, char* argv[])
{
	UdpClient *c = new UdpClient();
	c->run();
	return 0;
}
