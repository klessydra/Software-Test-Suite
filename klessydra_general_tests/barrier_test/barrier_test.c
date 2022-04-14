#include <stdio.h>
#include "functions.h"

int arr_hart_ID[THREAD_POOL_SIZE] = {};

int main(){
	
	__asm__("csrw 0x300, 0x8;" );// each thread enables it's own interrupt

	sync_barrier_reset();
	sync_barrier_thread_registration();

	if (Klessydra_get_coreID() == 0) {
		puts("Barrier Test\n");
	}

	int hart_ID = Klessydra_get_coreID();
	arr_hart_ID[hart_ID] = 1;
	if (Klessydra_get_coreID() == 0) {
		for (int i=0;i<THREAD_POOL_SIZE;i++){
			if (arr_hart_ID[i] == 1) {
				// Register Threads to be synchronized
				printf("Registered Thread %d\n", i);
			}
		}
	}
	// Register Threads
	if (Klessydra_get_coreID() == 0) {
		puts("\nSynchronizing\n");
	}
	// Synchronise Threads
	sync_barrier();
	if (Klessydra_get_coreID() == 0) {
		puts("Sync Success !!\n");
		puts("TEST PASS\n");
	}
	// These lasts syncs are in order to wait for hart 0 to print the msg above before letting the other harts return from the main
	sync_barrier_thread_registration();
	sync_barrier(); 
	return 0;

}
