# Parallel-Matrix-Matrix-Multiplication
Parallel Matrix Matrix Multiplication 

# overview / description
Matrix-matrix multiplication (mmm) is a cumbersome, but widely used, application so it makes sense for it to serve in many benchmarking suites for evaluating system performance. That's why we ramp it up by not just giving you an option with sequential mmm, but parallelized mmm!

# how to make this program work?
[1] make all
[2] ./mmm <mode> [num threads] <size> 
[2.1] ./mmm s 12 .... which is sequential mode with a 12x12 square matrix
[2.2] ./mmm p 2 12 .... which is parallel mode with a 12x12 square matrix with 2 threads.
[3] happiness