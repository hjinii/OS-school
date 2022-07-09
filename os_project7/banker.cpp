#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

ifstream inp("banker.inp");
ofstream out("banker.out");

typedef struct {
    int n;
    vector<int> Max;
    vector<int> Allo;
    bool end;
}Process;

typedef struct {
    int process_n;
    vector<int> play;
}Command;

bool compare_need(vector<int> &Av, vector<Process> &p, int m, int num) {
    int i;
    for (i = 0; i < m; i++) {
        //Need가 현재 자원보다 크면
        if ((p[num].Max[i] - p[num].Allo[i]) > Av[i])
            return false;
    }
    return true;
}

bool request(vector<Process> p, int n, int m, vector<int> Av, int process_n, vector<int> play) {
    for (int i = 0; i < m; i++) {
        if (play[i] > Av[i]) return false;
    }
    //요청한 자원이 Available 자원보다 큰 경우

    for (int i = 0; i < m; i++) {
        Av[i] -= play[i];
        p[process_n].Allo[i] += play[i];
    }

    bool check = true;
    for (int i = 0; i < m; i++) {
        if (p[process_n].Max[i] != p[process_n].Allo[i]) {
            check = false;
            break;
        }
    }
    //요청한 자원을 할당했을때 MAX값과 같아졌을때 check = true

    if (check) {
        p[process_n].end = true;
    }
    //check == true이면 end를 true로 바꾼다

    int next = 1;
    while (next) {
        next = 0;
        for (int i = 0; next == 0 && i < n; i++) {
            //확인 안했다면
            if (p[i].end == false) {
                //Need와 Av비교
                if (compare_need(Av, p, m, i)) {
                    //i번째 프로세스가 수락됨
                    next = 1;
                    for (int j = 0; j < m; j++)
                        // i는 할당 받고 푸는 작업을 해야함
                        Av[j] += p[i].Allo[j];
                    p[i].end = true;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (p[i].end == false) {
            //불안정 상태
            return false;
        }
    }

    return true;
}

void banker(vector<Process> &p, int n, int m, vector<int> &Av) {
    queue<Command> q;

    while (1) {
        string s;
        inp >> s;
        // quit 입력시 종료
        if (s.compare("quit") == 0) {
            break;
        }
        else {
            int process_n;
            vector<int> play;

            inp >> process_n;

            for (int i = 0; i < m; i++) {
                int temp;
                inp >> temp;
                play.push_back(temp);
            }

            // request(요청) 입력
            if (s.compare("request") == 0) {
                bool check = false;
                for (int i = 0; i < m; i++) {
                     //Need > request일 경우 요청 무시
                    if (play[i] > (p[process_n].Max[i] - p[process_n].Allo[i])) {
                        check = true;
                        break;
                    }
                }
                //안정 상태
                if (!check) {
                    check = request(p, n, m, Av, process_n, play);
                    if (check) {
                        for (int i = 0; i < m; i++) {
                            Av[i] -= play[i];
                            p[process_n].Allo[i] += play[i];
                        }
                    }
                    //request가 Av보다 큰 경우
                    //불안정 상태 -> 큐 저장
                    else {
                        Command c;
                        c.play = play;
                        c.process_n = process_n;
                        q.push(c);
                    }
                }

                //Av 출력
                for (int i = 0; i < m; i++) {
                    out << Av[i] << " ";
                }
                out << endl;
            }
            else {
                //자원 일단 반환
                for (int i = 0; i < m; i++) {
                    Av[i] += play[i];
                    p[process_n].Allo[i] -= play[i];
                }

                int count = q.size();

                //대기큐 가장 앞의 것 pop
                for (int i = 0; i < count; i++) {
                    vector<int> temp_play = q.front().play;
                    int temp_num = q.front().process_n;
                    q.pop();

                    //Need > request일 경우 할당X -> 큐 저장X 종료
                    bool check = false;
                    for (int i = 0; i < m; i++) {
                        if (temp_play[i] > (p[temp_num].Max[i] - p[temp_num].Allo[i])) {
                            check = true;
                            break;
                        }
                    }
                    if (!check) {
                        check = request(p, n, m, Av, temp_num, temp_play);
                        if (check) {
                            for (int j = 0; j < m; j++) {
                                Av[j] -= temp_play[j];
                                p[temp_num].Allo[j] += temp_play[j];
                            }
                        }
                        else {
                            Command com_temp;
                            com_temp.play = temp_play;
                            com_temp.process_n = temp_num;
                            q.push(com_temp);
                        }
                    }
                }
                //Available 결과 출력
                for (int i = 0; i < m; i++) {
                    out << Av[i] << "  ";
                }
                out << endl;
            }
        }
    }
}

int main(void) {
    int n, m;
    inp >> n >> m;

    vector<int> Av;
    vector<Process> p;

    //Available 저장
    for (int i = 0; i < m; i++) {
        int temp;
        inp >> temp;
        Av.push_back(temp);
    }

    //Max 저장
    for (int i = 0; i < n; i++) {
        Process temp;
        temp.n = i;
        temp.end = false;
        for (int j = 0; j < m; j++) {
            int num;
            inp >> num;
            temp.Max.push_back(num);
        }
        p.push_back(temp);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int num;
            inp >> num;
            p[i].Allo.push_back(num);
            Av[j] -= num;
        }
    }
    //Banker 알고리즘
    banker(p, n, m, Av);
}
