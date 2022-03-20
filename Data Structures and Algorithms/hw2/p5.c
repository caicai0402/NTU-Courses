// Intersecting triangles

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

typedef struct triangle{
    int p, q, r;
}Triangle;

Triangle box[3000000];
int left[3000000], right[3000000];
long long ans;

int cmp(const void *l, const void *r){
    Triangle *a = (Triangle *)l, *b = (Triangle *)r;
    if(a->p == b->p){
        return b->r - a->r;
    }
    else
        return a->p - b->p;
}

void merge(int l, int m, int r){
    int i[3] = {l, l, l}, j[3] = {m + 1, m + 1, m + 1};
    int index[2] = {0};
    int temp[2][r-l+1];
    while(i[0] <= m || j[0] <= r || i[1] <= m || j[1] <= r || (i[2] <= m && j[2] <= r)){
        if(i[2] <= m && j[2] <= r){
            if(right[i[2]] >= left[j[2]]){
                ans += m - i[2] + 1;
                j[2] ++;
            }
            else
                i[2] ++;
        }
        if(i[0] <= m || j[0] <= r){
            if(i[0] > m){
                temp[0][index[0]] = right[j[0]];
                index[0] ++;
                j[0] ++;
            }
            else if(j[0] > r){
                temp[0][index[0]] = right[i[0]];
                index[0] ++;
                i[0] ++;
            }
            else if(right[i[0]] <= right[j[0]]){
                temp[0][index[0]] = right[i[0]];
                index[0] ++;
                i[0] ++;
            }
            else{
                temp[0][index[0]] = right[j[0]];
                index[0] ++;
                j[0] ++;
            }
        }
        if(i[1] <= m || j[1] <= r){
            if(i[1] > m){
                temp[1][index[1]] = left[j[1]];
                index[1] ++;
                j[1] ++;
            }
            else if(j[1] > r){
                temp[1][index[1]] = left[i[1]];
                index[1] ++;
                i[1] ++;
            }
            else if(left[i[1]] <= left[j[1]]){
                temp[1][index[1]] = left[i[1]];
                index[1] ++;
                i[1] ++;
            }
            else{
                temp[1][index[1]] = left[j[1]];
                index[1] ++;
                j[1] ++;
            }
        }
    }
    for(int k = 0; k < index[0]; k++){
        right[k + l] = temp[0][k];
        left[k + l] = temp[1][k];
    }
    return ;
}

void sort(int l, int r){
    if(l >= r)
        return;
    int m = (l + r) / 2;
    sort(l, m);
    sort(m+1, r);
    merge(l, m, r);
    return ;
}

void mymergesort(int n){
    sort(0, n-1);
    return ;
}

int main() {
    generator.init();
    int t = generator.getT();
    while(t--){
        int n, *p, *q, *r;
        generator.getData(&n, &p, &q, &r);
        ans = 0;
        for(int i = 0; i < n; i++){
            box[i].p = p[i];
            if(q[i] < r[i]){
                box[i].q = q[i];
                box[i].r = r[i];
            }
            else{
                box[i].q = r[i];
                box[i].r = q[i];
            }
        }
        qsort(box, n, sizeof(Triangle), cmp);
        for(int i = 0; i < n; i++){
            left[i] = box[i].q;
            right[i] = box[i].r;
        }
        mymergesort(n);
        printf("%lld\n", ans);
    }
}