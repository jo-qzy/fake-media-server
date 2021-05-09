#include "fake_media_server.h"

int main()
{
    FakeMediaServer server("conf/fake.conf");

    server.Run();

    return 0;
}
