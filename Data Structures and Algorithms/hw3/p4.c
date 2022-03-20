// Too Many Emails

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define maxlen 100007

char D[maxlen], G[maxlen], temp[maxlen];
int D_len, G_len;
bool appeared[52];
int check[52];

void build_check(){
    for(int i = 0; i < G_len; i++){
        if(islower(G[i])){
            check[G[i] - 'a'] ++;
            appeared[G[i] - 'a'] = true;
        }
        else if(isupper(G[i])){
            check[G[i] - 'A' + 26] ++;
            appeared[G[i] - 'A' + 26] = true;
        }
    }
    return ;
}

void delete_garbled(int temp_head, int temp_tail){
    int min = maxlen, temp_G_len = G_len;
    int head = temp_head, tail = temp_tail;
    while(temp_head <= D_len - G_len){
        while(temp_G_len > 0 && temp_tail < D_len){
            if(islower(D[temp_tail]) && appeared[D[temp_tail] - 'a']){
                check[D[temp_tail] - 'a'] --;
                if(check[D[temp_tail] - 'a'] >= 0)
                    temp_G_len --;   
            }
            else if(isupper(D[temp_tail]) && appeared[D[temp_tail] - 'A' + 26]){
                check[D[temp_tail] - 'A' + 26] --;
                if(check[D[temp_tail] - 'A' + 26] >= 0)
                    temp_G_len --;
            }
            temp_tail ++;
        }
        if(temp_G_len == 0){
            if(min > temp_tail - temp_head){
                min = temp_tail - temp_head;
                head = temp_head;
                tail = temp_tail;
                if(min == G_len)
                    break;
            }
            if(islower(D[temp_head]) && appeared[D[temp_head] - 'a']){
                check[D[temp_head] - 'a'] ++;
                if(check[D[temp_head] - 'a'] > 0)
                    temp_G_len ++;
            }
            else if(isupper(D[temp_head]) && appeared[D[temp_head] - 'A' + 26]){
                check[D[temp_head] - 'A' + 26] ++;
                if(check[D[temp_head] - 'A' + 26] > 0)
                    temp_G_len ++;
            }
            temp_head ++;
        }
        else
            break;
    }
    if(min < maxlen){
        strcpy(temp, &D[tail]);
        strcpy(&D[head], temp);
        D_len -= min;
    }
    return ;
}

void breaker(char string[], int len, int end){
    if(len == 0)
        return ;
    unsigned int prefix = 0, suffix = 0;
    for(int i = 0; i < len/2; i++){
        prefix += string[i];
        suffix += string[len-1-i];
        if(prefix == suffix){
            if(strncmp(string, &string[len-1-i], i+1) == 0){
                char c = string[i+1];
                string[i+1] = '\0';
                printf("%s|", string);
                string[i+1] = c;
                breaker(&string[i+1], len-(i+1)*2, false);
                c = string[len];
                string[len] = '\0';
                printf("%s%c", &string[len-1-i], end?'\n':'|');
                string[len] = c;
                return ;
            }
        }
    }
    char c = string[len];
    string[len] = '\0';
    printf("%s%c", string, end?'\n':'|');
    string[len] = c;
    return ;
}

int main(void){
    int t;
    scanf("%d", &t);
    for(int i = 0; i < t; i++){
        memset(D, '\0', sizeof(D));
        memset(G, '\0', sizeof(G));
        scanf("%s%s", D, G);
        D_len = strlen(D);
        G_len = strlen(G);
        memset(appeared, 0, sizeof(appeared));
        memset(check, 0, sizeof(check));
        build_check();
        for(int j = 0; j <= D_len - G_len; j++)
            if((islower(D[j]) && appeared[D[j] - 'a']) || (isupper(D[j]) && appeared[D[j] - 'A' + 26])){
                delete_garbled(j, j);
                break;
        }
        breaker(D, D_len, true);
    }
    return 0;
}