// Alice's Bookshelf

/*
參考網站：
https://cp-algorithms.com/data_structures/treap.html?fbclid=IwAR2sndzfm5yj_y1QYHQehx1znz96-AxcKk-tQiPRZNMqyGBT40BWo-XfU2k#toc-tgt-6
https://morris821028.github.io/2015/06/16/zj-b405/?fbclid=IwAR0se2dmfgVfi3m_NfauYlmYwNdaIxPQD8purILC4qvBl0L6-68GD0vblqo

另外我還有跟我朋友 徐晨祐 B08902149 討論上面網站的邏輯
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXN 800000
#define MAXQ 800000

typedef struct node{
    int priority;
    int random;
    int size;
    int add;
    int max;
    bool reverse;
    struct node *left, *right;
}Node;

Node *root = NULL;
Node nodes[MAXN + MAXQ];
int A[MAXN];
int idx = 0;

static inline Node *newNode(int priority){
    nodes[idx].priority = priority;
    nodes[idx].random = rand();
    nodes[idx].size = 1;
    nodes[idx].add = 0;
    nodes[idx].max = priority;
    nodes[idx].reverse = false;
    nodes[idx].left = NULL;
    nodes[idx].right = NULL;
    idx ++;
    return &nodes[idx-1];
}

static inline int getSize(Node *node){
    return node==NULL?0:node->size;
}

static inline void pushUp(Node *node){
    if(node == NULL)
        return ;
    node->max = node->priority + node->add;
    if(node->left != NULL)
        node->max = node->max > node->left->max + node->left->add + node->add?node->max:node->left->max + node->left->add + node->add;
    if(node->right != NULL)
        node->max = node->max > node->right->max + node->right->add + node->add?node->max:node->right->max + node->right->add + node->add;
    return ;
}

static inline void pushDown(Node *node){
    if(node == NULL)
        return ;
    if(node->add != 0){
        node->priority += node->add;
        node->max += node->add;
        if(node->left != NULL)
            node->left->add += node->add;
        if(node->right != NULL)
            node->right->add += node->add;
        node->add = 0;
    }
    if(node->reverse == true){
        Node *temp;
        temp = node->left;
        node->left = node->right;
        node->right = temp;
        if(node->left != NULL)
            node->left->reverse ^= true;
        if(node->right != NULL)
            node->right->reverse ^= true;
        node->reverse = false;
    }
    return ;
}

static inline void split(Node *node, Node **left, Node **right, int pos, int temp){
    if(node == NULL){
        *left = NULL;
        *right = NULL;
    }
    else{
        pushDown(node);
        int size = getSize(node->left) + 1;
        if(pos < size + temp){
            *right = node;
            split(node->left, left, &node->left, pos, temp);
        }
        else{
            *left = node;
            split(node->right, &node->right, right, pos, temp + size);
        }
        node->size = getSize(node->left) + getSize(node->right) + 1;
        pushUp(node);
    }
    return ;
}

static inline Node *merge(Node *left, Node *right){
    pushDown(left);
    pushDown(right);
    if(left == NULL)
        return right;
    if(right == NULL)
        return left;
    if(left->random > right->random){
        left->right = merge(left->right, right);
        left->size = getSize(left->left) + getSize(left->right) + 1;
        pushUp(left);
        return left;
    }
    else{
        right->left = merge(left, right->left);
        right->size = getSize(right->left) + getSize(right->right) + 1;
        pushUp(right);
        return right;
    }
}

static inline void helper(Node *node){
    if(node == NULL)
        return;
    Node *max = node;
    if(node->left != NULL && node->left->random > max->random)
        max = node->left;
    if(node->right != NULL && node->right->random > max->random)
        max = node->right;
    if(max != node){
        int temp = node->random;
        node->random = max->random;
        max->random = temp;
        helper(max);
    }
    return ;
}

static inline Node *init(int *A, int N){
    if(N == 0)
        return NULL;
    int mid = N / 2;
    Node *node = newNode(A[mid]);
    node->left = init(A, mid);
    node->right = init(A+mid+1, N-mid-1);
    helper(node);
    node->size = getSize(node->left) + getSize(node->right) + 1;
    pushUp(node);
    return node;
}

static inline int dfs(){
    Node *node = root;
    int pos, temp = 0;
    while(1){
        pushDown(node);
        if(node->left != NULL && node->left->max + node->left->add == node->max)
            node = node->left;
        else if(node->priority == node->max)
            return getSize(node->left) + 1 + temp;
        else if(node->right != NULL && node->right->max + node->right->add == node->max){
            temp += getSize(node->left) + 1;
            node = node->right;
        }
    }
    return 0;
}

int main(void){
    int N, Q;
    scanf("%d%d", &N, &Q);
    for(int i = 0; i < N; i++)
        scanf("%d", &A[i]);
    root = init(A, N);
    int operation;
    int p, k, l, r;
    Node *temp1, *temp2, *temp3;
    for(int i = 0; i < Q; i++){
        scanf("%d", &operation);
        switch(operation){
            case 6:
                k = dfs();
                split(root, &temp1, &temp2, k, 0);
                split(temp1, &temp1, &temp3, k-1, 0);
                root = merge(temp1, temp2);
                break;
            case 1:
                scanf("%d%d", &p, &k);
                split(root, &temp1, &temp2, k, 0);
                root = merge(merge(temp1, newNode(p)), temp2);
                break;
            case 3:
                scanf("%d%d%d", &l, &r, &p);
                split (root, &temp1, &temp2, l-1, 0);
                split (temp2, &temp2, &temp3, r-l+1, 0);
                temp2->add += p;
                root = merge(merge(temp1, temp2), temp3);
                break;
            case 5:
                scanf("%d%d", &l, &r);
                if(r > l){
                    split (root, &temp1, &temp2, l-1, 0);
                    split (temp2, &temp2, &temp3, r-l+1, 0);
                    temp2->reverse ^= true;
                    root = merge(merge(temp1, temp2), temp3);
                }
                break;
            case 2:
                scanf("%d", &k);
                split(root, &temp1, &temp2, k, 0);
                split(temp1, &temp1, &temp3, k-1, 0);
                root = merge(temp1, temp2);
                break;
            case 4:
                scanf("%d%d", &l, &r);
                split (root, &temp1, &temp2, l-1, 0);
                split (temp2, &temp2, &temp3, r-l+1, 0);
                printf("%d\n", temp2->max);
                root = merge(merge(temp1, temp2), temp3);
                break;
        }        
    }
    return 0;
}
