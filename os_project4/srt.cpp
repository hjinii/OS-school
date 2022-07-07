#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string.h>
#include <fstream>

using namespace std;

struct process {
    int arrive_t;
    int cpu_t;
    int start_t;
    int complete_t;
    int turnaround_t;
    int wait_t;
};

int main()
{
    int process_n;
    struct process p[1000];

    ifstream fr;
    fr.open("srt.inp");

    if(!fr.is_open()){
        cerr << "Could not open the file" << endl;
        exit(1);
    }
    else{
        fr >> process_n;
        for(int i=0; i<process_n; i++){
            fr >> p[i].arrive_t;
            fr >> p[i].cpu_t;
        }
    }

    int total_wait_t;
    int total_idle_t;
    int cpu_remaining[process_n];
    int is_complete[process_n];
    memset(is_complete,0,sizeof(is_complete));

    for(int i=0; i<process_n; i++){
        cpu_remaining[i] = p[i].cpu_t;
    }

    int current_t = 0;
    int complte = 0;
    int prev = 0;

    while(complte != process_n){
        int idx = -1;
        int min = 1000;
        for(int i=0; i<process_n; i++){
            if(p[i].arrive_t <= current_t && is_complete[i] == 0){
                if(cpu_remaining[i] < min){
                    min = cpu_remaining[i];
                    idx = i;
                }
                if(cpu_remaining[i] == min){
                    if(p[i].arrive_t < p[idx].arrive_t){
                        min = cpu_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1){
            if(cpu_remaining[idx] == p[idx].cpu_t){
                p[idx].start_t = current_t;
                total_idle_t += p[idx].start_t - prev;
            }
            cpu_remaining[idx] -= 1;
            current_t++;
            prev = current_t;

            if(cpu_remaining[idx] == 0){
                p[idx].complete_t = current_t;
                p[idx].turnaround_t = p[idx].complete_t - p[idx].arrive_t;
                p[idx].wait_t = p[idx].turnaround_t - p[idx].cpu_t;

                total_wait_t += p[idx].wait_t;

                is_complete[idx] = 1;
                complte++;
            }
        }
        else{
            current_t++;
        }
    }

    ofstream fw;
    fw.open("srt.out");
    fw << total_wait_t;

    cout << "파일 쓰기 완료. srt.out을 열어 확인하세요" << endl;

    fr.close();
    fw.close();
}
