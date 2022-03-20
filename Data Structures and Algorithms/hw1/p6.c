// K-Least Element in the Sequence

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000
#define MAXLIST 100

typedef struct list{
    int *ptr;
    int *sorted;
    int num, reverse;
    struct list *pre, *next;
}List;

List *new(List *pre, List *next){
    List *newList = (List *)malloc(sizeof(List));
    newList->num = 0;
    newList->reverse = 0;
    newList->ptr = (int *)malloc(sizeof(int) * MAXLEN);
    newList->sorted = (int *)malloc(sizeof(int) * MAXLEN);
    newList->pre = pre;
    newList->next = next;
    return newList;
}

int cmp(const void *l, const void *r){
    int *a = (int *)l, *b = (int *)r;
    return *a - *b;
}

List *findList(List *ptr, int *l, int *r){
    while(*l - ptr->num >= 0 && ptr->next != NULL){
        *l -= ptr->num;
        if(r != NULL)
            *r -= ptr->num;
        ptr = ptr->next;
    }
    return ptr;
}

void reverse(List *index){
    index->reverse = 0;
    int temp;
    for(int i = 0; i < index->num/2; i++){
        temp = index->ptr[i];
        index->ptr[i] = index->ptr[index->num-1-i];
        index->ptr[index->num-1-i] = temp;
    }
    return ;
}

int stop(int less, int equal, int len, int k){
    if(less >= k)
        return 1;
    else if(less + equal + len < k)
        return -1;
    else if(less + equal >= k && k - less > len)
        return 0;
    else
        return 666;
}

int test(List *index1, List *index2, int l, int r, int k, int len, int t){
    int less = 0, equal = 0, result;
    if(index1 == index2){
        for(int i = l; i <= r; i++){
            if(index1->ptr[i] < t)
                less ++;
            else if(index1->ptr[i] == t)
                equal ++;
            len --;
            result = stop(less, equal, len, k);
            if(result != 666)
                return result;
        }
    }
    else{
        int index = 0;
        while(index < index1->num-l || index < r+1){
            if(index < index1->num-l){
                if(index1->ptr[index+l] < t)
                    less ++;
                else if(index1->ptr[index+l] == t)
                    equal ++;
                len --;
            }
            if(index < r+1){
                if(index2->ptr[index] < t)
                    less ++;
                else if(index2->ptr[index] == t)
                    equal ++;
                len --;
            }
            result = stop(less, equal, len, k);
            if(result != 666)
                return result;
            index ++;
        }
        index1 = index1->next;
        while(index1 != index2){
            int low[2] = {0, 0}, up[2] = {index1->num-1, index1->num-1};
            while((low[0] <= up[0] || low[1] <= up[1])){
                if(low[0] <= up[0]){
                    if(index1->sorted[(low[0] + up[0]) / 2] < t)
                        low[0] = (low[0] + up[0]) / 2 + 1;
                    else
                        up[0] = (low[0] + up[0]) / 2 - 1;
                }
                if(low[1] <= up[1]){
                    if(index1->sorted[(low[1] + up[1]) / 2] > t)
                        up[1] = (low[1] + up[1]) / 2 - 1;
                    else
                        low[1] = (low[1] + up[1]) / 2 + 1;
                }
            }
            less += low[0];
            equal += low[1] - low[0];
            len -= index1->num;
            result = stop(less, equal, len, k);
            if(result != 666)
                return result;
            index1 = index1->next;
        }
    }
    return 0;
}

int main(void){
    int n, q;
    scanf("%d%d", &n, &q);
    List *head = new(NULL, NULL), *temp = head;
    int listnum = 0, data;
    while(n > 0){
        temp->next = new(temp, NULL);
        temp = temp->next;
        for(int i = 0; i <= (n - 1)/(MAXLIST - listnum) && n > 0; i++){
            scanf("%d", &data);
            temp->ptr[temp->num] = data;
            temp->sorted[temp->num] = data;
            temp->num ++;
        }
        qsort(temp->sorted, temp->num, sizeof(int), cmp);
        n -= (n - 1)/(MAXLIST - listnum) + 1;
        listnum ++;
    }
    char cmd[8];
    for(int i = 0; i < q; i++){
        scanf("%s", cmd);
        if(cmd[0] == 'I'){
            int pos, x;
            scanf("%d%d", &pos, &x);
            pos --;
            List *index = findList(head->next, &pos, NULL);
            if(index->reverse == 1)
                pos = index->num - pos;
            int low = 0, up = index->num - 1;
            while(low <= up){
                if(index->sorted[(low + up) / 2] <= x)
                    low = (low + up) / 2 + 1;
                else if(index->sorted[(low + up) / 2] > x)
                    up = (low + up) / 2 - 1;
            }
            memcpy(&index->ptr[pos+1], &index->ptr[pos], sizeof(int)*(index->num-pos));
            index->ptr[pos] = x;
            memcpy(&index->sorted[low+1], &index->sorted[low], sizeof(int)*(index->num-low));
            index->sorted[low] = x;
            index->num ++;
            if(index->num == MAXLEN){
                if(index->reverse == 0){
                    index->next = new(index, index->next);
                    if(index->next->next != NULL)
                        index->next->next->pre = index->next;
                    index->next->num = MAXLEN/2;
                    memcpy(index->next->ptr, &index->ptr[MAXLEN/2], sizeof(int) * MAXLEN/2);
                    memcpy(index->next->sorted, index->next->ptr, sizeof(int) * MAXLEN/2);
                    qsort(index->next->sorted, MAXLEN/2, sizeof(int), cmp);
                }
                else{
                    index->pre = new(index->pre, index);
                    if(index->pre->pre != NULL)
                        index->pre->pre->next = index->pre;
                    index->pre->num = MAXLEN/2;
                    index->pre->reverse = 1;
                    memcpy(index->pre->ptr, &index->ptr[MAXLEN/2], sizeof(int) * MAXLEN/2);
                    memcpy(index->pre->sorted, index->pre->ptr, sizeof(int) * MAXLEN/2);
                    qsort(index->pre->sorted, MAXLEN/2, sizeof(int), cmp);
                }
                index->num = MAXLEN/2;
                memcpy(index->sorted, index->ptr, sizeof(int) * MAXLEN/2);
                qsort(index->sorted, MAXLEN/2, sizeof(int), cmp);
                listnum ++;
            }
        }
        else if(cmd[0] == 'D'){
            int pos;
            scanf("%d", &pos);
            pos --;
            List *index = findList(head->next, &pos, NULL);
            if(index->reverse == 1)
                pos = index->num - pos - 1;
            if(index->num == 1 && listnum > 1){
                if(index->next != NULL)
                    index->next->pre = index->pre;
                if(index->pre != NULL)
                    index->pre->next = index->next;
                free(index->ptr);
                free(index->sorted);
                free(index);
                listnum --;
            }
            else{
                int low = 0, up = index->num - 1;
                while(low <= up){
                    if(index->sorted[(low + up) / 2] < index->ptr[pos])
                        low = (low + up) / 2 + 1;
                    else if(index->sorted[(low + up) / 2] > index->ptr[pos])
                        up = (low + up) / 2 - 1;
                    else
                        break;
                }
                memcpy(&index->ptr[pos], &index->ptr[pos+1], sizeof(int)*(index->num-pos-1));
                memcpy(&index->sorted[(low + up) / 2], &index->sorted[(low + up) / 2 + 1], sizeof(int)*(index->num-1-(low + up)/2));
                index->num --;
            }
        }
        else if(cmd[0] == 'R'){
            int l, r;
            scanf("%d%d", &l, &r);
            l --; r --;
            List *index1 = findList(head->next, &l, &r), *index2 = findList(index1, &r, NULL);
            if(index1 == index2){
                if(index1->reverse == 0)
                    for(int i = 0; i <= (r-l)/2; i++){
                        data = index1->ptr[l+i];
                        index1->ptr[l+i] = index1->ptr[r-i];
                        index1->ptr[r-i] = data;
                    }
                else
                    for(int i = 0; i <= (r-l)/2; i++){
                        data = index1->ptr[index1->num-1-l-i];
                        index1->ptr[index1->num-1-l-i] = index1->ptr[index1->num-1-r+i];
                        index1->ptr[index1->num-1-r+i] = data;
                    }
            }
            else{
                if(index1->reverse == 1)
                    reverse(index1);
                if(index2->reverse == 1)
                    reverse(index2);
                if(index1->num - l == r + 1)
                    for(int i = 0; i < r+1; i++){
                        data = index2->ptr[i];
                        index2->ptr[i] = index1->ptr[index1->num-1-i];
                        index1->ptr[index1->num-1-i] = data;
                    }
                else{
                    if(index1->num - l > r + 1){
                        if(index2->num - (r+1) + (index1->num-l) >= MAXLEN){
                            index2->next = new(index2, index2->next);
                            if(index2->next->next != NULL)
                                index2->next->next->pre = index2->next;
                            index2->next->num = index2->num-r-1;
                            index2->num = r + 1;
                            memcpy(index2->next->ptr, &index2->ptr[r+1], sizeof(int)*index2->next->num);
                            memcpy(index2->next->sorted, index2->next->ptr, sizeof(int)*index2->next->num);
                            qsort(index2->next->sorted, index2->next->num, sizeof(int), cmp);
                            listnum ++;
                        }
                        memcpy(&index2->ptr[index1->num-l-r-1], index2->ptr, sizeof(int)*(index2->num));
                        for(int i = 0; i < index1->num-l; i++){
                            data = index1->ptr[l+i];
                            index1->ptr[l+i] = index2->ptr[index1->num-l-1-i];
                            index2->ptr[index1->num-l-1-i] = data;
                        }
                    }
                    else if(index1->num - l < r + 1){
                        if(index1->num - (index1->num-l) + (r+1) >= MAXLEN){
                            index1->next = new(index1, index1->next);
                            if(index1->next->next != NULL)
                                index1->next->next->pre = index1->next;
                            memcpy(index1->next->ptr, &index1->ptr[l], sizeof(int)*(index1->num-l));
                            memcpy(index1->sorted, index1->ptr, sizeof(int)*(l));
                            qsort(index1->sorted, l, sizeof(int), cmp);
                            index1->next->num = index1->num-l;
                            index1->num = l;
                            index1 = index1->next;
                            l = 0;
                            listnum ++;
                        }
                        for(int i = 0; i < r+1; i++){
                            data = index2->ptr[r-i];
                            index2->ptr[r-i] = index1->ptr[l+i];
                            index1->ptr[l+i] = data;
                        }
                        memcpy(index2->ptr, &index2->ptr[r+1-index1->num+l], sizeof(int)*(index2->num-(r+1)+(index1->num-l)));
                    }
                    index2->num = index2->num - (r+1) + (index1->num-l);
                    index1->num = index1->num - (index1->num-l) + (r+1); 
                }
                memcpy(index1->sorted, index1->ptr, sizeof(int)*index1->num);
                memcpy(index2->sorted, index2->ptr, sizeof(int)*index2->num);
                qsort(index1->sorted, index1->num, sizeof(int), cmp);
                qsort(index2->sorted, index2->num, sizeof(int), cmp);
                int *ptr;
                while(1){
                    index1 = index1->next;
                    index2 = index2->pre;
                    if(index1 == index2){
                        index1->reverse = index1->reverse == 0? 1:0;
                        break;
                    }
                    else if(index1->pre == index2)
                        break;
                    else{
                        ptr = index1->ptr;
                        index1->ptr = index2->ptr;
                        index2->ptr = ptr;
                        ptr = index1->sorted;
                        index1->sorted = index2->sorted;
                        index2->sorted = ptr;
                        data = index1->reverse;
                        index1->reverse = index2->reverse == 0? 1:0;
                        index2->reverse = data == 0? 1:0;
                        data = index1->num;
                        index1->num = index2->num;
                        index2->num = data;
                    }
                }
            }
        }
        else if(cmd[0] == 'Q'){
            int l, r, k, len;
            scanf("%d%d%d", &l, &r, &k);
            l --; r--; len = r-l+1;
            List *index1 = findList(head->next, &l, &r), *index2 = findList(index1, &r, NULL);
            if(index1->reverse == 1)
                reverse(index1);
            if(index2->reverse == 1)
                reverse(index2);
            int low = -100000, up = 100000, result;
            while(low <= up){
                result = low == up? 0:test(index1, index2, l, r, k, len, (low + up) / 2);
                if(result == 1)
                    up = (low + up) / 2 - 1;
                else if(result == -1)
                    low = (low + up) / 2 + 1;
                else{
                    printf("%d\n", (low + up) / 2);
                    break;
                }
            }
        }
    }
    return 0;
}