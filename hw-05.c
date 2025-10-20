#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
    이번 과제는 찾는 횟수를 직관적으로 비교하기 위한 과제이다.
    배열형태로 저장한 데이터에서 원하는 값을 찾을 때 얼마나 걸리는지,
    이진탐색트리 형태로 저장한 데이터에서 원하는 값을 찾을 때 얼마나 걸리는지,
    AVL 탐색트리(균형이진탐색트리) 형태로 저장한 데이터에서 원하는 값을 찾을 때 얼마나 걸리는지를 찾는 과정이다.

    이진탐색트리와 AVL탐색트리의 차이는 균형이 무너졌을 때 확연한 성능차이를 보일 것이다.
    AVL은 왼쪽 노드와 오른쪽 노드의 균형을 맞추기 위해서 왼쪽으로 회전시킬지, 오른쪽으로 회전시킬지를 정하여 균형을 맞추다보니 찾는 횟수가 일정하게 작을 것이다.
    하지만, 이진탐색트리는 한쪽으로 쏠려있는 불균형 이진트리에서는 이진탐색트리는 배열과 차이가 없을정도로 무의미한 알고리즘이 될 수 있기 때문에
    AVL을 사용하는 것이다.

    AVL은 총 4가지의 케이스로 분류해야한다.

단순 회전
    - LL유형 : 불균형 발생 노드의 왼쪽 자식노드와 자신의 왼쪽 노드에 의해 왼쪽으로 쏠려있는 경우
    - RR유형 : 불균형 발생 노드의 오른쪽 자식노드와 자식의 오른쪽 자식 노드에 의해 오른쪽으로 쏠려있는 경우
이중 회전
    - LR유형 : 불균형 발생 노드의 왼쪽 자식노드와 자식의 오른쪽 자식노드에 의해 왼쪽 서브트리가 쏠려있는 경우
    - RL유형 : 불균형 발생 노드의 오른쪽 자식노드와 자식의 왼쪽 자식노드에 의해 오른쪽 서브트가 쏠려있는 경우

    이러한 4가지 케이스를 if문으로 분류해서 코드를 작성해야 할듯. AVL을 구조체를 통해서 넣으려고 할때.

    이에 반해 이진트리는 저번에 사용하였던 것처럼
    넣어야하는 데이터의 첫 번째를 root노드로 만들고, root노드의 값과 다음 들어올 데이터의 크기를 비교해서 크면 오른쪽, 작으면 왼쪽으로 넣겠끔하여 비교하는 방식.

    넣는 방식은 어차피 이진트리와 AVL은 동일하고, AVL에 따로 회전 기능만 넣어주면 될 듯.


이렇게 넣는 방식이 완성이 되었다면, 총 4가지 형태의 데이터를 비교하여 출력해야함.
(1) 0~10000 사이의 무작위 정수 1000개(중복 X)
(2) 0~999까지 정렬된 정수 1000개
(3) 999~0까지 정렬된 정수 1000개
(4) for (int i = 0; i < 1000; i++) value[i] = i * (i % 2 + 2)로 구성된 정수 1000개

대략 이렇게 구현하면 될 듯.
AVL 부분에서 height 부분을 왼쪽 노드와 오른쪽 노드 2개를 비교해서 빼면 0이 되는 균형있는지만 판단하면 될 듯.
*/

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node *ft_rightRotation(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *ft_leftRotation(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int ft_Balance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* ft_insertAVL(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = ft_insertAVL(node->left, key);
    else if (key > node->key)
        node->right = ft_insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = ft_Balance(node);

    if (balance > 1 && key < node->left->key)
        return ft_rightRotation(node);

    if (balance < -1 && key > node->right->key)
        return ft_leftRotation(node);

    if (balance > 1 && key > node->left->key) {
        node->left = ft_leftRotation(node->left);
        return ft_rightRotation(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = ft_rightRotation(node->right);
        return ft_leftRotation(node);
    }

    return node;
}

// 이진탐색트리
Node* ft_insertBST(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = ft_insertBST(node->left, key);
    else if (key > node->key)
        node->right = ft_insertBST(node->right, key);

    return node;
}
// count를 포인터로 한 이유는 찾지 못하고 전부 비교한 경우를 쉽게 처리하기 위해서 이렇게 했음
// 배열은 하나씩 비교하고 같으면 그때 리턴
void ft_searchArray(int arr[], int n, int key, int* count) {
    *count = 0;
    for (int i = 0; i < n; i++) {
        (*count)++;
        if (arr[i] == key) {
            return;
        }
    }
}

// 찾는 방식은 AVL과 이진트리 모두 동일하기 때문에 하나로 통합하여 사용
void ft_searchTree(Node* root, int key, int* count) {
    if (root == NULL) {
        return;
    }

    (*count)++;
    if (key == root->key) {
        return;
    } else if (key < root->key) {
        ft_searchTree(root->left, key, count);
    } else {
        ft_searchTree(root->right, key, count);
    }
}

// (1)~(4) 데이터 생성
void generateData(int type, int arr[]) {
    switch (type) {
        case 1: { // 0~10000 사이 무작위 정수 1000개 (중복 X)
            int check[10001] = {0}; // 중복 체크용 배열
            int i = 0;
            while (i < 1000) {
                int r = rand() % 10001;
                if (check[r] == 0) {
                    check[r] = 1;
                    arr[i] = r;
                    i++;
                }
            }
            break;
        }
        case 2: // 0~999 정렬된 정수
            for (int i = 0; i < 1000; i++) arr[i] = i;
            break;
        case 3: // 999~0 정렬된 정수
            for (int i = 0; i < 1000; i++) arr[i] = 999 - i;
            break;
        case 4: // value[i] = i * (i % 2 + 2)
            for (int i = 0; i < 1000; i++) arr[i] = i * (i % 2 + 2);
            break;
        default:
            break;
    }
}

void generateSearchKeys(int arr[]) {
    for (int i = 0; i < 1000; i++) {
        arr[i] = rand() % 10001;
    }
}

void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    for (int dataType = 1; dataType <= 4; dataType++) {
        int data_to_insert[1000];
        generateData(dataType, data_to_insert);

        int array[1000];
        Node* bst_root = NULL;
        Node* avl_root = NULL;

        for (int i = 0; i < 1000; i++) {
            array[i] = data_to_insert[i];
            bst_root = ft_insertBST(bst_root, data_to_insert[i]);
            avl_root = ft_insertAVL(avl_root, data_to_insert[i]);
        }

        // 탐색용 난수 생성
        int data_to_search[1000];
        generateSearchKeys(data_to_search);

        long long array_comps = 0;
        long long bst_comps = 0;
        long long avl_comps = 0;

        for (int i = 0; i < 1000; i++) {
            int key_to_find = data_to_search[i];
            int comparisons = 0;

            // 배열 탐색
            comparisons = 0;
            ft_searchArray(array, 1000, key_to_find, &comparisons);
            array_comps += comparisons;

            // 이진탐색트리
            comparisons = 0;
            ft_searchTree(bst_root, key_to_find, &comparisons);
            bst_comps += comparisons;

            // AVL탐색트리
            comparisons = 0;
            ft_searchTree(avl_root, key_to_find, &comparisons);
            avl_comps += comparisons;
        }

        printf("--- 데이터 (%d) 처리 결과 ---\n", dataType);
        printf("Array: 데이터 (%d)에서 평균 %.2f회 탐색\n", dataType, (double)array_comps / 1000.0);
        printf("BST:   데이터 (%d)에서 평균 %.2f회 탐색\n", dataType, (double)bst_comps / 1000.0);
        printf("AVL:   데이터 (%d)에서 평균 %.2f회 탐색\n", dataType, (double)avl_comps / 1000.0);
        printf("\n");

        freeTree(bst_root);
        freeTree(avl_root);
    }

    return 0;
}