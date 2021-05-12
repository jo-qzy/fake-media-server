//
// Created by BoringWednesday on 2021/5/9.
//

#include "conf.h"
#include "core.h"
#include <fstream>
#include <string>

using namespace std;

ServerConf *ServerConf::conf_instance = nullptr;

ServerConf *ServerConf::get_server_conf()
{
    if (conf_instance == nullptr) {
        LOG(FATAL) << "please parse conf first";

        throw bad_exception();
    }

    return conf_instance;
}

int ServerConf::parse_conf(const char *conf_path)
{
    if (conf_instance) {
        LOG(ERROR) << "conf is already parsed.";
    }

    if (conf_path == nullptr) {
        LOG(ERROR) << "conf path is nullptr.";

        return -1;
    }

    if (conf_instance == nullptr) {
        conf_instance = new ServerConf(conf_path);
    }

    if (conf_instance->parse_conf_internal() != 0) {
        delete conf_instance;
        conf_instance = nullptr;

        return -1;
    }

    return 0;
}

MainConf *ServerConf::get_main_conf()
{
    ServerConf *srv_conf = get_server_conf();

    // TODO: get conf
    // srv_conf->LookUp()

    return srv_conf->main_conf;
}

ServerConf::ServerConf(const char *conf_path)
    : path(conf_path), main_conf()
{}

int ServerConf::parse_conf_internal()
{
    // TODO: get conf
    if (parse_main_conf() != 0) {
        LOG(ERROR) << "parse main conf failed";

        return -1;
    }

    return 0;
}

int ServerConf::parse_main_conf()
{
    main_conf = new MainConf;

    // TODO: parse main conf
    main_conf->master_mode = false;
    main_conf->worker_num = 1;

    return 0;
}
