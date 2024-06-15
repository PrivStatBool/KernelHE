cd "$(dirname "$0")"

gcc -o testPlainKernelKmeans testPlainKernelKmeans.c -lm

chmod 755 testPlainKernelKmeans

./testPlainKernelKmeans



