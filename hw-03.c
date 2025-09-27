#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

// input : (A(B(CD)E(FG(HI(JK(L(M(N))))))))
// (A (B (C D) E (G H (I J (K (L) M (N))))))
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
    if (is_empty())
        return -1;
    return stack[top--];
}

int peek() {
    if (is_empty())
        return -1;
    return stack[top];
}

void build_tree(const char arr[], int* arr_index, char result_tree[], int tree_index, int* max_index_value) {
    while (arr[*arr_index] && !isalpha(arr[*arr_index])) {
        (*arr_index)++;
    }
    if (arr[*arr_index] && isalpha(arr[*arr_index])) {
        result_tree[tree_index] = arr[*arr_index];
        if (tree_index > *max_index_value) {
            *max_index_value = tree_index;
        }
        (*arr_index)++;
    }

    // 공백처리 부분
    while (arr[*arr_index] && isspace(arr[*arr_index])) {
        (*arr_index)++;
    }

    if (arr[*arr_index] == '(') {
        (*arr_index)++;

        // 왼쪽 자식를 위한 재귀함수 => tree_index * 2
        build_tree(arr, arr_index, result_tree, tree_index * 2, max_index_value);

        // 공백처리 부분
        while (arr[*arr_index] && isspace(arr[*arr_index])) {
            (*arr_index)++;
        }
        if (arr[*arr_index] != ')') {
            // 오른쪽 자식을 위한 재귀함수 => tree_index * 2 + 1
            build_tree(arr, arr_index, result_tree, tree_index * 2 + 1, max_index_value);
        }

        while (arr[*arr_index] && arr[*arr_index] != ')') {
            (*arr_index)++;
        }
        if (arr[*arr_index] == ')') {
            (*arr_index)++;
        }
    }
}


void pre_order(char result_tree[], int max_index) {
    top = -1;
    push(1);

    while (!is_empty()) { // top이 비어있지 않을때까지 while문이 작동되는 구조임.
        int cur_index = pop();

        if (cur_index > max_index || !isalpha(result_tree[cur_index])) {
            continue;
        }

        printf("%c ", result_tree[cur_index]);
        int right_node = cur_index * 2 + 1;
        // 오른쪽 노드는 현재 노드의 인덱스에서 2*inde + 1을 통해서 이동
        int left_node = cur_index * 2;
        // 왼쪽 노드는 현재 노드의 인덱스에서 *2만 하면 됨.

        if (right_node <= max_index && isalpha(result_tree[right_node])) {
            push(right_node);
        }
        if (left_node <= max_index && isalpha(result_tree[left_node])) {
            push(left_node);
        }
    }
}

void in_order(char result_tree[], int max_index) {
    top = -1;
    int cur_index = 1;
    while ((cur_index > 0 && cur_index <= max_index && isalpha(result_tree[cur_index])) || !is_empty()) {
        while (cur_index > 0 && cur_index <= max_index && isalpha(result_tree[cur_index])) {
            push(cur_index);
            cur_index = cur_index * 2;
        }
        cur_index = pop();
        printf("%c ", result_tree[cur_index]);
        cur_index = cur_index * 2 + 1;
    }
}

void post_order(char result_tree[], int max_index) {
    if (max_index < 1) return;
    int s1[256], s2[256];
    int top1 = -1, top2 = -1;
    s1[++top1] = 1;
    while (top1 != -1) {
        int node_index = s1[top1--];

        s2[++top2] = node_index;

        int left_node = node_index * 2;
        int right_node = node_index * 2 + 1;

        if (left_node <= max_index && isalpha(result_tree[left_node])) {
            s1[++top1] = left_node;
        }
        if (right_node <= max_index && isalpha(result_tree[right_node])) {
            s1[++top1] = right_node;
        }
    }
    while (top2 != -1) {
        printf("%c ", result_tree[s2[top2--]]);
    }
}

int main(void) {
    char input_arr[256];

    scanf("%[^\n]", input_arr);

    char result_tree[256];
    for (int i = 0; i < 256; ++i) {
        result_tree[i] = '\0';
    }

    int arr_index = 0;
    int max_index = 0;
    build_tree(input_arr, &arr_index, result_tree, 1, &max_index);

    // 제대로 index에 들어갔는지 판단하기 위한 반복문
    // for (int i = 0; i <= max_index; ++i) {
    //     if (result_tree[i] != '\0') {
    //          printf("arr[%d] = %c \n", i, result_tree[i]);
    //     }
    // }
    // printf("\n");

    printf("pre-order: ");
    pre_order(result_tree, max_index);
    printf("\n");
    printf("in-order: ");
    in_order(result_tree, max_index);
    printf("\n");
    printf("post-order: ");
    post_order(result_tree, max_index);
    printf("\n");

    return 0;
}
