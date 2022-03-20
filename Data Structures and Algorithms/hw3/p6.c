// Recover Graph

#include <stdio.h>
#include <stdlib.h>
#define maxlen 200007
#define hashlen 30000000

typedef struct edge{
    int x, y;
    int edge_num;
    int ptr_num;
    int d;
    struct edge *ptr[2];
    struct edge *next;
}Edge;

Edge *hash_table[hashlen] = {NULL};
Edge edges[maxlen];
int edge_num = 0;

void edge_init(int x, int y){
    edges[edge_num].x = x;
    edges[edge_num].y = y;
    edges[edge_num].edge_num = edge_num;
    edges[edge_num].ptr_num = 0;
    edges[edge_num].d = 0;
    edges[edge_num].ptr[0] = NULL;
    edges[edge_num].ptr[1] = NULL;
    edges[edge_num].next = NULL;
    return ;
}

int hash_function(int x, int y){
    int key = ((y + x) * 13579 + (y* - x) * 246810) * 77777;
    return key > 0? key % hashlen : (-key) % hashlen;
}

void hashing(int x, int y){
    int key = hash_function(x, y);
    if(hash_table[key] == NULL)
        hash_table[key] = &edges[edge_num];
    else{
        Edge *temp = hash_table[key];
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = &edges[edge_num];
    }
    return ;
}

int find(int x, int y){
    int key = hash_function(x, y);
    Edge *temp = hash_table[key];
    while(temp != NULL){
        if(temp->x == x && temp->y == y)
            return temp->edge_num;
        temp = temp->next;
    }
    return -1;
}

void tpsort(){
    Edge *head = NULL, *temp = NULL, *iter = NULL;
    for(int i = 0; i < edge_num; i++){
        if(edges[i].d == 0){
            if(head == NULL && temp == NULL){
                head = &edges[i];
                temp = &edges[i];
                edges[i].d = i;
            }
            else{
                temp->d = i;
                edges[i].d = i;
                temp = &edges[i];
            }
        }
    }
    Edge *answer[maxlen];
    int index = 0;
    while(head != NULL){
        iter = head;
        head = NULL;
        temp = NULL;
        while(1){
            answer[index] = iter;
            index ++;
            for(int i = 0; i < iter->ptr_num; i++){
                iter->ptr[i]->d --;
                if(iter->ptr[i]->d == 0){
                    if(head == NULL && temp == NULL){
                        head = iter->ptr[i];
                        temp = iter->ptr[i];
                        temp->d = temp->edge_num;
                    }
                    else{
                        temp->d = iter->ptr[i]->edge_num;
                        temp = iter->ptr[i];
                        temp->d = temp->edge_num;
                    }
                }
            }
            if(iter->d == iter->edge_num)
                break;
            iter = &edges[iter->d];
        }
    }
    if(index == edge_num){
        printf("Yes\n");
        for(int i = 0; i < index; i++)
            printf("%d %d\n", answer[i]->x, answer[i]->y);
    }
    else
        printf("No\n");
    return ;
}

int main(void){
    int N, len, x, y, last, temp;
    scanf("%d", &N);
    for(int i = 1; i <= N; i++){
        scanf("%d", &len);
        for(int j = 0; j < len; j++){
            x = i;
            scanf("%d", &y);
            if(x < y){
                edge_init(x, y);
                hashing(x, y);
                if(j != 0){
                    edges[last].ptr[edges[last].ptr_num] = &edges[edge_num];
                    edges[last].ptr_num ++;
                    edges[edge_num].d ++;
                }
                last = edge_num;
                edge_num ++;
            }
            else if(x > y){
                x += y;
                y = x - y;
                x -= y;
                temp = find(x, y);
                if(temp == -1){
                    printf("No\n");
                    return 0;
                }
                if(j != 0){
                    edges[last].ptr[edges[last].ptr_num] = &edges[temp];
                    edges[last].ptr_num ++;
                    edges[temp].d ++;
                }
                last = temp;
            }
        }
    }
    tpsort();
    return 0;
}