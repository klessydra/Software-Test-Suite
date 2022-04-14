#include <stdio.h>
#include "functions.h"

int released[THREAD_POOL_SIZE] = {};
int lock = 0;
int *plock = &lock;

int main()
{

	__asm__("csrw 0x300, 0x8;" );// each thread enables it's own interrupt

	sync_barrier_reset();
	sync_barrier_thread_registration();

	int hart_ID = Klessydra_get_coreID();
	released[hart_ID] = 0;


	if (hart_ID == 0) {
		released[hart_ID] = 0;
		if (THREAD_POOL_SIZE == 1) {
			puts("There is one hart, run the test again with a \"THREAD_POOL_SIZE\" configuration greater than 1");
			goto TEST_FAIL;
		}
		else {
			printf("Releasing harts in the core from WFI in:\n");
			for (int i=10; i>0; i--) {
				printf("%d\n", i);
				if (i == 1) {
					for (char i=1; i<THREAD_POOL_SIZE; i++) {
						send_sw_irq(i); // wake up hart i
					}
				}
			}
		}
	}
	#if THREAD_POOL_SIZE > 1
	else {
		Klessydra_WFI(); // put harts to sleep, they should be woken up by hart 0
		released[hart_ID] = 1;
		int lock_val = klessydra_lock_set(plock);
		if (lock_val == 0) {
			unsigned int sum_released = 0;
			if (THREAD_POOL_SIZE > 1) {
				for (int i=1; i<THREAD_POOL_SIZE; i++) { // hart 0 is the one waking up all the other harts
					sum_released += released[i];
					if (released[i] == 1)
						printf("SUCCESS: hart %d has been released\n", i);
					else 
						printf("FAIL: hart %d did not release successfully\n", i);
				}
			}
			else {
				sum_released = released[0];
			}
			if (sum_released == THREAD_POOL_SIZE-1)
				goto TEST_PASS;
			else
				goto TEST_FAIL;
		}
	}

	sync_barrier();

	TEST_PASS:

	puts("\nTEST_PASS");
	return 0;

	#endif

	TEST_FAIL:

	puts("\nTEST_FAIL");
	return 1;

}
