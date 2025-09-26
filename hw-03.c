#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// input : (A(B(CD)E(FG(HI(JK(L(M(N))))))))
// 스택을 이용해서 전위, 중위, 후위 순회를 구현
// 배열에 이진트리를 넣기 위해서는 재귀를 통해서 구현
/*
    배열에서 '('를 만나게 되면 재귀 안에 index *2을 넣어 왼쪽 자식 노드를 확인하고,
    ')'를 만나지 않고 또 다시 노드를 만나게 되면 그 index + 1한다. 함수 자체가 void기 때문에
    위 경우가 아니라면 함수가 끝나므로 다시 index는 *2를 하기 전으로 돌아오기 때문에 제대로 삽입이 가능할 듯.
 */
int stack[999];
int top = -1;

bool is_empty() {
    return top == -1;
}
void push(int item) {
    stack[++top] = item;
}
int pop() {
    return is_empty() ? -1 : stack[top--];
}
int peek() {
    return is_empty() ? -1 : stack[top];
}

void build_tree(const char* arr, int* arr_pos, char result_tree[], int tree_pos, int* max_index_value) {
    while (arr[*arr_pos] && (isspace(arr[*arr_pos]) || arr[*arr_pos] == '(')) {
        (*arr_pos)++;
    }

    if (arr[*arr_pos] && isalpha(arr[*arr_pos])) {
        result_tree[tree_pos] = arr[*arr_pos];
        if (tree_pos > *max_index_value) {
            *max_index_value = tree_pos;
        }
        (*arr_pos)++;

        while (arr[*arr_pos] && isspace(arr[*arr_pos])) {
            (*arr_pos)++;
        }

        if (arr[*arr_pos] == '(') {
            build_tree(arr, arr_pos, result_tree, tree_pos * 2, max_index_value);
        }

        while (arr[*arr_pos] && isspace(arr[*arr_pos])) {
            (*arr_pos)++;
        }

        if (arr[*arr_pos] && arr[*arr_pos] != ')') {
            build_tree(arr, arr_pos, result_tree, tree_pos + 1, max_index_value);
        }
    }

    while (arr[*arr_pos] && isspace(arr[*arr_pos])) {
        (*arr_pos)++;
    }
    if (arr[*arr_pos] == ')') {
        (*arr_pos)++;
    }
}


void pre_order(char result_tree[], int size) {
    if (size < 1) return;
    top = -1;
    push(1);
    while (!is_empty()) {
        int current_index = pop();
        if (current_index > size || !isalpha(result_tree[current_index]))
            continue;
        printf("%c ", result_tree[current_index]);
        int right_child = current_index * 2 + 1;
        int left_child = current_index * 2;
        if (right_child <= size && isalpha(result_tree[right_child])) push(right_child);
        if (left_child <= size && isalpha(result_tree[left_child])) push(left_child);
    }
}

void in_order(char result_tree[], int size) {
    top = -1;
    int current_index = 1;
    while ((current_index > 0 && current_index <= size && isalpha(result_tree[current_index])) || !is_empty()) {
        while (current_index > 0 && current_index <= size && isalpha(result_tree[current_index])) {
            push(current_index);
            current_index = current_index * 2;
        }
        current_index = pop();
        printf("%c ", result_tree[current_index]);
        current_index = current_index * 2 + 1;
    }
}

void post_order(char result_tree[], int size) {
    if (size < 1) return;
    int s1[256], s2[256];
    int top1 = -1, top2 = -1;
    s1[++top1] = 1;
    while (top1 != -1) {
        int node_index = s1[top1--];
        s2[++top2] = node_index;
        int left_child = node_index * 2;
        int right_child = node_index * 2 + 1;
        if (left_child <= size && isalpha(result_tree[left_child]))
            s1[++top1] = left_child;
        if (right_child <= size && isalpha(result_tree[right_child]))
            s1[++top1] = right_child;
    }
    while (top2 != -1) {
        printf("%c ", result_tree[s2[top2--]]);
    }
}

int main(void) {
    char input_arr[256];

    if (scanf("%[^\n]", input_arr) != 1) {
        return 1;
    }

    char result_tree[256];
    for (int i = 0; i < 256; ++i) {
        result_tree[i] = '\0';
    }

    int arr_pos = 0;
    int max_index = 0;
    build_tree(input_arr, &arr_pos, result_tree, 1, &max_index);

    // for (int i = 0; i <= max_index; ++i) {
    //     if (result_tree[i] != '\0') {
    //          printf("arr[%d] = %c \n", i, result_tree[i]);
    //     }
    // }
    // printf("\n");
    int size = max_index;

    printf("pre-order: ");
    pre_order(result_tree, size);
    printf("\n");
    printf("in-order: ");
    in_order(result_tree, size);
    printf("\n");
    printf("post-order: ");
    post_order(result_tree, size);
    printf("\n");

    return 0;
}
