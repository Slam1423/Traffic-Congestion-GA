//���ܣ�traffic programming.
#include<random>
#include<vector>
#include<iostream>
#include<cmath>
#include<ctime>
#include <cstdlib>
#include <bitset>
#include<iomanip>
using namespace std;
const double PI = 3.141592653589793;//����һ�����ɸı�ĳ���ֵPI
const int Po_Size = 500;//��Ⱥ��ģ
const int Ev_Algebra = 2000;//��������
const double Ov_Probability = 0.550; //�������,������������ж����������Ƿ���Ҫ����
const double Va_Probability = 0.050;//�������,������������ж���һ�����Ƿ���Ҫ����
const int De_Variable = 14;//�����Ա����ĸ���14,��Ϊ���ǵĽ����������ʮ�ĸ�.
const int length1 = 7; //�������Ǿ�ȷ��С�������λ.
const double C = 0.2;
double X[De_Variable];
const double q[De_Variable] = { 1158, 1411, 437, 994, 437, 1712, 1578, 1011, 228, 228, 536, 1341, 440, 2256 };
const double qMax[De_Variable] = { 2000, 2000, 800, 1500, 2000, 2000, 4000, 3000, 1500, 3000, 2000, 2000, 2000, 4000 };
const int coeffient = 300;

class X_Range //�Ա���ȡֵ��Χ�࣬�����ڶ��������������
{
private:
	double Upper;//�������Ͻ�ȡֵ
	double Lower;//�������½�ȡֵ
public:
	X_Range(double m_Lower, double m_Upper);//���캯��
	double GetUpper()const;//��ȡ��������
	double GetLower()const;//��ȡ��������
};

X_Range::X_Range(double m_Lower, double m_Upper)
{
	this->Lower = m_Lower;
	this->Upper = m_Upper;
}

double X_Range::GetLower()const
{
	return this->Lower;
}

double X_Range::GetUpper()const
{
	return this->Upper;
}

class Individual //���������
{
private:
	double Variable[De_Variable];//��ű���x1,x2,x3........
	double Fitness;//��Ӧֵ
	double ReFitness;//��Ӧֵ����
	double SumFitness;//�ۼӸ��ʣ�Ϊ����ת��׼��
public:
	Individual() {}//Ĭ�Ϲ��캯��
	Individual(double* m_Variable);//���캯��
	double* GetVariable();//��ȡ����ֵ
	void ChaFitness(const double m_fitness);//�޸���Ӧֵ
	void ChaReFitness(const double m_ReFitness);//�޸���Ӧֵ����
	void ChaSumFitness(const double m_SumFitness);//�޸��ۼӸ���
	double GetFitness()const;//��ȡ��Ӧֵ
	double GetReFitness()const;//��ȡ��Ӧֵ����
	double GetSumFitness()const;//��ȡ�ۼӸ���
};

vector<Individual> individual;
vector<Individual> midIndividual;
vector<Individual> nextIndividual;
X_Range x(0.1, 0.4);

Individual::Individual(double* m_Variable) //Individual��Ĺ��캯�������������Գ�ʼ��
{
	for (int i = 0; i < De_Variable; i++)
	{
		if (m_Variable[i] < x.GetLower() || m_Variable[i] > x.GetUpper()) // ��Ϊ�������н���ڵ����ű�ȡֵ��Χ����[0,1]
		{
			cout << "��ʼ��x(" << i << ")=" << m_Variable[i] << "��������Ҫ��" << endl;
		}
		else
		{
			Variable[i] = m_Variable[i];
		}
	}
	this->Fitness = 0;
	this->ReFitness = 0;
	this->SumFitness = 0;
}

double* Individual::GetVariable()
{
	return this->Variable;
}

double Individual::GetFitness()const
{
	return this->Fitness;
}

double Individual::GetReFitness()const
{
	return this->ReFitness;
}

double Individual::GetSumFitness()const
{
	return this->SumFitness;
}

void Individual::ChaFitness(const double m_fitness)
{
	this->Fitness = m_fitness;
}

void Individual::ChaReFitness(const double m_ReFitness)
{
	this->ReFitness = m_ReFitness;
}

void Individual::ChaSumFitness(const double m_SumFitness)
{
	this->SumFitness = m_SumFitness;
}

void Initialize();//�����ʼ����Ⱥ���õ���һ������
void CaculaFitness();//����������Ӧֵ
void CaculaReFitness();//����������Ӧֵ����
void CalculaSumFitness();//�����ۼӸ������
void select();//��Ⱥѡ��
void crossing();//�ӽ�
void variating();//����
void genetic_algorithm();//�Ŵ��㷨
double F(double *);

void Initialize()
{
	//��ʼ����Ⱥ���õ���һ������
	for (int i = 0; i < De_Variable; i++)
	{
		X[i] = double(q[i] / qMax[i]);
		cout << X[i] << endl;
	}
	for (int i = 0; i < Po_Size; i++)
	{
		double variables[De_Variable];
		for (int j = 0; j < De_Variable; j++)
		{
			double curRand = (x.GetUpper() - x.GetLower())*rand() / RAND_MAX + x.GetLower();
			//cout << curRand << endl;
			variables[j] = curRand;
		}
		Individual cur(variables);
		individual.push_back(cur);
	}
	return;
}

void CaculaFitness()
{
	//������Ӧ�Ⱥ�������ʹ��Ŀ�꺯������ΪĿ�꺯��ȡ������Сֵ����������Ӧ�ÿ����ø�Ŀ�꺯����һ���Ͻ��ȥ��Ŀ�꺯����
	//�����õ��ĺ����ȷǸ����������ֵ���Ӷ�����Ϊ��Ӧ�Ⱥ�����
	//����Ӧ�ǽ���Ⱥ�����и��嶼������Ե���Ӧ��
	for (int i = 0; i < Po_Size; i++)
	{
		individual[i].ChaFitness(F(individual[i].GetVariable()));
	}
	return;
}

double F(double *var) // ��Ӧ�Ⱥ���
{
	double theSum = 0;
	for (int i = 0; i < De_Variable; i++)
	{
		//cout << var[i] << " " << X[i] << endl;
		theSum += C/2 - C * pow(1 - var[i], 2) / (2 * (1 - var[i] * X[i]));
		//cout << C - C * pow(1 - var[i], 2) / (2 * (1 - var[i] * X[i])) << endl;
	}
	return theSum;
}

void CaculaReFitness()
{
	//���������ռ�ĸ��ʴ�С
	double theSum = 0;
	for (int i = 0; i < Po_Size; i++)
	{
		theSum += individual[i].GetFitness();
	}
	//������������ռ���̵ĸ��ʴ�С
	for (int i = 0; i < Po_Size; i++)
	{
		individual[i].ChaReFitness(individual[i].GetFitness() / theSum);
	}
	return;
}

void CalculaSumFitness()
{
	double curSum = 0;
	for (int i = 0; i < Po_Size; i++)
	{
		curSum += individual[i].GetReFitness();
		individual[i].ChaSumFitness(curSum);
	}
	return;
}

void select()
{
	//ѡ��ÿ�����尴�������������ۼӸ��ʽ���ѡ����һ�������ĸ���洢��midpopulation�У���midpopulation��ѡ����ĸ��壬���Ǹ�����Ŀ����
	for (int i = 0; i < Po_Size; i++)
	{
		//����0~1�������Ȼ�����ۼӸ��ʽ��бȽϣ�ȷ����i��ѡ�������һ������
		double curRand = rand() / RAND_MAX;
		int flag = 0;
		for (int j = 0; j < Po_Size; j++)
		{
			if (curRand <= individual[j].GetSumFitness())
			{
				flag = 1;
				midIndividual.push_back(individual[j]);
				break;
			}
		}
		if (!flag)
		{
			midIndividual.push_back(individual[Po_Size - 1]);
		}
	}
	return;
}

void crossing()
{
	for (int i = 0; i < Po_Size; i += 2)
	{
		double p = rand() / RAND_MAX;
		if (p < Ov_Probability)
		{
			bool p = true;
			double newx1[De_Variable];
			double newx2[De_Variable];
			for (int j = 0; j < De_Variable; j++)
			{
				//ÿ������λ���н���
				//���ȶ�midpopulation[i]��midpopulation[i+1]��variable[j]���б���
				bitset<length1> arr1(midIndividual[i].GetVariable()[i]*coeffient);
				bitset<length1> arr2(midIndividual[i + 1].GetVariable()[i + 1]*coeffient);
				//����ȷ�����������ʼ��λ�����ǿ����������0~length1-1֮�������
				int crossLocation = rand() % length1;
				int temp;
				//���潫arr1��arr2�ڵ�crossLocation֮���Ԫ�ؽ��н���
				for (int k = crossLocation; k < length1; k++)
				{
					temp = arr1[k];
					arr1[k] = arr2[k];
					arr2[k] = temp;
				}
				//�жϽ����Ļ����Ƿ����ڿ�������
				//���Ƚ��������arr1��arr2��ԭ������������
				//cout << arr1 << " " << arr2 << endl;
				//cout << arr1.to_ullong() << " " << arr2.to_ullong() << endl;
				double num1 = double(arr1.to_ullong() / coeffient);
				double num2 = double(arr2.to_ullong() / coeffient);
				//cout << num1 << " " << num2 << endl;
				newx1[j] = num1;
				newx2[j] = num2;
				if (num1 < x.GetLower() || num1>x.GetUpper() || num2 < x.GetLower() || num2>x.GetUpper())
				{
					p = false;
					break;
				}
			}
			if (p)
			{
				//cout << "����ɹ�" << endl;
				Individual child1(newx1);
				Individual child2(newx2);
				nextIndividual.push_back(child1);
				nextIndividual.push_back(child2);
			}
			else
			{
				nextIndividual.push_back(midIndividual[i]);
				nextIndividual.push_back(midIndividual[i + 1]);
			}
		}
		else
		{
			//����ֱ���Ŵ�����һ��
			nextIndividual.push_back(individual[i]);
			nextIndividual.push_back(individual[i + 1]);
		}
	}
	midIndividual.clear();
	return;
}

void variating()
{
	double newx[De_Variable];
	for (int i = 0; i < Po_Size; i++)
	{
		double p = rand() / RAND_MAX;
		if (p < Va_Probability)
		{
			bool p = true;
			for (int j = 0; j < De_Variable; j++)
			{
				//�Ե�j��������в���
				int varLocation = rand() % length1;
				bitset<length1> arr(nextIndividual[i].GetVariable()[j]*coeffient);
				arr.flip(varLocation);
				double temp = arr.to_ullong();
				newx[j] = nextIndividual[i].GetVariable()[j];
				if (!(temp<x.GetLower() || temp>x.GetUpper()))
				{
					newx[j] = temp;
				}
			}
			//cout << "����ɹ�" << endl;
			Individual child(newx);
			individual.push_back(child);
		}
		else
		{
			individual[i] = nextIndividual[i];
		}
	}
	nextIndividual.clear();
	return;
}

void genetic_algorithm()
{
	//ʵ��ģ����Ŵ����̣��������Ev_Algebra=1000��
	Initialize();
	for (int i = 0; i < Ev_Algebra; i++)
	{
		CaculaFitness();//����������Ӧֵ
		CaculaReFitness();//����������Ӧֵ����
		CalculaSumFitness();//�����ۼӸ������
		select();//��Ⱥѡ��
		crossing();//�ӽ�
		variating();//����
		cout << "��" << i << "�����" << endl;
	}
	CaculaFitness();
	//������һ��ÿһ��������Ա���ֵ�Լ����Ӧ����Ӧ��
	double maxFitness = 0;
	double max1[De_Variable];
	for (int i = 0; i < Po_Size; i++)
	{
		if (maxFitness < individual[i].GetFitness())
		{
			maxFitness = individual[i].GetFitness();
			for (int j = 0; j < De_Variable; j++)
			{
				max1[j] = individual[i].GetVariable()[j];
			}
		}
	}
	for (int i = 0; i < De_Variable; i++)
	{
		cout << "�����" << i << "�����ű�:" << max1[i] << endl;
	}
	cout << "����Ӧ��Ϊ:" << maxFitness << endl;
	return;
}

int main()
{
	srand(2000);
	genetic_algorithm();
	return 0;
}