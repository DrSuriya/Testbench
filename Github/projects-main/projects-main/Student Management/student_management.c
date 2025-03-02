#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "student_management.h"

void freeCourse(struct grades *course)
{
	free(course->courseName);
	free(course);
}

void freeStudent(struct student *student)
{
	free(student->name);

	struct grades *currentCourse = student->courses;
	while (currentCourse != NULL) {
		struct grades *nextCourse = currentCourse->nextCourse;
		freeCourse(currentCourse);
		currentCourse = nextCourse;
	}

	free(student);
}

void freeList(struct student *list)
{
	struct student *currentStudent = list;
	while (currentStudent != NULL) {
		struct student *nextStudent = currentStudent->next;
		freeStudent(currentStudent);
		currentStudent = nextStudent;
	}
}

struct student *searchStudent(struct student *list, int studentID)
{
	struct student *currentStudent = list;
	while (currentStudent != NULL) {
		if (currentStudent->id == studentID) {
			return currentStudent;
		}
		currentStudent = currentStudent->next;
	}
	return NULL;
}

struct student *updateCGPA(struct student *list, int studentID)
{
	struct student *currentStudent = list;
	while (currentStudent != NULL) {
		if (currentStudent->id == studentID) {
			break;
		}
		currentStudent = currentStudent->next;
	}

	if (currentStudent == NULL) {
		printf("Student with ID %d not found.\n", studentID);
		return list;
	}

	struct grades *studentCourse = currentStudent->courses;
	float studentGPA = 0.0;
	float totalCredits = 0.0;

	while (studentCourse != NULL) {
		totalCredits += studentCourse->courseCredit;

		if (studentCourse->courseGrade >= 80 && studentCourse->courseGrade <= 100) {
			studentGPA += 12.0 * studentCourse->courseCredit;
		} else if (studentCourse->courseGrade >= 70 && studentCourse->courseGrade < 80) {
			studentGPA += 3.0 * studentCourse->courseCredit;
		} else if (studentCourse->courseGrade >= 60 && studentCourse->courseGrade < 70) {
			studentGPA += 2.0 * studentCourse->courseCredit;
		} else if (studentCourse->courseGrade >= 50 && studentCourse->courseGrade < 60) {
			studentGPA += 1.0 * studentCourse->courseCredit;
		} else {
			studentGPA += 0.0;
		}

		studentCourse = studentCourse->nextCourse;
	}

	if (totalCredits != 0.0) {
		currentStudent->gpa = studentGPA / totalCredits;
	} else {
		currentStudent->gpa = 0.0;
	}

	return list;
}

struct student *insertCourse(struct student *list, int studentID, char *name, int grade, float credit)
{
	struct student *currentStudent = searchStudent(list, studentID);
	if (currentStudent == NULL) {
		return list;
	}

	struct grades *newCourse = malloc(sizeof(struct grades));
	newCourse->courseName = malloc(strlen(name) + 1);
	strcpy(newCourse->courseName, name);
	newCourse->courseGrade = grade;
	newCourse->courseCredit = credit;
	newCourse->nextCourse = NULL;

	if (currentStudent->courses == NULL) {
		currentStudent->courses = newCourse;
	} else {
		struct grades *tail = currentStudent->courses;
		while (tail->nextCourse != NULL) {
			tail = tail->nextCourse;
		}
		tail->nextCourse = newCourse;
	}

	list = updateCGPA(list, studentID);
	return list;
}

void deleteCourse(struct student *list, int studentID, char *courseName)
{
	struct student *currentStudent = searchStudent(list, studentID);
	if (currentStudent == NULL) {
		return;
	}

	struct grades *prevCourse = NULL;
	struct grades *currentCourse = currentStudent->courses;

	while (currentCourse != NULL) {
		if (strcmp(currentCourse->courseName, courseName) == 0) {
			if (prevCourse == NULL) {
				currentStudent->courses = currentCourse->nextCourse;
			} else {
				prevCourse->nextCourse = currentCourse->nextCourse;
			}

			freeCourse(currentCourse);
			break;
		}

		prevCourse = currentCourse;
		currentCourse = currentCourse->nextCourse;
	}

	updateCGPA(list, studentID);
}

struct student *insertStudent(struct student *list, char *studentName, int studentID)
{
	struct student *newStudent = malloc(sizeof(struct student));
	newStudent->name = malloc(strlen(studentName) + 1);
	strcpy(newStudent->name, studentName);
	newStudent->id = studentID;
	newStudent->courses = NULL;
	newStudent->gpa = 0.0;
	newStudent->next = NULL;

	if (list == NULL) {
		return newStudent;
	}

	struct student *tail = list;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	tail->next = newStudent;

	return list;
}

void deleteStudent(struct student **list, int studentID)
{
	struct student *prevStudent = NULL;
	struct student *currentStudent = *list;

	while (currentStudent != NULL) {
		if (currentStudent->id == studentID) {
			if (prevStudent == NULL) {
				*list = currentStudent->next;
			} else {
				prevStudent->next = currentStudent->next;
			}

			free(currentStudent->name);
			struct grades *currentCourse = currentStudent->courses;
			while (currentCourse != NULL) {
				struct grades *nextCourse = currentCourse->nextCourse;
				freeCourse(currentCourse);
				currentCourse = nextCourse;
			}
			freeStudent(currentStudent);
			break;
		}

		prevStudent = currentStudent;
		currentStudent = currentStudent->next;
	}
}

void printStudent(struct student *student)
{
	printf("Name: %s\n", student->name);
	printf("ID: %d\n", student->id);
	printf("GPA: %.2f\n", student->gpa);
}

void printAllStudents(struct student *list)
{
	struct student *currentStudent = list;
	while (currentStudent != NULL) {
		printStudent(currentStudent);
		printf("\n");
		currentStudent = currentStudent->next;
	}
}

void printAllCourses(struct student *list, int studentID)
{
	struct student *currentStudent = searchStudent(list, studentID);
	int foundStudent = 0;

	while (currentStudent != NULL) {
		printf("Student ID: %d\n", currentStudent->id);
		printf("Courses:\n");
		struct grades *currentCourse = currentStudent->courses;

		while (currentCourse != NULL) {
			printf("%s: Grade %d, Credit %.1f\n", currentCourse->courseName, currentCourse->courseGrade,
				currentCourse->courseCredit);
			currentCourse = currentCourse->nextCourse;
		}

		if (currentStudent->id == studentID) {
			foundStudent = 1;
			printf("GPA for %s is %.1f\n", currentStudent->name, currentStudent->gpa);
		}

		printf("\n");
		currentStudent = currentStudent->next;
	}

	if (!foundStudent) {
		printf("Student with ID %d not found.\n", studentID);
	}
}

void printEverything(struct student *list)
{
	struct student *currentStudent = list;
	while (currentStudent != NULL) {
		printf("Student ID: %d\n", currentStudent->id);
		printf("GPA: %.2f\n", currentStudent->gpa);
		printf("Courses:\n");
		struct grades *currentCourse = currentStudent->courses;
		while (currentCourse != NULL) {
			printf("%s: Grade %d, Credit %.1f\n", currentCourse->courseName, currentCourse->courseGrade,
				currentCourse->courseCredit);
			currentCourse = currentCourse->nextCourse;
		}
		printf("\n");
		currentStudent = currentStudent->next;
	}
}

void sortStudentsByID(struct student **list)
{
	if (*list == NULL || (*list)->next == NULL) {
		return;
	}

	struct student *currentStudent = *list;
	struct student *prevStudent = NULL;
	bool swapped;

	do {
		swapped = false;
		currentStudent = *list;

		while (currentStudent->next != NULL) {
			if (currentStudent->id > currentStudent->next->id) {
				struct student *nextStudent = currentStudent->next;
				struct student *nextNextStudent = nextStudent->next;

				if (prevStudent == NULL) {
					*list = nextStudent;
				} else {
					prevStudent->next = nextStudent;
				}

				nextStudent->next = currentStudent;
				currentStudent->next = nextNextStudent;

				prevStudent = nextStudent;
				swapped = true;
			} else {
				prevStudent = currentStudent;
				currentStudent = currentStudent->next;
			}
		}
	} while (swapped);
}

void sortStudentsByName(struct student **list)
{
	if (*list == NULL || (*list)->next == NULL) {
		return;
	}

	struct student *currentStudent = *list;
	struct student *prevStudent = NULL;
	bool swapped;

	do {
		swapped = false;
		currentStudent = *list;

		while (currentStudent->next != NULL) {
			if (strcmp(currentStudent->name, currentStudent->next->name) > 0) {
				struct student *nextStudent = currentStudent->next;
				struct student *nextNextStudent = nextStudent->next;

				if (prevStudent == NULL) {
					*list = nextStudent;
				} else {
					prevStudent->next = nextStudent;
				}

				nextStudent->next = currentStudent;
				currentStudent->next = nextNextStudent;

				prevStudent = nextStudent;
				swapped = true;
			} else {
				prevStudent = currentStudent;
				currentStudent = currentStudent->next;
			}
		}
	} while (swapped);
}


void createEmptyFile(const char *filename) {
}


void saveEverythingToFile(struct student *list, const char *filename)
{
}

void handleChoice(int choice, struct student **studentList)
{
	switch (choice) {
	case 1:
	{
		char name[100];
		int id;

		printf("Enter student name: ");
		while (getchar() != '\n') ;
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';

		while (1) {
			printf("Enter student ID: ");
			if (scanf("%d", &id) != 1) {
				printf("Invalid ID. Please enter a valid ID.\n");
				while (getchar() != '\n') ;
				continue;
			}

			if (id < 0) {
				printf("Invalid ID. ID cannot be negative.\n");
				continue;
			}

			if (searchStudent(*studentList, id) != NULL) {
				printf("Student with ID %d already exists. Please enter a new ID.\n", id);
				continue;
			}

			break;
		}

		*studentList = insertStudent(*studentList, name, id);
		printf("Student added successfully.\n");
		break;
	}
	case 2:
	{
		int id;
		printf("Enter student ID to remove: ");
		while (scanf("%d", &id) != 1) {
			while (getchar() != '\n') ;	// Clear the input buffer
			printf("Invalid input. Please enter a valid ID: ");
		}

		struct student *currentStudent = searchStudent(*studentList, id);
		while (currentStudent == NULL) {
			printf("Student with ID %d not found. Please enter a valid ID: ", id);
			while (scanf("%d", &id) != 1) {
				while (getchar() != '\n') ;	// Clear the input buffer
				printf("Invalid input. Please enter a valid ID: ");
			}
			currentStudent = searchStudent(*studentList, id);
		}

		printf("%s removed successfully.\n", currentStudent->name);
		deleteStudent(studentList, id);
		break;

	}
	case 3:
		printf("\nAll Students:\n");
		printAllStudents(*studentList);
		break;
	case 4:
	{
		int id;
		char courseName[100];
		int grade;
		float credit;

		printf("Enter student ID: ");
		scanf("%d", &id);
		struct student *currentStudent = searchStudent(*studentList, id);
		if (currentStudent == NULL) {
			printf("Student with ID %d not found.\n", id);
			break;
		}

		printf("Adding course to %s\n", currentStudent->name);
		printf("Enter course name: ");
		getchar();
		fgets(courseName, sizeof(courseName), stdin);

		courseName[strcspn(courseName, "\n")] = '\0';
		printf("Enter course grade: (0-100) ");
		scanf("%d", &grade);

		while (grade < 0 || grade > 100) {
			printf("Invalid grade. Please enter a grade between 0 and 100: ");
			scanf("%d", &grade);
		}

		printf("Enter course credit: (1.0 or 0.5) ");
		scanf("%f", &credit);

		while (credit != 1.0 && credit != 0.5) {
			printf("Invalid credit. Please enter either 1.0 or 0.5: ");
			scanf("%f", &credit);
		}
		*studentList = insertCourse(*studentList, id, courseName, grade, credit);
		printf("Course added successfully.\n");
		break;
	}
	case 5:
	{
		int id;
		printf("Enter student ID to remove: ");
		while (scanf("%d", &id) != 1) {
			while (getchar() != '\n') ;	// Clear the input buffer
			printf("Invalid input. Please enter a valid ID: ");
		}

		struct student *currentStudent = searchStudent(*studentList, id);
		while (currentStudent == NULL) {
			printf("Student with ID %d not found. Please enter a valid ID: ", id);
			while (scanf("%d", &id) != 1) {
				while (getchar() != '\n') ;	// Clear the input buffer
				printf("Invalid input. Please enter a valid ID: ");
			}
			currentStudent = searchStudent(*studentList, id);
		}

		printf("%s removed successfully.\n", currentStudent->name);
		deleteStudent(studentList, id);
		break;

	}

	case 6:
	{
		int id;
		printf("\nEnter student ID: ");
		scanf("%d", &id);

		struct student *currentStudent = searchStudent(*studentList, id);
		if (currentStudent == NULL) {
			printf("Student with ID %d not found.\n", id);
			break;
		}

		printf("\nCourses for Student ID %d:\n", id);
		printAllCourses(*studentList, id);
		break;
	}
	case 7:
		printf("\nEverything:\n");
		printEverything(*studentList);
		break;
	case 8:
		sortStudentsByName(studentList);
		printf("Students sorted by name.\n");
		break;
	case 9:
		sortStudentsByID(studentList);
		printf("Students sorted by ID.\n");
		break;
case 10:
{
    char filename[100];
    printf("Doesn't Work");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    saveEverythingToFile(*studentList, filename);
    break;
}

	case 11:
		printf("Exiting the program.\n");
		break;
	default:
		printf("Invalid choice. Please try again.\n");
		break;
	}
}
