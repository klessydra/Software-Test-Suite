#include "offline_script_hybrid_opt.hpp"
fixed_point computed_arm_ucb[KA];   // UCB computed for every configuration

int main()
{
  sync_barrier_reset();               // Thread initialization
  sync_barrier_thread_registration();
  __asm__("csrw 0x300, 0x8;");       // each thread enables it's own interrupt

 /*#if OP_C==0
    start_count();
  #endif                        // Initialization: load model parameters previously saved in values.cpp*/
  update_hr_opt(chosen_action,reward);       // Update model parameters starting from chosen action and obtained reward

/* #if OP_C==0
    int cycle=finish_count();
 #endif
  
  #if OP_C==0
    if (Klessydra_get_coreID()==0)
    {
     #if ACCL==1
      printf("ACCL: %d\n", cycle);
     #else
      printf("Not-ACCL: %d\n", cycle);
     #endif
    }
  #endif*/
  sync_barrier();                     // Thread synchronization
  return 0;
}
