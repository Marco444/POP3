
#include "metrics.h"
#include <string.h>

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

void getMetricsSnapshot(MetricsSnapshot* snapshot) {
    memcpy(snapshot, &metrics, sizeof(MetricsSnapshot));
}
