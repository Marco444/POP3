// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "metrics.h"
#include <string.h>
#include <stdio.h>
static Metrics_snapshot metrics;

void metrics_init() {
    memset(&metrics, 0, sizeof(metrics));
}

void metrics_register_new_client() {
    metrics.current_connection_count++;
    metrics.total_connection_count++;
    if (metrics.current_connection_count > metrics.max_concurrent_connections)
        metrics.max_concurrent_connections = metrics.current_connection_count;
}

void metrics_register_bytes_transferred(size_t bytes) {
  metrics.total_bytes_transferred += bytes;
}
void metrics_register_client_disconnected() {
    metrics.current_connection_count--;
}

void metrics_register_mails_retrieved() {
    metrics.total_mails_retrieved++;
}

void metrics_register_mails_deleted() {
    metrics.total_mails_deleted++;
}
void print_metric(){
    printf("Current connections: %ld\n", metrics.current_connection_count);
    printf("Max concurrent connections: %ld\n", metrics.max_concurrent_connections);
    printf("Total connections: %ld\n", metrics.total_connection_count);
    printf("Total mails retrieved: %ld\n", metrics.total_mails_retrieved);
    printf("Total mails deleted: %ld\n", metrics.total_mails_deleted);
}
void get_metrics_snapshot(Metrics_snapshot* snapshot) {
    memcpy(snapshot, &metrics, sizeof(Metrics_snapshot));
}

int get_current_connection_count() {
    return metrics.current_connection_count;
}