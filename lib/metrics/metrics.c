// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "metrics.h"
#include <string.h>
#include <stdio.h>
static MetricsSnapshot metrics;

void metricsInit() {
    memset(&metrics, 0, sizeof(metrics));
}

void metricsRegisterNewClient() {
    metrics.currentConnectionCount++;
    metrics.totalConnectionCount++;
    if (metrics.currentConnectionCount > metrics.maxConcurrentConnections)
        metrics.maxConcurrentConnections = metrics.currentConnectionCount;
}

void metricsRegisterClientDisconnected() {
    metrics.currentConnectionCount--;
}

void metricsRegisterMailsRetrieved() {
    metrics.totalMailsRetrieved++;
}

void metricsRegisterMailsDeleted() {
    metrics.totalMailsDeleted++;
}
void print_metric(){
    printf("Current connections: %ld\n", metrics.currentConnectionCount);
    printf("Max concurrent connections: %ld\n", metrics.maxConcurrentConnections);
    printf("Total connections: %ld\n", metrics.totalConnectionCount);
    printf("Total mails retrieved: %ld\n", metrics.totalMailsRetrieved);
    printf("Total mails deleted: %ld\n", metrics.totalMailsDeleted);
}
void getMetricsSnapshot(MetricsSnapshot* snapshot) {
    memcpy(snapshot, &metrics, sizeof(MetricsSnapshot));
}
