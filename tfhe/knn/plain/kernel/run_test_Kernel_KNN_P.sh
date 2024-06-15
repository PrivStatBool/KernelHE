# Change to the script's directory
cd "$(dirname "$0")"

gcc -o testPlainKernelKNN testPlainKernelKNN.c -lm

chmod 755 testPlainKernelKNN

./testPlainKernelKNN
