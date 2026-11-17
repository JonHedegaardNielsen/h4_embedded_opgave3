#ifndef PROJECTDEFINES_H
#define PROJECTDEFINES_H

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

#endif
