#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student_management.h"

int main(void)
{
	struct student *studentList = NULL;
	int choice = 0;
	int scanfResult_option = 0;

	while (choice != 11) {
		printf("Student Management System\n");
		printf("1. Add Student\n");
		printf("2. Remove Student\n");
		printf("3. Print All Students\n");
		printf("4. Add Course\n");
		printf("5. Remove Course\n");
		printf("6. Print Courses for Student\n");
		printf("7. Print Everything\n");
		printf("8. Sort by Name\n");
		printf("9. Sort by ID\n");
		printf("10. Save to File\n");
		printf("11. Exit\n");
		printf("Enter your choice: \n");

		scanfResult_option = scanf("%d", &choice);

		if (scanfResult_option != 1) {
			while (getchar() != '\n');
			printf("Invalid input. Please enter a valid choice.\n");
			continue;  
		}

		handleChoice(choice, &studentList);

		printf("\n");
	}

	freeList(studentList);
	return 0;
}

