//功能：traffic programming.
#include<random>
#include<vector>
#include<iostream>
#include<cmath>
#include<ctime>
#include <cstdlib>
#include <bitset>
#include<iomanip>
using namespace std;
const double PI = 3.141592653589793;//定义一个不可改变的常量值PI
const int Po_Size = 500;//种群规模
const int Ev_Algebra = 2000;//进化代数
const double Ov_Probability = 0.550; //交叉概率,交叉概率用于判断两两个体是否需要交叉
const double Va_Probability = 0.050;//变异概率,变异概率用于判断任一个体是否需要变异
const int De_Variable = 14;//函数自变量的个数14,因为我们的交叉口数量是十四个.
const int length1 = 7; //这样我们精确到小数点后两位.
const double C = 0.2;
double X[De_Variable];
const double q[De_Variable] = { 1158, 1411, 437, 994, 437, 1712, 1578, 1011, 228, 228, 536, 1341, 440, 2256 };
const double qMax[De_Variable] = { 2000, 2000, 800, 1500, 2000, 2000, 4000, 3000, 1500, 3000, 2000, 2000, 2000, 4000 };
const int coeffient = 300;

class X_Range //自变量取值范围类，适用于多个变量，可行域
{
private:
	double Upper;//变量的上界取值
	double Lower;//变量的下界取值
public:
	X_Range(double m_Lower, double m_Upper);//构造函数
	double GetUpper()const;//获取变量上限
	double GetLower()const;//获取变量下限
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

class Individual //定义个体类
{
private:
	double Variable[De_Variable];//存放变量x1,x2,x3........
	double Fitness;//适应值
	double ReFitness;//适应值概率
	double SumFitness;//累加概率，为轮盘转做准备
public:
	Individual() {}//默认构造函数
	Individual(double* m_Variable);//构造函数
	double* GetVariable();//获取变量值
	void ChaFitness(const double m_fitness);//修改适应值
	void ChaReFitness(const double m_ReFitness);//修改适应值概率
	void ChaSumFitness(const double m_SumFitness);//修改累加概率
	double GetFitness()const;//获取适应值
	double GetReFitness()const;//获取适应值概率
	double GetSumFitness()const;//获取累加概率
};

vector<Individual> individual;
vector<Individual> midIndividual;
vector<Individual> nextIndividual;
X_Range x(0.1, 0.4);

Individual::Individual(double* m_Variable) //Individual类的构造函数，输入数组以初始化
{
	for (int i = 0; i < De_Variable; i++)
	{
		if (m_Variable[i] < x.GetLower() || m_Variable[i] > x.GetUpper()) // 因为这里所有交叉口的绿信比取值范围都是[0,1]
		{
			cout << "初始化x(" << i << ")=" << m_Variable[i] << "，不满足要求" << endl;
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

void Initialize();//随机初始化种群，得到第一代个体
void CaculaFitness();//计算个体的适应值
void CaculaReFitness();//计算个体的适应值概率
void CalculaSumFitness();//计算累加个体概率
void select();//种群选择
void crossing();//杂交
void variating();//变异
void genetic_algorithm();//遗传算法
double F(double *);

void Initialize()
{
	//初始化种群，得到第一代个体
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
	//这里适应度函数不能使用目标函数，因为目标函数取的是最小值，所以我们应该考虑用该目标函数的一个上界减去该目标函数，
	//那样得到的函数既非负又是求最大值，从而可做为适应度函数。
	//这里应是将种群中所有个体都计算各自的适应度
	for (int i = 0; i < Po_Size; i++)
	{
		individual[i].ChaFitness(F(individual[i].GetVariable()));
	}
	return;
}

double F(double *var) // 适应度函数
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
	//计算各自所占的概率大小
	double theSum = 0;
	for (int i = 0; i < Po_Size; i++)
	{
		theSum += individual[i].GetFitness();
	}
	//下面计算各自所占轮盘的概率大小
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
	//选择，每个个体按照上面计算出的累加概率进行选择，这一步产生的个体存储在midpopulation中，即midpopulation是选择出的个体，但是个体数目不变
	for (int i = 0; i < Po_Size; i++)
	{
		//生成0~1随机数，然后与累加概率进行比较，确定第i次选择的是哪一个个体
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
				//每个基因位进行交叉
				//首先对midpopulation[i]和midpopulation[i+1]的variable[j]进行编码
				bitset<length1> arr1(midIndividual[i].GetVariable()[i]*coeffient);
				bitset<length1> arr2(midIndividual[i + 1].GetVariable()[i + 1]*coeffient);
				//下面确定随机交叉起始点位，我们考虑随机生成0~length1-1之间的整数
				int crossLocation = rand() % length1;
				int temp;
				//下面将arr1和arr2在第crossLocation之后的元素进行交叉
				for (int k = crossLocation; k < length1; k++)
				{
					temp = arr1[k];
					arr1[k] = arr2[k];
					arr2[k] = temp;
				}
				//判断交叉后的基因是否还落在可行域中
				//首先将交叉完的arr1和arr2还原到浮点数类型
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
				//cout << "交叉成功" << endl;
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
			//否则直接遗传给下一代
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
				//对第j个基因进行操作
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
			//cout << "变异成功" << endl;
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
	//实际模拟该遗传过程，共需迭代Ev_Algebra=1000代
	Initialize();
	for (int i = 0; i < Ev_Algebra; i++)
	{
		CaculaFitness();//计算个体的适应值
		CaculaReFitness();//计算个体的适应值概率
		CalculaSumFitness();//计算累加个体概率
		select();//种群选择
		crossing();//杂交
		variating();//变异
		cout << "第" << i << "代完成" << endl;
	}
	CaculaFitness();
	//输出最后一代每一个个体的自变量值以及其对应的适应度
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
		cout << "交叉口" << i << "的绿信比:" << max1[i] << endl;
	}
	cout << "其适应度为:" << maxFitness << endl;
	return;
}

int main()
{
	srand(2000);
	genetic_algorithm();
	return 0;
}