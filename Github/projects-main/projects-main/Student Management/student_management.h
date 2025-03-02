/**
 * Structure to represent course grades.
 */
struct grades {
	char *courseName;            /**< Name of the course */
	int courseGrade;             /**< Grade obtained in the course */
	float courseCredit;          /**< Credit hours of the course */
	struct grades *nextCourse;   /**< Pointer to the next course */
};

/**
 * Structure to represent a student.
 */
struct student {
	char *name;                  /**< Name of the student */
	int id;                      /**< Student ID */
	struct grades *courses;      /**< Pointer to the courses of the student */
	float gpa;                   /**< GPA of the student */
	struct student *next;        /**< Pointer to the next student */
};

/**
 * Frees the memory allocated for a course.
 *
 * course Pointer to the course to be freed.
 */
void freeCourse(struct grades *course);

/**
 * Frees the memory allocated for a student, including their courses.
 *
 * student Pointer to the student to be freed.
 */
void freeStudent(struct student *student);

/**
 * Frees the memory allocated for a linked list of students.
 *
 * list Pointer to the head of the student list.
 */
void freeList(struct student *list);

/**
 * Searches for a student in a linked list by their student ID.
 *
 * list The head of the student list.
 * studentID The ID of the student to search for.
 * return Pointer to the found student, or NULL if not found.
 */
struct student *searchStudent(struct student *list, int studentID);

/**
 * Updates the GPA of a student in the student list based on their course grades.
 *
 * list The head of the student list.
 * studentID The ID of the student to update the GPA for.
 * return Pointer to the updated student list.
 */
struct student *updateCGPA(struct student *list, int studentID);

/**
 * Inserts a course for a specific student in the student list.
 *
 * list The head of the student list.
 * studentID The ID of the student to add the course to.
 * name The name of the course.
 * grade The grade obtained in the course.
 * credit The credit hours of the course.
 * return Pointer to the updated student list.
 */
struct student *insertCourse(struct student *list, int studentID, char *name, int grade, float credit);

/**
 * Deletes a course for a specific student in the student list.
 *
 * list The head of the student list.
 * studentID The ID of the student to delete the course from.
 * courseName The name of the course to delete.
 */
void deleteCourse(struct student *list, int studentID, char *courseName);

/**
 * Inserts a new student into the student list.
 *
 * list The head of the student list.
 * studentName The name of the student.
 * studentID The ID of the student.
 * return Pointer to the updated student list.
 */
struct student *insertStudent(struct student *list, char *studentName, int studentID);

/**
 * Deletes a student from the student list.
 *
 * list Pointer to the head of the student list.
 * studentID The ID of the student to delete.
 */
void deleteStudent(struct student **list, int studentID);

/**
 * Prints the information of a student.
 *
 * student Pointer to the student to print.
 */
void printStudent(struct student *student);

/**
 * Prints the information of all students in the student list.
 *
 * list The head of the student list.
 */
void printAllStudents(struct student *list);

/**
 * Prints the information of all courses for a specific student.
 *
 * list The head of the student list.
 * studentID The ID of the student to print courses for.
 */
void printAllCourses(struct student *list, int studentID);

/**
 * Prints the information of all students and their courses in the student list.
 *
 * list The head of the student list.
 */
void printEverything(struct student *list);

/**
 * Sorts the student list in ascending order based on student IDs.
 *
 * list Pointer to the head of the student list.
 */
void sortStudentsByID(struct student **list);

/**
 * Sorts the student list in alphabetical order based on student names.
 *
 * list Pointer to the head of the student list.
 */
void sortStudentsByName(struct student **list);

/**
 * Handles the user's choice in the student management system.
 *
 * choice The user's selected choice.
 * studentList Pointer to the head of the student list.
 */
void handleChoice(int choice, struct student **studentList);

