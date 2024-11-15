#chmod +x test4.sh
#./test4.sh
g++ interrupts.cpp -o simulator
./simulator trace.txt external_files.txt vector_table1.txt output.txt
