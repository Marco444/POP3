
#ifndef POP3_HEADER
#define POP3_HEADER

void handleNewPOP3Connection(struct selector_key * key);
void clean_user_data(void *user_data);
#endif
