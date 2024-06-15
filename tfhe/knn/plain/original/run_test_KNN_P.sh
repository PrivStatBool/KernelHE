# Change to the script's directory
cd "$(dirname "$0")"

gcc -o testPlainKNN testPlainKNN.c -lm

chmod 755 testPlainKNN

./testPlainKNN
