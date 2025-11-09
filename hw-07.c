#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define V 10
#define E 20
#define INF INT_MAX

struct Edge {
    int u, v;
};

void swap(struct Edge *a, struct Edge *b) {
    struct Edge temp = *a;
    *a = *b;
    *b = temp;
}

void create_random_graph(int dist[V][V], int next_hop[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;
                next_hop[i][j] = i;
            } else {
                dist[i][j] = INF;
                next_hop[i][j] = -1;
            }
        }
    }

    int total_possible_edges = V * (V - 1) / 2;
    struct Edge* all_edges = malloc(total_possible_edges * sizeof(struct Edge));
    int idx = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            all_edges[idx].u = i;
            all_edges[idx].v = j;
            idx++;
        }
    }

    for (int i = total_possible_edges - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(&all_edges[i], &all_edges[j]);
    }

    printf("--- 생성된 무작위 간선 (총 %d개) ---\n", E);
    for (int i = 0; i < E; i++) {
        int u = all_edges[i].u;
        int v = all_edges[i].v;
        printf("(%d, %d) ", u, v);

        dist[u][v] = 1;
        dist[v][u] = 1;
        next_hop[u][v] = v;
        next_hop[v][u] = u;
    }
    printf("\n------------------------------------\n");

    free(all_edges);
}

void floyd_warshall(int dist[V][V], int next_hop[V][V]) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {

                if (dist[i][k] == INF || dist[k][j] == INF) {
                    continue;
                }

                int new_dist = dist[i][k] + dist[k][j];

                if (dist[i][j] > new_dist) {
                    dist[i][j] = new_dist;
                    next_hop[i][j] = next_hop[i][k];
                }
            }
        }
    }
}

void print_all_pairs_paths(int dist[V][V], int next_hop[V][V]) {
    printf("\n--- 모든 노드 쌍 (45개) 간의 최단 경로 ---\n");
    int pair_count = 0;

    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            pair_count++;

            if (dist[i][j] == INF) {
                printf("%2d. (%d, %d) : 연결되지 않음 (거리: ∞)\n", pair_count, i, j);
            } else {
                printf("%2d. (%d, %d) : 거리 = %d, 경로 = [", pair_count, i, j, dist[i][j]);

                int curr = i;
                printf("%d", curr);
                while (curr != j) {
                    curr = next_hop[curr][j];
                    printf(", %d", curr);
                }
                printf("]\n");
            }
        }
    }
}

int main() {
    srand(time(NULL));

    int dist[V][V];
    int next_hop[V][V];

    create_random_graph(dist, next_hop);
    floyd_warshall(dist, next_hop);
    print_all_pairs_paths(dist, next_hop);

    return 0;
}