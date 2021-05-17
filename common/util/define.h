//
// Created by BoringWednesday on 2021/5/17.
//

#ifndef FAKE_MEDIA_SERVER_DEFINE_H
#define FAKE_MEDIA_SERVER_DEFINE_H


enum Status {
    FMS_CLOSE       = -2,
    FMS_ERR         = -1,
    FMS_OK          =  0,
    FMS_AGAIN       =  1,
    FMS_MORE_DATA   =  2
};

enum ConnectStatus {
    FMS_DISCONNECTED = 1,
    FMS_CONNECTING   = 0,
    FMS_CONNECTED    = 1
};


#endif //FAKE_MEDIA_SERVER_DEFINE_H
