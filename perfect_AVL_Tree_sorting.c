#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// [1] 데이터 구조 및 설정
// ==========================================
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
} Student;

typedef struct AVLNode {
    Student data;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// 전역 변수: 비교 횟수 카운팅
long long avl_comparisons = 0;

// ==========================================
// [2] 유틸리티 및 Helper 함수
// ==========================================

int get_height(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* create_node(Student data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

// 배열 뒤집기 (데이터 이동만 함 -> 비교 횟수 0)
void reverse_array(Student* arr, int n) {
    int left = 0, right = n - 1;
    while (left < right) {
        Student temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++; right--;
    }
}

// ==========================================
// [3] Standard AVL Insert (무작위 데이터용)
// ==========================================

AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    return x;
}

AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    return y;
}

int get_balance(AVLNode* node) {
    if (node == NULL) return 0;
    return get_height(node->left) - get_height(node->right);
}

AVLNode* insert_standard(AVLNode* node, Student data) {
    if (node == NULL) return create_node(data);

    avl_comparisons++;
    if (data.id < node->data.id)
        node->left = insert_standard(node->left, data);
    else if (data.id > node->data.id)
        node->right = insert_standard(node->right, data);
    else
        return node;

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance(node);

    if (balance > 1) {
        avl_comparisons++;
        if (data.id < node->left->data.id) return right_rotate(node);
        else { node->left = left_rotate(node->left); return right_rotate(node); }
    }
    if (balance < -1) {
        avl_comparisons++;
        if (data.id > node->right->data.id) return left_rotate(node);
        else { node->right = right_rotate(node->right); return left_rotate(node); }
    }
    return node;
}

// ==========================================
// [4] Optimized Construction (정렬된 데이터용)
// ==========================================

// 분할 정복으로 트리 조립 (비교 0회)
AVLNode* sorted_array_to_avl(Student* arr, int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    AVLNode* node = create_node(arr[mid]);
    node->left = sorted_array_to_avl(arr, start, mid - 1);
    node->right = sorted_array_to_avl(arr, mid + 1, end);
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    return node;
}

// ==========================================
// [5] Smart Builder Logic (핵심!)
// ==========================================

// 오름차순 확인
int is_sorted_asc(Student* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        avl_comparisons++;
        if (arr[i].id > arr[i+1].id) return 0;
    }
    return 1;
}

// 내림차순 확인
int is_sorted_desc(Student* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        avl_comparisons++;
        if (arr[i].id < arr[i+1].id) return 0;
    }
    return 1;
}

// [핵심] 상황에 따라 오름차순/내림차순/랜덤 모두 대응
AVLNode* build_smart_avl(Student* arr, int n) {
    // 1. 오름차순 검사 (Best Case 1)
    if (is_sorted_asc(arr, n)) {
        // 이미 오름차순 -> 바로 조립
        return sorted_array_to_avl(arr, 0, n - 1);
    }

    // 2. 내림차순 검사 (Best Case 2)
    // 주의: 앞선 검사에서 실패했으므로 comparisons 누적됨
    if (is_sorted_desc(arr, n)) {
        // 역순이네? -> 뒤집자! (ID 비교 비용 0)
        reverse_array(arr, n);
        // 뒤집으면 오름차순이 되므로 바로 조립
        return sorted_array_to_avl(arr, 0, n - 1);
    }

    // 3. 랜덤 데이터 (General Case)
    // 어쩔 수 없이 표준 삽입 수행
    AVLNode* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert_standard(root, arr[i]);
    }
    return root;
}

// 메모리 해제
void free_avl(AVLNode* root) {
    if (root != NULL) {
        free_avl(root->left);
        free_avl(root->right);
        free(root);
    }
}

// 파일 로드
Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) { perror("File Open Error"); return NULL; }
    char line[MAX_LINE_LEN];
    int capacity = 10, count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            arr = realloc(arr, sizeof(Student) * capacity);
        }
        Student s;
        char* token = strtok(line, ","); s.id = atoi(token);
        token = strtok(NULL, ","); strncpy(s.name, token, MAX_NAME_LEN);
        token = strtok(NULL, ","); s.gender = token[0];
        token = strtok(NULL, ","); s.korean = atoi(token);
        token = strtok(NULL, ","); s.english = atoi(token);
        token = strtok(NULL, ","); s.math = atoi(token);
        arr[count++] = s;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

int main() {
    int count = 0;
    Student* original_data = load_students("dataset_id_ascending.csv", &count);

    if (!original_data) {
        printf("Error: 파일을 찾을 수 없습니다.\n");
        return 1;
    }

    printf("Smart AVL Tree Optimization (Auto-Detect Asc/Desc)\n");
    printf("Total Students: %d\n", count);
    printf("Performing 10 iterations per test case...\n\n");

    Student* test_data = malloc(sizeof(Student) * count);

    // =========================================================
    // TEST 1: 오름차순 데이터 (ID Ascending)
    // =========================================================
    long long total_comparisons_1 = 0;

    for (int k = 0; k < 10; k++) {
        // 데이터 초기화
        memcpy(test_data, original_data, sizeof(Student) * count);

        avl_comparisons = 0;
        AVLNode* root = build_smart_avl(test_data, count);
        total_comparisons_1 += avl_comparisons;

        // 메모리 해제 (반복 실행 시 필수)
        free_avl(root);
    }

    printf("[1. ID Ascending Input]\n");
    printf("  - Average Comparisons (10 runs): %lld\n", total_comparisons_1 / 10);
    printf("  - Expectation: Low (Approx N) -> Detected Ascending\n\n");


    // =========================================================
    // TEST 2: 내림차순 데이터 (ID Descending)
    // =========================================================
    long long total_comparisons_2 = 0;

    for (int k = 0; k < 10; k++) {
        // 데이터 초기화 후 강제로 뒤집어서 '내림차순 입력' 상태 만들기
        memcpy(test_data, original_data, sizeof(Student) * count);
        reverse_array(test_data, count);

        avl_comparisons = 0;
        AVLNode* root = build_smart_avl(test_data, count);
        total_comparisons_2 += avl_comparisons;

        free_avl(root);
    }

    printf("[2. ID Descending Input]\n");
    printf("  - Average Comparisons (10 runs): %lld\n", total_comparisons_2 / 10);
    printf("  - Expectation: Low (Approx N) -> Detected Descending & Reversed\n\n");

    free(test_data);
    free(original_data);
    return 0;
}