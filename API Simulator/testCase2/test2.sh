#chmod +x test2.sh
#./test2.sh
g++ interrupts.cpp -o simulator
./simulator trace.txt external_files.txt vector_table1.txt output.txt
