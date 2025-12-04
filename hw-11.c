#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);

    if (!arr) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) {
                perror("Reallocation failed");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = temp;
        }

        Student s;
        char* token = strtok(line, ",");
        if(token) s.id = atoi(token);

        token = strtok(NULL, ",");
        if(token) strncpy(s.name, token, MAX_NAME_LEN);

        token = strtok(NULL, ",");
        if(token) s.gender = token[0];

        token = strtok(NULL, ",");
        if(token) s.korean = atoi(token);

        token = strtok(NULL, ",");
        if(token) s.english = atoi(token);

        token = strtok(NULL, ",");
        if(token) s.math = atoi(token);

        arr[count++] = s;
    }

    fclose(fp);

    Student* tight = realloc(arr, sizeof(Student) * count);
    if (!tight) {
        *out_count = count;
        return arr;
    }
    arr = tight;
    *out_count = count;
    return arr;
}

void UA_Insert(Student** arr, int* count, int* capacity, Student data, long long* cp) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *arr = realloc(*arr, sizeof(Student) * (*capacity));
    }
    (*arr)[*count] = data;
    (*count)++;
}

int UA_Search(Student* arr, int count, int id, long long* cp) {
    for (int i = 0; i < count; i++) {
        (*cp)++;
        if (arr[i].id == id) {
            return i;
        }
    }
    return -1;
}

void UA_Delete(Student* arr, int* count, int id, long long* cp) {
    int idx = UA_Search(arr, *count, id, cp); // 검색 비용 포함
    if (idx != -1) {
        if (idx != *count - 1) {
            arr[idx] = arr[*count - 1];
        }
        (*count)--;
    }
}

int SA_BinarySearch_Index(Student* arr, int count, int id, long long* cp) {
    int left = 0;
    int right = count - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*cp)++; // arr[mid].id 와 id 비교
        if (arr[mid].id == id) return mid;

        if (arr[mid].id < id) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int SA_FindInsertIndex(Student* arr, int count, int id, long long* cp) {
    int left = 0;
    int right = count - 1;
    int result = count;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*cp)++;
        if (arr[mid].id == id) return mid;

        if (arr[mid].id < id) {
            left = mid + 1;
        } else {
            result = mid;
            right = mid - 1;
        }
    }
    return result;
}

void SA_Insert(Student** arr, int* count, int* capacity, Student data, long long* cp) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *arr = realloc(*arr, sizeof(Student) * (*capacity));
    }

    int idx = SA_FindInsertIndex(*arr, *count, data.id, cp);

    if (idx < *count) {
        memmove(&(*arr)[idx + 1], &(*arr)[idx], sizeof(Student) * (*count - idx));
    }

    (*arr)[idx] = data;
    (*count)++;
}

int SA_Search(Student* arr, int count, int id, long long* cp) {
    return SA_BinarySearch_Index(arr, count, id, cp);
}

void SA_Delete(Student* arr, int* count, int id, long long* cp) {
    int idx = SA_BinarySearch_Index(arr, *count, id, cp);
    if (idx != -1) {
        if (idx < *count - 1) {
            memmove(&arr[idx], &arr[idx + 1], sizeof(Student) * (*count - 1 - idx));
        }
        (*count)--;
    }
}

int max(int a, int b) { return (a > b) ? a : b; }
int getHeight(AVLNode* N) { return (N == NULL) ? 0 : N->height; }

AVLNode* newNode(Student data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

int getBalance(AVLNode* N) {
    if (N == NULL) return 0;
    return getHeight(N->left) - getHeight(N->right);
}

AVLNode* AVL_Insert(AVLNode* node, Student data, long long* cp) {
    if (node == NULL) return newNode(data);

    (*cp)++;
    if (data.id < node->data.id)
        node->left = AVL_Insert(node->left, data, cp);
    else if (data.id > node->data.id)
        node->right = AVL_Insert(node->right, data, cp);
    else
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && data.id < node->left->data.id) {
        (*cp)++;
        return rightRotate(node);
    }
    
    if (balance < -1 && data.id > node->right->data.id) {
        (*cp)++;
        return leftRotate(node);
    }
    
    if (balance > 1 && data.id > node->left->data.id) {
        (*cp)++;
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    if (balance < -1 && data.id < node->right->data.id) {
        (*cp)++;
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* AVL_Search(AVLNode* root, int id, long long* cp) {
    if (root == NULL) return NULL;

    (*cp)++;
    if (root->data.id == id) return root;

    if (root->data.id < id) return AVL_Search(root->right, id, cp);
    return AVL_Search(root->left, id, cp);
}

AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL) current = current->left;
    return current;
}

AVLNode* AVL_Delete(AVLNode* root, int id, long long* cp) {
    if (root == NULL) return root;

    (*cp)++;
    if (id < root->data.id)
        root->left = AVL_Delete(root->left, id, cp);
    else if (id > root->data.id)
        root->right = AVL_Delete(root->right, id, cp);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data; // 데이터 복사
            root->right = AVL_Delete(root->right, temp->data.id, cp);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void free_AVL(AVLNode* root) {
    if(root == NULL) return;
    free_AVL(root->left);
    free_AVL(root->right);
    free(root);
}

int main() {
    int total_students = 0;
    Student* raw_data = load_students("dataset_id_ascending.csv", &total_students);

    if (!raw_data) return 1;

    printf("Total students loaded: %d\n\n", total_students);

    long long ua_comp = 0, sa_comp = 0, avl_comp = 0;

    int ua_cap = 10, ua_cnt = 0;
    Student* ua_arr = malloc(sizeof(Student) * ua_cap);

    int sa_cap = 10, sa_cnt = 0;
    Student* sa_arr = malloc(sizeof(Student) * sa_cap);

    AVLNode* avl_root = NULL;

    printf("[1] Inserting %d items...\n", total_students);

    ua_comp = 0;
    for(int i=0; i<total_students; i++) UA_Insert(&ua_arr, &ua_cnt, &ua_cap, raw_data[i], &ua_comp);
    printf("Unsorted Array Insert Comparisons: %lld\n", ua_comp);

    sa_comp = 0;
    for(int i=0; i<total_students; i++) SA_Insert(&sa_arr, &sa_cnt, &sa_cap, raw_data[i], &sa_comp);
    printf("Sorted Array Insert Comparisons:   %lld\n", sa_comp);

    avl_comp = 0;
    for(int i=0; i<total_students; i++) avl_root = AVL_Insert(avl_root, raw_data[i], &avl_comp);
    printf("AVL Tree Insert Comparisons:       %lld\n\n", avl_comp);

    int search_targets[] = {
        raw_data[0].id,
        raw_data[total_students/2].id,
        raw_data[total_students-1].id,
        999999
    };
    int num_searches = 4;

    printf("[2] Searching %d items...\n", num_searches);

    ua_comp = 0;
    for(int i=0; i<num_searches; i++) UA_Search(ua_arr, ua_cnt, search_targets[i], &ua_comp);
    printf("Unsorted Array Search Comparisons: %lld\n", ua_comp);

    sa_comp = 0;
    for(int i=0; i<num_searches; i++) SA_Search(sa_arr, sa_cnt, search_targets[i], &sa_comp);
    printf("Sorted Array Search Comparisons:   %lld\n", sa_comp);

    avl_comp = 0;
    for(int i=0; i<num_searches; i++) AVL_Search(avl_root, search_targets[i], &avl_comp);
    printf("AVL Tree Search Comparisons:       %lld\n\n", avl_comp);

    printf("[3] Deleting %d items...\n", num_searches);

    ua_comp = 0;
    for(int i=0; i<num_searches; i++) UA_Delete(ua_arr, &ua_cnt, search_targets[i], &ua_comp);
    printf("Unsorted Array Delete Comparisons: %lld\n", ua_comp);

    sa_comp = 0;
    for(int i=0; i<num_searches; i++) SA_Delete(sa_arr, &sa_cnt, search_targets[i], &sa_comp);
    printf("Sorted Array Delete Comparisons:   %lld\n", sa_comp);

    avl_comp = 0;
    for(int i=0; i<num_searches; i++) avl_root = AVL_Delete(avl_root, search_targets[i], &avl_comp);
    printf("AVL Tree Delete Comparisons:       %lld\n\n", avl_comp);

    free(raw_data);
    free(ua_arr);
    free(sa_arr);
    free_AVL(avl_root);

    return 0;
}
