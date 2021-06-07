//
// Created by BoringWednesday on 2021/6/7.
//

/*
 * RTMP Handshake
 *
 * ------------------- Client -------------------
 * Send C0 + C1
 *  C0: 0x03(RTMP) 0x08(RTMPE)
 *  C1: timestamp       4 bytes
 *      zero            4 bytes
 *      random bytes    1528 bytes
 * Receive S0 + S1
 *  S0: 0x03(RTMP) 0x08(RTMPE)
 *  S1: timestamp       4 bytes
 *      zero            4 bytes
 *      random bytes    1528 bytes
 *
 * Send C2
 *  C2: C2 is a copy of S1    1536 bytes
 *
 * Receive S2
 *  C2: C2 is a copy of S1    1536 bytes
 *
 * ------------------- Server -------------------
 * Server Handshake is the opposite of a client
 *
 */
