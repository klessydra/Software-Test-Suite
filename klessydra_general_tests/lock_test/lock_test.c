#include<stdio.h>
#include"functions.h"

int *plock_1;	
int *plock_2;	
int lock_1 = 0;		
int lock_2 = 0;		

int lock_aq_count = 0;
int lock_rl_count = 0;

int main()
{
	sync_barrier_reset();
	sync_barrier_thread_registration();

	plock_1 = &lock_1;

	int hartID = Klessydra_get_coreID();

	klessydra_lock_acquire(plock_1);
	// lock region below

	lock_aq_count++; // this count shows many harts are in the lock region, and it should be 1

	printf("Lock Acquired by hart %d, hart count in lock region = %d\n", hartID, lock_aq_count); // print the values after the lock acquire
	if (lock_aq_count != 1) { // 1 hart in the lock region at a time
		goto TEST_FAIL;
	}

	lock_aq_count = 0;
	klessydra_lock_release(plock_1);

	lock_rl_count++;

	sync_barrier();
	sync_barrier_thread_registration();

	if (Klessydra_get_coreID() == 0) {
		printf("\nlock_rl_count = %d, THREAD_POOL_SIZE = %d\n", lock_rl_count, THREAD_POOL_SIZE);
		if (lock_rl_count == THREAD_POOL_SIZE) {
			puts("\nSuccess: region locks acquired, then released successfuly");
			goto TEST_PASS;
		}
		else{
			puts("\nError: Not all harts were released successfuly from the lock region");
			goto TEST_FAIL;
		}
	}
	sync_barrier();

	TEST_PASS:

	puts("\nTEST_PASS");
	return 0;

	TEST_FAIL:

	puts("\nTEST_FAIL");
	return 1;

}
