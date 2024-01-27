#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct AVLNode {
    char cityName[50];
    int totalTrips;
    int departureTrips;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
};

int height(struct AVLNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(struct AVLNode* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

struct AVLNode* newNode(char cityName[]) {
    struct AVLNode* node = (struct AVLNode*)malloc(sizeof(struct AVLNode));
    strcpy(node->cityName, cityName);
    node->totalTrips = 0;
    node->departureTrips = 0;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct AVLNode* rightRotate(struct AVLNode* y) {
    struct AVLNode* x = y->left;
    struct AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));
    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));

    return x;
}

struct AVLNode* leftRotate(struct AVLNode* x) {
    struct AVLNode* y = x->right;
    struct AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + ((height(x->left) > height(x->right)) ? height(x->left) : height(x->right));
    y->height = 1 + ((height(y->left) > height(y->right)) ? height(y->left) : height(y->right));

    return y;
}

struct AVLNode* insert(struct AVLNode* node, char cityName[]) {
    if (node == NULL)
        return newNode(cityName);

    if (strcmp(cityName, node->cityName) < 0)
        node->left = insert(node->left, cityName);
    else if (strcmp(cityName, node->cityName) > 0)
        node->right = insert(node->right, cityName);
    else
        return node;

    node->height = 1 + ((height(node->left) > height(node->right)) ? height(node->left) : height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(cityName, node->left->cityName) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(cityName, node->right->cityName) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(cityName, node->left->cityName) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(cityName, node->right->cityName) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inOrderTraversal(struct AVLNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%s - Total: %d, Departures: %d\n", root->cityName, root->totalTrips, root->departureTrips);
        inOrderTraversal(root->right);
    }
}

void freeAVLTree(struct AVLNode* root) {
    if (root != NULL) {
        freeAVLTree(root->left);
        freeAVLTree(root->right);
        free(root);
    }
}

int main() {
    FILE* file = fopen("data/data.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    char line[256];
    fgets(line, sizeof(line), file);

    struct AVLNode* root = NULL;

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ";");
        char cityName[50];

        token = strtok(NULL, ";");
        token = strtok(NULL, ";");
        token = strtok(NULL, ";");

        if (token != NULL) {
            sscanf(token, "%s", cityName);
        }

        root = insert(root, cityName);
    }

    fclose(file);

    inOrderTraversal(root);

    FILE* dataFile = fopen("temp/t-output.txt", "w");
    if (dataFile == NULL) {
        fprintf(stderr, "Error creating data file for gnuplot.\n");
        return 1;
    }

    struct AVLNode* current = root;
    while (current != NULL) {
        fprintf(dataFile, "%s %d %d\n", current->cityName, current->totalTrips, current->departureTrips);
        current = current->right;
    }

    fclose(dataFile);

    freeAVLTree(root);

    return 0;
}

