set(perfbench.fft_SOURCES fft.c fft_test.c)
add_application(perfbench.fft2 "${perfbench.fft_SOURCES}"
  LABELS "perfbench" LIBS "perfbench.core2")
