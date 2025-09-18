#ifndef TAB_COMPLETION_H
#define TAB_COMPLETION_H

#include "types.h"

#define INPUT_BUF 512  // Make sure it's the same as in console.c

// Function declaration - use just the buffer rather than the whole structure
void tab_completion(char *partial_cmd, int cmd_len, 
                   char *buf, uint *e_ptr, uint w);

#endif // TAB_COMPLETION_H