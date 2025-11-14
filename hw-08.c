#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DATA_SIZE 10000
#define MAX_VALUE 1000000
#define NUM_RUNS 100

// 단순 삽입 정렬
long long insertion_sort(int arr[], int n) {
    long long comparisons = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
    return comparisons;
}

// 셸 정렬
long long shell_sort(int arr[], int n) {
    long long comparisons = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {

        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
    return comparisons;
}

// 셸 정렬 (Knuth 간격)
long long shell_sort_knuth(int arr[], int n) {
    long long comparisons = 0;

    int gaps[50];
    int h = 1, k = 0;

    while (h < n) {
        gaps[k++] = h;
        h = 3 * h + 1;
    }

    for (int gi = k - 1; gi >= 0; gi--) {
        int gap = gaps[gi];

        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp)
                    arr[j] = arr[j - gap];
                else
                    break;
            }
            arr[j] = temp;
        }
    }

    return comparisons;
}

// 셸 정렬 (Ciura 간격)
const int ciura_gaps[] = {3937, 1750, 701, 301, 132, 57, 23, 10, 4, 1}; // 10,000개 데이터 기준으로 설정함.
const int num_gaps = sizeof(ciura_gaps) / sizeof(ciura_gaps[0]);

long long shell_sort_ciura(int arr[], int n) {
    long long comparisons = 0;

    for (int k = 0; k < num_gaps; k++) {
        int gap = ciura_gaps[k];

        if (gap >= n) {
            continue;
        }

        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;

            for (j = i; j >= gap; j -= gap) {
                comparisons++;
                if (arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                } else {
                    break;
                }
            }
            arr[j] = temp;
        }
    }
    return comparisons;
}

int main() {
    srand((unsigned int)time(NULL));

    long long total_insertion_comps = 0;
    long long total_shell_comps = 0;
    long long total_shell_comps_ciura = 0;
    long long total_shell_comps_knuth = 0;

    int *original_data = (int *)malloc(DATA_SIZE * sizeof(int));
    int *data_for_insertion = (int *)malloc(DATA_SIZE * sizeof(int));
    int *data_for_shell = (int *)malloc(DATA_SIZE * sizeof(int));
    int *data_for_ciura = (int *)malloc(DATA_SIZE * sizeof(int));
    int *data_for_knuth = (int *)malloc(DATA_SIZE * sizeof(int));

    if (original_data == NULL || data_for_insertion == NULL || data_for_shell == NULL) {
        printf("메모리 할당 실패!\n");
        return 1;
    }

    printf("데이터: %d개 (0 ~ %d 범위)\n", DATA_SIZE, MAX_VALUE);
    printf("실행 횟수: %d회\n", NUM_RUNS);
    printf("테스트 시작...\n");

    time_t start_time = time(NULL);

    for (int i = 0; i < NUM_RUNS; i++) {
        for (int j = 0; j < DATA_SIZE; j++) {
            original_data[j] = rand() % (MAX_VALUE + 1);
        }

        memcpy(data_for_insertion, original_data, DATA_SIZE * sizeof(int));
        memcpy(data_for_shell, original_data, DATA_SIZE * sizeof(int));
        memcpy(data_for_ciura, original_data, DATA_SIZE * sizeof(int));
        memcpy(data_for_knuth, original_data, DATA_SIZE * sizeof(int));

        total_insertion_comps += insertion_sort(data_for_insertion, DATA_SIZE);
        total_shell_comps += shell_sort(data_for_shell, DATA_SIZE);
        total_shell_comps_knuth += shell_sort_knuth(data_for_knuth, DATA_SIZE);
        total_shell_comps_ciura += shell_sort_ciura(data_for_ciura, DATA_SIZE);

        if ((i + 1) % 10 == 0) {
            printf("  ... %d/%d회 실행 완료\n", i + 1, NUM_RUNS);
        }
    }

    time_t end_time = time(NULL);
    printf("테스트 완료. (총 소요 시간: %ld초)\n", (end_time - start_time));

    double avg_insertion = (double)total_insertion_comps / NUM_RUNS;
    double avg_shell = (double)total_shell_comps / NUM_RUNS;
    double avg_knuth = (double)total_shell_comps_knuth / NUM_RUNS;
    double avg_shell_ciura = (double)total_shell_comps_ciura / NUM_RUNS;

    printf("\n--- 100회 실행 평균 비교 횟수 ---\n");
    printf("단순 삽입 정렬: %.0f 회\n", avg_insertion);
    printf("쉘 정렬 (N/2 간격): %.0f 회\n", avg_shell);
    printf("셀 정렬 (knuth 간격): %.0f 회\n", avg_knuth);
    printf("쉘 정렬 (Ciura 간격): %.0f 회\n", avg_shell_ciura);

    free(original_data);
    free(data_for_insertion);
    free(data_for_shell);
    free(data_for_ciura);
    free(data_for_knuth);

    return 0;
}
/*
    과제 8 문제
        - 0~1,000,000 사이의 무작위 데이터로 이루어진 1만개의 데이터를 기본적인 쉘 정렬 및 단순 삽입 정렬로 정렬하고, 각 정렬 방식의 평균 비교 횟수를 출력하시오.
          + 각 정렬에 대해 최종 출력은 100회의 실행 후 평균을 통해 계산한다.
          + 쉘 정렬의 간격 조정 방식은 배열 길이의 1/2, 1/4, 1/8...로 줄이는 방식을 사용한다.

        - 또한, 주어진 조건 하에 쉘 정렬에 대해, 조건상에서 최적의 쉘 간격을 설정하는 알고리즘을 생각하여 해당 방법으로도 쉘 정렬을 수행하고, 비교 횟수를 최하로 줄인 결과도 출력해본다.
*/

/*
    기존 단순 삽입 정렬이란?
    - 과정 설명
        1) 인덱스 0의 숫자와 나머지 인덱스에서 가장 작은 것을 찾기
        2) 그 두 숫자를 스왑. (swap 함수 활용)
        3) 위 과정을 배열의 개수만큼 반복

    셸 정렬이란?
    - 과정 설명
        1) 먼저 정렬해야 할 리스트를 일정한 기준에 따라 분류
        2) 연속적이지 않은 여러 개의 부분 리스트를 생성
        3) 각 부분 리스트를 삽입 정렬을 이용하여 정렬
        4) 모든 부분 리스트가 정렬되면 다시 전체 리스트를 더 적은 개수의 부분 리스트로 만든 후에 알고리즘을 반복
        5) 위의 과정을 부분 리스트의 개수가 1이 될 때까지 반복
*/

/*
    셀 정렬의 성능은 어떤 간격(gap)을 사용하느냐에 따라 극명하게 달라짐.
    10,000개 정도의 데이터(N)에 대해 가장 우수하다고 알려진 간격은 Ciura(시우라)의 간격이다.

    1. 왜 N/2 간격은 최적이 아닐까?
    N/2, N/4, ... , 1 방식은 셀 정렬의 창시자인 도널드 셀이 처음 제시한 방식임.
    하지만 약점이 2가지 존재함.
        1) 최악의 경우 시간복잡도(O)가 O(N^2)이다.
            만약 데이터의 개수(N)가 2의 거듭제곱일 경우, 모든 간격이 짝수가 된다.
            이 경우, 짝수 인덱스는 짝수 인덱스끼리, 홀수 인덱스는 홀수 인덱스끼리만 비교되다가 마지막 간격이 1이 되어서야 처음으로 서로 비교된다.
            이는 셸 정렬의 장점(멀리 있는 요소를 미리 옮기는 것)을 크게 해치며, 사실상 단순 삽입 정렬과 비슷한 O(N^2) 수준의 비교가 필요하게 된다.
        2) 비효울적인 비교이다.
            간격들이 2라는 공통 약수를 가진다.
            간격들이 서로소일수록 더 효율적으로 요소를 섞을 수 있다고 알려져 있다.

    2. 최적의 간격 알고리즘
    찾아본 결과 아직까지 "절대적인 최적"의 간격을  찾는 공식은 발견되지 않았다.
    하지만, 많은 연구에서 밝혀진 사실은 "평균적으로 가장 빠른" 간격들이 알려져있다.

        1) Ciura(시우라)의 간격
        가장 실용적이고 평균 속도가 빠른 것으로 알려진 간격이다. 하지만 이 간격은 수학 공식이 아니다.
        실험과 통계를 통해 찾아낸 값이다.

            Ciura 간격 : ... 1750, 701, 301, 132, 57, 23, 10, 4, 1

        - 특징 : 약 2.2 ~ 2.3배씩 줄어드는 경향을 보임.
        - 구현 : 10,000개의 데이터라면 1750부터 시작하거나, 더 큰 데이터(N > 4000)의 경우 1750에 2.25를 곱한 값(약 3937)부터 시작하여
            1이 될 때까지 이 리스트를 역순으로 사용한다.
        - 데이터가 10,000개일 때, 이 간갹을 사용하면 N/2간격(약 18만회)보다 비교 횟수가 약 20~30% 더 감소하여 약 13~14만회 수준까지 줄어든다.

        2) Knuth(크누스)의 간격 (이건 수학적, 실용적인 알고리즘임)
        이건 도널드 크누스가 제안한 간격으로, 구현하기 쉽고 성능이 매우 우수함.

            Knuth 간격 공식 : h = 3h + 1
            생성되는 간격 : 1, 4, 13, 40, 121, 364, 1093, 3280, 9841, ...

        - 특징 : N/2 간격의 최악의 경우의 시간복잡도인 O(N^2)가 아닌, O(N^1.5)의 시간 복잡도를 보장하게 된다.
        - 구현 :
                1) 배열 크기(N)보다 작은 최대 간격(h)을 찾는다. (예 : N = 10,000이면 9,841부터 시작)
                2) 9841, 3280, 1093, ... 13, 4, 1 순서로 간격을 줄여가며 정렬을 수행한다.



        실제로 두 코드를 사용하여 정렬을 해본 결과 :

                --- 100회 실행 평균 비교 횟수 ---
                단순 삽입 정렬: 24989835 회
                쉘 정렬 (N/2 간격): 265900 회
                쉘 정렬 (Ciura 간격): 191398 회
                셀 정렬 (knuth 간격): 235426 회

                Ciura 간격이 가장 빠르다는 것을 알 수 있었다.
                > 이건 지금 현제 데이터가 10,000개라는 값으로 진행하였기 때문에 최적화된 알고리즘이 된 것이다.

*/