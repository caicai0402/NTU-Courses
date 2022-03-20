// Evil Eval

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[1000000];
int l;
int pos;

double parser(){
    double ans = 0, num, temp;
    int test = 1;
    while(1){
        if(pos == l){
            ans += num;
            break;
        }
        else if(input[pos] == '('){
            pos ++;
            num = parser();
            num *= test;
        }
        else if(input[pos] == ')'){
            ans += num;
            pos ++;
            break;
        }
        else if(input[pos] == '+'){
            test = 1;
            ans += num;
            pos ++;
        }
        else if(input[pos] == '-'){
            test = -1;
            ans += num;
            pos ++;
        }
        else if(input[pos] == '*'){
            pos ++;
            if(input[pos] == '('){
                pos ++;
                temp = parser();
            }
            else{
                temp = atoi(&input[pos]);
                while(0 <= input[pos]-'0' && input[pos]-'0' <= 9)
                    pos ++;
            }
            num *= temp;
        }
        else if(input[pos] == '/'){
            pos ++;
            if(input[pos] == '('){
                pos ++;
                temp = parser();
            }
            else{
                temp = atoi(&input[pos]);
                while(0 <= input[pos]-'0' && input[pos]-'0' <= 9)
                    pos ++;
            }
            num /= temp;
        }
        else{
            num = atoi(&input[pos]);
            num *= test;
            while(0 <= input[pos]-'0' && input[pos]-'0' <= 9)
                pos ++;
        }
    }
    return ans;
}

int main(){
    while(scanf("%s", input) != EOF){
        l = strlen(input);
        pos = 0;
        printf("%.15lf\n", parser());
    }
    return 0;
}