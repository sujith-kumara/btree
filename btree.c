#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3
#define MIN_KEYS ((MAX_KEYS + 1) / 2)

struct BTreeNode {
    int keys[MAX_KEYS];
    struct BTreeNode* children[MAX_KEYS + 1];
    int num_keys;
    int is_leaf;
};

// Function to create a new B-tree node
struct BTreeNode* createNode(int is_leaf) {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    return newNode;
}

// Function to search for a key in the B-tree
int search(struct BTreeNode* root, int key) {
    int i = 0;
    while (i < root->num_keys && key > root->keys[i]) {
        i++;
    }
    if (i < root->num_keys && key == root->keys[i]) {
        return 1; // Key found
    }
    if (root->is_leaf) {
        return 0; // Key not found
    }
    return search(root->children[i], key); // Recursively search in the appropriate child
}

// Function to split a child node
void splitChild(struct BTreeNode* parent, int index, struct BTreeNode* child) {
    struct BTreeNode* newChild = createNode(child->is_leaf);
    parent->num_keys++;
    
    for (int i = parent->num_keys - 1; i > index; i--) {
        parent->keys[i] = parent->keys[i - 1];
        parent->children[i + 1] = parent->children[i];
    }
    
    parent->keys[index] = child->keys[MIN_KEYS];
    
    for (int i = 0; i < MIN_KEYS; i++) {
        newChild->keys[i] = child->keys[i + MIN_KEYS + 1];
    }
    
    if (!child->is_leaf) {
        for (int i = 0; i <= MIN_KEYS; i++) {
            newChild->children[i] = child->children[i + MIN_KEYS + 1];
        }
    }
    
    child->num_keys = MIN_KEYS;
    newChild->num_keys = MIN_KEYS;
    
    parent->children[index + 1] = newChild;
}

// Function to insert a key into the B-tree
void insert(struct BTreeNode** root, int key) {
    struct BTreeNode* currentNode = *root;
    
    if (currentNode->num_keys == MAX_KEYS) {
        struct BTreeNode* newRoot = createNode(0);
        newRoot->children[0] = *root;
        *root = newRoot;
        splitChild(newRoot, 0, currentNode);
    }
    
    while (!currentNode->is_leaf) {
        int i = currentNode->num_keys - 1;
        while (i >= 0 && key < currentNode->keys[i]) {
            i--;
        }
        i++;
        
        struct BTreeNode* nextNode = currentNode->children[i];
        
        if (nextNode->num_keys == MAX_KEYS) {
            splitChild(currentNode, i, nextNode);
            if (key > currentNode->keys[i]) {
                i++;
            }
        }
        
        currentNode = currentNode->children[i];
    }
    
    int i = currentNode->num_keys - 1;
    while (i >= 0 && key < currentNode->keys[i]) {
        currentNode->keys[i + 1] = currentNode->keys[i];
        i--;
    }
    currentNode->keys[i + 1] = key;
    currentNode->num_keys++;
}
void inOrderTraversal(struct BTreeNode* node) {
    if (node) {
        int i;
        for (i = 0; i < node->num_keys; i++) {
            // Recursively traverse the left child
            inOrderTraversal(node->children[i]);
            
            // Print the current key
            printf("%d ", node->keys[i]);
        }
        
        // Traverse the last child
        inOrderTraversal(node->children[i]);
    }
}

int main() {
    struct BTreeNode* root = createNode(1); // Create the root node as a leaf
    
    int keys[] = {2, 4, 3, 7, 9, 8, 5};
    int num_keys = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < num_keys; i++) {
        insert(&root, keys[i]);
    }

    printf("B-tree contents: ");
    // You can implement a traversal function to print the B-tree contents here
    inOrderTraversal(root);
    
    printf("\n");

    int searchKey = 7;
    if (search(root, searchKey)) {
        printf("%d is found in the B-tree.\n", searchKey);
    } else {
        printf("%d is not found in the B-tree.\n", searchKey);
    }

    return 0;
}
