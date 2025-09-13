#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_NODES 256
#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64
#define MAX_INPUT_LEN 512

int tokenize(const char* s, char* tokens[]);
void input_array_tree(const char* s, char* tree_array[], int max_nodes);
void parse_children(char* tokens[], int* token_index, char* tree_array[], int parent_array_index, int max_nodes);
int check_tree_height(char* tree_array[], int max_size);
int check_node_count(char* tree_array[], int max_size);
int check_leaf_node_count(char* tree_array[], int max_size);


int main() {
    char input_str[MAX_INPUT_LEN];
    char* tree[MAX_NODES];

    scanf(" %[^\n]", input_str);

    input_array_tree(input_str, tree, MAX_NODES);

    int height = check_tree_height(tree, MAX_NODES);
    int node_count = check_node_count(tree, MAX_NODES);
    int leaf_count = check_leaf_node_count(tree, MAX_NODES);

    printf("%d, %d, %d\n", height, node_count, leaf_count);

    for (int i = 0; i < MAX_NODES; ++i) {
        if (tree[i] != NULL) {
            free(tree[i]);
        }
    }
    return 0;
}

int tokenize(const char* s, char* tokens[]) {
    int count = 0;
    const char* p = s;

    while (*p != '\0' && count < MAX_TOKENS) {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (*p == '(' || *p == ')' || isalpha((unsigned char)*p)) {
            char* token_str = (char*)malloc(2 * sizeof(char));
            if (token_str == NULL) {
                fprintf(stderr, "ERROR");
                return -1;
            }
            token_str[0] = *p;
            token_str[1] = '\0';
            tokens[count++] = token_str;
        }
        p++;
    }

    if (count < MAX_TOKENS) {
        tokens[count] = NULL;
    }
    return count;
}

int check_tree_height(char* tree_array[], int max_size) {
    int max_idx = 0;
    for (int i = 1; i < max_size; i++) {
        if (tree_array[i] != NULL) max_idx = i;
    }
    if (max_idx <= 1) return 0;
    return (int)floor(log2(max_idx));
}

int check_node_count(char* tree_array[], int max_size) {
    int count = 0;
    for (int i = 1; i < max_size; i++) {
        if (tree_array[i] != NULL) count++;
    }
    return count;
}

int check_leaf_node_count(char* tree_array[], int max_size) {
    int leaf_count = 0;
    for (int i = 1; i < max_size; i++) {
        if (tree_array[i] != NULL) {
            int first_child_idx = 2 * i;
            if (first_child_idx >= max_size || tree_array[first_child_idx] == NULL) {
                leaf_count++;
            }
        }
    }
    return leaf_count;
}

void input_array_tree(const char* s, char* tree_array[], int max_nodes) {
    if (s == NULL || strlen(s) == 0) return;
    for (int i = 0; i < max_nodes; i++) tree_array[i] = NULL;

    char* local_tokens[MAX_TOKENS] = {0};
    int token_count = tokenize(s, local_tokens);
    if (token_count < 3) {
        for (int i = 0; i < token_count; i++) free(local_tokens[i]);
        return;
    }

    if (1 < max_nodes) tree_array[1] = strdup(local_tokens[1]);
    else {
        fprintf(stderr, "ERROR");
        for (int i = 0; i < token_count; i++) free(local_tokens[i]);
        return;
    }

    int current_token_index = 3;
    parse_children(local_tokens, &current_token_index, tree_array, 1, max_nodes);
    for (int i = 0; i < token_count; i++) free(local_tokens[i]);
}

void parse_children(char* tokens[], int* token_index, char* tree_array[], int parent_array_index, int max_nodes) {
    int child_offset = 0;
    while (tokens[*token_index] != NULL && strcmp(tokens[*token_index], ")") != 0) {
        char* node_value = tokens[*token_index];
        int is_parent = (tokens[*token_index + 1] != NULL && strcmp(tokens[*token_index + 1], "(") == 0);
        int child_array_index = 2 * parent_array_index + child_offset;

        if (child_array_index >= max_nodes) {
            int paren_level = 0;
            while(tokens[*token_index] != NULL) {
                 if (strcmp(tokens[*token_index], "(") == 0) paren_level++;
                 if (strcmp(tokens[*token_index], ")") == 0) {
                    if (paren_level == 0) break;
                    paren_level--;
                 }
                (*token_index)++;
            }
            return;
        }

        tree_array[child_array_index] = strdup(node_value);
        (*token_index)++;

        if (is_parent) {
            (*token_index)++;
            parse_children(tokens, token_index, tree_array, child_array_index, max_nodes);
            (*token_index)++;
        }
        child_offset++;
    }
}