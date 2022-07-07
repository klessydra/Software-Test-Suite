// Linear Hybrid UCB algorithm
// This algorithm loads model parameters previously saved in values.cpp and use it to 
// compute UCB for every possible configuration and choosing the max one.
#include "offline_script_hybrid_opt.hpp"


fixed_point computed_arm_ucb[KA];            // UCB computed for every configuration

int main()
{
  sync_barrier_reset();               // Barrier reset
  sync_barrier_thread_registration(); // Thread initialization
  __asm__("csrw 0x300, 0x8;" );       // Each thread enables it's own interrupt

 #if OP_C==0
    start_count();
  #endif

  offline_mode_hr_opt();                  // Configuration choice
  update_hr_opt(chosen_action,reward);       // Update model parameters starting from chosen action and obtained reward


 #if OP_C==0
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
  #endif
  sync_barrier();                     // Thread synchronization
  return 0;
}