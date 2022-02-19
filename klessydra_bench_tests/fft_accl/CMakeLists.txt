set(perfbench.fft_SOURCES main.c)
add_application(perfbench.fft_accl "${perfbench.fft_SOURCES}"
  LABELS "perfbench" LIBS "perfbench.core")
