#include "server.h"

int main(int argc, char *argv[])
{
    FakeMediaServer server(argv[2]);

    server.run();

    return 0;
}
