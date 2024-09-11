
int currentAssignments = 5;
int numberStudents = 4;

List<int> sophia = new List<int> { 93, 87, 98, 95, 100 };
List<int> nicolas = new List<int> { 93, 87, 98, 95, 100 };
List<int> zahirah = new List<int> { 93, 87, 98, 95, 100 };
List<int> jeong = new List<int> { 93, 87, 98, 95, 100 };

List<List<int>> studentsList = new List<List<int>> { sophia, nicolas, zahirah, jeong };
List<string> studentNames = new List<string>{"Sophia", "Nicolas", "Zahirah", "Jeong"};

Console.WriteLine("Student\t\tGrade");
for (int i = 0; i < numberStudents; i++)
{
    string name = studentNames[i];
    List<int> student = studentsList[i];
    int sum = 0;

    foreach (int grades in student)
    {
        sum += grades;
    }
    
    float average = (float)sum / currentAssignments; 
    Console.WriteLine($"{name}\t\t{average:F2}");
}
