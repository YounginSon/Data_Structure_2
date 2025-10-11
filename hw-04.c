#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*
 1. 이진 탐색트리를 연결자료형으로 생성
 2. 숫자를 검색하게 하고, -1을 입력하면 검색이 종료되는 시스템으로 구성하는게 좋음.(검색할떄 마다 실행을 하는건 너무 비효율적임)

 먼저 배열에 100개의 랜덤한 정수를 넣고, arr[0]에 있는 값을 연결리스트 자료형을 이용한 이진탐색 트리에 root로 만들어야함.
 그래서 root보다 다음 배열의 숫자가 크면 부모노드의 오른쪽에 저장하고, 작으면 왼쪽 노드에 저장하는 식으로 계속 계단식으로 내려가겠끔 저장하게 만들기.

 구조체를 활용하여 Key, left, right로 만들기.
 left와 right가 NULL이면 다음 주소를 가르키고 있지 않는 것이기 때문에 leaf노드로 판단하면 될 듯.

 선형으로 검색할 때와 이진 탐색 트리를 통해 검색할 때 횟수는 comparisons라는 변수를 활용해서 방문할 때마다 ++시켜주면 그만임.

 시간 부분은 제미나이 -> clock을 이용해서 시작할 때 스톱워치를 키고, 끝나면 스톱워치를 종료하는 식으로 측정함.
 
 */

typedef struct TreeNode
{
    int key;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* create_node(int key)
{
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

TreeNode* insert_node(TreeNode* root, int key)
{
    if (root == NULL)
        return create_node(key);
    if (key < root->key)
        root->left = insert_node(root->left, key);
    else if (key > root->key)
        root->right = insert_node(root->right, key);
    return root;
}

int linear_search(int arr[], int size, int target, int* comparisons)
{
    *comparisons = 0;
    for (int i = 0; i < size; i++) 
    {
        (*comparisons)++;
        if (arr[i] == target)
            return i;
    }
    return -1;
}

TreeNode* binary_tree_search(TreeNode* root, int target, int* comparisons)
{
    *comparisons = 0;
    TreeNode* current = root;
    while (current != NULL)
    {
        (*comparisons)++;
        if (target == current->key)
            return current;
        else if (target < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

void free_tree(TreeNode* root)
{
    if (root != NULL)
    {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main(void)
{
    srand(time(NULL));

    int numbers[100];
    TreeNode* root = NULL;

    printf("0~1000 사이의 무작위 숫자 100개를 생성합니다...\n");
    printf("--------------------------------------------------\n");

    int count = 0;
    while (count < 100)
    {
        int num = rand() % 1001;
        int is_duplicate = 0;
        for (int i = 0; i < count; i++)
        {
            if (numbers[i] == num)
            {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate)
        {
            numbers[count] = num;
            root = insert_node(root, num);
            printf("%d ", numbers[count]);
            count++;
        }
    }
    printf("\n--------------------------------------------------\n\n");

    while (1)
    {
        int target;
        printf("검색할 숫자를 입력하세요 (종료하려면 -1 입력): ");
        scanf("%d", &target);

        if (target == -1)
        {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        printf("\n======= '%d' 탐색 결과 비교 =======\n", target);

        int linear_comparisons = 0;
        clock_t start_linear = clock();
        int found_index = linear_search(numbers, 100, target, &linear_comparisons);
        clock_t end_linear = clock();
        double linear_time = ((double)(end_linear - start_linear)) / CLOCKS_PER_SEC;

        printf("1. [배열 선형 탐색]\n");
        if (found_index != -1)
            printf("   -> 결과를 찾았습니다!\n");
        else
            printf("   -> 결과를 찾지 못했습니다.\n");
        printf("   - 탐색 횟수: %d 회\n", linear_comparisons);
        printf("   - 소요 시간: %.8f 초\n", linear_time);

        int bst_comparisons = 0;
        clock_t start_bst = clock();
        TreeNode* found_node = binary_tree_search(root, target, &bst_comparisons);
        clock_t end_bst = clock();
        double bst_time = ((double)(end_bst - start_bst)) / CLOCKS_PER_SEC;

        printf("\n2. [이진 탐색 트리]\n");
        if (found_node != NULL)
            printf("   -> 결과를 찾았습니다!\n");
        else
            printf("   -> 결과를 찾지 못했습니다.\n");
        printf("   - 탐색 횟수: %d 회\n", bst_comparisons);
        printf("   - 소요 시간: %.8f 초\n", bst_time);

        printf("\n3. [종합 비교]\n");
        int comparison_diff = linear_comparisons - bst_comparisons;

        if (comparison_diff > 0)
            printf("   -> 이진 탐색 트리가 %d회 더 적게 비교하여 효율적이었습니다.\n", comparison_diff);
        else if (comparison_diff < 0)
            printf("   -> 드물게 배열 탐색이 %d회 더 적게 비교했습니다.\n", -comparison_diff);
        else
            printf("   -> 두 방식의 탐색 횟수가 동일했습니다.\n");

        if (linear_time > bst_time)
            printf("   -> 이진 탐색 트리가 %.8f초 더 빨랐습니다.\n", linear_time - bst_time);
        else
            printf("   -> 시간 차이는 거의 없거나 배열 탐색이 더 빨랐습니다.\n");

        printf("===================================\n\n");
    }
    free_tree(root);

    return 0;
}
