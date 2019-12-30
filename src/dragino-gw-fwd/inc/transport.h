/*
 * transport.h
 * 
 *  Created on: Feb 10, 2017
 *      Author: Jac Kersing
 */

#ifndef _TRANSPORT_H
#define _TRANSPORT_H
enum server_type {
    semtech,
    ttn_gw_bridge,
    mqtt,
    gwtraf
};

struct _queue {
    struct _queue *next;
    int nbpkt;
    char *status;		// pointer to semtech status report
    struct lgw_pkt_rx_s data[NB_PKT_MAX];
};

typedef struct _queue Queue;

/* struct for thread arg */
struct _thrdarg {
    int idx;
    Queue  *queue;
};
typedef struct _thrdarg Thrdarg;  

struct _server {
    enum server_type type;	// type of server
    bool enabled;		// server enabled
    bool upstream;		// upstream enabled
    bool downstream;		// downstream enabled
    bool statusstream;		// status stream enabled
    char addr[64];		// server address
    char port_up[8];		// uplink port for semtech proto
    char port_down[8];		// downlink port for semtech proto
    int max_stall;		// max number of missed responses
    char gw_id[64];		// gateway ID for TTN
    char gw_key[200];		// gateway key to connect to TTN
    int gw_port;		// gateway port
    bool live;			// Server is life?
    bool connecting;		// Connection setup in progress
    bool critical;		// Transport critical? Should connect at startup?
    int sock_up;		// Semtech up socket
    int sock_down;		// Semtech down socket
    pthread_mutex_t  mx_queue;  // control access to the queue for each server
    sem_t send_sem;		// semaphore for sending data
    pthread_t t_down;		// semtech down thread
    pthread_t t_up;		// upstream thread
    time_t contact;		// time of last contact
    Queue *queue;		// queue of packets uplink data
    TTN *ttn;			// TTN connection object
};
typedef struct _server Server;

void transport_init();
void transport_start();
void transport_stop();
void transport_status();
void transport_data_up(int nb_pkt, struct lgw_pkt_rx_s *rxpkt, bool send_report);
void transport_status_up(uint32_t, uint32_t, uint32_t, uint32_t);
void transport_send_downtraf(char *json, int len);
int init_socket(const char *servaddr, const char *servport, const char *rectimeout, int len); 
#endif				// _TRANSPORT_H
