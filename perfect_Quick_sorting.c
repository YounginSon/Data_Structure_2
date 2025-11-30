// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// // ==========================================
// // [1] 데이터 구조 및 설정
// // ==========================================
// #define MAX_NAME_LEN 50
// #define MAX_LINE_LEN 200
//
// // [최적화 파라미터]
// // Random Data 시뮬레이션 결과, Threshold 12 구간에서 삽입 정렬 전환 시 효율 최고
// #define DUAL_PIVOT_THRESHOLD 12
//
// typedef struct {
//     int id;
//     char name[MAX_NAME_LEN];
//     char gender;
//     int korean;
//     int english;
//     int math;
// } Student;
//
// // 비교 함수 포인터 타입 정의
// typedef int (*CompareFunc)(const Student* a, const Student* b);
//
// // ==========================================
// // [2] 파일 입출력 함수
// // ==========================================
//
// // CSV 파일 저장
// void save_students(const char* filename, Student* arr, int n) {
//     FILE* fp = fopen(filename, "w");
//     if (!fp) {
//         perror("Failed to open file for writing");
//         return;
//     }
//     fprintf(fp, "ID,NAME,GENDER,KOREAN_GRADE,ENGLISH_GRADE,MATH_GRADE\n");
//     for (int i = 0; i < n; i++) {
//         fprintf(fp, "%d,%s,%c,%d,%d,%d\n",
//             arr[i].id, arr[i].name, arr[i].gender,
//             arr[i].korean, arr[i].english, arr[i].math);
//     }
//     fclose(fp);
// }
//
// // 학생 데이터 로드
// Student* load_students(const char* filename, int* out_count) {
//     FILE* fp = fopen(filename, "r");
//     if (!fp) {
//         perror("Failed to open file");
//         return NULL;
//     }
//
//     char line[MAX_LINE_LEN];
//     int capacity = 10;
//     int count = 0;
//     Student* arr = malloc(sizeof(Student) * capacity);
//
//     if (!arr) return NULL;
//
//     fgets(line, sizeof(line), fp); // 헤더 스킵
//
//     while (fgets(line, sizeof(line), fp)) {
//         if (count >= capacity) {
//             capacity *= 2;
//             Student* temp = realloc(arr, sizeof(Student) * capacity);
//             if (!temp) { free(arr); fclose(fp); return NULL; }
//             arr = temp;
//         }
//
//         Student s;
//         char* token = strtok(line, ",");
//         if(token) s.id = atoi(token);
//
//         token = strtok(NULL, ",");
//         if(token) strncpy(s.name, token, MAX_NAME_LEN);
//
//         token = strtok(NULL, ",");
//         if(token) s.gender = token[0];
//
//         token = strtok(NULL, ",");
//         if(token) s.korean = atoi(token);
//
//         token = strtok(NULL, ",");
//         if(token) s.english = atoi(token);
//
//         token = strtok(NULL, ",");
//         if(token) s.math = atoi(token);
//
//         arr[count++] = s;
//     }
//     fclose(fp);
//
//     Student* tight = realloc(arr, sizeof(Student) * count);
//     if (tight) arr = tight;
//
//     *out_count = count;
//     return arr;
// }
//
// // ==========================================
// // [3] Quick Sort Optimization Logic
// // ==========================================
//
// // 1. 작은 구간에 대한 삽입 정렬 (Insertion Sort)
// void insertion_sort_range(Student* arr, int low, int high, CompareFunc cmp, long long* comparisons) {
//     for (int i = low + 1; i <= high; i++) {
//         Student temp = arr[i];
//         int j = i;
//         while (j > low) {
//             (*comparisons)++;
//             if (cmp(&arr[j - 1], &temp) > 0) {
//                 arr[j] = arr[j - 1];
//                 j--;
//             } else {
//                 break;
//             }
//         }
//         arr[j] = temp;
//     }
// }
//
// // 2. 피벗 선정을 위한 5개 원소 정렬 (Sorting Network)
// void sort_5_elements(Student* arr, int m1, int m2, int m3, int m4, int m5, CompareFunc cmp, long long* comparisons) {
//     #define SWAP_IF_GREATER(i, j) { \
//         (*comparisons)++; \
//         if (cmp(&arr[i], &arr[j]) > 0) { \
//             Student t = arr[i]; arr[i] = arr[j]; arr[j] = t; \
//         } \
//     }
//
//     SWAP_IF_GREATER(m1, m2); SWAP_IF_GREATER(m4, m5);
//     SWAP_IF_GREATER(m1, m3); SWAP_IF_GREATER(m2, m3);
//     SWAP_IF_GREATER(m1, m4); SWAP_IF_GREATER(m3, m4);
//     SWAP_IF_GREATER(m2, m5); SWAP_IF_GREATER(m2, m3);
//     SWAP_IF_GREATER(m4, m5);
//
//     #undef SWAP_IF_GREATER
// }
//
// // 3. 핵심 알고리즘: Dual-Pivot Quick Sort
// void dual_pivot_quick_sort(Student* arr, int low, int high, CompareFunc cmp, long long* comparisons) {
//     if (high - low < DUAL_PIVOT_THRESHOLD) {
//         insertion_sort_range(arr, low, high, cmp, comparisons);
//         return;
//     }
//
//     // Median of 5 Strategy
//     int length = high - low + 1;
//     int sixth = length / 6;
//     int m1 = low + sixth;
//     int m2 = low + 2 * sixth;
//     int m3 = low + length / 2;
//     int m4 = high - 2 * sixth;
//     int m5 = high - sixth;
//
//     sort_5_elements(arr, m1, m2, m3, m4, m5, cmp, comparisons);
//
//     // 피벗 배치 (P1: 2/6지점, P2: 4/6지점)
//     Student t1 = arr[low]; arr[low] = arr[m2]; arr[m2] = t1;
//     Student t2 = arr[high]; arr[high] = arr[m4]; arr[m4] = t2;
//
//     Student p1 = arr[low];
//     Student p2 = arr[high];
//
//     // 파티셔닝
//     int l = low + 1;
//     int g = high - 1;
//     int k = l;
//
//     while (k <= g) {
//         (*comparisons)++;
//         if (cmp(&arr[k], &p1) < 0) {
//             Student temp = arr[k]; arr[k] = arr[l]; arr[l] = temp;
//             l++;
//         }
//         else {
//             (*comparisons)++;
//             if (cmp(&arr[k], &p2) > 0) {
//                 while (k < g) {
//                     (*comparisons)++;
//                     if (cmp(&arr[g], &p2) > 0) {
//                         g--;
//                     } else {
//                         break;
//                     }
//                 }
//                 Student temp = arr[k]; arr[k] = arr[g]; arr[g] = temp;
//                 g--;
//
//                 (*comparisons)++;
//                 if (cmp(&arr[k], &p1) < 0) {
//                     Student temp2 = arr[k]; arr[k] = arr[l]; arr[l] = temp2;
//                     l++;
//                 }
//             }
//         }
//         k++;
//     }
//
//     l--; g++;
//     Student temp1 = arr[low]; arr[low] = arr[l]; arr[l] = temp1;
//     Student temp2 = arr[high]; arr[high] = arr[g]; arr[g] = temp2;
//
//     // 재귀 호출
//     dual_pivot_quick_sort(arr, low, l - 1, cmp, comparisons);
//     (*comparisons)++;
//     if (cmp(&p1, &p2) < 0) {
//         dual_pivot_quick_sort(arr, l + 1, g - 1, cmp, comparisons);
//     }
//     dual_pivot_quick_sort(arr, g + 1, high, cmp, comparisons);
// }
//
// // 4. [전처리 최적화 1] 이미 정렬된 상태 확인 (Early Exit)
// int is_already_sorted(Student* arr, int n, CompareFunc cmp, long long* comparisons) {
//     for (int i = 0; i < n - 1; i++) {
//         (*comparisons)++;
//         if (cmp(&arr[i], &arr[i+1]) > 0) return 0; // 정렬 안됨
//     }
//     return 1; // 정렬됨
// }
//
// // 5. [전처리 최적화 2] 역순 정렬 상태 확인 (Reverse Check)
// int is_reverse_sorted(Student* arr, int n, CompareFunc cmp, long long* comparisons) {
//     for (int i = 0; i < n - 1; i++) {
//         (*comparisons)++;
//         if (cmp(&arr[i], &arr[i+1]) < 0) return 0; // 정방향 요소가 있음 (역순 아님)
//     }
//     return 1; // 완벽한 역순
// }
//
// // 6. 배열 뒤집기 유틸리티
// void reverse_array(Student* arr, int n) {
//     int left = 0, right = n - 1;
//     while (left < right) {
//         Student temp = arr[left];
//         arr[left] = arr[right];
//         arr[right] = temp;
//         left++; right--;
//     }
// }
//
// // 7. 메인 정렬 래퍼 함수 (최종 호출용)
// long long quick_sort_optimized(Student* arr, int n, CompareFunc cmp) {
//     long long comparisons = 0;
//
//     // [Step 1] 이미 정렬된 경우 -> 즉시 종료 (Cost: N)
//     if (is_already_sorted(arr, n, cmp, &comparisons)) {
//         return comparisons;
//     }
//
//     // [Step 2] 역순 정렬된 경우 -> 뒤집고 종료 (Cost: N + Swap)
//     if (is_reverse_sorted(arr, n, cmp, &comparisons)) {
//         reverse_array(arr, n);
//         return comparisons;
//     }
//
//     // [Step 3] 무작위 데이터 -> Dual-Pivot Quick Sort 수행 (Cost: N log N)
//     dual_pivot_quick_sort(arr, 0, n - 1, cmp, &comparisons);
//
//     return comparisons;
// }
//
// // ==========================================
// // [4] 비교 함수 정의
// // ==========================================
//
// // 1. ID 기준
// int cmp_id_asc(const Student* a, const Student* b) { return a->id - b->id; }
// int cmp_id_desc(const Student* a, const Student* b) { return b->id - a->id; }
//
// // 2. 이름 기준
// int cmp_name_asc(const Student* a, const Student* b) { return strcmp(a->name, b->name); }
// int cmp_name_desc(const Student* a, const Student* b) { return strcmp(b->name, a->name); }
//
// // 3. 성적 기준 (합계 -> 국어 -> 영어 -> 수학)
// int cmp_grade_asc(const Student* a, const Student* b) {
//     int sum_a = a->korean + a->english + a->math;
//     int sum_b = b->korean + b->english + b->math;
//     if (sum_a != sum_b) return sum_a - sum_b;
//     if (a->korean != b->korean) return b->korean - a->korean;
//     if (a->english != b->english) return b->english - a->english;
//     return b->math - a->math;
// }
// int cmp_grade_desc(const Student* a, const Student* b) {
//     int sum_a = a->korean + a->english + a->math;
//     int sum_b = b->korean + b->english + b->math;
//     if (sum_a != sum_b) return sum_b - sum_a;
//     if (a->korean != b->korean) return b->korean - a->korean;
//     if (a->english != b->english) return b->english - a->english;
//     return b->math - a->math;
// }
//
// // ==========================================
// // [5] 메인 함수
// // ==========================================
//
// int main() {
//     int count = 0;
//     Student* original_data = load_students("dataset_id_ascending.csv", &count);
//
//     if (!original_data) {
//         printf("Error: 파일을 찾을 수 없습니다.\n");
//         return 1;
//     }
//
//     printf("Quick Sort Optimization Test (Dual-Pivot + Median-of-5 + Pre-checks)\n");
//     printf("Total Students Loaded: %d\n", count);
//     printf("Performing 10 iterations per test case...\n\n");
//
//     Student* test_arr = malloc(sizeof(Student) * count);
//
//     // *Gender 항목 제외* (Quick Sort는 Unstable 하므로)
//     struct {
//         char* title;
//         char* filename;
//         CompareFunc func;
//     } tests[] = {
//         {"1. ID Ascending", "qs_opt_1_ID_Asc.csv", cmp_id_asc},
//         {"1. ID Descending", "qs_opt_1_ID_Desc.csv", cmp_id_desc},
//         {"2. Name Ascending", "qs_opt_2_Name_Asc.csv", cmp_name_asc},
//         {"2. Name Descending", "qs_opt_2_Name_Desc.csv", cmp_name_desc},
//         {"3. Grade Complex Ascending", "qs_opt_3_Grade_Asc.csv", cmp_grade_asc},
//         {"3. Grade Complex Descending", "qs_opt_3_Grade_Desc.csv", cmp_grade_desc},
//     };
//
//     int num_tests = sizeof(tests) / sizeof(tests[0]);
//
//     for (int i = 0; i < num_tests; i++) {
//         long long total_comparisons = 0;
//
//         // [수정됨] 10회 반복 수행
//         for (int k = 0; k < 10; k++) {
//             // 매 반복마다 데이터 초기화 (정렬되지 않은 원본 상태로 복구)
//             // 이것이 없으면 2번째 반복부터는 Early Exit에 걸려버림
//             memcpy(test_arr, original_data, sizeof(Student) * count);
//
//             total_comparisons += quick_sort_optimized(test_arr, count, tests[i].func);
//         }
//
//         // 평균 계산
//         long long average_comparisons = total_comparisons / 10;
//
//         printf("[%s]\n", tests[i].title);
//         printf("  - Average Comparisons (10 runs): %lld\n\n", average_comparisons);
//     }
//
//     free(test_arr);
//     free(original_data);
//     return 0;
// }