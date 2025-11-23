#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 35000

typedef struct {
    int id;
    char name[50];
    char gender[10];
    int korean;
    int english;
    int math;
    long long product;
} Student;

Student students[MAX_STUDENTS];
Student temp_students[MAX_STUDENTS];

int student_count = 0;
long long sort_comparisons = 0;

void loadData(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("파일 열기 실패: %s\n", filename);
        return;
    }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp);

    int i = 0;
    while (fscanf(fp, "%d,%[^,],%[^,],%d,%d,%d",
                  &students[i].id, students[i].name, students[i].gender,
                  &students[i].korean, &students[i].english, &students[i].math) != EOF) {
        students[i].product = (long long)students[i].korean * students[i].english * students[i].math;
        i++;
        if (i >= MAX_STUDENTS) break;
    }
    student_count = i;
    fclose(fp);
}

void performLinearSearch(long long target) {
    printf("\n--- [2. 순차 탐색] ---\n");
    int comparisons = 0;
    int found = 0;
    int found_id = -1;

    for (int i = 0; i < student_count; i++) {
        comparisons++;
        if (students[i].product == target) {
            found = 1;
            found_id = students[i].id;
            break;
        }
    }
    printf("비교 횟수: %d회\n", comparisons);

    if (found) {
        printf("결과: 찾았습니다! (ID: %d)\n", found_id);
    } else {
        printf("결과: 데이터를 찾지 못했습니다.\n");
    }
}

void merge(Student arr[], int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        sort_comparisons++;
        if (arr[i].product <= arr[j].product) {
            temp_students[k++] = arr[i++];
        } else {
            temp_students[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp_students[k++] = arr[i++];
    }
    while (j <= right) {
        temp_students[k++] = arr[j++];
    }

    for (int l = left; l <= right; l++) {
        arr[l] = temp_students[l];
    }
}

void mergeSort(Student arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void performBinarySearch(long long target) {
    printf("\n--- [3. 정렬 후 이진 탐색 (병합 정렬)] ---\n");

    sort_comparisons = 0;
    mergeSort(students, 0, student_count - 1);
    printf(">> 병합 정렬 완료 (비교 횟수: %lld회)\n", sort_comparisons);

    int search_comparisons = 0;
    int found = 0;
    int found_id = -1;
    int left = 0, right = student_count - 1;

    while (left <= right) {
        search_comparisons++;
        int mid = (left + right) / 2;

        if (students[mid].product == target) {
            found = 1;
            found_id = students[mid].id;
            break;
        }
        else if (students[mid].product < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    printf(">> 이진 탐색 완료 (비교 횟수: %d회)\n", search_comparisons);
    printf(">> [총 비용] 정렬 + 탐색: %lld회\n", sort_comparisons + search_comparisons);

    if (found) {
        printf("결과: 찾았습니다! (ID: %d)\n", found_id);
    } else {
        printf("결과: 데이터를 찾지 못했습니다.\n");
    }
}

int main() {
    srand(time(NULL));

    loadData("dataset_id_ascending.csv");
    printf("데이터 %d개 로드 완료.\n", student_count);

    long long target = rand() % 1000001;
    printf("목표 값(Target): %lld\n", target);

    performLinearSearch(target);
    performBinarySearch(target);

    return 0;
}

/*
    과제 10
    이번 과제는 csv 파일에 있는 정보를 가지고 구조체에 저장한 뒤에 임의의 값을 랜덤으로 가져온 후
    그 값을 찾는데 얼마나 많은 시간이 걸리는지 확인해보는 것이 목표이다.

    데이터를 한 번씩 비교하여 맞는지 확인하는 순차탐색은 32033회로 일정하지만, 정렬 후 이진 탐색을 하는 방법에서는
    정렬을 할 때 비교 횟수가 많이 들기 때문에 결과적으로 보면 기존 순차 탐색보다 비효율적인 것을 알 수 있는 과제였다.
*/