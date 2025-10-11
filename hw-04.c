#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode {
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* create_node(int key) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

TreeNode* insert_node(TreeNode* root, int key) {
    if (root == NULL) {
        return create_node(key);
    }
    if (key < root->key) {
        root->left = insert_node(root->left, key);
    } else if (key > root->key) {
        root->right = insert_node(root->right, key);
    }
    return root;
}

int linear_search(int arr[], int size, int target, int* comparisons) {
    *comparisons = 0;
    for (int i = 0; i < size; i++) {
        (*comparisons)++; // 비교할 때마다 횟수 1 증가
        if (arr[i] == target) {
            return i; // 찾았을 경우 인덱스 반환
        }
    }
    return -1; // 찾지 못했을 경우
}

TreeNode* binary_tree_search(TreeNode* root, int target, int* comparisons) {
    *comparisons = 0;
    TreeNode* current = root;
    while (current != NULL) {
        (*comparisons)++; // 비교할 때마다 횟수 1 증가
        if (target == current->key) {
            return current; // 찾았을 경우 해당 노드 포인터 반환
        } else if (target < current->key) {
            current = current->left; // 작으면 왼쪽으로
        } else {
            current = current->right; // 크면 오른쪽으로
        }
    }
    return NULL; // 찾지 못했을 경우
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    srand(time(NULL));

    int numbers[100];
    TreeNode* root = NULL;

    printf("0~1000 사이의 무작위 숫자 100개를 생성합니다...\n");
    printf("--------------------------------------------------\n");

    int count = 0;
    while (count < 100) {
        int num = rand() % 1001;
        int is_duplicate = 0;
        // 중복 검사
        for (int i = 0; i < count; i++) {
            if (numbers[i] == num) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            numbers[count] = num;
            root = insert_node(root, num);
            printf("%d ", numbers[count]);
            count++;
        }
    }
    printf("\n--------------------------------------------------\n\n");

    while (1) {
        int target;
        printf("검색할 숫자를 입력하세요 (종료하려면 -1 입력): ");
        scanf("%d", &target);

        if (target == -1) {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        printf("\n======= '%d' 탐색 결과 비교 =======\n", target);

        // 배열의 선형 탐색
        int linear_comparisons = 0;
        clock_t start_linear = clock(); // 시간 측정 시작
        int found_index = linear_search(numbers, 100, target, &linear_comparisons);
        clock_t end_linear = clock(); // 시간 측정 종료
        double linear_time = ((double)(end_linear - start_linear)) / CLOCKS_PER_SEC;

        printf("1. [배열 선형 탐색]\n");
        if (found_index != -1) {
            printf("   -> 결과를 찾았습니다!\n");
        } else {
            printf("   -> 결과를 찾지 못했습니다.\n");
        }
        printf("   - 탐색 횟수: %d 회\n", linear_comparisons);
        printf("   - 소요 시간: %.8f 초\n", linear_time);


        // 이진 탐색 트리의 이진 탐색
        int bst_comparisons = 0;
        clock_t start_bst = clock(); // 시간 측정 시작
        TreeNode* found_node = binary_tree_search(root, target, &bst_comparisons);
        clock_t end_bst = clock(); // 시간 측정 종료
        double bst_time = ((double)(end_bst - start_bst)) / CLOCKS_PER_SEC;

        printf("\n2. [이진 탐색 트리]\n");
        if (found_node != NULL) {
            printf("   -> 결과를 찾았습니다!\n");
        } else {
            printf("   -> 결과를 찾지 못했습니다.\n");
        }
        printf("   - 탐색 횟수: %d 회\n", bst_comparisons);
        printf("   - 소요 시간: %.8f 초\n", bst_time);


        printf("===================================\n\n");
    }
    free_tree(root);

    return 0;
}
