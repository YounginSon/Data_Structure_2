#include <ctype.h>
#include <stdio.h>

// input : (A (B (C D) E (F (G))))

typedef enum {
    OPEN,
    CLOSE,
    ROOT
} t_status;

int check_tree(const char s[], int *index, t_status status)
{
    int count = 0;

    while (s[*index] != '\0')
    {
        while (isspace(s[*index]))
            (*index)++;

        if (s[*index] == ')')
        {
            if (status != OPEN)
                return -1;
            status = CLOSE;
            (*index)++;
            break;
        }

        if (s[*index] == '(')
        {
            (*index)++;
            int count_2 = check_tree(s, index, OPEN);
            if (count_2 > 2 || count_2 == -1)
                return count_2;
            continue;
        }
        count++;
        (*index)++;
    }
    if (status == OPEN)
        return -1;
    return count;
}

int main(void)
{
    char str[256];
    scanf("%[^\n]", str);

    int index = 0;

    while (str[index] != '\0')
    {
        if (str[index] == '\n')
        {
            printf("ERROR\n");
            return 0;
        }
        index++;
    }

    index = 0;
    int result = check_tree(str, &index, ROOT);

    if (result < 2)
        printf("TRUE\n");
    else if (result > 2)
        printf("FALSE\n");
    return 0;
}