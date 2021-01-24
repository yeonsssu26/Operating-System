#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
int main() {

	cout << "resource 개수 입력 >> "; int cnt_rsc; cin >> cnt_rsc;
	vector<int> init; cout << "resource vector 입력 >> ";
	for (int i = 0; i < cnt_rsc; i++) {
		int tmp; cin >> tmp;
		init.push_back(tmp);
	}

	cout << "process 개수 입력 >> "; int cnt_prcs; cin >> cnt_prcs;

	cout << "alloc vector 입력 >> "; // alloc은 사용자로부터 입력받기 // 0 1 0 2 0 0 3 0 2 2 1 1 0 0 2
	vector<vector<int>> alloc(cnt_prcs, vector<int>(cnt_rsc));
	for (int i = 0; i < cnt_prcs; i++) { // input alloc
		for (int j = 0; j < cnt_rsc; j++) {
			int tmp; cin >> tmp;
			alloc[i][j] = tmp;
		}
	}

	vector<int> sum_alloc(cnt_rsc);
	for (int i = 0; i < cnt_rsc; i++) { // input sum_alloc
		for (int j = 0; j < cnt_prcs; j++)
			sum_alloc[i] += alloc[j][i];
	}

	vector<vector<int>> max(cnt_prcs, vector<int>(cnt_rsc));
	srand((unsigned int)time(NULL)); // random
	for (int i = 0; i < cnt_prcs; i++) { // input max
		for (int j = 0; j < cnt_rsc; j++) { // max[i] vector <= init (10,5,7)
			int tmp;
			while (true) {
				tmp = rand() % 10 + 1;
				if (tmp > init[j]) continue;
				else {
					if (tmp < alloc[i][j]) continue;
					else break;
				}
			}
			max[i][j] = tmp;
		}
	}

	vector<vector<int>> need(cnt_prcs, vector<int>(cnt_rsc));
	for (int i = 0; i < cnt_prcs; i++) { // input need
		for (int j = 0; j < cnt_rsc; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}

	while (1) {
		vector<int> avail;
		for (int i = 0; i < init.size(); i++) 
			avail.push_back(init[i] - sum_alloc[i]);

		cout << endl << "check alloc input" << endl;
		for (int i = 0; i < cnt_prcs; i++) { // check alloc
			for (int j = 0; j < cnt_rsc; j++)
				cout << alloc[i][j] << ' ';
			cout << endl;
		}

		cout << endl << "sum_alloc : "; // check sum_alloc
		for (int i = 0; i < sum_alloc.size(); i++)
			cout << sum_alloc[i] << ' ';
		cout << endl;

		cout << endl << "check max input" << endl;
		for (int i = 0; i < cnt_prcs; i++) { // check max
			for (int j = 0; j < cnt_rsc; j++)
				cout << max[i][j] << ' ';
			cout << endl;
		}

		cout << endl << "check avail input" << endl;
		for (int i = 0; i < avail.size(); i++) // check avail
			cout << avail[i] << ' ';
		cout << endl;

		cout << endl << "check need vector" << endl;
		for (int i = 0; i < cnt_prcs; i++) { // check need
			for (int j = 0; j < cnt_rsc; j++)
				cout << need[i][j] << ' ';
			cout << endl;
		}cout << endl;

		cout << "Enter process number to request >> "; int req_prcs; cin >> req_prcs;
		cout << "Enter request vector >>"; vector<int> req;

		for (int i = 0; i < cnt_rsc; i++) {
			int tmp; cin >> tmp;
			req.push_back(tmp);
		}

		int flag = 1;
		
		for (int i = 0; i < req.size(); i++) { //1) request vector <= need vector
			if (req.at(i) > need[req_prcs].at(i)) flag = 0;
		}

		if (flag == 1) { //2) request vector <= avail vector
			for (int i = 0; i < req.size(); i++) {
				if (req.at(i) > avail.at(i)) flag = 0;
			}
		}

		vector<int> pretend_avail; //3) pretend
		for (int i = 0; i < avail.size(); i++) { pretend_avail.push_back(avail.at(i)); }
		if (flag == 1) {
			for (int i = 0; i < req.size(); i++) {
				pretend_avail[i] -= req.at(i);
				alloc[req_prcs][i] += req.at(i);
				sum_alloc[i] += req.at(i);
				need[req_prcs][i] -= req.at(i);
			}
		}

		if (flag == 1) {
			for (int i = 0; i < avail.size(); i++) {
				if (avail.at(i) < 0) flag = 0;
			}
		}

		if (flag == 1) {
			for (int i = 0; i < req.size(); i++) {
				if (need[req_prcs].at(i) < 0) flag = 0;
			}
		}

		vector<bool> finish; vector<int> seqc;
		int deadlock = 0, i = 0; int cnt_false = 0, cnt_fail = 0;

		for (int i = 0; i < need.size(); i++) finish.push_back(false); // initialization finish vector

		if (flag == 1) {

			for (int i = 0; i < finish.size(); i++) { //deadlock check
				if (finish[i] == false) cnt_false++;
			}

			while (deadlock == 0) {
				if (seqc.size() == finish.size()) break;
				if (i == finish.size()) {
					i = 0;
				}

				if (finish[i] == true) { i++; continue; }
				else {
					int cnt = 0;
					for (int j = 0; j < need[i].size(); j++) {
						if (need[i][j] > pretend_avail[j]) cnt++;
					}

					if (cnt == 0) {
						for (int j = 0; j < avail.size(); j++) { pretend_avail[j] += alloc[i][j]; }
						
						seqc.push_back(i);
						finish[i] = true;

						cnt_false = 0, cnt_fail = 0;

						for (int i = 0; i < finish.size(); i++) { //deadlock check
							if (finish[i] == false) cnt_false++;
						}
					}
					else {
						cnt_fail++;
						if (cnt_fail == cnt_false) deadlock = 1;
					}i++;
				}
			}
		}

		//print res
		if (flag == 0) cout << "unsafe" << endl;
		else {
			if (deadlock == 1) {
				cout << "deadlock" << endl;

				for (int i = 0; i < req.size(); i++) { // set back
					pretend_avail[i] += req.at(i);
					alloc[req_prcs][i] -= req.at(i);
					sum_alloc[i] -= req.at(i);
					need[req_prcs][i] += req.at(i);
				}

			}
			else {
				for (int j = 0; j < avail.size(); j++) { avail[j] = pretend_avail[j]; } // pretend_avail -> avail

				cout << "currently safe : ";
				for (int i = 0; i < seqc.size(); i++) {
					cout << "P" << seqc.at(i) << ' ';
				} cout << "is a safe sequence" << endl;
			}
		}cout << endl;

		cout << "continue? [y/n] >> "; char start; cin >> start;
		if (start == 'n') break;
	}
	return 0;
}