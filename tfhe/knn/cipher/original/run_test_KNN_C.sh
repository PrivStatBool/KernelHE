# Change to the script's directory
cd "$(dirname "$0")"

gcc -o testKNNStep1 testKNNStep1.c -ltfhe-spqlios-fma -lm
gcc -o testKNNStep2 testKNNStep2.c -ltfhe-spqlios-fma -lm
gcc -o testKNNStep3 testKNNStep3.c -ltfhe-spqlios-fma -lm

chmod 755 testKNNStep1
chmod 755 testKNNStep2 
chmod 755 testKNNStep3

./testKNNStep1
./testKNNStep2
./testKNNStep3
