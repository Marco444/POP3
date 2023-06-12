
#include "write_buffer_helpers.h"
long write_in_buffer(struct selector_key * key,char * buff,size_t size,size_t offset){
    for (size_t i = offset;i < size; i++)
        {
            if(buffer_can_write(&((struct connection_state *)key->data)->commands.write_buffer))
                buffer_write(&((struct connection_state *)key->data)->commands.write_buffer, buff[i]);
            else
                return i;
        }
    return -1;
}
bool enters_the_buffer(struct selector_key *key, char * buff){
    size_t size;
    size_t capacity;
    buffer_write_ptr(&((struct connection_state *)key->data)->commands.write_buffer, &capacity);
    size = strlen(buff);

    return capacity >= size;
}

