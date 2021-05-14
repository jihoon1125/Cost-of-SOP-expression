#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

int trans_num(char**miniset, int set_num, int bit_length);//transistor의 개수를 계산해줄 함수
void search_set(char **table, char**prime_implicant, int prime_check, int min_num, char**miniset, int mini_num, int bit_length);//prime_implicant의 minimum set을 구하는 함수
void PItable(char **prime_implicant, int prime_check, char ** min, int min_num, int bit_length);// PI table을 구현할 함수
int get_prime2(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num);//prime_implicant의 메모리 크기를 반환해줄 함수
void column(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num);//quine mccluskey method의 process를 진행하는 함수
void my_QM();

int main(void)
{
	clock_t start_time = 0;
	clock_t end_time = 0;
	float run_time = 0;

	start_time = clock();
	/////////////////////////////////////////////////////////////////////////////////////
	// 여러분의 Quine-McCluskey 함수 my_QM()을 작성하세요
	// 수행시간 측정을 위해 start_time과 end_time 사이에 my_QM()을 call합니다. (위반할 경우 감점)
	// 이 template은 수정하지 말고, my_QM()함수 작성 후 이 template파일과 함께 compile해 주세요
	/////////////////////////////////////////////////////////////////////////////////////
	my_QM();
	end_time = clock();
	run_time = (float)(end_time - start_time) / (CLOCKS_PER_SEC);

	cout << "알고리즘 수행 시간: " << run_time << " 초" << endl;

	return 0;
}

void my_QM()
{
	char **min;
	char**hamming, ***next_hamming;
	int bit_length, dont_num = 0, min_num = 0, d = 0, m = 0, hamming_count = 0, i, t;
	int *ham, *ham_arrange;
	char data[64], *context, **data_c;
	bool bit_check = false;
	ifstream file("input_minterm.txt");

	if (!file) cout << "파일이 존재하지 않습니다!";//파일 존재 확인

	while (!file.eof())
	{
		file.getline(data, 64);
		if (bit_check == false)
		{
			bit_length = atoi(strtok_s(data, " ", &context));//파일의  첫줄 숫자를 bit_length로 지정
			bit_check = true;
			continue;
		}
		else
		{
			if (*strtok_s(data, " ", &context) == 'd')//bit_length 지정 후, d가 확인 되면 dont_num++, 
				                                     //m가 확인되면 min_num++
				dont_num++;
			else
				min_num++;
		}
	}

	min = new char *[min_num];//min의 메모리 할당

	for (int i = 0; i < min_num; i++)//bit_length 만큼 2차원 메모리 할당
		min[i] = new char[bit_length];

	file.close();

	ifstream fin("input_minterm.txt");//파일 재오픈

	bit_check = false;

	data_c = new char *[min_num + dont_num];
	for (int i = 0; i < min_num + dont_num; i++)
		data_c[i] = new char[64];

	i = 0;
	t = 0;
	ham = new int[bit_length + 1];//bit_length+1만큼의 경우만큼 1이 존재할 수 있는 경우의 수가 있으므로 bit_length+1만큼 동적할당.
	ham_arrange = new int[bit_length + 1];// 위와 같은 이유.

	hamming = new char *[min_num + dont_num];//파일의 모든 dont, true minterm들을 저장할 hamming 포인터의 메모리를 항의 총 개수만큼 할당.
	for (int i = 0; i < min_num + dont_num; i++)
		hamming[i] = new char[bit_length];

	for (int i = 0; i < bit_length + 1; i++)
	{
		ham[i] = 0;
		ham_arrange[i] = 0;//ham과 ham_arrange의 모든 값을 0으로 초기화. 처음에는 아무것도
		                  //저장되어 있지 않으므로 각 배열의 1의 개수를 나타내는 이 배열들의 각 원소의 값은 0이어야 한다.
	}

	while (!fin.eof())
	{
		fin.getline(data_c[t], 64);

		if (bit_check == false)
		{
			bit_check = true;
			continue;
		}
		else
		{
			hamming_count = 0;
			if (*strtok_s(data_c[t], " ", &context) == 'm')
			{
				min[i] = strtok_s(NULL, " ", &context);//m이 확인되면 min배열에 차례대로 minterm을 저장.
				for (int k = 0; k < bit_length; k++)
				{
					if (min[i][k] == '1')//min[i]의 1의 개수만큼 hamming_count++
						hamming_count++;
				}
				ham[hamming_count]++;//hamming_count는 1의 개수를 의미하므로 ham[hamming_count]는 hamming_count에 해당하는 배열의 개수들을 의미한다.
				hamming[t] = min[i];//hamming에 min 배열 저장.
				i++;

			}
			else
			{
				hamming[t] = strtok_s(NULL, " ", &context);//hamming에 dont care 배열 저장.
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[t][k] == '1')
						hamming_count++;//1의 개수만큼 hamming_count++
				}
				ham[hamming_count]++;
			}
			t++;
		}
	}

	file.close();

	next_hamming = new char**[bit_length + 1];//지저분하게 저장된 hamming 배열을 정리하여 저장할 next_hamming 메모리 할당.

	for (int i = 0; i < bit_length + 1; i++)
		next_hamming[i] = new char*[ham[i]];

	for (int i = 0; i < bit_length + 1; i++)
	{
		for (int j = 0; j < ham[i]; j++)
			next_hamming[i][j] = new char[bit_length];
	}


	for (int i = 0; i < min_num + dont_num; i++)
	{
		t = 0;

		for (int j = 0; j < bit_length; j++)
		{
			if (hamming[i][j] == '1')
				t++;
		}
		next_hamming[t][ham_arrange[t]] = hamming[i];//hamming의 1의 개수에 따라 그룹을 나누고, 해당하는 그룹에 차근차근 hamming값을 저장한다.
		ham_arrange[t]++;
	}

	column(next_hamming, bit_length, ham, bit_length, min, min_num);//column함수 실행

	/*for (int i = 0; i < bit_length + 1; i++)//이 이후로는 메모리 de allocation. 주석을 단 부분은 주석을 안달면 에러가 납니다..
	{
	for (int j = 0; j < ham[i]; j++)
	delete[] next_hamming[i][j];
	}*/

	for (int i = 0; i < bit_length + 1; i++)
		delete[] next_hamming[i];

	delete[] next_hamming;

	delete[] ham;
	delete[] ham_arrange;

	/*	for (int i = 0; i < min_num; i++)
	delete[] min[i];*/

	delete[] min;

	for (int i = 0; i < min_num + dont_num; i++)
		delete[] data_c[i];

	delete[] data_c;

	/*for (int i = 0; i < min_num + dont_num; i++)
	delete[] hamming[i];*/
	delete[] hamming;
}

int get_prime2(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num)//column함수와 거의 동일
{
	int n = 0; int static prime_check1 = 0;
	int check = 0, check_over = 0;
	static char **prime_implicant;
	int ham_distance = 0;
	int *next_ham;
	char***next_col;
	int col_length;


	next_ham = new int[ham_length];//
	next_col = new char**[ham_length];

	for (int i = 0; i < ham_length; i++)
	{
		col_length = 0;
		for (int j = 0; j < ham[i]; j++)
		{
			for (int e = 0; e < ham[i + 1]; e++)
			{
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[i][j][k] != hamming[i + 1][e][k])
						ham_distance++;
				}
				if (ham_distance == 1)
					col_length++;
				ham_distance = 0;
			}
		}
		next_col[i] = new char*[col_length];
		next_ham[i] = col_length;
	}

	for (int i = 0; i < ham_length; i++)
	{
		for (int j = 0; j < next_ham[i]; j++)
			next_col[i][j] = new char[bit_length];
	}


	for (int i = 0; i < ham_length; i++)
	{
		for (int j = 0; j < ham[i]; j++)
		{
			for (int e = 0; e < ham[i + 1]; e++)
			{
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[i][j][k] != hamming[i + 1][e][k])
						ham_distance++;
				}
				if (ham_distance == 1)
				{
					for (int k = 0; k < bit_length; k++)
					{
						if (hamming[i][j][k] != hamming[i + 1][e][k])
							next_col[i][n][k] = 'X';
						else
							next_col[i][n][k] = hamming[i][j][k];

					}
					n++;
				}
				ham_distance = 0;
			}
			if (n == next_ham[i])
				break;
		}
		n = 0;
	}


	for (int i = 0; i < ham_length + 1; i++)
	{
		for (int j = 0; j < ham[i]; j++)
		{
			check = 0;
			for (int e = 0; e < ham_length; e++)
			{
				for (int d = 0; d < next_ham[e]; d++)
				{
					for (int k = 0; k < bit_length; k++)
					{
						if (hamming[i][j][k] != next_col[e][d][k])
							ham_distance++;
					}
					if (ham_distance == 1)
						check++;
					ham_distance = 0;
				}

			}
			if (check == 0)
			{
				prime_check1++;// column함수와 다른 부분이다. 별다른 중복검사 없이 일단 check가 0이면 prime_check1++
			}

		}
	}
	if (ham_length == 0)//ham_length가 0이 되었을 시 prime_check1 반환
		return prime_check1;
	else get_prime2(next_col, bit_length, next_ham, ham_length - 1, min, min_num);//계속 변화되는 next_ham, next_col과 ham_length를 1 씩 떨어트리며 recursive 함수 진행.
}

void PItable(char **prime_implicant, int prime_check, char ** min, int min_num, int bit_length)
{
	char **table;
	char **miniset;
	bool minterm = true;
	int ess_check = 0;
	int m = 0;
	table = new char *[prime_check];//table의 메모리 할당
	miniset = new char*[prime_check];//miniset의 메모리 할당
	for (int i = 0; i < prime_check; i++)
		table[i] = new char[min_num];

	for (int i = 0; i < prime_check; i++)
		miniset[i] = new char[bit_length];

	for (int i = 0; i < prime_check; i++)
	{

		for (int j = 0; j < min_num; j++)
		{
			for (int k = 0; k < bit_length; k++)
			{
				if (prime_implicant[i][k] != 'X')//don't care부분이 아닌 경우
				{
					if (prime_implicant[i][k] != min[j][k])//minterm과 prime_implicant의 값이 일치하지 않을때
					{
						minterm = false;//사실 여부를 false로 초기화.
						break;
					}
				}
			}
			if (minterm == true)//사실여부가 true이면 해당부분을 기호 'X'로 초기화.
				table[i][j] = 'X';
			else
				table[i][j] = '0';//아니면 해당부분을 기호 '0'으로 초기화.
			minterm = true;
		}
	}
		
	for (int i = 0; i < min_num; i++)
	{
		ess_check = 0;//essential prime implicant를 check하는 변수
		for (int j = 0; j < prime_check; j++)
		{
			if (table[j][i] == 'X')//table의 각 column에서 X가 확인될때마다 ess_check++;
				ess_check++;
		}
		if (ess_check == 1)//ess_check==1일 경우, 해당하는 행은 essential prime implicant이다
		{
			for (int j = 0; j < prime_check; j++)
			{
				if (table[j][i] == 'X')//essential prime implicant가 확인된 상태
				{
					miniset[m] = prime_implicant[j];//해당 prime_implicant를 miniset배열에 추가.
					m++;//miniset개수 증가
					for (int k = 0; k < min_num; k++)
					{
						if (table[j][k] == 'X') {//해당 prime_implicant의 행의 모든 X 를 0으로 바꾸고 X였던 열의 모든 행을 0으로 바꾼다. 
							for (int d = 0; d < prime_check; d++)
								table[d][k] = '0';

						}
					}
				}
			}
		}
	}
		
	search_set(table, prime_implicant, prime_check, min_num, miniset, m, bit_length);//essential prime implicant를 지운후 최소항을 구하는 함수 실행


	/*for (int i = 0; i < m; i++)
	delete[] miniset[i];*/
	delete[] miniset;//miniset과 table의 메모리 deallocation

	for (int i = 0; i < prime_check; i++)
		delete table[i];
	delete[] table;
}

void column(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num)
{
	int n = 0; int static prime, prime_check2 = 0;
	int check = 0, check_over = 0;
	static char **prime_implicant;
	int ham_distance = 0;
	int *next_ham;
	char***next_col;
	int col_length;

	next_ham = new int[ham_length];//next_ham의 메모리 할당
	next_col = new char**[ham_length];//next_col의 메모리 할당

	for (int i = 0; i < ham_length; i++)
	{
		col_length = 0;//col_length를 0으로 초기화
		for (int j = 0; j < ham[i]; j++)
		{
			for (int e = 0; e < ham[i + 1]; e++)
			{
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[i][j][k] != hamming[i + 1][e][k])//1이 i개 있는 그룹과 i+1개 있는 그룹간의 반복 비교
						ham_distance++;
				}
				if (ham_distance == 1)//그룹 비교 후 ham_distance가 1인 경우
					col_length++;//col_length 증가
				ham_distance = 0;
			}
		}
		next_col[i] = new char*[col_length];
		next_ham[i] = col_length;//next_ham[i]값은 col_length와 같다.
	}

	for (int i = 0; i < ham_length; i++)
	{
		for (int j = 0; j < next_ham[i]; j++)
			next_col[i][j] = new char[bit_length];
	}


	for (int i = 0; i < ham_length; i++)
	{
		for (int j = 0; j < ham[i]; j++)
		{
			for (int e = 0; e < ham[i + 1]; e++)
			{
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[i][j][k] != hamming[i + 1][e][k])
						ham_distance++;
				}
				if (ham_distance == 1)
				{
					for (int k = 0; k < bit_length; k++)
					{
						if (hamming[i][j][k] != hamming[i + 1][e][k])//hamming distance가 1인 경우 해당하는 next_col의 부분을 don't care 기호인 X로 표시한다.
							next_col[i][n][k] = 'X';
						else
							next_col[i][n][k] = hamming[i][j][k];//1이 아닌 경우 next_col은 hamming의 값 그대로 저장해준다.

					}
					n++;//ham_distance가 1일 때만 증가
				}
				ham_distance = 0;
			}
			if (n == next_ham[i])//n==next_ham[i]인 경우 해당 next_ham의 값이 다 찬 것이므로 더 이상 반복문이 실행되지 않아도 괜찮다.
				break;
		}
		n = 0;
	}


	if (prime_check2 == 0) {//prime_check가 0, 즉 이 함수가 처음 동작 될때만 실행됨을 의미한다.
		prime = get_prime2(hamming, bit_length, ham, ham_length, min, min_num);//get_prime2함수로 얻은 값을 prime값으로 초기화
		prime_implicant = new char*[prime];//prime의 크기만큼 prime_implicant 메모리 할당.
		for (int i = 0; i <prime; i++)
			prime_implicant[i] = new char[bit_length];
	}

	for (int i = 0; i < ham_length + 1; i++)
	{
		for (int j = 0; j < ham[i]; j++)
		{
			check = 0;
			for (int e = 0; e < ham_length; e++)
			{
				for (int d = 0; d < next_ham[e]; d++)
				{
					for (int k = 0; k < bit_length; k++)
					{
						if (hamming[i][j][k] != next_col[e][d][k])//hamming과 next_col의 bit가 같지 않을 때, ham_distance 증가
							ham_distance++;
					}
					if (ham_distance == 1)
						check++;
					ham_distance = 0;
				}

			}
			if (check == 0)//prime_implicant들은 next_col의 그룹에 하나도 해당되지 않으므로 check가 0일때에만 가능
			{
				prime_implicant[prime_check2] = hamming[i][j];//prime_implicant 저장.
				if (prime_check2 > 0)
				{
					for (int j = 0; j < prime_check2; j++)
					{
						check_over = 0;
						for (int i = 0; i < bit_length; i++)
						{
							if (prime_implicant[prime_check2][i] == prime_implicant[j][i])//중복검사
								check_over++;
						}
						if (check_over == bit_length)//중복 확인
						{
							prime_check2--;//prime_implicant의 개수를 하나 줄인다.
							break;
						}
					}
				}

				prime_check2++;//prime_implicant의 개수 증가
			}

		}
	}

	if (ham_length == 0)//recursive의 끝에 도달했을 때
	{		
		PItable(prime_implicant, prime_check2, min, min_num, bit_length);//최종 prime_implicant와 prime_check를 기반으로 PI table을 호출한다.


		delete[] next_col;//memory 동적할당 해제
		delete[] next_ham;
		/*for (int i = 0; i < prime; i++)
		delete[] prime_implicant[i];*/
		delete[] prime_implicant;
	}

	else column(next_col, bit_length, next_ham, ham_length - 1, min, min_num);//recursive 동작
}

void search_set(char **table, char**prime_implicant, int prime_check, int min_num, char**miniset, int mini_num, int bit_length)
{
	ofstream file("result.txt");//result.txt 파일 오픈
	for (int i = 0; i < min_num; i++)
	{
		for (int j = 0; j < prime_check; j++)
		{
			if (table[j][i] == 'X')//임의의 행에서 X가 발견되었을 때

			{
				miniset[mini_num] = prime_implicant[j];//essential prime implicant를 지웠던 방식으로 지운다.
				mini_num++;
				for (int k = 0; k < min_num; k++)
				{
					if (table[j][k] == 'X') {
						for (int d = 0; d < prime_check; d++)
							table[d][k] = '0';

					}
				}
			}
		}
	}

	for (int i = 0; i < mini_num; i++)//파일에 최소화된 prime implicant 항들을 저장한다.
	{
		for (int j = 0; j < bit_length; j++)
			file << miniset[i][j];
		file << endl;
	}

	file << "Cost(# of transistors): " << trans_num(miniset, mini_num, bit_length) << endl;//최소화된 항을 기반으로 한 transistor 개수 계산.
	cout << "파일 생성 완료!" << endl;
	file.close();

}

int trans_num(char**miniset, int set_num, int bit_length)
{
	int total_trans = 0;//total_trans 선언.

	for (int i = 0; i < set_num; i++)
	{
		for (int j = 0; j < bit_length; j++)
		{
			if (miniset[i][j] != 'X')//don't care가 아닌 항일 때
			{
				total_trans += 2;//2개씩 더해준다
				if (miniset[i][j] == '0')//0인 항이 있을 경우 inverter가 필요하므로 2개를 추가로 더한다
					total_trans += 2;
			}

		}
		total_trans += 2;//각 항의 원소의 개수만큼 AND게이트의 input으로 들어가므로 반복문이 한번 순환할 때마다 2개씩 더해준다
	}

	total_trans += (set_num * 2) + 2;//set_num*2+2만큼의 OR gate 트랜지스터가 필요하다.
	return total_trans;
}