#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

int trans_num(char**miniset, int set_num, int bit_length);//transistor�� ������ ������� �Լ�
void search_set(char **table, char**prime_implicant, int prime_check, int min_num, char**miniset, int mini_num, int bit_length);//prime_implicant�� minimum set�� ���ϴ� �Լ�
void PItable(char **prime_implicant, int prime_check, char ** min, int min_num, int bit_length);// PI table�� ������ �Լ�
int get_prime2(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num);//prime_implicant�� �޸� ũ�⸦ ��ȯ���� �Լ�
void column(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num);//quine mccluskey method�� process�� �����ϴ� �Լ�
void my_QM();

int main(void)
{
	clock_t start_time = 0;
	clock_t end_time = 0;
	float run_time = 0;

	start_time = clock();
	/////////////////////////////////////////////////////////////////////////////////////
	// �������� Quine-McCluskey �Լ� my_QM()�� �ۼ��ϼ���
	// ����ð� ������ ���� start_time�� end_time ���̿� my_QM()�� call�մϴ�. (������ ��� ����)
	// �� template�� �������� ����, my_QM()�Լ� �ۼ� �� �� template���ϰ� �Բ� compile�� �ּ���
	/////////////////////////////////////////////////////////////////////////////////////
	my_QM();
	end_time = clock();
	run_time = (float)(end_time - start_time) / (CLOCKS_PER_SEC);

	cout << "�˰��� ���� �ð�: " << run_time << " ��" << endl;

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

	if (!file) cout << "������ �������� �ʽ��ϴ�!";//���� ���� Ȯ��

	while (!file.eof())
	{
		file.getline(data, 64);
		if (bit_check == false)
		{
			bit_length = atoi(strtok_s(data, " ", &context));//������  ù�� ���ڸ� bit_length�� ����
			bit_check = true;
			continue;
		}
		else
		{
			if (*strtok_s(data, " ", &context) == 'd')//bit_length ���� ��, d�� Ȯ�� �Ǹ� dont_num++, 
				                                     //m�� Ȯ�εǸ� min_num++
				dont_num++;
			else
				min_num++;
		}
	}

	min = new char *[min_num];//min�� �޸� �Ҵ�

	for (int i = 0; i < min_num; i++)//bit_length ��ŭ 2���� �޸� �Ҵ�
		min[i] = new char[bit_length];

	file.close();

	ifstream fin("input_minterm.txt");//���� �����

	bit_check = false;

	data_c = new char *[min_num + dont_num];
	for (int i = 0; i < min_num + dont_num; i++)
		data_c[i] = new char[64];

	i = 0;
	t = 0;
	ham = new int[bit_length + 1];//bit_length+1��ŭ�� ��츸ŭ 1�� ������ �� �ִ� ����� ���� �����Ƿ� bit_length+1��ŭ �����Ҵ�.
	ham_arrange = new int[bit_length + 1];// ���� ���� ����.

	hamming = new char *[min_num + dont_num];//������ ��� dont, true minterm���� ������ hamming �������� �޸𸮸� ���� �� ������ŭ �Ҵ�.
	for (int i = 0; i < min_num + dont_num; i++)
		hamming[i] = new char[bit_length];

	for (int i = 0; i < bit_length + 1; i++)
	{
		ham[i] = 0;
		ham_arrange[i] = 0;//ham�� ham_arrange�� ��� ���� 0���� �ʱ�ȭ. ó������ �ƹ��͵�
		                  //����Ǿ� ���� �����Ƿ� �� �迭�� 1�� ������ ��Ÿ���� �� �迭���� �� ������ ���� 0�̾�� �Ѵ�.
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
				min[i] = strtok_s(NULL, " ", &context);//m�� Ȯ�εǸ� min�迭�� ���ʴ�� minterm�� ����.
				for (int k = 0; k < bit_length; k++)
				{
					if (min[i][k] == '1')//min[i]�� 1�� ������ŭ hamming_count++
						hamming_count++;
				}
				ham[hamming_count]++;//hamming_count�� 1�� ������ �ǹ��ϹǷ� ham[hamming_count]�� hamming_count�� �ش��ϴ� �迭�� �������� �ǹ��Ѵ�.
				hamming[t] = min[i];//hamming�� min �迭 ����.
				i++;

			}
			else
			{
				hamming[t] = strtok_s(NULL, " ", &context);//hamming�� dont care �迭 ����.
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[t][k] == '1')
						hamming_count++;//1�� ������ŭ hamming_count++
				}
				ham[hamming_count]++;
			}
			t++;
		}
	}

	file.close();

	next_hamming = new char**[bit_length + 1];//�������ϰ� ����� hamming �迭�� �����Ͽ� ������ next_hamming �޸� �Ҵ�.

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
		next_hamming[t][ham_arrange[t]] = hamming[i];//hamming�� 1�� ������ ���� �׷��� ������, �ش��ϴ� �׷쿡 �������� hamming���� �����Ѵ�.
		ham_arrange[t]++;
	}

	column(next_hamming, bit_length, ham, bit_length, min, min_num);//column�Լ� ����

	/*for (int i = 0; i < bit_length + 1; i++)//�� ���ķδ� �޸� de allocation. �ּ��� �� �κ��� �ּ��� �ȴ޸� ������ ���ϴ�..
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

int get_prime2(char ***hamming, int bit_length, int * ham, int ham_length, char **min, int min_num)//column�Լ��� ���� ����
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
				prime_check1++;// column�Լ��� �ٸ� �κ��̴�. ���ٸ� �ߺ��˻� ���� �ϴ� check�� 0�̸� prime_check1++
			}

		}
	}
	if (ham_length == 0)//ham_length�� 0�� �Ǿ��� �� prime_check1 ��ȯ
		return prime_check1;
	else get_prime2(next_col, bit_length, next_ham, ham_length - 1, min, min_num);//��� ��ȭ�Ǵ� next_ham, next_col�� ham_length�� 1 �� ����Ʈ���� recursive �Լ� ����.
}

void PItable(char **prime_implicant, int prime_check, char ** min, int min_num, int bit_length)
{
	char **table;
	char **miniset;
	bool minterm = true;
	int ess_check = 0;
	int m = 0;
	table = new char *[prime_check];//table�� �޸� �Ҵ�
	miniset = new char*[prime_check];//miniset�� �޸� �Ҵ�
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
				if (prime_implicant[i][k] != 'X')//don't care�κ��� �ƴ� ���
				{
					if (prime_implicant[i][k] != min[j][k])//minterm�� prime_implicant�� ���� ��ġ���� ������
					{
						minterm = false;//��� ���θ� false�� �ʱ�ȭ.
						break;
					}
				}
			}
			if (minterm == true)//��ǿ��ΰ� true�̸� �ش�κ��� ��ȣ 'X'�� �ʱ�ȭ.
				table[i][j] = 'X';
			else
				table[i][j] = '0';//�ƴϸ� �ش�κ��� ��ȣ '0'���� �ʱ�ȭ.
			minterm = true;
		}
	}
		
	for (int i = 0; i < min_num; i++)
	{
		ess_check = 0;//essential prime implicant�� check�ϴ� ����
		for (int j = 0; j < prime_check; j++)
		{
			if (table[j][i] == 'X')//table�� �� column���� X�� Ȯ�εɶ����� ess_check++;
				ess_check++;
		}
		if (ess_check == 1)//ess_check==1�� ���, �ش��ϴ� ���� essential prime implicant�̴�
		{
			for (int j = 0; j < prime_check; j++)
			{
				if (table[j][i] == 'X')//essential prime implicant�� Ȯ�ε� ����
				{
					miniset[m] = prime_implicant[j];//�ش� prime_implicant�� miniset�迭�� �߰�.
					m++;//miniset���� ����
					for (int k = 0; k < min_num; k++)
					{
						if (table[j][k] == 'X') {//�ش� prime_implicant�� ���� ��� X �� 0���� �ٲٰ� X���� ���� ��� ���� 0���� �ٲ۴�. 
							for (int d = 0; d < prime_check; d++)
								table[d][k] = '0';

						}
					}
				}
			}
		}
	}
		
	search_set(table, prime_implicant, prime_check, min_num, miniset, m, bit_length);//essential prime implicant�� ������ �ּ����� ���ϴ� �Լ� ����


	/*for (int i = 0; i < m; i++)
	delete[] miniset[i];*/
	delete[] miniset;//miniset�� table�� �޸� deallocation

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

	next_ham = new int[ham_length];//next_ham�� �޸� �Ҵ�
	next_col = new char**[ham_length];//next_col�� �޸� �Ҵ�

	for (int i = 0; i < ham_length; i++)
	{
		col_length = 0;//col_length�� 0���� �ʱ�ȭ
		for (int j = 0; j < ham[i]; j++)
		{
			for (int e = 0; e < ham[i + 1]; e++)
			{
				for (int k = 0; k < bit_length; k++)
				{
					if (hamming[i][j][k] != hamming[i + 1][e][k])//1�� i�� �ִ� �׷�� i+1�� �ִ� �׷찣�� �ݺ� ��
						ham_distance++;
				}
				if (ham_distance == 1)//�׷� �� �� ham_distance�� 1�� ���
					col_length++;//col_length ����
				ham_distance = 0;
			}
		}
		next_col[i] = new char*[col_length];
		next_ham[i] = col_length;//next_ham[i]���� col_length�� ����.
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
						if (hamming[i][j][k] != hamming[i + 1][e][k])//hamming distance�� 1�� ��� �ش��ϴ� next_col�� �κ��� don't care ��ȣ�� X�� ǥ���Ѵ�.
							next_col[i][n][k] = 'X';
						else
							next_col[i][n][k] = hamming[i][j][k];//1�� �ƴ� ��� next_col�� hamming�� �� �״�� �������ش�.

					}
					n++;//ham_distance�� 1�� ���� ����
				}
				ham_distance = 0;
			}
			if (n == next_ham[i])//n==next_ham[i]�� ��� �ش� next_ham�� ���� �� �� ���̹Ƿ� �� �̻� �ݺ����� ������� �ʾƵ� ������.
				break;
		}
		n = 0;
	}


	if (prime_check2 == 0) {//prime_check�� 0, �� �� �Լ��� ó�� ���� �ɶ��� ������� �ǹ��Ѵ�.
		prime = get_prime2(hamming, bit_length, ham, ham_length, min, min_num);//get_prime2�Լ��� ���� ���� prime������ �ʱ�ȭ
		prime_implicant = new char*[prime];//prime�� ũ�⸸ŭ prime_implicant �޸� �Ҵ�.
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
						if (hamming[i][j][k] != next_col[e][d][k])//hamming�� next_col�� bit�� ���� ���� ��, ham_distance ����
							ham_distance++;
					}
					if (ham_distance == 1)
						check++;
					ham_distance = 0;
				}

			}
			if (check == 0)//prime_implicant���� next_col�� �׷쿡 �ϳ��� �ش���� �����Ƿ� check�� 0�϶����� ����
			{
				prime_implicant[prime_check2] = hamming[i][j];//prime_implicant ����.
				if (prime_check2 > 0)
				{
					for (int j = 0; j < prime_check2; j++)
					{
						check_over = 0;
						for (int i = 0; i < bit_length; i++)
						{
							if (prime_implicant[prime_check2][i] == prime_implicant[j][i])//�ߺ��˻�
								check_over++;
						}
						if (check_over == bit_length)//�ߺ� Ȯ��
						{
							prime_check2--;//prime_implicant�� ������ �ϳ� ���δ�.
							break;
						}
					}
				}

				prime_check2++;//prime_implicant�� ���� ����
			}

		}
	}

	if (ham_length == 0)//recursive�� ���� �������� ��
	{		
		PItable(prime_implicant, prime_check2, min, min_num, bit_length);//���� prime_implicant�� prime_check�� ������� PI table�� ȣ���Ѵ�.


		delete[] next_col;//memory �����Ҵ� ����
		delete[] next_ham;
		/*for (int i = 0; i < prime; i++)
		delete[] prime_implicant[i];*/
		delete[] prime_implicant;
	}

	else column(next_col, bit_length, next_ham, ham_length - 1, min, min_num);//recursive ����
}

void search_set(char **table, char**prime_implicant, int prime_check, int min_num, char**miniset, int mini_num, int bit_length)
{
	ofstream file("result.txt");//result.txt ���� ����
	for (int i = 0; i < min_num; i++)
	{
		for (int j = 0; j < prime_check; j++)
		{
			if (table[j][i] == 'X')//������ �࿡�� X�� �߰ߵǾ��� ��

			{
				miniset[mini_num] = prime_implicant[j];//essential prime implicant�� ������ ������� �����.
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

	for (int i = 0; i < mini_num; i++)//���Ͽ� �ּ�ȭ�� prime implicant �׵��� �����Ѵ�.
	{
		for (int j = 0; j < bit_length; j++)
			file << miniset[i][j];
		file << endl;
	}

	file << "Cost(# of transistors): " << trans_num(miniset, mini_num, bit_length) << endl;//�ּ�ȭ�� ���� ������� �� transistor ���� ���.
	cout << "���� ���� �Ϸ�!" << endl;
	file.close();

}

int trans_num(char**miniset, int set_num, int bit_length)
{
	int total_trans = 0;//total_trans ����.

	for (int i = 0; i < set_num; i++)
	{
		for (int j = 0; j < bit_length; j++)
		{
			if (miniset[i][j] != 'X')//don't care�� �ƴ� ���� ��
			{
				total_trans += 2;//2���� �����ش�
				if (miniset[i][j] == '0')//0�� ���� ���� ��� inverter�� �ʿ��ϹǷ� 2���� �߰��� ���Ѵ�
					total_trans += 2;
			}

		}
		total_trans += 2;//�� ���� ������ ������ŭ AND����Ʈ�� input���� ���Ƿ� �ݺ����� �ѹ� ��ȯ�� ������ 2���� �����ش�
	}

	total_trans += (set_num * 2) + 2;//set_num*2+2��ŭ�� OR gate Ʈ�������Ͱ� �ʿ��ϴ�.
	return total_trans;
}