#ifndef WRITE_LIST
#define WRITE_LIST

#include <stdbool.h>
#include <stddef.h>
#define MAX_CMD_SIZE 47 //incluye el \r\n

enum CMD_ID {
    EHLO,
    USER,
    PASS
};

struct cmd {
    enum CMD_ID cmd_id;
    size_t offset;
    bool is_done;
};

typedef struct queue_cdt * queue_adt;

typedef struct cmd * elem_type;

queue_adt new_queue();

void free_queue(queue_adt q);

int queue(queue_adt q, elem_type elem);

int dequeue(queue_adt q, elem_type *elem);

int is_empty(queue_adt q);

void to_begin(queue_adt q);

int has_next(queue_adt q);

elem_type next(queue_adt q);
elem_type peek(queue_adt q) ;

#endif 