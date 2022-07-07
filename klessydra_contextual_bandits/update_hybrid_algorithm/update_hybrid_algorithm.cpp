#include "offline_script_hybrid.hpp"
Linear_UCB_Hybrid linucb_hybrid_policy_object(KA,D,K,0.2);
matrix<K,K> A_node_inv;
matrix<K,1> beta_hat;
fixed_point computed_arm_ucb[KA];            // UCB computed for every configuration

int main()
{
  sync_barrier_reset();               // Thread initialization
  sync_barrier_thread_registration();
  __asm__("csrw 0x300, 0x8;");       // each thread enables it's own interrupt

 /*#if OP_C==0
    start_count();
  #endif                        // Initialization: load model parameters previously saved in values.cpp*/
  if(Klessydra_get_coreID()==0)
  {
    update_hr(chosen_action,reward);       // Update model parameters starting from chosen action and obtained reward
    return 0;
  }
  else
   __asm__ (" wfi ;");
  //sync_barrier();                     // Thread synchronization
  
}
