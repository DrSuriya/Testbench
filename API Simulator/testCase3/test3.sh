#chmod +x test3.sh
#./test3.sh
g++ interrupts.cpp -o simulator
./simulator trace.txt external_files.txt vector_table1.txt output.txt
