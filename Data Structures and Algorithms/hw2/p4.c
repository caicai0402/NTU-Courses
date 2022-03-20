// Fake Binary Search Tree

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int count = 0, box[1000000][3];

typedef struct node{
    int w, l, r;
    struct node *left;
    struct node *right;
}Node;

Node *newNode(int w, int l, int r, Node *left, Node *right){
    Node *node = (Node *)malloc(sizeof(Node));
    assert(node != NULL);
    node->w = w;
    node->l = l;
    node->r = r;
    node->left = left;
    node->right = right;
    return node;
}

void *buildTree(Node *node, int l, int r, int min, int max){
    if(min < node->w && node->w < max)
        count ++;
    if(l != -1){
        node->left = newNode(box[l-1][0], box[l-1][1], box[l-1][2], NULL, NULL);
        node->left = buildTree(node->left, box[l-1][1], box[l-1][2], min, node->w<max?node->w:max);
    }
    if(r != -1){
        node->right = newNode(box[r-1][0], box[r-1][1], box[r-1][2], NULL, NULL);
        node->right = buildTree(node->right, box[r-1][1], box[r-1][2], node->w>min?node->w:min, max);
    }
    return node;
}

void freeTree(Node *root){
    if(root == NULL)
        return ;
    else{
        freeTree(root -> left);
        freeTree(root -> right);
        free(root);
    }
    return ;
}

int main(void){
    int n;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%d%d%d", &box[i][0], &box[i][1], &box[i][2]);
    Node *root = newNode(box[0][0], box[0][1], box[0][2], NULL, NULL);
    root = buildTree(root, box[0][1], box[0][2], 0, 1000000001);
    printf("%d\n", count);
    freeTree(root);
    return 0;
}