//
// Created by BoringWednesday on 2021/5/9.
//

#include <common/log.h>

void test_log()
{
    LOG(INFO) << "test1";
    LOG(INFO) << "test2";

    LOG(INFO) << "test3";

}

int main()
{
    test_log();

    return 0;
}
