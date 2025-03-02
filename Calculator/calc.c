#include <stdio.h>

int main()
{
    char operator, choice;
    double num1, num2, result;

    do {
        printf("Enter an operator (+, -, *, /): ");
        scanf(" %c", &operator);

        printf("Enter two numbers: ");
        scanf("%lf %lf", &num1, &num2);

        switch(operator)
        {
            case '+':
                result = num1 + num2;
                printf("%.2lf + %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '-':
                result = num1 - num2;
                printf("%.2lf - %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '*':
                result = num1 * num2;
                printf("%.2lf * %.2lf = %.2lf\n", num1, num2, result);
                break;

            case '/':
                if(num2 == 0)
                {
                    printf("you cannot divide by zero\n");
                }
                else
                {
                    result = num1 / num2;
                    printf("%.2lf / %.2lf = %.2lf\n", num1, num2, result);
                }
                break;

            default:
                printf("invalid operator\n");
        }

        printf("Do you wanna go again? (Y/N): ");
        scanf(" %c", &choice);

    } while(choice == 'y' || choice == 'Y');

    printf("bye bye\n");

    return 0;
}
