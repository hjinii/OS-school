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
        //Need�� ���� �ڿ����� ũ��
        if ((p[num].Max[i] - p[num].Allo[i]) > Av[i])
            return false;
    }
    return true;
}

bool request(vector<Process> p, int n, int m, vector<int> Av, int process_n, vector<int> play) {
    for (int i = 0; i < m; i++) {
        if (play[i] > Av[i]) return false;
    }
    //��û�� �ڿ��� Available �ڿ����� ū ���

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
    //��û�� �ڿ��� �Ҵ������� MAX���� ���������� check = true

    if (check) {
        p[process_n].end = true;
    }
    //check == true�̸� end�� true�� �ٲ۴�

    int next = 1;
    while (next) {
        next = 0;
        for (int i = 0; next == 0 && i < n; i++) {
            //Ȯ�� ���ߴٸ�
            if (p[i].end == false) {
                //Need�� Av��
                if (compare_need(Av, p, m, i)) {
                    //i��° ���μ����� ������
                    next = 1;
                    for (int j = 0; j < m; j++)
                        // i�� �Ҵ� �ް� Ǫ�� �۾��� �ؾ���
                        Av[j] += p[i].Allo[j];
                    p[i].end = true;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (p[i].end == false) {
            //�Ҿ��� ����
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
        // quit �Է½� ����
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

            // request(��û) �Է�
            if (s.compare("request") == 0) {
                bool check = false;
                for (int i = 0; i < m; i++) {
                     //Need > request�� ��� ��û ����
                    if (play[i] > (p[process_n].Max[i] - p[process_n].Allo[i])) {
                        check = true;
                        break;
                    }
                }
                //���� ����
                if (!check) {
                    check = request(p, n, m, Av, process_n, play);
                    if (check) {
                        for (int i = 0; i < m; i++) {
                            Av[i] -= play[i];
                            p[process_n].Allo[i] += play[i];
                        }
                    }
                    //request�� Av���� ū ���
                    //�Ҿ��� ���� -> ť ����
                    else {
                        Command c;
                        c.play = play;
                        c.process_n = process_n;
                        q.push(c);
                    }
                }

                //Av ���
                for (int i = 0; i < m; i++) {
                    out << Av[i] << " ";
                }
                out << endl;
            }
            else {
                //�ڿ� �ϴ� ��ȯ
                for (int i = 0; i < m; i++) {
                    Av[i] += play[i];
                    p[process_n].Allo[i] -= play[i];
                }

                int count = q.size();

                //���ť ���� ���� �� pop
                for (int i = 0; i < count; i++) {
                    vector<int> temp_play = q.front().play;
                    int temp_num = q.front().process_n;
                    q.pop();

                    //Need > request�� ��� �Ҵ�X -> ť ����X ����
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
                //Available ��� ���
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

    //Available ����
    for (int i = 0; i < m; i++) {
        int temp;
        inp >> temp;
        Av.push_back(temp);
    }

    //Max ����
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
    //Banker �˰���
    banker(p, n, m, Av);
}
