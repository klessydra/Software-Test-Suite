// Linear Hybrid UCB algorithm
// This algorithm loads model parameters previously saved in values.cpp and use it to 
// compute UCB for every possible configuration and choosing the max one.

#include "offline_script_hybrid.hpp"
Linear_UCB_Hybrid linucb_hybrid_policy_object(KA,D,K,0.2);
matrix<K,K> A_node_inv;
matrix<K,1> beta_hat;
fixed_point computed_arm_ucb[KA];            // UCB computed for every configuration

int main()
{
  sync_barrier_reset();               // Barrier reset
  sync_barrier_thread_registration(); // Thread initialization
  __asm__("csrw 0x300, 0x8;" );       // Each thread enables it's own interrupt

 /*#if OP_C==0
    start_count();
  #endif*/
  
  #if SINGLE_THREAD==1
  if (Klessydra_get_coreID()==0)
  {
  #endif
  
    offline_mode_hr();                  // Configuration choice
    //update_hr(chosen_action,reward);       // Update model parameters starting from chosen action and obtained reward
    
    /*#if OP_C==0
    int cycle= finish_count(); 
      
   /* if (Klessydra_get_coreID()==0)
    {
     #if ACCL==1
        printf("ACCL: %d\n", cycle);
     #else
        printf("Not-ACCL: %d\n", cycle);
     #endif
    }
    #endif*/
    return 0;
  #if SINGLE_THREAD==1
  }
  else
    __asm__("wfi;");
  #endif
  sync_barrier();
  //sync_barrier();                     // Thread synchronization
  
}