set -xe
cd $1
mkdir -p bin
gcc -o bin/$1 main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./bin/$1
