#include "errno.h"

void default_constraint_handler(const char *restrict msg, void *restrict ptr, errno_t error);

extern constraint_handler_t __constraint_handler__;
constraint_handler_t __constraint_handler__ = default_constraint_handler;
int __errno__ = 0;

constraint_handler_t set_constraint_handler_s(constraint_handler_t handler)
{
    constraint_handler_t ret = __constraint_handler__;
    __constraint_handler__ = handler;

    return ret;
}

void default_constraint_handler(const char *restrict msg, void *restrict ptr, errno_t error)
{
    (void)msg;
    (void)ptr;
    (void)error;
}