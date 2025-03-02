string studentName ="Sophia Johnson";

Dictionary<string, List<int>> studentCourses = new Dictionary<string, List<int>>
{
    { "English 101", new List<int> { 4, 3}},
    { "Algebra 101", new List<int> { 3, 3}},
    { "Biology 101", new List<int> { 3, 4}},
    { "Computer Science 101", new List<int> { 3, 4}},
    { "Psychology 101", new List<int> { 4, 3}}
};

Console.WriteLine($"{studentName}\n\nCourse\t\t\t\t\tGrade\tCredit Hours");

decimal totalGrade = 0;
decimal totalHours = 0;

foreach (var course in studentCourses)
{
    Console.WriteLine($"{course.Key, -20} {course.Value[0], 7} {course.Value[1], 15}");
    totalGrade += (course.Value[0] * course.Value[1]);
    totalHours += course.Value[1];
}

decimal calculatedGPA = totalGrade/totalHours;

Console.WriteLine($"{calculatedGPA:F2}");
