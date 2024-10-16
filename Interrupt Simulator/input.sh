#chmod +x test1.sh
#./test1.sh
gcc interrupts.cpp -o interrupts
./interrupts trace1.txt execution1.txt vector_table.txt
#./interrupts trace1.txt execution1.txt additionalFiles/vector_table.txt
