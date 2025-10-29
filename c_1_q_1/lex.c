#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f1;
    char c, str[100], special[100];
    int lineno = 1, num = 0, i = 0, sp_index = 0;
    int first_num = 1;

    system("clear");

    printf("\nEnter C program (Ctrl+D to end):\n");

    f1 = fopen("input.txt", "w");
    while ((c = getchar()) != EOF)
        putc(c, f1);
    fclose(f1);

    f1 = fopen("input.txt", "r");

    printf("\nThe numbers in the program are: ");
    while ((c = getc(f1)) != EOF)
    {
        if (isdigit(c))
        {
            num = c - '0';
            c = getc(f1);
            while (isdigit(c))
            {
                num = num * 10 + (c - '0');
                c = getc(f1);
            }
            if (first_num)
            {
                printf("%d", num);
                first_num = 0;
            }
            else
            {
                printf(" %d", num);
            }
            ungetc(c, f1);
        }
    }

    rewind(f1); // Reset file pointer

    printf("\n\nThe keywords and identifiers are:\n");

    while ((c = getc(f1)) != EOF)
    {
        if (isalpha(c))
        {
            i = 0;
            str[i++] = c;
            c = getc(f1);
            while (isalnum(c) || c == '_' || c == '$')
            {
                str[i++] = c;
                c = getc(f1);
            }
            str[i] = '\0';

            if (
                strcmp("for", str) == 0 || strcmp("while", str) == 0 || strcmp("do", str) == 0 ||
                strcmp("int", str) == 0 || strcmp("float", str) == 0 || strcmp("char", str) == 0 ||
                strcmp("double", str) == 0 || strcmp("static", str) == 0 || strcmp("switch", str) == 0 ||
                strcmp("case", str) == 0)
            {
                printf("%s is a keyword\n", str);
            }
            else
            {
                printf("%s is an identifier\n", str);
            }
            ungetc(c, f1);
        }
        else if (c == '\n')
        {
            lineno++;
        }
    }

    rewind(f1); // Reset again for special characters

    printf("\nSpecial characters are: ");
    while ((c = getc(f1)) != EOF)
    {
        if (!(isalnum(c) || isspace(c) || c == '_' || c == '$'))
        {
            printf("%c ", c);
        }
    }

    printf("\n\nTotal no. of lines are: %d\n", lineno);

    fclose(f1);
    printf("Press enter to exit....");
    getchar();
    return 0;
}

