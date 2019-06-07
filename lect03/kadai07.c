#include <stdio.h>
#define NALPHA 26

int main(){
    int count[NALPHA][NALPHA];
    int c,i,j;
    int first,second;
    for(i=0;i<NALPHA;i++){
        for(j=0;j<NALPHA;j++){
            count[i][j] = 0;
        }
    }
    second = ' ';
    while((c=getchar())!=EOF){
        first = second;
        second = c;
        if(first >= 'a' && first <= 'z' && second >= 'a' && second <= 'z'){
            count[first-'a'][second-'a']++;
        }
    }
    for(i=0;i<NALPHA;i++){
        for(j=0;j<NALPHA;j++){
            printf("[%c][%c]=%d\n",'a'+i,'a'+j,count[i][j]);
        }
    }
    return 0;
}