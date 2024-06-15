# Navigate to the directory where the script is located
cd "$(dirname "$0")"

gcc -o testKernelKmeansStep1 testKernelKmeansStep1.c -ltfhe-spqlios-fma -lm
gcc -o testKernelKmeansStep2 testKernelKmeansStep2.c -ltfhe-spqlios-fma -lm
gcc -o testKernelKmeansStep3 testKernelKmeansStep3.c -ltfhe-spqlios-fma -lm

chmod 755 testKernelKmeansStep1
chmod 755 testKernelKmeansStep2 
chmod 755 testKernelKmeansStep3

./testKernelKmeansStep1
./testKernelKmeansStep2
./testKernelKmeansStep3
