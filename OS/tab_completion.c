#include "types.h"
#include "defs.h"
#include "tab_completion.h"
#include "fs.h"
#include "file.h" 

// consputc is now declared in defs.h

void tab_completion(char *partial_cmd, int cmd_len, char *buf, uint *e_ptr, uint w)
{
    static char command_names[25][DIRSIZ]; // Static storage for command names
    char *matches[25];  // Pointers to matching commands
    int match_count = 0;
    int i, j;
    struct inode *dir_ptr;
    struct dirent dir_entry;
    int offset;
    
    // Handle the case when cmd_len is 0 (empty command)
    if (cmd_len <= 0) {
        return;
    }
    
    // Open root directory to find executable files
    if((dir_ptr = namei("/")) == 0) {
        return; // Failed to open root directory
    }
    
    ilock(dir_ptr);
    
    // Read directory entries
    for(offset = 0; offset < dir_ptr->size && match_count < 25; offset += sizeof(dir_entry)) {
        if(readi(dir_ptr, (char*)&dir_entry, offset, sizeof(dir_entry)) != sizeof(dir_entry)) {
            break;
        }
        
        if(dir_entry.inum == 0) {
            continue; // Skip empty entries
        }
        
        // Skip "." and ".." entries
        if(dir_entry.name[0] == '.' && (dir_entry.name[1] == 0 || 
           (dir_entry.name[1] == '.' && dir_entry.name[2] == 0))) {
            continue;
        }
        
        // Check if this entry matches our partial command
        if(strncmp(partial_cmd, dir_entry.name, cmd_len) == 0) {
            // Copy the command name to our static storage
            for(i = 0; i < DIRSIZ && dir_entry.name[i] != 0; i++) {
                command_names[match_count][i] = dir_entry.name[i];
            }
            command_names[match_count][i] = 0; // Null terminate
            
            matches[match_count] = command_names[match_count];
            match_count++;
        }
    }
    
    iunlockput(dir_ptr);
    
    // Handle matches
    if (match_count == 0) {
        // No matches, do nothing
        return;
    } else if (match_count == 1) {
        // Complete with the only match
        for (i = cmd_len; matches[0][i]; i++) {
            buf[(*e_ptr)++ % INPUT_BUF] = matches[0][i];
            consputc(matches[0][i]);
        }
    } else {
        // Multiple matches, print options
        consputc('\n');
        for (i = 0; i < match_count; i++) {
            for (j = 0; matches[i][j]; j++) {
                consputc(matches[i][j]);
            }
            consputc(' ');
        }
        consputc('\n');
        
        // Re-print prompt and current input
        consputc('$');
        consputc(' ');
        for (i = w; i < *e_ptr; i++) {
            consputc(buf[i % INPUT_BUF]);
        }
    }
}