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
//     if (!arr) {
//         perror("Memory allocation failed");
//         fclose(fp);
//         return NULL;
//     }
//
//     // 헤더 스킵
//     fgets(line, sizeof(line), fp);
//
//     while (fgets(line, sizeof(line), fp)) {
//         if (count >= capacity) {
//             capacity *= 2;
//             Student* temp = realloc(arr, sizeof(Student) * capacity);
//             if (!temp) {
//                 perror("Reallocation failed");
//                 free(arr);
//                 fclose(fp);
//                 return NULL;
//             }
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
//     *out_count = count;
//     return arr;
// }
//
// // ==========================================
// // [3] Shell Sort Logic (Ciura + Pre-checks)
// // ==========================================
//
// // 1. Ciura 간격 생성 함수
// int* generate_ciura_gaps(int n, int* gap_count) {
//     int base_gaps[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
//     int base_len = sizeof(base_gaps) / sizeof(base_gaps[0]);
//
//     int* gaps = malloc(sizeof(int) * 50);
//     int count = 0;
//
//     // 기본 수열 복사
//     for (int i = 0; i < base_len; i++) {
//         if (base_gaps[i] >= n) break;
//         gaps[count++] = base_gaps[i];
//     }
//
//     // 확장 (2.25배)
//     while (1) {
//         int next = (int)(gaps[count - 1] * 2.25);
//         if (next >= n) break;
//         gaps[count++] = next;
//     }
//
//     // 큰 간격부터 수행해야 하므로 배열 뒤집기 (Reverse)
//     for(int i=0; i<count/2; i++) {
//         int temp = gaps[i];
//         gaps[i] = gaps[count-1-i];
//         gaps[count-1-i] = temp;
//     }
//
//     *gap_count = count;
//     return gaps;
// }
//
// // 2. [전처리] 정렬 여부 확인 (Early Exit)
// int is_sorted(Student* arr, int n, CompareFunc cmp, long long* comparisons) {
//     for (int i = 0; i < n - 1; i++) {
//         (*comparisons)++;
//         if (cmp(&arr[i], &arr[i+1]) > 0) return 0; // 오름차순 깨짐
//     }
//     return 1;
// }
//
// // 3. [전처리] 역순 정렬 여부 확인 (Reverse Check)
// int is_reverse_sorted(Student* arr, int n, CompareFunc cmp, long long* comparisons) {
//     for (int i = 0; i < n - 1; i++) {
//         (*comparisons)++;
//         if (cmp(&arr[i], &arr[i+1]) < 0) return 0; // 역순 깨짐
//     }
//     return 1;
// }
//
// // 4. [전처리] 배열 뒤집기 (Swap)
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
// // 5. 셸 정렬 메인 함수
// long long shell_sort_ciura(Student* arr, int n, CompareFunc cmp) {
//     long long comparisons = 0;
//
//     // [최적화 1] 이미 정렬된 경우 -> 즉시 종료 (Cost: 1 * N)
//     if (is_sorted(arr, n, cmp, &comparisons)) {
//         return comparisons;
//     }
//
//     // [최적화 2] 역순인 경우 -> 뒤집고 종료 (Cost: 1 * N)
//     if (is_reverse_sorted(arr, n, cmp, &comparisons)) {
//         reverse_array(arr, n);
//         return comparisons;
//     }
//
//     // [본 알고리즘] 무작위 데이터 -> Ciura Shell Sort 수행
//     int gap_count = 0;
//     int* gaps = generate_ciura_gaps(n, &gap_count);
//
//     for (int g = 0; g < gap_count; g++) {
//         int gap = gaps[g];
//         for (int i = gap; i < n; i++) {
//             Student temp = arr[i];
//             int j = i;
//             while (j >= gap) {
//                 comparisons++;
//                 if (cmp(&arr[j - gap], &temp) > 0) {
//                     arr[j] = arr[j - gap];
//                     j -= gap;
//                 } else {
//                     break;
//                 }
//             }
//             arr[j] = temp;
//         }
//     }
//
//     free(gaps);
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
// // 3. 성적 합계 기준 (복합 정렬)
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
//     // 파일명은 실제 환경에 맞게 수정 필요
//     Student* original_data = load_students("dataset_id_ascending.csv", &count);
//
//     if (!original_data) {
//         printf("Error: 파일을 찾을 수 없습니다.\n");
//         return 1;
//     }
//
//     printf("Shell Sort Optimization Test (Ciura + Pre-checks)\n");
//     printf("Total Students Loaded: %d\n", count);
//     printf("Performing 1000 iterations per test case...\n\n");
//
//     Student* test_arr = malloc(sizeof(Student) * count);
//
//     // *Gender 항목 제외* (Shell Sort는 Unstable 하므로)
//     struct {
//         char* title;
//         CompareFunc func;
//     } tests[] = {
//         {"1. ID Ascending", cmp_id_asc},
//         {"1. ID Descending", cmp_id_desc},
//         {"2. Name Ascending", cmp_name_asc},
//         {"2. Name Descending", cmp_name_desc},
//         {"3. Grade Complex Ascending", cmp_grade_asc},
//         {"3. Grade Complex Descending", cmp_grade_desc},
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
//             memcpy(test_arr, original_data, sizeof(Student) * count);
//
//             // 셸 정렬 수행 및 비교 횟수 누적
//             total_comparisons += shell_sort_ciura(test_arr, count, tests[i].func);
//         }
//
//         // 평균 계산
//         long long average_comparisons = total_comparisons / 10;
//
//         printf("[%s]\n", tests[i].title);
//         printf("  - Average Comparisons (10 runs): %lld\n", average_comparisons);
//         printf("\n");
//     }
//
//     free(test_arr);
//     free(original_data);
//     return 0;
// }