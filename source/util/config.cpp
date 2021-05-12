//
// Created by BoringWednesday on 2021/5/11.
//

#include "config.h"
#include "log.h"
#include <cstdio>

#define CONFIG_TYPE_NONE    0
#define CONFIG_TYPE_GROUP   1
#define CONFIG_TYPE_INT     2
#define CONFIG_TYPE_INT64   3
#define CONFIG_TYPE_FLOAT   4
#define CONFIG_TYPE_STRING  5
#define CONFIG_TYPE_BOOL    6
#define CONFIG_TYPE_ARRAY   7
#define CONFIG_TYPE_LIST    8

struct config_value_t
{
    int                     i_val;
    long long               ll_val;
    double                  f_val;
    char                   *s_val;
    struct config_list_t   *list;
};

struct config_setting_t
{
    char                       *name;
    short                       type;
    config_value_t              value;
    struct config_setting_t    *parent;
    void                       *hook;
};

struct config_list_t
{
    unsigned int        length;
    config_setting_t  **elements;
};

int Config::read_config(const char *path) const
{
    FILE   *stream = fopen(path, "rt");
    char    buffer[1024];

    if (stream == nullptr) {
        LOG(ERROR) << "open config file error.";

        return -1;
    }

    while (fgets(buffer, 1024, stream)) {

    }

    return 0;
}


bool parse_flag(string &conf, bool &value)
{
    if (conf == "on") {
        value = true;

        return true;
    } else if (conf == "off") {
        value = false;

        return true;
    }

    return false;
}

bool parse_num(string &conf, int &value)
{
    stringstream ss;

    // TODO: when parse error? I don't know
    ss << conf;
    ss >> value;

    return true;
}
