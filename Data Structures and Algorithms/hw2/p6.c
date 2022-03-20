// Package Arrangement

// Reference
// 1. 我有跟徐晨祐同學討論 heap 相關的邏輯
// 2. 也有參照課本 「Introduction to Algorithms」

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define maxLine 100007
#define maxPackage 100007

typedef struct cmd{
    char operation;
    int par1, par2;
}Cmd;

typedef struct package{
    int h;
    struct package *pre, *next;
}Package;

typedef struct node{
    int h;
    int degree;
    bool marked;
    struct node *parent;
    struct node *left;
    struct node *right;
    struct node *child;
}Node;

typedef struct heap{
    Node *max;
}Heap;

typedef struct line{
    int len;
    Package *head;
    Package *tail;
}Line;

Cmd cmds[maxPackage + maxLine];
Package packages[maxPackage];
Node nodes[maxPackage];
Heap heaps[maxLine];
Line lines[maxLine];
int target[maxPackage];
int pos[maxPackage];
int map[maxLine];
int idx;

void heap_insert(int h, int l){
    Heap *H = &heaps[l];
    Node *x = &nodes[h];
    x->h = h;
    x->degree = 0;
    x->marked = false;
    x->parent = NULL;
    x->child = NULL;
    if(lines[l].len == 0){
        H->max = x;
        x->left = x;
        x->right = x;
    }
    else{
        x->left = H->max->left;
        x->right = H->max;
        H->max->left->right = x;
        H->max->left = x;
        if(H->max->h < x->h)
            H->max = x;        
    }
    return ;
}

void push(int h, int l){
    packages[h].h = h;
    packages[h].next = NULL;
    if(lines[l].len == 0){
        packages[h].pre = NULL;
        lines[l].head = &packages[h];
        lines[l].tail = &packages[h];
        pos[h] = l;
    }
    else{
        packages[h].pre = lines[l].tail;
        lines[l].tail->next = &packages[h];
        lines[l].tail = &packages[h];
        pos[h] = pos[lines[l].head->h];
    }
    heap_insert(h, l);
    lines[l].len ++;
    return ;
}

void heap_union(int lb, int lr){
    Heap *H1 = &heaps[lb], *H2 = &heaps[lr];
    Node *temp = H1->max->left;
    H1->max->left->right = H2->max;
    H1->max->left = H2->max->left;
    H2->max->left->right = H1->max;
    H2->max->left = temp;
    if(H2->max->h < H1->max->h)
        H2->max = H1->max;
    return ;
}

void merge(int lb, int lr){
    if(lines[lb].len == 0)
        return;
    else if(lines[lr].len == 0){
        lines[lr].head = lines[lb].head;
        lines[lr].tail = lines[lb].tail;
        heaps[lr].max = heaps[lb].max;
        map[pos[lines[lr].head->h]] = lr;
    }
    else{
        Package *temp;
        int l;
        if(lines[lr].len < lines[lb].len){
            temp = lines[lr].head;
            l = pos[lines[lb].head->h];
            for(int i = 0; i < lines[lr].len && temp != NULL; i++){
                pos[temp->h] = l;
                temp = temp->next;
            }
        }
        else{
            temp = lines[lb].head;
            l = pos[lines[lr].head->h];
            for(int i = 0; i < lines[lb].len; i++){
                pos[temp->h] = l;
                temp = temp->next;
            }
        }
        lines[lr].tail->next = lines[lb].head;
        lines[lb].head->pre = lines[lr].tail;
        lines[lr].tail = lines[lb].tail;
        map[l] = lr;
        heap_union(lb, lr);
    }
    lines[lr].len += lines[lb].len;
    lines[lb].len = 0;
    lines[lb].head = NULL;
    lines[lb].tail = NULL;
    heaps[lb].max = NULL;
    return ;
}

void heap_link(Node *y, Node *x){
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if(x->child == NULL){
        x->child = y;
        y->left = y;
        y->right = y;
    }
    else{
        y->left = x->child->left;
        y->right = x->child;
        x->child->left->right = y;
        x->child->left = y;
        if(y->h > x->child->h)
            x->child = y;
    }
    x->degree ++;
    return ;
}

void heap_consolidate(int l){
    Heap *H = &heaps[l];
    int maxdegree = log(lines[l].len + 1) / log(2) + 7, d;
    Node *A[maxdegree], *now = H->max, *next = now, *x, *y, *temp;
    for(int i = 0; i < maxdegree; i++)
        A[i] = NULL;
    bool test = true;
    while(A[now->degree] != now && test){
        d = now->degree;
        next = now->right;
        if(A[d] == NULL){
            A[d] = now;
            now = next;
        }
        else{
            while(A[d] != NULL){
                y = A[d];
                x = now;
                if(x->h < y->h){
                    temp = x;
                    x = y;
                    y = temp;
                }
                if(y == next)
                    test = false;
                heap_link(y, x);
                A[d] = NULL;
                d ++;
                now = x;
                if(A[d] == NULL)
                    A[d] = now;
                if(A[d] == now)
                    break;
            }
            if(test)
                now = next;
        }
    }
    H->max = now;
    temp = now;
    now = now->right;
    while(now != temp){
        if(H->max->h < now->h)
            H->max = now;
        now = now->right;
    }
    return ;
}

void heap_extract_max(int l){
    Heap *H = &heaps[l];
    Node *z = H->max;
    Node *x = z->child;
    if(x == NULL){
        if(z->right == z){
            H->max = NULL;
            return ;
        }
        else{
            z->right->left = z->left;
            z->left->right = z->right;
            H->max = z->right;
        }
    }
    else{
        while(x->parent != NULL){
            x->parent = NULL;
            x = x->right;
        }
        if(z->right != z){
            z->left->right = x;
            z->right->left = x->left;
            x->left->right = z->right;
            x->left = z->left;
        }
        H->max = x;
    }
    heap_consolidate(l);
    return ;
}

void heap_cut(Heap *H, Node *x, Node *y){
    if(x == x->right)
        y->child = NULL;
    else{
        x->right->left = x->left;
        x->left->right = x->right;
        if(x == y->child)
            y->child = x->right;
    }
    y->degree --;
    x->parent = NULL;
    x->marked = false;
    x->left = H->max;
    x->right = H->max->right;
    H->max->right->left = x;
    H->max->right = x;
    return ;
}

void heap_cascading_cut(Heap *H, Node *y){
    Node *z = y->parent;
    if(z != NULL){
        if(y->marked == false)
            y->marked = true;
        else{
            heap_cut(H, y, z);
            heap_cascading_cut(H, z);
        }
    }
    return ;
}

void heap_increase_key(int h, int l, int k){
    Heap *H = &heaps[l];
    Node *x = &nodes[h];
    Node *y = x->parent;
    x->h = k;
    if(y != NULL && x->h > y->h){
        heap_cut(H, x, y);
        heap_cascading_cut(H, y);
    }
    if(x->h > H->max->h)
        H->max = x;
    return ;
}

void heap_delete(int h, int l){
    heap_increase_key(h, l, maxPackage);
    heap_extract_max(l);
    return ;
}

void check(int N){
    while(idx < N && pos[target[idx]] != -1){
        int h = target[idx];
        int l = map[pos[h]];
        if(heaps[l].max == &nodes[h]){
            if(packages[h].pre != NULL)
                packages[h].pre->next = packages[h].next;
            if(packages[h].next != NULL)
                packages[h].next->pre = packages[h].pre;
            if(lines[l].head == &packages[h])
                lines[l].head = lines[l].head->next;
            else if(lines[l].tail == &packages[h])
                lines[l].tail = lines[l].tail->pre;
            heap_extract_max(l);
            lines[l].len --;
            if(lines[l].len == 0){
                lines[l].head = NULL;
                lines[l].tail = NULL;
                heaps[l].max = NULL;
            }
            idx ++;
        }
        else if(lines[l].head == &packages[h]){
            lines[l].head = lines[l].head->next;
            if(lines[l].head != NULL)
                lines[l].head->pre = NULL;
            heap_delete(h, l);
            lines[l].len --;
            if(lines[l].len == 0){
                lines[l].head = NULL;
                lines[l].tail = NULL;
                heaps[l].max = NULL;
            }
            idx ++;
        }
        else if(lines[l].tail == &packages[h]){   
            lines[l].tail = lines[l].tail->pre;
            if(lines[l].tail != NULL)
                lines[l].tail->next = NULL;
            heap_delete(h, l);
            lines[l].len --;
            if(lines[l].len == 0){
                lines[l].head = NULL;
                lines[l].tail = NULL;
                heaps[l].max = NULL;
            }
            idx ++;   
        }
        else
            break;
    }
    return ;
}

int main(void){
    int T;
    scanf("%d", &T);
    for(int i = 0; i < T; i++){
        int N, O, L;
        scanf("%d%d%d", &N, &O, &L);
        char string[6];
        for(int j = 0; j < O; j++){
            scanf("%s%d%d", string, &cmds[j].par1, &cmds[j].par2);
            cmds[j].operation = string[0];
        }
        for(int j = 0; j < L; j++){
            lines[j].len = 0;
            lines[j].head = NULL;
            lines[j].tail = NULL;
            heaps[j].max = NULL;
            map[j] = j;
        }
        for(int j = 0; j < N; j++){
            scanf("%d", &target[j]);
            pos[j+1] = -1;
        }
        target[N] = 0;
        pos[0] = -1;
        idx = 0;
        for(int j = 0; j < O; j++){
            if(cmds[j].operation == 'p'){
                int h = cmds[j].par1, l = cmds[j].par2;
                if(h == target[idx])
                    idx ++;
                else
                    push(h, l);
            }
            else{
                int lb = cmds[j].par1, lr = cmds[j].par2;
                merge(lb, lr);
            }
            if(idx < N && pos[target[idx]] != -1)
                check(N);
        }
        if(idx == N)
            printf("possible\n");
        else
            printf("impossible\n"); 
    }
    return 0;
}
