#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> // strlen 사용을 위해 추가

// --- 스택 구현 (기존과 동일) ---
int stack[9999];
int top = -1;

bool is_empty() { return top == -1; }
void push(int item) { stack[++top] = item; }
int pop() { return is_empty() ? -1 : stack[top--]; }
int peek() { return is_empty() ? -1 : stack[top]; }

/**
 * @brief 간결하게 재설계된 트리 생성 함수
 * @param arr 입력 문자열
 * @param arr_pos 입력 문자열의 현재 위치를 가리키는 포인터
 * @param result_tree 결과를 저장할 배열
 * @param tree_pos 결과 배열에 현재 노드를 저장할 위치
 * @param max_index_value 사용된 최대 인덱스 추적용 포인터
 */
void build_tree(const char* arr, int* arr_pos, char result_tree[], int tree_pos, int* max_index_value) {
    // 공백이나 여는 괄호는 건너뜀
    while (arr[*arr_pos] && (isspace(arr[*arr_pos]) || arr[*arr_pos] == '(')) {
        (*arr_pos)++;
    }

    // 현재 위치에 노드(알파벳)가 있으면 트리에 추가
    if (arr[*arr_pos] && isalpha(arr[*arr_pos])) {
        // 현재 노드를 배열의 지정된 위치에 저장
        result_tree[tree_pos] = arr[*arr_pos];
        if (tree_pos > *max_index_value) {
            *max_index_value = tree_pos;
        }
        (*arr_pos)++; // 다음 문자로 이동

        // 바로 다음에 여는 괄호가 나오면 자식 노드가 있다는 의미
        if (arr[*arr_pos] == '(') {
            // 첫 번째 자식은 항상 왼쪽 자식 (tree_pos * 2)
            build_tree(arr, arr_pos, result_tree, tree_pos * 2, max_index_value);

            // 왼쪽 자식 서브트리 파싱이 끝난 후, 다음 문자가 알파벳이면 오른쪽 자식임
            if (arr[*arr_pos] && isalpha(arr[*arr_pos])) {
                // 두 번째 자식은 오른쪽 자식 (tree_pos * 2 + 1)
                build_tree(arr, arr_pos, result_tree, tree_pos * 2 + 1, max_index_value);
            }
        }
    }

    // 현재 서브트리의 파싱이 끝났으므로 닫는 괄호를 건너뜀
    if (arr[*arr_pos] == ')') {
        (*arr_pos)++;
    }
}


// --- 순회 함수들 (기존과 동일) ---
void pre_order(char result_tree[], int size) {
    if (size < 1) return;
    top = -1;
    push(1);
    while (!is_empty()) {
        int current_index = pop();
        if (current_index > size || !isalpha(result_tree[current_index])) continue;
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
        if (left_child <= size && isalpha(result_tree[left_child])) s1[++top1] = left_child;
        if (right_child <= size && isalpha(result_tree[right_child])) s1[++top1] = right_child;
    }
    while (top2 != -1) {
        printf("%c ", result_tree[s2[top2--]]);
    }
}


int main(void) {
    char arr[256];
    scanf("%[^\n]", arr);

    char result_tree[256];
    for (int i = 0; i < 256; ++i) {
        result_tree[i] = '\0'; // '0' 대신 널 문자로 초기화하는 것이 더 안전합니다.
    }

    int arr_pos = 0;
    int max_index = 0;
    // 루트 노드는 인덱스 1부터 시작
    build_tree(arr, &arr_pos, result_tree, 1, &max_index);

    int size = max_index; // 최대 인덱스가 곧 배열의 유효 크기

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
