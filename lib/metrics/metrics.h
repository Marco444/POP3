#ifndef _METRICS_H_
#define _METRICS_H_

#include <stdlib.h>

typedef struct {
    size_t current_connection_count;
    size_t total_connection_count;   //conections handled through the server lifetime
    size_t max_concurrent_connections; //maximum number of connections handled
    size_t total_mails_retrieved;
    size_t total_mails_deleted; 
    size_t total_bytes_transferred;
} Metrics_snapshot;

void metrics_init();

void metrics_register_new_client();

void metrics_register_client_disconnected();

void metrics_register_mails_retrieved();

void metrics_register_mails_deleted();

void metrics_register_bytes_transferred(size_t byte );

void print_metric();

void get_metrics_snapshot(Metrics_snapshot* snapshot);

#endif
