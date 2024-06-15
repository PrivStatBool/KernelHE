# Navigate to the directory where the script is located
cd "$(dirname "$0")"

gcc -o testKmeans_newStep1 testKmeans_newStep1.c -ltfhe-spqlios-fma -lm
gcc -o testKmeans_newStep2 testKmeans_newStep2.c -ltfhe-spqlios-fma -lm
gcc -o testKmeans_newStep3 testKmeans_newStep3.c -ltfhe-spqlios-fma -lm

chmod 755 testKmeans_newStep1
chmod 755 testKmeans_newStep2 
chmod 755 testKmeans_newStep3

./testKmeans_newStep1
./testKmeans_newStep2
./testKmeans_newStep3
