#ifndef DEBLED_H
#define DEBLED_H

#define DEBLED1	(1<<4)
#define DEBLED2	(1<<5)
#define DEBLED3	(1<<6)
#define DEBLED4	(1<<7)

void debled_init(void);
void debled_set(const int target, const int state);

#endif

