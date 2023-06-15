
#ifndef PROTOS_WRITE_BUFFER_HELPERS_H
#define PROTOS_WRITE_BUFFER_HELPERS_H

#include "../commands/command_service.h"

int write_in_fd_monitor(struct selector_key *key);
long write_in_buffer_monitor(struct selector_key * key,char * buff,size_t size,size_t offset);
int write_in_fd(struct selector_key *key);
long write_in_buffer(struct selector_key * key,char * buff,size_t size,size_t offset);
bool enters_the_buffer(struct selector_key *key, char * buff);
bool enters_the_buffer_monitor(struct selector_key *key, char * buff);
#endif
