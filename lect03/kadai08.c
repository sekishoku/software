#include <stdio.h>
#define NALPHA 26

int main(){
    int count[NALPHA][NALPHA];
    double prob[NALPHA][NALPHA];
    int c,i,j,sum;
    int first,second,minidx,maxidx;
    double min,max;
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
        sum = 0;
        for(j=0;j<NALPHA;j++){
            sum += count[i][j];
        }
        for(j=0;j<NALPHA;j++){
            prob[i][j] = (double)count[i][j]/(double)sum;
        }
    }
    for(i=0;i<NALPHA;i++){
        min = 2.0;
        max = -1.0;
        for(j=0;j<NALPHA;j++){
            if(prob[i][j]<min && prob[i][j]!=0){
                min = prob[i][j];
                minidx = j;
            }
            if(prob[i][j]>max){
                max = prob[i][j];
                maxidx = j;
            }
        }
        printf("[%c]:min=%c(%e),max=%c(%e)\n",'a'+i,'a'+minidx,min,'a'+maxidx,max);
    }
    return 0;
}