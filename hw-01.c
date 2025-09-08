#include <ctype.h>
#include <stdio.h>

// input : (A (B (C D) E (F (G))))
// int main(int argc, char argv[])

typedef enum {
    OPEN,
    CLOSE,
    ROOT
} t_status;

// int check_root(const char s[], int *index) {
//     (*index) = 0;
//     int pre_count = 0;
//     int cur_count = 0;
//     int root_count = 0;
//     // (A(B(CD)E(FG(HIJ))))
//     while (s[*index] != '\0') {
//         if (isspace(s[*index]))
//             (*index)++;
//         if (s[*index] == '(') {
//             index++;
//             if (isalpha(s[*index])) {
//                 root_count++;
//                 (*index)++;
//             } else if (s[*index] == '(') {
//                 pre_count++;
//                 (*index)++;
//             } else
//                 (*index)++;
//         } else
//             (*index)++;
//     }
//     if (root_count > 1)
//         return 0;
//     return 1;
// }

int check_par(const char s[], int *index) {
    int pre_count = 0; // '('의 개수를 의미
    int cur_count = 0; // ')'의 개수를 의미

    while (s[*index] != '\0') {
        if (s[*index] == '(') {
            pre_count++;
            (*index)++;
        } else if (s[*index] == ')') {
            if (pre_count < cur_count)
                return 0;
            cur_count++;
            (*index)++;
        } else
            (*index)++;
    }

    if (pre_count != cur_count)
        return 0;
    return 1;
}

int check_tree(const char s[], int *index, t_status status) {
    int count = 0;

    while (s[*index] != '\0') {
        while (isspace(s[*index]))
            (*index)++;

        if (s[*index] == ')') {
            if (status != OPEN)
                return -1;
            status = CLOSE;
            (*index)++;
            break;
        }

        if (s[*index] == '(') {
            (*index)++;
            int count_2 = check_tree(s, index, OPEN);
            if (count_2 > 2 || count_2 == -1)
                return count_2;
            continue;
        }
        // printf("count : %d\n", count);
        count++;
        (*index)++;
    }

    if (status == OPEN)
        return -1;

    return count;
}

int main(void) {
    char str[128];
    printf("Input:");
    scanf("%[^\n]", str);

    int index = 0;
    int result = -1;

    while (str[index] != '\0') {
        if (str[index] == '\n') {
            printf("\nERROR\n");
            return 0;
        }
        index++;
    }

    index = 0;
    if (check_par(str, &index)) {
        index = 0;
        result = check_tree(str, &index, ROOT);
    }

    // printf("RESULT: %d", result);
    if (result == -1)
        printf("\nERROR\n");
    else if (result < 2)
        printf("\nOutput: TRUE\n");
    else if (result > 2)
        printf("\nOutput: FALSE\n");
    return 0;
}
