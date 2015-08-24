#ifndef PING_UNHASH_H
#define PING_UNHASH_H

#include <stdbool.h>

#define PING_UNHASH_REQUEUE_MAX_REQUEST_COUNT	1024

extern bool ping_unhash_read_value_at_address(unsigned long address, int *value);
extern bool ping_unhash_read_values_at_address(unsigned long address, int *values, int count);
extern bool ping_unhash_write_value_at_address(unsigned long address, int value);
extern bool ping_unhash_write_values_at_address(unsigned long address, const int *values, int count);

extern bool ping_unhash_run_exploit(unsigned long int address, int value,
                                    bool(*exploit_callback)(void* user_data), void *user_data);


#endif /* PING_UNHASH_H */
