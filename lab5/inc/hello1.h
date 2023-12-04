// Check if the macro HELLO1_H is not defined. If it's not defined, the header file has not been included yet.
#ifndef HELLO1_H

// If the macro was not defined, defines the macro HELLO1_H
#define HELLO1_H

int print_hello(unsigned int count);
void exit_hello(void);

#endif

