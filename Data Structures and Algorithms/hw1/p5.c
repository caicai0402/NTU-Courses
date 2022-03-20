// Waston and Abili

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct train{
    int id;
    struct train *pre, *next;
}Train;

typedef struct rail{
    Train *head[2], *tail[2];
    int num;
}Rail;

Train *new(l){
    Train *newTrain = (Train *)malloc(sizeof(Train));
    newTrain->id = l;
    newTrain->pre = NULL;
    newTrain->next = NULL;
    return newTrain;
}

void print(int k, Rail box[]){
    Train *temp;
    for(int i = 0; i < k; i++){
        temp = box[i].head[0]->next;
        if(box[i].num > 0){
            printf("%d", temp->id);
            temp = temp->next;
        }
        for(int j = 1; j < box[i].num; j++){
            printf(" %d", temp->id);
            temp = temp->next;
        }
        printf("\n");
    }
    return ;
}

int main(void){
    int k, n;
    scanf("%d%d", &k, &n);
    Rail box[k];
    for(int i = 0; i < k; i++){
        box[i].num = 0;
        box[i].head[0] = new(-1);
        box[i].tail[0] = box[i].head[0];
        box[i].tail[1] = new(-1);
        box[i].head[1] = box[i].tail[1];
    }
    char cmd[8];
    for(int i = 0; i < n; i++){
        scanf("%s", cmd);
        if(strcmp(cmd, "enter") == 0){
            int r, l;
            scanf("%d%d", &r, &l);
            box[r].tail[0]->next = new(l);
            box[r].tail[0]->next->pre = box[r].tail[0];
            box[r].tail[0] = box[r].tail[0]->next;
            box[r].head[1]->pre = new(l);
            box[r].head[1]->pre->next = box[r].head[1];
            box[r].head[1] = box[r].head[1]->pre;
            box[r].num ++;
        }
        else if(strcmp(cmd, "leave") == 0){
            int r;
            scanf("%d", &r);
            if(box[r].num > 0){
                box[r].tail[0] = box[r].tail[0]->pre;
                free(box[r].tail[0]->next);
                box[r].tail[0]->next = NULL;
                box[r].head[1] = box[r].head[1]->next;
                free(box[r].head[1]->pre);
                box[r].head[1]->pre = NULL;
                box[r].num --;
            }
        }
        else if(strcmp(cmd, "migrate") == 0){
            int ra, rb;
            scanf("%d%d", &ra, &rb);
            if(box[ra].num > 0){
                box[rb].num += box[ra].num;
                box[ra].num = 0;
                box[rb].tail[0]->next = box[ra].head[1];
                box[rb].tail[0]->next->pre = box[rb].tail[0];
                box[rb].head[1]->pre = box[ra].tail[0];
                box[rb].head[1]->pre->next = box[rb].head[1];
                box[rb].tail[0] = box[ra].tail[1]->pre;
                box[rb].head[1] = box[ra].head[0]->next;
                box[ra].tail[0] = box[ra].head[0];
                box[ra].head[1] = box[ra].tail[1];
            }
        }
    }
    print(k, box);
    return 0;
}