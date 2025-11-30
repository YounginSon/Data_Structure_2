#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define ITERATIONS 10

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    int sum_grade;
} Student;

typedef int (*CompareFunc)(const Student* a, const Student* b);

long long g_comparisons = 0;

// ==========================================
// 1. 데이터 로드 및 유틸리티
// ==========================================

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패 (dataset_id_ascending.csv 확인 필요)");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);

    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) { free(arr); fclose(fp); return NULL; }
            arr = temp;
        }

        Student s;
        char* token = strtok(line, ",");
        s.id = atoi(token);

        token = strtok(NULL, ",");
        memset(s.name, 0, MAX_NAME_LEN);
        if(token) strncpy(s.name, token, MAX_NAME_LEN - 1);

        token = strtok(NULL, ",");
        s.gender = token ? token[0] : ' ';

        token = strtok(NULL, ",");
        s.korean = atoi(token);

        token = strtok(NULL, ",");
        s.english = atoi(token);

        token = strtok(NULL, ",");
        s.math = atoi(token);

        s.sum_grade = s.korean + s.english + s.math;

        arr[count++] = s;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

void copy_array(Student* dest, Student* src, int n) {
    memcpy(dest, src, sizeof(Student) * n);
}

void reverse_array(Student* arr, int n) {
    for (int i = 0; i < n / 2; i++) {
        Student temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

// ==========================================
// 2. 비교 함수들 (Comparators)
// ==========================================

int cmp_id_asc(const Student* a, const Student* b) { return a->id - b->id; }
int cmp_id_desc(const Student* a, const Student* b) { return b->id - a->id; }

int cmp_name_asc(const Student* a, const Student* b) { return strcmp(a->name, b->name); }
int cmp_name_desc(const Student* a, const Student* b) { return strcmp(b->name, a->name); }

int cmp_gender_asc(const Student* a, const Student* b) { return a->gender - b->gender; }
int cmp_gender_desc(const Student* a, const Student* b) { return b->gender - a->gender; }

int cmp_grade_asc(const Student* a, const Student* b) {
    if (a->sum_grade != b->sum_grade) return a->sum_grade - b->sum_grade;
    if (a->korean != b->korean) return a->korean - b->korean;
    if (a->english != b->english) return a->english - b->english;
    return a->math - b->math;
}
int cmp_grade_desc(const Student* a, const Student* b) {
    if (a->sum_grade != b->sum_grade) return b->sum_grade - a->sum_grade;
    if (a->korean != b->korean) return b->korean - a->korean;
    if (a->english != b->english) return b->english - a->english;
    return b->math - a->math;
}

int compare(const Student* a, const Student* b, CompareFunc comp) {
    g_comparisons++;
    return comp(a, b);
}

void swap(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// ==========================================
// 3. 정렬 알고리즘 구현
// ==========================================

void bubble_sort(Student* arr, int n, CompareFunc comp) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare(&arr[j], &arr[j + 1], comp) > 0) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void selection_sort(Student* arr, int n, CompareFunc comp) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (compare(&arr[j], &arr[min_idx], comp) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) swap(&arr[i], &arr[min_idx]);
    }
}

void insertion_sort(Student* arr, int n, CompareFunc comp) {
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            g_comparisons++;
            if (comp(&arr[j], &key) > 0) {
                arr[j + 1] = arr[j];
                j = j - 1;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

void shell_sort(Student* arr, int n, CompareFunc comp) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap; j -= gap) {
                g_comparisons++;
                if (comp(&arr[j - gap], &temp) > 0) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
}

void quick_sort_recursive(Student* arr, int left, int right, CompareFunc comp) {
    if (left < right) {
        int i = left, j = right;
        Student pivot = arr[(left + right) / 2];

        while (i <= j) {
            while (compare(&arr[i], &pivot, comp) < 0) i++;
            while (compare(&arr[j], &pivot, comp) > 0) j--;
            if (i <= j) {
                swap(&arr[i], &arr[j]);
                i++; j--;
            }
        }
        quick_sort_recursive(arr, left, j, comp);
        quick_sort_recursive(arr, i, right, comp);
    }
}
void quick_sort(Student* arr, int n, CompareFunc comp) {
    quick_sort_recursive(arr, 0, n - 1, comp);
}

void heapify(Student* arr, int n, int i, CompareFunc comp) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && compare(&arr[l], &arr[largest], comp) > 0) largest = l;
    if (r < n && compare(&arr[r], &arr[largest], comp) > 0) largest = r;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest, comp);
    }
}
void heap_sort(Student* arr, int n, CompareFunc comp) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, comp);
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0, comp);
    }
}

void merge(Student* arr, Student* temp, int left, int mid, int right, CompareFunc comp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (compare(&arr[i], &arr[j], comp) <= 0) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; i++) arr[i] = temp[i];
}

void merge_sort_recursive(Student* arr, Student* temp, int left, int right, CompareFunc comp) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort_recursive(arr, temp, left, mid, comp);
        merge_sort_recursive(arr, temp, mid + 1, right, comp);
        merge(arr, temp, left, mid, right, comp);
    }
}
void merge_sort(Student* arr, int n, CompareFunc comp) {
    Student* temp = malloc(sizeof(Student) * n);
    if(temp) {
        merge_sort_recursive(arr, temp, 0, n - 1, comp);
        free(temp);
    }
}

// ==========================================
// 8. Radix Sort
// ==========================================
int get_radix_value(Student s, int mode, int pos_or_exp) {
    if (mode == 0 || mode == 3 || mode == 4 || mode == 5 || mode == 6) {
        int val = 0;
        if (mode == 0) val = s.id;
        else if (mode == 3) val = s.sum_grade;
        else if (mode == 4) val = s.korean;
        else if (mode == 5) val = s.english;
        else if (mode == 6) val = s.math;

        return (val / pos_or_exp) % 10;
    }
    else if (mode == 1) {
        if (pos_or_exp < 0 || pos_or_exp >= MAX_NAME_LEN) return 0;
        return (unsigned char)s.name[pos_or_exp];
    }
    else if (mode == 2) {
        return (unsigned char)s.gender;
    }
    return 0;
}

void count_sort_generic(Student* arr, int n, int mode, int pos_or_exp, int base, int ascending) {
    Student* output = malloc(sizeof(Student) * n);
    int* count = calloc(base, sizeof(int));

    for (int i = 0; i < n; i++) {
        int bucket = get_radix_value(arr[i], mode, pos_or_exp);

        if (!ascending) {
            bucket = base - 1 - bucket;
        }

        count[bucket]++;
    }

    for (int i = 1; i < base; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int bucket = get_radix_value(arr[i], mode, pos_or_exp);

        if (!ascending) {
            bucket = base - 1 - bucket;
        }

        output[count[bucket] - 1] = arr[i];
        count[bucket]--;
    }

    for (int i = 0; i < n; i++) arr[i] = output[i];

    free(count);
    free(output);
}

void radix_sort(Student* arr, int n, int criteria, int ascending) {
    if (criteria == 0) {
        int max_val = 0;
        for (int i = 0; i < n; i++) if (arr[i].id > max_val) max_val = arr[i].id;

        for (int exp = 1; max_val / exp > 0; exp *= 10) {
            count_sort_generic(arr, n, 0, exp, 10, ascending);
        }
    }
    else if (criteria == 1) {
        for (int pos = MAX_NAME_LEN - 1; pos >= 0; pos--) {
            count_sort_generic(arr, n, 1, pos, 256, ascending);
        }
    }
    else if (criteria == 2) {
        count_sort_generic(arr, n, 2, 0, 256, ascending);
    }
    else if (criteria == 3) {
        for (int exp = 1; 100 / exp > 0; exp *= 10)
            count_sort_generic(arr, n, 6, exp, 10, ascending);

        for (int exp = 1; 100 / exp > 0; exp *= 10)
            count_sort_generic(arr, n, 5, exp, 10, ascending);

        for (int exp = 1; 100 / exp > 0; exp *= 10)
            count_sort_generic(arr, n, 4, exp, 10, ascending);

        for (int exp = 1; 300 / exp > 0; exp *= 10)
            count_sort_generic(arr, n, 3, exp, 10, ascending);
    }
}

// ==========================================
// 9. Tree Sort (Modified to Iterative)
// ==========================================
typedef struct Node {
    Student data;
    struct Node* left;
    struct Node* right;
} Node;

Node* create_node(Student s) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = s;
    node->left = node->right = NULL;
    return node;
}

Node* insert_node(Node* root, Student s, CompareFunc comp) {
    if (root == NULL) return create_node(s);

    Node* current = root;
    while (1) {
        g_comparisons++;
        if (comp(&s, &current->data) < 0) {
            if (current->left == NULL) {
                current->left = create_node(s);
                break;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = create_node(s);
                break;
            }
            current = current->right;
        }
    }
    return root;
}

void inorder_traversal(Node* root, Student* arr, int* idx) {
    if (root != NULL) {
        inorder_traversal(root->left, arr, idx);
        arr[(*idx)++] = root->data;
        inorder_traversal(root->right, arr, idx);
    }
}

void free_tree(Node* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

void tree_sort(Student* arr, int n, CompareFunc comp) {
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert_node(root, arr[i], comp);
    }
    int idx = 0;
    inorder_traversal(root, arr, &idx);
    free_tree(root);
}

// ==========================================
// 4. 메인 실행 로직
// ==========================================

void run_sorting_test(Student* original, int n, int criteria, int ascending) {
    char* crit_names[] = {"ID", "NAME", "GENDER", "GRADE"};
    char* order = ascending ? "ASC" : "DESC";

    printf("\n============================================\n");
    printf(" >>> Sorting by %s (%s) \n", crit_names[criteria], order);
    printf("============================================\n");

    CompareFunc comp = NULL;
    if (criteria == 0) comp = ascending ? cmp_id_asc : cmp_id_desc;
    else if (criteria == 1) comp = ascending ? cmp_name_asc : cmp_name_desc;
    else if (criteria == 2) comp = ascending ? cmp_gender_asc : cmp_gender_desc;
    else if (criteria == 3) comp = ascending ? cmp_grade_asc : cmp_grade_desc;

    char* algo_names[] = {
        "Bubble", "Selection", "Insertion", "Shell",
        "Quick", "Heap", "Merge", "Radix", "Tree"
    };

    Student* temp_arr = malloc(sizeof(Student) * n);

    for (int algo = 0; algo < 9; algo++) {
        if ((criteria == 1 || criteria == 3) && (algo == 5 || algo == 8)) {
            continue;
        }

        if (criteria == 2) {
            if (algo == 1 || algo == 3 || algo == 4 || algo == 5 || algo == 8) {
                continue;
            }
        }

        long long total_comp = 0;
        long long mem_usage = 0;

        for (int iter = 0; iter < ITERATIONS; iter++) {
            copy_array(temp_arr, original, n);
            g_comparisons = 0;

            switch(algo) {
                case 0: bubble_sort(temp_arr, n, comp); break;
                case 1: selection_sort(temp_arr, n, comp); break;
                case 2: insertion_sort(temp_arr, n, comp); break;
                case 3: shell_sort(temp_arr, n, comp); break;
                case 4: quick_sort(temp_arr, n, comp); break;
                case 5: heap_sort(temp_arr, n, comp); break;
                case 6: merge_sort(temp_arr, n, comp); break;
                case 7: radix_sort(temp_arr, n, criteria, ascending); break;
                case 8: tree_sort(temp_arr, n, comp); break;
            }
            total_comp += g_comparisons;
        }

        if (algo == 6 || algo == 7) mem_usage = (long long)n * sizeof(Student);
        if (algo == 8) mem_usage = (long long)n * sizeof(Node);

        printf("  [%-10s] comparisons: %12lld | Memory: %10lld bytes\n",
               algo_names[algo], total_comp / ITERATIONS, mem_usage);
    }

    free(temp_arr);
}

int main() {
    int count = 0;
    Student* students = load_students("dataset_id_ascending.csv", &count);

    if (!students) {
        printf("Error: Could not load dataset.\n");
        return 1;
    }

    printf("Loaded %d students successfully.\n", count);

    run_sorting_test(students, count, 0, 1);
    run_sorting_test(students, count, 0, 0);

    run_sorting_test(students, count, 1, 1);
    run_sorting_test(students, count, 1, 0);

    run_sorting_test(students, count, 2, 1);
    run_sorting_test(students, count, 2, 0);

    run_sorting_test(students, count, 3, 1);
    run_sorting_test(students, count, 3, 0);

    free(students);
    return 0;
}