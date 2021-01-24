#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void sort(vector<int>&_hole_size, vector<int>& _start_idx_of_hole);
int main() {
	cout << "Enter memory size >> "; int size_mem; cin >> size_mem;
	vector<int> mem;
	for (int i = 0; i < size_mem; i++) mem.push_back(0);

	int curr_idx = 0, cnt_hole = 0, able = 1;
	vector<int> hole_size; vector<int> start_idx_of_hole;
	vector<int> process_only;
	while (true) {
		cout << "Enter id and size >> "; int id, size; cin >> id >> size;
		if (size_mem < size) {
			cout << "request size is bigger than memory size" << endl;
			continue;
		}

		if (size == 0) { // release
			int start_release_idx;
			for (int i = 0; i < mem.size(); i++) {
				if (mem.at(i) == id) {
					start_release_idx = i;
					break;
				}
			}

			int end_release_idx;
			for (int i = 0; i < mem.size(); i++) {
				if (mem.at(i) == id) {
					end_release_idx = i; // release할 proess의 마지막 idx 저장
				}
			}

			for (int i = start_release_idx; i <= end_release_idx; i++) {
				mem[i] = 0;
			}

			// process vector release // vector erase
			vector<int>::iterator it = find(process_only.begin(), process_only.end(), id);
			int release_size = end_release_idx - start_release_idx + 1;
			process_only.erase(it, it + release_size);

			// after release : is hole?
			release_size = end_release_idx - start_release_idx + 1;
			if (mem.at(end_release_idx + 1) == 0) { // hole을 만들지 않는 release라면 curr_idx를 이동시킨다
				curr_idx -= release_size;
			}
		}
		else { // allocate
			int hole_size_tmp = 0, end_hole_idx = 0, start_hole_idx = 0;
			for (int i = 0; i < mem.size(); i++) {
				if (mem.at(i) == 0) {
					hole_size_tmp++;
					end_hole_idx = i;
				}
				else {
					if (i != 0) {
						if (mem.at(i - 1) == 0) {
							hole_size.push_back(hole_size_tmp);

							start_hole_idx = end_hole_idx - hole_size_tmp + 1;
							start_idx_of_hole.push_back(start_hole_idx);

							hole_size_tmp = 0;
						}
					}
				}
				if (i == mem.size() - 1) {
					if (hole_size_tmp != 0) {
						hole_size.push_back(hole_size_tmp);

						start_hole_idx = end_hole_idx - hole_size_tmp + 1;
						start_idx_of_hole.push_back(start_hole_idx);
					}
				}
			}
			cnt_hole = hole_size.size() - 1;

			// allocate
			if (cnt_hole == 0) { // hole이 없으면 순차적으로 넣고 
				for (int i = curr_idx; i < curr_idx + size; i++) {
					mem[i] = id;
				}
				curr_idx += size;
			}
			else { // hole이 있으면 best fit으로 넣는다 // best fit : allocate to the smallest hole
				int need = 0;
				for (int i = 0; i < hole_size.size(); i++) {
					if (hole_size.at(i) >= size) break; // best fit
					else { // compaction
						if (i == hole_size.size() - 1) need = 1;
					}
				}

				if (need == 1) { // need to compaction

					int sum_hole = 0;
					for (int i = 0; i < hole_size.size(); i++) {
						sum_hole += hole_size.at(i);
					}
					if (sum_hole < size) {
						cout << "cannot allocate process " << id << endl; // hole size 총합보다 request size가 클 때 // allocate 할 수 없음
						cout << "request size is bigger than sum of hole size" << endl;
						able = 0;
					}
					else {
						for (int i = 0; i < process_only.size(); i++) { // 앞쪽은 process
							mem[i] = process_only.at(i);
						}

						// alloc
						int start_alloc = process_only.size();
						for (int i = start_alloc; i < start_alloc + size; i++) {
							mem[i] = id;
						}


						for (int i = start_alloc + size; i < mem.size(); i++) { // 뒷쪽은 hole
							mem[i] = 0;
						}
					}

				}
				else {
					int min_idx = 0;

					sort(hole_size, start_idx_of_hole); // sort function

					while (1) { // find min_hole_size
						if (hole_size.at(min_idx) < size)  min_idx++;
						else break;
					}

					for (int i = start_idx_of_hole.at(min_idx); i < start_idx_of_hole.at(min_idx) + hole_size.at(min_idx); i++) {
						mem[i] = id;
					}
					curr_idx += size;
				}
			}

			// process vector에도 push_back
			for (int i = 0; i < mem.size(); i++) {
				if (mem.at(i) == id) {
					process_only.push_back(id);
				}
			}
		}

		if (able == 1) {
			hole_size.clear();
			start_idx_of_hole.clear();

			int hole_size_tmp = 0, end_hole_idx = 0, start_hole_idx = 0;
			for (int i = 0; i < mem.size(); i++) {
				if (mem.at(i) == 0) {
					hole_size_tmp++;
					end_hole_idx = i;
				}
				else {
					if (i != 0) {
						if (mem.at(i - 1) == 0) {
							hole_size.push_back(hole_size_tmp); hole_size_tmp = 0;
							start_hole_idx = end_hole_idx - hole_size_tmp + 1;
							start_idx_of_hole.push_back(start_hole_idx);

						}
					}
				}
				if (i == mem.size() - 1) {
					if (hole_size_tmp != 0) {
						hole_size.push_back(hole_size_tmp);
						start_idx_of_hole.push_back(start_hole_idx);
					}
				}
			}
			cnt_hole = hole_size.size() - 1;

			int idx = 0, mem_height;
			if (mem.size() % 10 != 0) {
				mem_height = mem.size() / 10 + 1;
			}
			else {
				mem_height = mem.size() / 10;
			}

			int flag = 1;
			for (int i = 0; i < mem_height; i++) { // check mem
				for (int j = 0; j < 10; j++) {
					cout << mem.at(idx) << ' ';
					idx++;
					if (idx == mem.size()) { flag = 0; break; }
				}cout << endl;
				if (flag == 0) break;
			}cout << endl;

			// print res
			int free_size = mem.size() - process_only.size();
			double avg_size = double(free_size) / (cnt_hole + 1);

			cout << "free size : " << free_size << "K free" << endl;
			cout << "hole : " << cnt_hole + 1 << " block(s)" << endl;
			cout << "average size = " << avg_size << "K " << endl << endl;
		}

		cout << "continue? [y/n] >> "; char question; cin >> question;
		if (question == 'n') break;
		else {
			hole_size.clear();
			start_idx_of_hole.clear();
			able = 1;
		}
	}
	return 0;
}
void sort(vector<int>&_hole_size, vector<int>& _start_idx_of_hole) {
	int tmp;
	for (int i = 0; i < _hole_size.size() - 1; i++) { // bubble sort
		for (int j = i + 1; j < _hole_size.size(); j++) {
			if (_hole_size.at(i) > _hole_size.at(j)) {
				tmp = _hole_size.at(i); _hole_size[i] = _hole_size[j]; _hole_size[j] = tmp; // sort _hole_size
				tmp = _start_idx_of_hole.at(i); _start_idx_of_hole[i] = _start_idx_of_hole[j]; _start_idx_of_hole[j] = tmp; // sort _start_idx_of_hole
			}
		}
	}
}