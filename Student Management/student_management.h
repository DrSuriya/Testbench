struct grades {
	char *courseName;
	int courseGrade;
	float courseCredit;
	struct grades *nextCourse;
};

struct student {
	char *name;
	int id;
	struct grades *courses;
	float gpa;
	struct student *next;
};
/**
 * Frees the memory allocated for a course.
 *
 * @param course Pointer to the course to be freed.
 */
void freeCourse(struct grades *course);

/**
 * Frees the memory allocated for a student, including their courses.
 *
 * @param student Pointer to the student to be freed.
 */
void freeStudent(struct student *student);

/**
 * Frees the memory allocated for a linked list of students.
 *
 * @param list Pointer to the head of the student list.
 */
void freeList(struct student *list);

/**
 * Searches for a student in a linked list by their student ID.
 *
 * @param list The head of the student list.
 * @param studentID The ID of the student to search for.
 * @return Pointer to the found student, or NULL if not found.
 */
struct student *searchStudent(struct student *list, int studentID);

/**
 * Updates the GPA of a student in the student list based on their course grades.
 *
 * @param list The head of the student list.
 * @param studentID The ID of the student to update the GPA for.
 * @return Pointer to the updated student list.
 */
struct student *updateCGPA(struct student *list, int studentID);

/**
 * Inserts a course for a specific student in the student list.
 *
 * @param list The head of the student list.
 * @param studentID The ID of the student to add the course to.
 * @param name The name of the course.
 * @param grade The grade obtained in the course.
 * @param credit The credit hours of the course.
 * @return Pointer to the updated student list.
 */
struct student *insertCourse(struct student *list, int studentID, char *name, int grade, float credit);

/**
 * Deletes a course for a specific student in the student list.
 *
 * @param list The head of the student list.
 * @param studentID The ID of the student to delete the course from.
 * @param courseName The name of the course to delete.
 */
void deleteCourse(struct student *list, int studentID, char *courseName);

/**
 * Inserts a new student into the student list.
 *
 * @param list The head of the student list.
 * @param studentName The name of the student.
 * @param studentID The ID of the student.
 * @return Pointer to the updated student list.
 */
struct student *insertStudent(struct student *list, char *studentName, int studentID);

/**
 * Deletes a student from the student list.
 *
 * @param list Pointer to the head of the student list.
 * @param studentID The ID of the student to delete.
 */
void deleteStudent(struct student **list, int studentID);

/**
 * Prints the information of a student.
 *
 * @param student Pointer to the student to print.
 */
void printStudent(struct student *student);

/**
 * Prints the information of all students in the student list.
 *
 * @param list The head of the student list.
 */
void printAllStudents(struct student *list);

/**
 * Prints the information of all courses for a specific student.
 *
 * @param list The head of the student list.
 * @param studentID The ID of the student to print courses for.
 */
void printAllCourses(struct student *list, int studentID);

/**
 * Prints the information of all students and their courses in the student list.
 *
 * @param list The head of the student list.
 */
void printEverything(struct student *list);

/**
 * Sorts the student list in ascending order based on student IDs.
 *
 * @param list Pointer to the head of the student list.
 */
void sortStudentsByID(struct student **list);

/**
 * Sorts the student list in alphabetical order based on student names.
 *
 * @param list Pointer to the head of the student list.
 */
void sortStudentsByName(struct student **list);

/**
 * Creates an empty .txt file in the deskstop of the user
 *
 * filename is a string that contains what the user wants to call the file.
 */
void createEmptyFile(const char *filename);

/**
 * Creates an empty .txt file in the deskstop of the user
 *
 * filename is a string that contains what the user wants to call the file.
 */
void saveEverythingToFile(struct student *list, const char *filename);

/**
 * Handles the user's choice in the student management system.
 *
 * @param choice The user's selected choice.
 * @param studentList Pointer to the head of the student list.
 */
void handleChoice(int choice, struct student **studentList);

