 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>

 #define VERTICES 100

 typedef struct AdjListNode {
     int dest;
     struct AdjListNode* next;
 } AdjListNode;

 typedef struct AdjList {
     AdjListNode* head;
 } AdjList;

 typedef struct Graph {
     int V; // 정점의 개수
     void* representation;
 } Graph;

 long long comparison_count = 0;

 /* ================================================================
  * 인접 리스트 관련 함수
  * ================================================================ */

 AdjListNode* newAdjListNode(int dest) {
     AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
     newNode->dest = dest;
     newNode->next = NULL;
     return newNode;
 }


 void addEdgeList(Graph* graph, int src, int dest) {
     AdjList* adjList = (AdjList*)graph->representation;

     comparison_count = 0;
     AdjListNode* crawler = adjList[src].head;
     while (crawler) {
         comparison_count++;
         if (crawler->dest == dest) return;
         crawler = crawler->next;
     }

     AdjListNode* newNode = newAdjListNode(dest);
     newNode->next = adjList[src].head;
     adjList[src].head = newNode;

     newNode = newAdjListNode(src);
     newNode->next = adjList[dest].head;
     adjList[dest].head = newNode;
 }

 void removeEdgeList(Graph* graph, int src, int dest) {
     AdjList* adjList = (AdjList*)graph->representation;
     comparison_count = 0;

     AdjListNode* crawler = adjList[src].head;
     AdjListNode* prev = NULL;
     while (crawler != NULL) {
         comparison_count++;
         if (crawler->dest == dest) {
             if (prev == NULL) adjList[src].head = crawler->next;
             else prev->next = crawler->next;
             free(crawler);
             break;
         }
         prev = crawler;
         crawler = crawler->next;
     }

     crawler = adjList[dest].head;
     prev = NULL;
     while (crawler != NULL) {
         comparison_count++;
         if (crawler->dest == src) {
             if (prev == NULL) adjList[dest].head = crawler->next;
             else prev->next = crawler->next;
             free(crawler);
             break;
         }
         prev = crawler;
         crawler = crawler->next;
     }
 }


 void checkConnectionList(Graph* graph, int src, int dest) {
     AdjList* adjList = (AdjList*)graph->representation;
     comparison_count = 0;
     AdjListNode* crawler = adjList[src].head;
     while (crawler) {
         comparison_count++;
         if (crawler->dest == dest) {
             return;
         }
         crawler = crawler->next;
     }
 }

 void printNeighborsList(Graph* graph, int v) {
     AdjList* adjList = (AdjList*)graph->representation;
     comparison_count = 0;
     AdjListNode* crawler = adjList[v].head;
     while (crawler) {
         comparison_count++;
         crawler = crawler->next;
     }
 }

 /* ================================================================
  * 인접 행렬 관련 함수
  * ================================================================ */

 void addEdgeMatrix(Graph* graph, int src, int dest) {
     int** matrix = (int**)graph->representation;
     comparison_count = 1;
     if (matrix[src][dest] == 0) {
         matrix[src][dest] = 1;
         matrix[dest][src] = 1;
     }
 }

 void removeEdgeMatrix(Graph* graph, int src, int dest) {
     int** matrix = (int**)graph->representation;
     comparison_count = 1;
     if (matrix[src][dest] == 1) {
         matrix[src][dest] = 0;
         matrix[dest][src] = 0;
     }
 }

 void checkConnectionMatrix(Graph* graph, int src, int dest) {
     int** matrix = (int**)graph->representation;
     comparison_count = 1;
     if (matrix[src][dest] == 1) {
         return;
     }
 }

 void printNeighborsMatrix(Graph* graph, int v) {
     int** matrix = (int**)graph->representation;
     comparison_count = 0;
     for (int i = 0; i < graph->V; ++i) {
         comparison_count++;
         if (matrix[v][i] == 1) {
         }
     }
 }


 /* ================================================================
  * 테스트 실행 및 메인 함수
  * ================================================================ */

 void run_test_case(const char* case_name, int num_vertices, int num_edges, int is_matrix) {
     printf("케이스: %s\n", case_name);

     Graph graph;
     graph.V = num_vertices;
     long memory_usage = 0;

     if (is_matrix) {
         int** matrix = (int**)malloc(num_vertices * sizeof(int*));
         for (int i = 0; i < num_vertices; i++) {
             matrix[i] = (int*)calloc(num_vertices, sizeof(int));
         }
         graph.representation = matrix;
         memory_usage = num_vertices * num_vertices * sizeof(int);
     } else {
         AdjList* adjList = (AdjList*)malloc(num_vertices * sizeof(AdjList));
         for (int i = 0; i < num_vertices; i++) {
             adjList[i].head = NULL;
         }
         graph.representation = adjList;
         memory_usage = num_vertices * sizeof(AdjList);
     }

     int (*edges)[2] = malloc(num_edges * sizeof(*edges));
     int count = 0;
     int** temp_matrix = (int**)malloc(num_vertices * sizeof(int*));
     for(int i = 0; i < num_vertices; i++) temp_matrix[i] = (int*)calloc(num_vertices, sizeof(int));

     while (count < num_edges) {
         int u = rand() % num_vertices;
         int v = rand() % num_vertices;
         if (u != v && temp_matrix[u][v] == 0) {
             temp_matrix[u][v] = 1;
             temp_matrix[v][u] = 1;
             edges[count][0] = u;
             edges[count][1] = v;
             count++;
         }
     }

     for (int i = 0; i < num_edges; i++) {
         if (is_matrix) {
             addEdgeMatrix(&graph, edges[i][0], edges[i][1]);
         } else {
             addEdgeList(&graph, edges[i][0], edges[i][1]);
             memory_usage += 2 * sizeof(AdjListNode);
         }
     }

     printf("메모리 사용량: %ld Bytes\n", memory_usage);

     int test_u = rand() % num_vertices;
     int test_v = rand() % num_vertices;
     while(test_u == test_v) test_v = rand() % num_vertices;

     if (is_matrix) {
         removeEdgeMatrix(&graph, edges[0][0], edges[0][1]);
         long remove_comp = comparison_count;
         addEdgeMatrix(&graph, edges[0][0], edges[0][1]);
         long add_comp = comparison_count;
         printf("간선 삽입/삭제 비교: 약 %ld번\n", (remove_comp + add_comp) / 2);
     } else {
         removeEdgeList(&graph, edges[0][0], edges[0][1]);
         long remove_comp = comparison_count;
         addEdgeList(&graph, edges[0][0], edges[0][1]);
         long add_comp = comparison_count;
         printf("간선 삽입/삭제 비교: 약 %ld번\n", (remove_comp + add_comp) / 2);
     }

     if (is_matrix) {
         checkConnectionMatrix(&graph, test_u, test_v);
     } else {
         checkConnectionList(&graph, test_u, test_v);
     }
     printf("두 정점의 연결 확인 비교: %lld번\n", comparison_count);

     if (is_matrix) {
         printNeighborsMatrix(&graph, test_u);
     } else {
         printNeighborsList(&graph, test_u);
     }
     printf("한 노드의 인접 노드 출력 비교: %lld번\n", comparison_count);

     printf("----------------------------------------\n");

     for(int i = 0; i < num_vertices; i++) free(temp_matrix[i]);
     free(temp_matrix);
     free(edges);
     if(is_matrix) {
         for(int i = 0; i < num_vertices; i++) free(((int**)graph.representation)[i]);
         free(graph.representation);
     } else {
         for(int i = 0; i < num_vertices; i++) {
             AdjListNode* crawler = ((AdjList*)graph.representation)[i].head;
             while(crawler) {
                 AdjListNode* temp = crawler;
                 crawler = crawler->next;
                 free(temp);
             }
         }
         free(graph.representation);
     }
 }


 int main() {
     srand(time(NULL));

     run_test_case("희소그래프-인접행렬", VERTICES, 100, 1);
     run_test_case("희소그래프-인접리스트", VERTICES, 100, 0);
     run_test_case("밀집그래프-인접행렬", VERTICES, 4000, 1);
     run_test_case("밀집그래프-인접리스트", VERTICES, 4000, 0);

     return 0;
 }

/*
    이번 과제는 그래프(Graph)를 표현하는 두 가지 주요 방식인
    '인접 행렬(Adjacency Matrix)'과 '인접 리스트(Adjacency List)'의 성능과 메모리 사용량을 비교하기 위한 과제

    그래프의 특징, 특히 '밀집도(Density)'에 따라 어떤 표현 방식이 더 효율적인지 판단하는 작업

    1. 인접 행렬 (Adjacency Matrix)
    - V x V 크기의 2차원 배열을 사용하여, 정점 i와 정점 j 사이에 간선이 있으면 matrix[i][j] = 1, 없으면 0으로 표시
    - (장점) 두 정점의 연결 확인(checkConnection)이나 간선 삽입/삭제(addEdge/removeEdge)가 O(1) 시간 복잡도로 매우 빠름
            배열의 특정 인덱스에 한 번만 접근하면 되기 때문에, 코드에서 comparison_count를 1로 설정
    - (단점) 특정 정점의 모든 이웃을 찾으려면(printNeighbors) 해당 행(row)을 모두 순회해야 하므로 O(V)의 시간이 걸림
    - (단점) 간선의 수(E)와 관계없이 항상 O(V^2)의 메모리 공간을 차지

    2. 인접 리스트 (Adjacency List)
    - 정점의 개수(V)만큼의 배열을 만들고, 각 배열의 인덱스(정점)마다 자신과 연결된 이웃 정점들의 리스트를 (보통 연결 리스트로) 저장
    - (장점) 실제 간선의 수(E)만큼만 노드를 생성하므로, 메모리 사용량이 O(V+E)로 효율적이다. (무방향 그래프이므로 O(V+2E))
    - (장점) 특정 정점의 모든 이웃을 찾는(printNeighbors) 연산이 해당 정점의 차수(degree)만큼, 즉 O(deg(v))의 시간만 걸림.
    - (단점) 두 정점의 연결 확인(checkConnection)이나 간선 삽입/삭제(removeEdge)를 하려면,
            최악의 경우 해당 정점의 연결 리스트를 끝까지 순회해야 하므로 O(deg(v))의 시간이 걸림
            코드에서 while(crawler) 루프를 돌며 comparison_count를 증가시키는 이유

    이러한 이론적 성능 차이를 실제 데이터로 확인하기 위해 두 가지 극단적인 케이스를 테스트해야함
    (정점 V = 100으로 고정)

    (1) 희소 그래프 (Sparse Graph) : 정점 100개, 간선 100개 (E ≈ V)
        - 간선의 수가 정점의 수와 비슷하게 매우 적은 경우
        - [예상] 인접 행렬은 100x100=10000칸의 메모리를 쓰지만, 인접 리스트는 100개의 배열 + 200개의 노드(무방향) 정도만 사용하므로
                메모리 면에서 인접 리스트가 압도적으로 유리할 것
        - [예상] 연산 속도:
            - '연결 확인': 인접 행렬(O(1))이 인접 리스트(O(deg(v)), 평균 1)보다 약간 빠르거나 비슷할 것
            - '이웃 출력': 인접 리스트(O(deg(v)), 평균 1)가 인접 행렬(O(V), 100)보다 훨씬 빠를 것

    (2) 밀집 그래프 (Dense Graph) : 정점 100개, 간선 4000개 (E ≈ V^2)
        - 간선의 수가 최대치(약 100*99/2 = 4950)에 가깝게 매우 많은 경우이다.
        - [예상] 메모리 면에서 인접 행렬(10000칸)과 인접 리스트(100개 배열 + 8000개 노드)의 차이가 희소 그래프만큼 크지 않을 것
        - [예상] 연산 속도:
            - '연결 확인': 인접 행렬(O(1))이 인접 리스트(O(deg(v)), 평균 80)보다 압도적으로 빠를 것
            - '이웃 출력': 인접 행렬(O(V), 100)과 인접 리스트(O(deg(v)), 평균 80)가 비슷한 성능을 보일 것

    이 코드는 run_test_case 함수를 통해 위 4가지 조합(희소-행렬, 희소-리스트, 밀집-행렬, 밀집-리스트)을
    모두 실행하고, 각 연산(삽입/삭제, 연결확인, 이웃출력)에 대한 comparison_count와
    총 메모리 사용량을 출력하여 이 예상 시나리오가 맞는지 확인
*/