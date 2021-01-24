#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
int GCD(int num1, int num2);
int main() {

	cout << "periodic task�� ���� �Է� : "; int cntPTask; cin >> cntPTask;
	cout << "aperiodic task�� ���� �Է� : "; int cntAPTask; cin >> cntAPTask;

	vector<vector<int>> pTask(cntPTask, vector<int>(2));
	vector<vector<int>> apTask(cntAPTask, vector<int>(2));
	vector<vector<int>> pServer(1, vector<int>(2));

	int c_p, t_p;
	cout << "periodic task�� compuation time�� �ֱ� �Է� : ";
	for (int i = 0; i < pTask.size(); i++) {
		cin >> c_p >> t_p;
		pTask[i][0] = c_p; pTask[i][1] = t_p;
	}

	int c_ap, arv_ap;
	cout << "aperiodic task�� compuation time�� �ֱ� �Է� : ";
	for (int i = 0; i < apTask.size(); i++) {
		cin >> c_ap >> arv_ap;
		apTask[i][0] = c_ap; apTask[i][1] = arv_ap;
	}

	cout << "polling server�� computation time�� �ֱ� �Է� : ";
	cin >> pServer[0][0] >> pServer[0][1];


	// t�� ª�� task�� �켱������ ���� // ��������
	vector<int> tmpVec;
	for (int i = 0; i < pTask.size(); i++) {
		for (int j = 0; j < pTask.size() - 1; j++) {
			if (pTask[j][1] > pTask[j + 1][1]) {
				tmpVec = pTask[j]; pTask[j] = pTask[j + 1]; pTask[j + 1] = tmpVec;
			}
		}
	}

	// arrival time�� ������������ ���� // ��������
	for (int i = 0; i < apTask.size(); i++) {
		for (int j = 0; j < apTask.size() - 1; j++) {
			if (apTask[j][1] > apTask[j + 1][1]) {
				tmpVec = apTask[j]; apTask[j] = apTask[j + 1]; apTask[j + 1] = tmpVec;
			}
		}
	}

	// hyperpriod�� �ּҰ����
	int hyperPrd = 1, gcd = 1;
	for (int i = 0; i < pTask.size() - 1; i++) {
		if (i == 0) gcd = GCD(pTask[0][1], pTask[1][1]);
		else {
			if (gcd >= pTask[i][1]) gcd = GCD(gcd, pTask[i + 1][1]);
			else gcd = GCD(pTask[i][1], pTask[i + 1][1]);
		}

	}
	
	for (int i = 0; i < pTask.size(); i++) {
		int tmp = 1;
		tmp = pTask[i][1] / gcd;
		if (tmp != 0) hyperPrd *= tmp;
	}
	hyperPrd *= gcd;

	int sum = 0, diff, min, min_ap, cmpTime; double avg;
	vector<int> chart;
	vector<int> candidate;
	vector<int> ap_candidate;
	vector<int> arvTime; // AP�����ð�
	vector<int> hndTime; // APó���ð�
	for (int sec = 0; sec < hyperPrd; sec++) {

		// AP �߻������� candidate�� push_back // AP = -100
		for (int i = 0; i < apTask.size(); i++) {
			if (sec == apTask[i][1]) {
				cmpTime = apTask[i][0];
				for (int j = 0; j < cmpTime; j++) {
					ap_candidate.push_back(-100);
					arvTime.push_back(sec);
				}
			}
		}

		if (sec == 0) {
			for (int i = 0; i < pTask.size(); i++) {
				cmpTime = pTask[i][0];
				for (int j = 0; j < cmpTime; j++) {
					candidate.push_back(i + 1);
				}
			}

		}
		else {
			// �ֱ⸦ �������� candidate�� push_back() 
			for (int i = 0; i < pTask.size(); i++) {
				if (sec%pTask[i][1] == 0) {
					cmpTime = pTask[i][0];
					for (int j = 0; j < cmpTime; j++) {
						candidate.push_back(i + 1);
					}
				}
			}
		}
		// slack = 101
		if (candidate.size() == 0)
			candidate.push_back(101);

		// ���� �켱����(min) ���� pop_back
		for (int i = 0; i < candidate.size(); i++) {
			if (i == 0) min = candidate.at(0);
			else {
				if (min > candidate.at(i)) min = candidate.at(i);
			}
		}

		vector<int>::iterator it = find(candidate.begin(), candidate.end(), min);
		if (sec%pServer[0][1] == 0) { // polling server�� �ֱ��϶�
			// ap_candidate���� chart�� push_back�Ѵ�
			// polling task�� compuation time��ŭ ap�� chart�� push_back
			if (find(ap_candidate.begin(), ap_candidate.end(), -100) != ap_candidate.end()) {
				for (int i = 0; i < pServer[0][0]; i++) {
					chart.push_back(-100);
					hndTime.push_back(sec);
					ap_candidate.pop_back();
				}
			}
			else {
				chart.push_back(min);
				candidate.erase(it);
			}
		}
		else { // polling server�� �ֱⰡ �ƴ� ��
			for (int i = 0; i < pServer[0][0]; i++) {
				chart.push_back(min);
				candidate.erase(it);
			}
		}

		// check deadline
		int flag = 0;
		for (int i = 0; i < pTask.size(); i++) {
			if (sec%hyperPrd == pTask[i][1]) {
				for (int j = 0; j < sec; j++) {
					if (find(chart.begin(), chart.end(), i + 1) == chart.end()) flag = -1;
				}
			}
		}
		if (flag == -1) {
			cout << "deadline miss" << endl; break;
		}
	}

	for (int i = 0; i < arvTime.size(); i++) { //sum
		diff = hndTime[i] - arvTime[i];
		sum += diff;
	}
	avg = (double)sum / apTask.size(); // avg

	for (int i = 0; i < chart.size(); i++) // print chart
		cout << i << "~" << i + 1 << " : " << chart.at(i) << endl;
	cout << "background average waiting time : " << avg << endl; // print avg
	return 0;
}
int GCD(int num1, int num2) {
	while (num2 != 0) {
		int res = num1 % num2;
		num1 = num2;
		num2 = res;
	}
	return num1;
}