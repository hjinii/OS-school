#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen("hrn.inp","r");

   if(fp==NULL) {
        fprintf(stderr, "File Open Fail \n");
        return 1;
   }

   int process_n;
   fscanf(fp, "%d",&process_n);

   int* arrive_t = (int*)malloc(sizeof(int)*process_n);
   int* cpu_t = (int*)malloc(sizeof(int)*process_n);
   int* wait_t = (int*)malloc(sizeof(int)*process_n);

   //도착시간, 실행시간 배열에 저장
   for(int i=0; i<process_n; i++){
        fscanf(fp, "%d", &arrive_t[i]);
        fscanf(fp, "%d", &cpu_t[i]);
   }

    int end_t;
    //cpu 할당된 프로세스 순으로 정렬
   for(int i=0; i<process_n; i++){
    //제일 첫번째 인덱스라면
    if(i == 0){
        end_t = arrive_t[i] + cpu_t[i];
    }

    //제일 마지막 인덱스라면
    else if(i == (process_n -1)){
        end_t = arrive_t[i] + cpu_t[i];
    }

    else{
        //우선순위 높은 것 먼저 cpu사용
        if(arrive_t[i] <= end_t){
            double priority = (end_t - arrive_t[i] + cpu_t[i])*1.0 / cpu_t[i]; // 우선순위 계산
            int count;
            int tmp;

            int k = i;
            while(arrive_t[k+1] <= end_t && arrive_t[k+1] != NULL){
                double priority_c = (end_t - arrive_t[k+1] + cpu_t[k+1])*1.0 / cpu_t[k+1];
                if(priority < priority_c){
                    priority = priority_c;
                    count = k+1;
                }
                k++;
            }

            //우선순위 높은 값을 앞으로 switching
            if(count != NULL){
                tmp = arrive_t[i];
                arrive_t[i] = arrive_t[count];
                arrive_t[count] = tmp;

                tmp = cpu_t[i];
                cpu_t[i] = cpu_t[count];
                cpu_t[count] = tmp;
            }

            end_t += cpu_t[i];
            count = 0;
            priority = 0;
        }
        //도착시간 점프할 시
        else{
            end_t = arrive_t[i] + cpu_t[i];
        }
    }
   }

   int cal_t=arrive_t[0];
   int sum=0;
   wait_t[0]=0;

   //waiting time calculate
   for(int i=1; i<process_n; i++){
        cal_t+=cpu_t[i-1];
        if(cal_t <= arrive_t[i]){
            cal_t = arrive_t[i];
            continue;
        }

        wait_t[i] = cal_t - arrive_t[i];
        sum+=wait_t[i];
   }

   FILE *fp_w = fopen("hrn.out","w");
   if(fp_w == NULL){
        fprintf(stderr,"result.out open fail\n");
        return 1;
   }

   fprintf(fp_w,"%d", sum);


   printf("처리완료. hrn.out을 열어 확인하세요.\n");

   fclose(fp);
   fclose(fp_w);

   return 0;
}
