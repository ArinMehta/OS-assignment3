#include "types.h"
#include "stat.h"
#include "user.h"

// argc is the number of strings the shell passed
// argv is an array of pointers to those strings

int main (int argc, char *argv[]){
    // First ill handle the error when argc<2
    if (argc<2){
        printf(1, "Usage: pause <seconds>\n");
        exit();
    }
    int seconds = atoi(argv[1]); // Since argv[1] is essentially read as a string so to convert to int
    sleep(seconds*10); // Since sleep takes ticks and not seconds
    exit();
}