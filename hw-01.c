#include <ctype.h>
#include <stdio.h>

// input : (A (B (C D) E (F (G))))

typedef enum {
    OPEN,
    CLOSE,
    ROOT
} t_status;

int check_root(const char s[], int *index)
{
    int i = 0;
    int depth = 0;
    int root_count = 0;
    int in_root_label = 0; // 루트에서 알파벳 연속 체크용

    while (s[i] != '\0')
    {
        unsigned char c = (unsigned char)s[i];

        if (c == '(') {
            depth++;
            in_root_label = 0; // 괄호 들어가면 초기화
            i++;
            continue;
        }

        if (c == ')')
        {
            if (depth > 0) depth--;
            in_root_label = 0;
            i++;
            continue;
        }

        if (isspace(c))
        {
            i++;
            continue;
        }

        if (isalpha(c) && depth == 1)
        {
            if (in_root_label)
            {
                return 0;
            }
            root_count++;
            in_root_label = 1;

            while (s[i] != '\0' && isalpha((unsigned char)s[i]))
            {
                i++;
                if (isalpha((unsigned char)s[i]))
                    return 0;
            }
            continue;
        }
        i++;
    }
    *index = i;

    return (root_count == 1) ? 1 : 0;
}

int check_par(const char s[], int *index)
{
    int pre_count = 0; // '('의 개수를 의미
    int cur_count = 0; // ')'의 개수를 의미

    while (s[*index] != '\0')
    {
        if (s[*index] == '(')
        {
            pre_count++;
            (*index)++;
        }
        else if (s[*index] == ')')
        {
            if (pre_count < cur_count)
                return 0;
            cur_count++;
            (*index)++;
        }
        else
            (*index)++;
    }

    if (pre_count != cur_count)
        return 0;
    return 1;
}

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
    printf("Input:");
    scanf("%[^\n]", str);

    int index = 0;
    int result = -1;

    while (str[index] != '\0')
    {
        if (str[index] == '\n')
        {
            printf("\nOutput: ERROR\n");
            return 0;
        }
        index++;
    }

    index = 0;
    if (check_par(str, &index) && check_root(str, &index))
    {
        index = 0;
        result = check_tree(str, &index, ROOT);
    }

    if (result == -1)
        printf("\nOutput: ERROR\n");
    else if (result < 2)
        printf("\nOutput: TRUE\n");
    else if (result > 2)
        printf("\nOutput: FALSE\n");
    return 0;
}
