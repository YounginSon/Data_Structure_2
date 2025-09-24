#include <ctype.h>
#include <stdio.h>
#include <stdbool.h> // bool 타입을 위해 추가

// --- 개선된 스택 구현 ---
int stack[9999];
int top = -1;

// 스택이 비어있는지 확인하는 함수
bool is_empty() {
    return top == -1;
}

// 스택에 아이템을 추가하는 함수
void push(int item) {
    stack[++top] = item;
}

// 스택에서 아이템을 꺼내 반환하는 함수
int pop() {
    if (is_empty()) {
        return -1; // 스택이 비어있으면 -1 반환 (오류 표시)
    }
    return stack[top--];
}

// 스택의 가장 위 아이템을 확인하는 함수 (꺼내지 않음)
int peek() {
    if (is_empty()) {
        return -1;
    }
    return stack[top];
}

// (A (B (C D) E (G H (I J (K (L) M (N)))))) 형태의 이진트리를 배열로 저장하는 방법
/*
    1. 스택을 이용해서 저장하는 방법. '('을 만났을 때 pop을 한 후에 push를 하기. 이떄 index를 ++를 같이 해주기.
    그렇다면 처음 값은 index 1에 저장해야하기 때문에 pop에서 아무 것도 0에는 들어가지 않고, push를 통해 A를 stack에 저장할 것이다.
    스택에 저장을 한 이후에 다음 열린 괄호 '('를 만나게되면 A를 pop한 후에 B를 넣음.


    차라리 괄호를 만나면 인덱스를 *2를 한 후에 재귀로 돌아가는 구조가 훨씬 괜찮을 듯.
*/

// 이 함수가 실행된다는건 이미 이진트리 검사를 마친 상태임. 이진트리로 가정하고 이 함수를 도는걸로.
char *insert_array_tree(char arr[], int *arr_index, char result_tree[], int *result_index, int *max_index_value) {
    // arr는 입력받은 값이고 이 값을 result 배열에 저장하는 형식
    // if (arr[*index] == '(')
    //     (*index)++;
    if (*max_index_value < *result_index) {
        *max_index_value = *result_index;
    }
    while (arr[*arr_index] != '\0') {
        while (isspace(arr[*arr_index])) {
            (*arr_index)++;
        }
        if (arr[*arr_index] == '(') {
            (*arr_index)++;
            while (isspace(arr[*arr_index]))
                (*arr_index)++;
            int count = 0;
            while (isalpha(arr[*arr_index])) {
                if (isspace(result_tree[*result_index])) {
                    *result_index *= 2;
                }
                result_tree[*result_index] = arr[*arr_index];
                (*arr_index)++;
                (*result_index)++;
                count++;
            }
            if (arr[*arr_index] == '(') {
                while (count != 0) {
                    count--;
                    (*result_index)--;
                }
                // 여기서 arr[*arr_index] 부분이 '('을 가르키고 있으니
                // arr[*arr_index -1]와 똑같은 영어단어를 갖고 있는 인덱스에서 *2를 하는게 맞을듯.
                for (int i = 0;i<*arr_index;i++) {
                    if (arr[*arr_index - 1] == result_tree[i]) {
                        *result_index = i * 2;
                        break;
                    }
                }
                insert_array_tree(arr, arr_index, result_tree, result_index, max_index_value);
            }
            else if (arr[*arr_index] == ')') {
                (*arr_index)++;
                while (count != 0) {
                    count--;
                    (*result_index)--;
                }
                *result_index /= 2;
                break;
            }
        }
        else if (arr[*arr_index] == ')') {
            (*arr_index)++;
            break;
        }
        else {
            if (isalpha(result_tree[*result_index])) {
                (*result_index)++;
            } // B가 존재하니 인덱스를 ++를 시켜서 E를 넣었음.
            result_tree[*result_index] = arr[*arr_index];
            (*arr_index)++;
            (*result_index) *= 2;
        }
    }

    return result_tree;
}

void pre_order(char result_tree[], int size) {
    // 1. 스택이 비어있는지 확인하고, 트리가 유효한지 검사합니다.
    if (size < 1) {
        return;
    }

    // 2. 스택을 초기화하고 루트 노드(인덱스 1)를 넣습니다.
    top = -1; // 스택 포인터 초기화
    push(1);

    // 3. 스택이 빌 때까지 반복합니다.
    while (!is_empty()) {
        // 4. 스택에서 현재 방문할 노드를 꺼냅니다.
        int current_index = pop();

        // 유효하지 않은 노드라면 건너뜁니다.
        if (current_index > size || !isalpha(result_tree[current_index])) {
            continue;
        }

        // 5. 현재 노드를 방문(출력)합니다. (Root)
        printf("%c ", result_tree[current_index]);

        int right_child = current_index * 2 + 1;
        int left_child = current_index * 2;

        // 6. 오른쪽 자식을 먼저 스택에 넣습니다. (Right)
        if (right_child <= size && isalpha(result_tree[right_child])) {
            push(right_child);
        }

        // 7. 왼쪽 자식을 나중에 스택에 넣습니다. (Left)
        if (left_child <= size && isalpha(result_tree[left_child])) {
            push(left_child);
        }
    }
}

// 중위 순회 (반복문)
// 중위 순회 (명시적 while 조건 사용)
void in_order(char result_tree[], int size) {
    top = -1; // 스택 초기화
    int current_index = 1; // 루트에서 시작

    // 현재 노드가 유효하거나 스택이 비어있지 않은 동안 반복
    while ((current_index > 0 && current_index <= size && isalpha(result_tree[current_index])) || !is_empty()) {

        // 1. 현재 노드가 유효하면 왼쪽 끝까지 계속 이동하며 스택에 push
        while (current_index > 0 && current_index <= size && isalpha(result_tree[current_index])) {
            push(current_index);
            current_index = current_index * 2; // 왼쪽 자식으로 이동
        }

        // 2. 왼쪽 끝에 도달했으므로 스택에서 pop
        current_index = pop();

        // 3. pop 한 노드를 방문(출력)
        printf("%c ", result_tree[current_index]);

        // 4. 오른쪽 서브트리를 탐색하기 위해 이동
        current_index = current_index * 2 + 1;
    }
}

// 후위 순회 (반복문, 2개의 스택 사용)
void post_order(char result_tree[], int size) {
    if (size < 1) return;

    // 이 함수 내에서만 사용할 로컬 스택 2개 선언
    int s1[256], s2[256];
    int top1 = -1, top2 = -1;

    // 1. 스택1에 루트 노드 push
    s1[++top1] = 1;

    // 2. 스택1이 빌 때까지 반복
    while (top1 != -1) {
        // 스택1에서 pop -> 스택2에 push
        int node_index = s1[top1--];
        s2[++top2] = node_index;

        int left_child = node_index * 2;
        int right_child = node_index * 2 + 1;

        // 왼쪽 자식을 먼저 스택1에 push
        if (left_child <= size && isalpha(result_tree[left_child])) {
            s1[++top1] = left_child;
        }
        // 그 다음 오른쪽 자식을 스택1에 push
        if (right_child <= size && isalpha(result_tree[right_child])) {
            s1[++top1] = right_child;
        }
    }

    // 3. 스택2의 모든 노드를 pop하며 출력
    while (top2 != -1) {
        printf("%c ", result_tree[s2[top2--]]);
    }
}

int main(void) {
    char arr[256]; // '('가 나오면 index *2가 아니라 '(' 전에 나온 값의 인덱스를 *2 시키는 방식으로 진행
    scanf("%[^\n]", arr);

    char result_tree[256];
    for (int i = 0; i < 256; ++i) {
        result_tree[i] = '0';
    }

    int arr_index = 0;
    int result_index = 1;
    int max_index_value = 0;
    insert_array_tree(arr, &arr_index, result_tree, &result_index, &max_index_value);
    if (isalpha(result_tree[max_index_value + 1])) {
        max_index_value += 2;
        result_tree[max_index_value] = '\0';
    }
    else {
        max_index_value++;
        result_tree[max_index_value] = '\0';
    }
    int size = 0;
    for (int i=0; result_tree[i] != '\0'; i++) {
        size++;
    } // 저장한 배열 크기를 재기 위함.
    size--; // index 형식으로 바꾸기 위함.

    // 전위, 중위, 후위 순차 출력 ##반복문을 통함
    printf("pre-order: ");
    pre_order(result_tree, size);
    printf("\n");
    printf("in-order: ");
    in_order(result_tree, size); // index로 size를 바로 전달함.
    printf("\n");
    printf("post-order: ");
    post_order(result_tree, size);
    // 배열에 제대로 값이 들어가는 것까지 확인함.
    // 이제 전위, 중위, 후위 순회에 대해서 반복문을 이용하여 출력해보자.
}