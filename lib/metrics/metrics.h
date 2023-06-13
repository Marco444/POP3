#ifndef _METRICS_H_
#define _METRICS_H_

#include <stdlib.h>

typedef struct {
    size_t currentConnectionCount;
    size_t totalConnectionCount;   //conections handled through the server lifetime
    size_t maxConcurrentConnections; //maximum number of connections handled
    size_t totalMailsRetrieved;
    size_t totalMailsDeleted; 
} MetricsSnapshot;

void metricsInit();

void metricsRegisterNewClient();

void metricsRegisterClientDisconnected();

void metricsRegisterMailsRetrieved();

void metricsRegisterMailsDeleted();

void getMetricsSnapshot(MetricsSnapshot* snapshot);

#endif
