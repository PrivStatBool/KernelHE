# Change to the script's directory
cd "$(dirname "$0")"

gcc -o testKernelKNNStep1 testKernelKNNStep1.c -ltfhe-spqlios-fma -lm
gcc -o testKernelKNNStep2 testKernelKNNStep2.c -ltfhe-spqlios-fma -lm
gcc -o testKernelKNNStep3 testKernelKNNStep3.c -ltfhe-spqlios-fma -lm

chmod 755 testKernelKNNStep1
chmod 755 testKernelKNNStep2 
chmod 755 testKernelKNNStep3

./testKernelKNNStep1
./testKernelKNNStep2
./testKernelKNNStep3
