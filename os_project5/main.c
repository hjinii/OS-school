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

   //�����ð�, ����ð� �迭�� ����
   for(int i=0; i<process_n; i++){
        fscanf(fp, "%d", &arrive_t[i]);
        fscanf(fp, "%d", &cpu_t[i]);
   }

    int end_t;
    //cpu �Ҵ�� ���μ��� ������ ����
   for(int i=0; i<process_n; i++){
    //���� ù��° �ε������
    if(i == 0){
        end_t = arrive_t[i] + cpu_t[i];
    }

    //���� ������ �ε������
    else if(i == (process_n -1)){
        end_t = arrive_t[i] + cpu_t[i];
    }

    else{
        //�켱���� ���� �� ���� cpu���
        if(arrive_t[i] <= end_t){
            double priority = (end_t - arrive_t[i] + cpu_t[i])*1.0 / cpu_t[i]; // �켱���� ���
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

            //�켱���� ���� ���� ������ switching
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
        //�����ð� ������ ��
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


   printf("ó���Ϸ�. hrn.out�� ���� Ȯ���ϼ���.\n");

   fclose(fp);
   fclose(fp_w);

   return 0;
}
