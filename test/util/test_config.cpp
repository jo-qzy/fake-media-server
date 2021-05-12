//
// Created by BoringWednesday on 2021/5/11.
//

#include <util/config.h>

void test_config()
{
    Config config;

    config.read_config("../conf/fake.conf");
}

int main()
{
    return 0;
}
