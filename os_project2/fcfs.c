#include <stdio.h>
#include <stdlib.h>

int main(void)
{
   FILE *fp = fopen("fcfs.inp","r");

   if(fp==NULL) {
        fprintf(stderr, "File Open Fail \n");
        return 1;
   }

   int process_n;
   fscanf(fp, "%d",&process_n);

   int* arrive_t = (int*)malloc(sizeof(int)*process_n);
   int* cpu_t = (int*)malloc(sizeof(int)*process_n);
   int* wait_t = (int*)malloc(sizeof(int)*process_n);

   for(int i=0; i<process_n; i++){
        fscanf(fp, "%d", &arrive_t[i]);
        fscanf(fp, "%d", &cpu_t[i]);
   }

   int cal=arrive_t[0];
   int sum=0;

   wait_t[0]=0;
   for(int i=1; i<process_n; i++){
        cal+=cpu_t[i-1];
        if(cal <= arrive_t[i]){
            cal = arrive_t[i];
            continue;
        }

        wait_t[i] = cal - arrive_t[i];
        sum+=wait_t[i];
   }

   FILE *fp_w = fopen("fcfs.out","w");
   if(fp_w == NULL){
        fprintf(stderr,"result.out open fail\n");
        return 1;
   }

   fprintf(fp_w,"%d", sum);

   printf("처리완료. fcfs.out을 열어 확인하세요.\n");

   fclose(fp);
   fclose(fp_w);

   return 0;
}
