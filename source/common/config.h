//
// Created by BoringWednesday on 2021/5/11.
//

/*
    The config parser code is referenced to libconfig:
        https://github.com/hyperrealm/libconfig
 */

#ifndef FAKE_MEDIA_SERVER_CONFIG_H
#define FAKE_MEDIA_SERVER_CONFIG_H

struct config_value_t;

struct config_setting_t;

struct config_list_t;

class Setting
{
public:
private:
};

class Config
{
public:
    Config() = default;
    virtual ~Config() = default;

    int read_config(const char *path) const;
};


#endif //FAKE_MEDIA_SERVER_CONFIG_H
