//
// Created by BoringWednesday on 2021/5/9.
//

#ifndef FAKE_MEDIA_SERVER_CONF_H
#define FAKE_MEDIA_SERVER_CONF_H


struct MainConf {
    bool    master_mode;
    int     worker_num;
};

class ServerConf {
public:
    static ServerConf *get_server_conf();
    static int parse_conf(const char *conf_path);

    static MainConf *get_main_conf();

private:
    explicit ServerConf(const char *conf_path);

    int parse_conf_internal();

    int parse_main_conf();

private:
    const char         *path;
    MainConf           *main_conf;
    static ServerConf  *conf_instance;
};


#endif //FAKE_MEDIA_SERVER_CONF_H
