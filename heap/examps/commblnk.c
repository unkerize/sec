
uint8_t comm_blink_counter = 0;
#define comm_blink_const 6

/*
void comm_blink_handler(void)
{
	if (comm_blink_counter == 0) return;
	
	if (--comm_blink_counter == 0) pin_clr(led2Port, led2Pin);
}

*/

/*
void comm_blink_start(void)
{
	pin_set(led2Port, led2Pin);
	
	comm_blink_counter = comm_blink_const;
}
*/
