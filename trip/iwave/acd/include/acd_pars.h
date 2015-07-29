  " ",
  " ----------------------------------- ACD ------------------------------------",
  " ",
  " Acoustic constant density modeling operators",
  " iwave/acd/include/acd.hh",
  " ",
  " --------------------------- begin acd parameters ---------------------------",
  " ",
  " Mapping Selection:",
    " ",
  "          deriv = 0           order of derivative of forward map to compute",
  "        adjoint = 0           0: forward, 1: adjoint  ",
  " ",
  " ------------------------------------------------------------------------",
  " FD info:",
  " ",
  "          order = 2           spatial half-order",
  "            cfl = 0.75        proportion of max dt/dx",
  "           cmin = 1.0         min permitted velocity (m/ms) - sanity check, m/ms",
  "           cmax = 4.5         max permitted velocity (m/ms) - used in dt comp m/ms",
  " ",
  " ------------------------------------------------------------------------",
  " Source info:",
  " ",
  "         source = <path>      path to input source data file (traces), ",
  "                              SU format, source position(s) indicated by",
  "                              RECEIVER coordinates (gx, gy, gelev) - may",
  "                              prepare input source with correct headers",
  "                              using trace/main/towed_array.x",
  "        sampord = 1           spatial interp order for sampling or adjoint",
  "                              sampling trace data to/from grid - 0 (nr nbr) ",
  "                              or 1 (multilin)",
  " ",
  " ------------------------------------------------------------------------",
  " Trace info:",
  " ",
  "          data  = <path>      data file, SU format - must exist ",
  "                              headers establish acquisition geometry",
  "                              output for forward modeling (including all",
  "                              orders of derivative), input for adjoint",
  " ",
  " ------------------------------------------------------------------------",
  " Model info:",
  " ",
  "            csq = <path>      input vel-squared reference file, ",
  "                              determines simulation spatial grid,",
  "         csq_d1 = <path>      input vel-squared perturbation for ",
  "                              derivative (born) modeling - required",
  "                              for deriv>0, adjoint=0, or deriv>1, ",
  "                              admoint=1, otherwise ignored",
  "         csq_b1 = <path>      output vel-squared perturbation (RTM image)",
  "                              for adjoint modeling, required for deriv=1,",
  "                              adjoint=1, otherwise ignored",
  "         csq_d2 = <path>      input vel-squared perturbation for deriv=2,",
  "                              adjoint=0 or deriv>2 adjoint=1, otherwise ",
  "                              ignored",
  "         csq_b2 = <path>      output vel-squared perturbation for deriv=2,",
  "                              adjoint=1, otherwise ignored",
  " ",
  " ------------------------------------------------------------------------",
  " MPI info:",
  " ",
  "        mpi_np1 = 1           number of subdomains along axis 1",
  "        mpi_np2 = 1           number of subdomains along axis 2",
  "        mpi_np3 = 1           number of subdomains along axis 3",
  "        partask = 1           number of shots to execute in parallel",
  " ",
  " ------------------------------------------------------------------------",
  " Output info:",
  " ",
  " FD ouput - written to coutxxx.txt on rank xxx",
  "       printact = 0           output at every time step",
  "                              0 - none",
  "                              1 - time step index",
  "                              2 - diagnostic messages from main routines",
  "                              > 2 - much more, useful only for serious ",
  "                                 debugging",
  "        dump_pi = 0           dump parallel/dom. decomp info",
  "       dump_lda = 0           dump grid data for allocated arrays",
  "       dump_ldc = 0           dump grid data for computational arrays",
  "       dump_ldr = 0           dump grid data for receive buffers",
  "       dump_lds = 0           dump grid data for send buffers",
  "      dump_term = 0           dump trace header data",
  " ",
  " ---------------------------end acd parameters ------------------------------",
