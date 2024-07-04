#include "stack.h"
#include <assert.h>

void new_stack(pile_t* stack) {
	stack->nmb_ellemnts=0;

	for (int i = 0; i < STK_SIZE; i++)
	{
		stack->donnes[i] = STK_NEUTRE;
	}
	
}

void push(pile_t* stack, stk_data param) {
	stack->donnes[stack->nmb_ellemnts]= param;
	stack->nmb_ellemnts++;

	assert(stack->nmb_ellemnts >= 0);
}

stk_data pop(pile_t* stack) {
	stack->nmb_ellemnts--;
	stk_data rt = stack->donnes[stack->nmb_ellemnts];
	stack->donnes[stack->nmb_ellemnts] = STK_NEUTRE;

	assert(stack->nmb_ellemnts >= 0);

	return rt;
}
