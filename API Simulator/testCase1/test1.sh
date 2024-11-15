#chmod +x test1.sh
#./test1.sh
g++ interrupts.cpp -o simulator
./simulator trace.txt external_files.txt vector_table1.txt output.txt
