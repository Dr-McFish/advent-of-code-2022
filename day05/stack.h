#ifndef STAKCS_H
#define STAKCS_H

#define STK_SIZE 100
#define STK_NEUTRE ' '

typedef char stk_data;

typedef struct stack_s {
	stk_data donnes[STK_SIZE];
	int nmb_ellemnts;
} pile_t;

void new_stack(pile_t* stack);
stk_data pop(pile_t* stack);
void push(pile_t* stack, stk_data param);


#endif