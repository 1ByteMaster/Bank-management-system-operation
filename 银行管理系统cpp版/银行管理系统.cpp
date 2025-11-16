#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <queue>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// 定义客户结构体
struct Banker
{
	string account_num; // 银行账户
	string name;		// 银行卡持有人姓名
	string sex;			// 银行卡持有人性别
	int password;		// 银行卡密码
	long long money;	// 银行卡金额
	int open_time;		// 开户日期
	string states;		// 开户状态
	string type;		// 客户类型
	Banker *next;
};
// 使用typedef时，这样定义更清晰，先定义结构体，再用typedef取别名
typedef Banker *BankList;

// 定义职员结构体
struct Staff
{
	string Number; // 工号
	string name;   // 姓名
	string sex;	   // 性别
	int age;	   // 年龄
	int password;  // 密码
	double grade;  // 客户所打的平均分
	int n;		   // 总接待人数
	Staff *next;
};
// 同样先定义结构体，再取别名
typedef Staff *StaffList;

// 叫号系统的号 全局变量
queue<int> normalQue;
queue<int> VIPQue;
int normalNum = 1, VIPNum = 1;
int cnt = 0;

// 定义一个较大的数来表示无穷大，用于初始化图的邻接矩阵中表示无边的情况
const int Infinity = 99999;
// 这里建议使用const修饰，使其成为常量，更符合语义

// 图的顶点结构体
typedef struct Vertex
{
	int position;			// 顶点编号
	char name[50];			// 顶点名称
	char introduction[200]; // 顶点简介
} Vertex;

// 图的边结构体，用于表示邻接矩阵中的元素，即边的权值等信息
typedef struct Arc
{
	int adj; // 边的权值，这里假设是表示距离等含义的数值
} ArcType;

// 图的数据结构定义
typedef struct MGraph
{
	int vexnum;			  // 顶点个数
	int arcnum;			  // 边的个数
	Vertex vexs[35];	  // 顶点数组，假设最多有35个顶点，可根据实际情况调整大小
	ArcType arcs[35][35]; // 邻接矩阵，用于存储边的信息，同样假设最多35个顶点
} MGraph;

// 全局变量声明，用于存储路径信息和顶点访问状态
int d[35];
int visited[35];
MGraph campus;
// 函数

void Choice_a_person(); // 选择待操作人员（银行职员/客户）
void Menu1();			// 职员系统主菜单
void InitList1();		// 初始化菜单
void SelectStaff();		// 查询职员信息
void AddStaff();		// 入职
void DeleteStaff();		// 离职
void Revise();			// 修改职员信息
void Readfile1();		// 读入文件
void Save1();			// 保存账户的信息
void Display1();		// 显示所有职员信息
void mytime();			// 时间显示函数

void InitList2();				// 初始化菜单
bool account_num_exist();		// 判断账户是否存在
void SelectAccount();			// 查询账户信息
void Open_an_ccount();			// 开户
void Pin_households();			// 销户
void Modify_account_password(); // 修改账户密码
void Sort_accountInfo();		// 账户排序
void deposit();					// 存款
void withdrawals();				// 取款
void transfer();				// 转账
double cal_year();				// 计算开户时长
void Calculation_interest();	// 利息计算
void Save2();					// 保存账户的信息
void Readfile2();				// 读入文件
void Menu2();					// 客户系统主菜单
void Wait();					// 增强用户交互体验
void Choice_a_file();			// 选择待操作的文件
void Display2();				// 显示所有用户信息

void Menu3();		   // 排队系统菜单
void Menu3_2();		   // 排队系统员工登录界面
void releaseVIP();	   // VIP放号
void releaseNormal();  // Normal放号
void endVIPServe();	   // 结束VIP服务(出队当前 提示下一号)
void endNormalServe(); // 结束Normal服务(出队当前 提示下一号)
void DisplayVIP();	   // 打印VIP队列当前状态
void DisplayNormal();  // 打印Normal队列当前状态
void Grade();		   // 客户为服务打分

void Menu4();			   // 网点系统菜单栏
MGraph initgraph();		   // 对图初始化
int locatevex();		   // 查找景点在图中的序号
void path();			   // 打印路径
int allpath();			   // 打印所有路径
void shortestpath_dij();   // 迪杰斯特拉求最短路径
int creatgragh();		   // 构造图的邻接矩阵
int newgraph();			   // 更新图的信息
int enarc();			   // 增加一条边
int envex();			   // 增加一个点
int delvex();			   // 删除一个点
int delarc();			   // 删除一条边
void printmatrix();		   // 打印邻接矩阵
int changegraph();		   // 图操作的主要调度函数
void shortestpath_floyd(); // Floyd查最短路径
void seeabout();		   // 查询景点信息
void browsecompus();	   // 显示所有顶点信息
void mainwork4();		   // 主要工作函数

// 职员管理系统初始化菜单
void InitList1(StaffList &SL)
{
	SL = new Staff;
	SL->next = NULL;
}

// 客户管理系统初始化菜单
void InitList2(BankList &BL)
{
	BL = new Banker;
	BL->next = NULL;
}

// 判断账户是否存在
bool account_num_exist(BankList &BL, string account_num)
{
	Banker *p = BL->next;
	while (p)
	{
		if (p->account_num == account_num)
			return true;
		p = p->next;
	}
	return false;
}
// 判断员工是否存在
bool staff_num_exist(StaffList &SL, string staff_num)
{
	Staff *p = SL->next;
	while (p)
	{
		if (p->Number == staff_num)
			return true;
		p = p->next;
	}
	return false;
}
// 1、查询职员信息
void SelectStaff(StaffList &SL)
{
	cout << "1:按工号查找 " << endl;
	cout << "2:按员工姓名查找 " << endl;
	string num;	 // 工号
	string name; // 姓名
	int ch;
	Staff *p = SL->next;
	bool isFound = false;
	cin >> ch;
	switch (ch)
	{
	case 1:
	{
		cout << "请输入要查询的工号: ";
		cin >> num;
		while (p)
		{
			if (p->Number == num)
			{
				cout << "工号\t\t" << "姓名\t" << "性别\t" << "年龄\t" << "密码" << endl;
				cout << p->Number << "\t" << p->name << "\t" << p->sex << "\t" << p->age << "\t" << p->password << endl;
				isFound = true;
				break;
			}
			p = p->next;
		}
	}
	break;
	case 2:
	{
		cout << "请输入要查询的姓名: ";
		cin >> name;
		while (p)
		{
			if (p->name == name)
			{
				cout << "工号\t\t" << "姓名\t" << "性别\t" << "年龄\t" << "密码" << endl;
				cout << p->Number << "\t" << p->name << "\t" << p->sex << "\t" << p->age << "\t" << p->password << endl;
				isFound = true;
				break;
			}
			p = p->next;
		}
	}
	break;
	default:
		cout << "输入有误！" << endl;
		break;
	}
	if (!isFound)
		cout << "未查找到！" << endl;
}
// 2、入职
void AddStaff(StaffList &SL)
{
	Staff *p = new Staff;
	p->next = NULL;
	cout << "请输入需要入职的工号：";
	cin >> p->Number;
	if (staff_num_exist(SL, p->Number))
	{
		cout << "入职失败，该员工已存在" << endl;
		return;
	}
	cout << "请输入入职员工的姓名：";
	cin >> p->name;
	cout << "请输入入职员工的性别(男，女)：";
	cin >> p->sex;
	cout << "请输入入职员工的年龄：";
	cin >> p->age;
	cout << "请输入员工账户的密码(6位)：";
	int password;
	char charPassword[20]; // 用于存储密码字符数组
	int i = 0;
	char ch;
	while ((ch = _getch()) != '\r') // \r是回车键
	{
		if (ch == '\b' && i > 0) // 处理退格键
		{
			cout << "\b \b";
			--i;
		}
		else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
		{
			charPassword[i++] = ch;
			cout << '*';
		}
	}
	charPassword[i] = '\0';		   // 字符串结束标志
	password = atoi(charPassword); // 将字符数组转换为整数
	cout << endl;
	p->password = password;

	Staff *pp = SL;
	while (pp->next)
	{
		pp = pp->next;
	}
	pp->next = p;
	cout << "入职成功！" << endl;
}
// 3、离职
void DeleteStaff(StaffList &SL)
{
	string account_num; // 账户
	int password;		// 密码
	Staff *p = SL;
	cout << "请输入需要离职的员工工号：";
	cin >> account_num;
	if (staff_num_exist(SL, account_num))
	{
		cout << "请输入需要离职的员工的密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p->next != NULL)
		{
			if (p->next->Number == account_num && p->next->password == password)
			{
				cout << "离职成功！" << endl;
				Staff *pt = p->next;
				p->next = pt->next;
				delete pt;
				return;
			}
			p = p->next;
		}
		cout << "\n输入的密码有误！" << endl;
	}
	else
	{
		cout << "此员工不存在！" << endl;
	}
}

// 4、修改职员密码
void Revise(StaffList &SL)
{
	string account_num; // 账户
	int password;		// 密码
	Staff *p = SL->next;
	cout << "请输入需要修改的员工工号：";
	cin >> account_num;
	if (staff_num_exist(SL, account_num))
	{
		cout << "请输入需要修改的员工的密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p != NULL)
		{
			if (p->Number == account_num && p->password == password)
			{
				cout << "请输入新的密码：";
				int password;
				char charPassword[20]; // 用于存储密码字符数组
				int i = 0;
				char ch;
				while ((ch = _getch()) != '\r') // \r是回车键
				{
					if (ch == '\b' && i > 0) // 处理退格键
					{
						cout << "\b \b";
						--i;
					}
					else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
					{
						charPassword[i++] = ch;
						cout << '*';
					}
				}
				charPassword[i] = '\0';		   // 字符串结束标志
				password = atoi(charPassword); // 将字符数组转换为整数
				cout << endl;
				p->password = password;
				cout << "修改成功！" << endl;
				return;
			}
			p = p->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "此员工不存在！" << endl;
	}
}

// 5、显示所有职员信息
void Display1(StaffList &SL)
{
	if (SL->next == NULL)
	{
		cout << "该职员表为空!" << endl;
	}
	else
	{
		Staff *p = SL->next;
		cout << "工号\t\t" << "姓名\t" << "性别\t" << "年龄\t" << "密码" << endl;
		while (p)
		{
			cout << p->Number << "\t" << p->name << "\t" << p->sex << "\t" << p->age << "\t" << p->password << endl;
			p = p->next;
		}
	}
}
// 6、保存职员信息
void Save1(StaffList &SL, string file)
{
	ofstream ofs;
	ofs.open(file.c_str());
	Staff *p = SL->next;
	while (p)
	{
		ofs << p->Number << "\t" << p->name << "\t" << p->sex << "\t" << p->age << "\t" << p->password << endl;

		p = p->next;
	}
	ofs.close();
	cout << "保存成功！" << endl;
}
// 1、查询账户信息
void SelectAccount(BankList &BL)
{
	cout << "1:按账号查找:" << endl;
	cout << "2:按开户人姓名查找：" << endl;
	cout << "3:按开户日期查找：" << endl;
	string account_num; // 账户
	string name;		// 姓名
	int open_time;		// 开户日期
	int ch;
	Banker *p = BL->next;
	cin >> ch;
	switch (ch)
	{
	case 1:
	{
		cout << "请输入需要查询的账号：";
		cin >> account_num;
		while (p)
		{
			if (p->account_num == account_num)
			{
				cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
				cout << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
					 << "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
				return;
			}
			p = p->next;
		}
	}
	break;
	case 2:
	{
		cout << "请输入需要查询的开户人姓名：";
		cin >> name;
		while (p)
		{
			if (p->name == name)
			{
				cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
				cout << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
					 << "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
				return;
			}
			p = p->next;
		}
	}
	break;
	case 3:
	{
		cout << "请输入需要查询的开户日期：";
		cin >> open_time;
		while (p)
		{
			if (p->open_time == open_time)
			{
				cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
				cout << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
					 << "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
				return;
			}
			p = p->next;
		}
	}
	break;
	default:
		cout << "输入有误！" << endl;
		break;
	}
	cout << "未查找到！" << endl;
}

// 2、开户
void Open_an_ccount(BankList &BL)
{
	Banker *p = new Banker;
	p->next = NULL;
	cout << "请输入需要开户的账户：";
	cin >> p->account_num;
	if (account_num_exist(BL, p->account_num))
	{
		cout << "开户失败，账户已存在" << endl;
		return;
	}
	cout << "请输入需要开户的姓名：";
	cin >> p->name;
	cout << "请输入需要开户的性别(男，女)：";
	cin >> p->sex;
	cout << "请输入需要开户的密码(6位)：";
	int password;
	char charPassword[20]; // 用于存储密码字符数组
	int i = 0;
	char ch;
	while ((ch = _getch()) != '\r') // \r是回车键
	{
		if (ch == '\b' && i > 0) // 处理退格键
		{
			cout << "\b \b";
			--i;
		}
		else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
		{
			charPassword[i++] = ch;
			cout << '*';
		}
	}
	charPassword[i] = '\0';		   // 字符串结束标志
	password = atoi(charPassword); // 将字符数组转换为整数
	cout << endl;
	p->password = password;
	cout << "请输入需要开户的金额：";
	cin >> p->money;
	cout << "请输入需要开户的开户日期(如：20050731)：";
	cin >> p->open_time;
	cout << "请输入需要开户的开户状态(活期，定期)：";
	cin >> p->states;
	Banker *pp = BL;
	while (pp->next)
	{
		pp = pp->next;
	}
	pp->next = p;
	cout << "开户成功！" << endl;
}

// 3、销户
void Pin_households(BankList &BL)
{
	string account_num; // 账户
	int password;		// 密码
	Banker *p = BL;
	cout << "请输入需要销户的账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		cout << "请输入需要销户的密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p->next != NULL)
		{
			if (p->next->account_num == account_num && p->next->password == password)
			{
				cout << "销户成功！" << endl;
				Banker *pt = p->next;
				p->next = pt->next;
				delete pt;
				return;
			}
			p = p->next;
		}
		cout << "\n输入的密码有误！" << endl;
	}
	else
	{
		cout << "此账户不存在！" << endl;
	}
}

// 4、修改账户密码
void Modify_account_password(BankList &BL)
{
	string account_num; // 账户
	int password;		// 密码
	Banker *p = BL->next;
	cout << "请输入需要修改的账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		cout << "请输入需要修改的账户密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p != NULL)
		{
			if (p->account_num == account_num && p->password == password)
			{
				cout << "请输入新的密码：";
				int password;
				char charPassword[20]; // 用于存储密码字符数组
				int i = 0;
				char ch;
				while ((ch = _getch()) != '\r') // \r是回车键
				{
					if (ch == '\b' && i > 0) // 处理退格键
					{
						cout << "\b \b";
						--i;
					}
					else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
					{
						charPassword[i++] = ch;
						cout << '*';
						
					}
				}
				charPassword[i] = '\0';		   // 字符串结束标志
				password = atoi(charPassword); // 将字符数组转换为整数
				cout << endl;
				p->password = password;
				cout << "修改成功！" << endl;
				return;
			}
			p = p->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "此账户不存在！" << endl;
	}
}

// 5、账户排序
void Sort_accountInfo(BankList &BL)
{
	int ch;
	cout << "1:按账号排序" << endl;
	cout << "2:按开户人姓名排序" << endl;
	cout << "3:按金额排序" << endl;
	cout << "4:按开户日期排序" << endl;
	cout << "请输入选择的排序编号：" << endl;
	cin >> ch;
	Banker *p1 = BL->next;
	switch (ch)
	{
	case 1:
	{
		for (; p1->next != NULL; p1 = p1->next)
		{
			for (Banker *p2 = p1->next; p2 != NULL; p2 = p2->next)
			{
				if (p1->account_num > p2->account_num) // 升序排列
				{
					Banker *p11 = p1->next;
					Banker *p22 = p2->next;
					Banker t = *p1;
					*p1 = *p2;
					*p2 = t;
					p1->next = p11;
					p2->next = p22;
				}
			}
		}
		cout << "排序成功！" << endl;
	}
	break;
	case 2:
	{
		for (; p1->next != NULL; p1 = p1->next)
		{
			for (Banker *p2 = p1->next; p2 != NULL; p2 = p2->next)
			{
				if (p1->name > p2->name) // 升序排列
				{
					Banker *p11 = p1->next;
					Banker *p22 = p2->next;
					Banker t = *p1;
					*p1 = *p2;
					*p2 = t;
					p1->next = p11;
					p2->next = p22;
				}
			}
		}
		cout << "排序成功！" << endl;
	}
	break;
	case 3:
	{
		for (; p1->next != NULL; p1 = p1->next)
		{
			for (Banker *p2 = p1->next; p2 != NULL; p2 = p2->next)
			{
				if (p1->money > p2->money) // 升序排列
				{
					Banker *p11 = p1->next;
					Banker *p22 = p2->next;
					Banker t = *p1;
					*p1 = *p2;
					*p2 = t;
					p1->next = p11;
					p2->next = p22;
				}
			}
		}
		cout << "排序成功！" << endl;
	}
	break;
	case 4:
	{
		for (; p1->next != NULL; p1 = p1->next)
		{
			for (Banker *p2 = p1->next; p2 != NULL; p2 = p2->next)
			{
				if (p1->open_time > p2->open_time) // 升序排列
				{
					Banker *p11 = p1->next;
					Banker *p22 = p2->next;
					Banker t = *p1;
					*p1 = *p2;
					*p2 = t;
					p1->next = p11;
					p2->next = p22;
				}
			}
		}
		cout << "排序成功！" << endl;
	}
	break;
	default:
		cout << "您的输入有误！" << endl;
	}
}

// 6、存款
void deposit(BankList &BL)
{
	string account_num; // 账户
	int password;		// 密码
	Banker *p = BL->next;
	cout << "请输入需要存款的账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		cout << "请输入需要存款的账户密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p != NULL)
		{
			if (p->account_num == account_num && p->password == password)
			{
				cout << "请输入存款金额：";
				long long m;
				cin >> m;
				p->money += m;
				cout << "存款成功！" << endl;
				return;
			}
			p = p->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "此账户不存在！" << endl;
	}
}

// 7、取款
void withdrawals(BankList &BL)
{
	string account_num; // 账户
	int password;		// 密码
	Banker *p = BL->next;
	cout << "请输入需要取款的账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		cout << "请输入需要取款的账户密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p != NULL)
		{
			if (p->account_num == account_num && p->password == password)
			{
				cout << "请输入您的取款金额：";
				long long m;
				cin >> m;
				if (p->money >= m)
				{
					p->money -= m;

					cout << "取款成功！" << endl;
					cout << "当前账户信息如下：" << endl;
					cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
					cout << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
						 << "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
					return;
				}
				else
				{
					cout << "余额不足！" << endl;
					return;
				}
			}
			p = p->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "此账户不存在！" << endl;
	}
}
void Readfile2(BankList &, string);

void Save2(BankList &BL, string file);

// 8、转账
void transfer(BankList &BL)
{
	Banker *p1 = BL->next;

	BankList bk;
	InitList2(bk);

	string bank1, bank2, account_num; // 银行名，账户
	int password;					  // 密码
	cout << "请输入转账方用户类型：";
	cin >> bank1;
	bank1 += ".txt";
	cout << "请输入收款方用户类型：";
	cin >> bank2;
	bank2 += ".txt";
	Readfile2(bk, bank2);
	Banker *p2 = bk->next;
	cout << "请输入转账方账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		cout << "请输入密码:";
		char charPassword[20]; // 用于存储密码字符数组
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		while (p1 != NULL)
		{
			if (p1->account_num == account_num && p1->password == password)
			{

				cout << "请输入收款方账户：";
				cin >> account_num;
				while (p2)
				{
					if (p2->account_num == account_num)
					{
						long long m;
						cout << "请输入转账金额：";
						cin >> m;
						if (bank1 == bank2)
						{
							if (p1->money >= m)
							{
								p1->money -= m;
								p2->money += m;
								cout << "转账成功！" << endl;
								cout << "当前账户信息如下：" << endl;
								cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
								cout << p1->account_num << "\t" << p1->name << "\t" << p1->sex
									 << "\t" << p1->password
									 << "\t" << p1->money << "\t" << p1->open_time << "\t"
									 << p1->states << endl;

								Save2(bk, bank2);
								return;
							}
							else
							{
								cout << "余额不足！" << endl;
								return;
							}
						}
						else
						{
							cout << "收取手续费：" << m * 0.01 << " 元！" << endl;
							if (p1->money >= m + m * 0.01)
							{
								p1->money -= m;
								p2->money += m;
								cout << "转账成功！" << endl;
								cout << "当前账户信息如下：" << endl;
								cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
								cout << p1->account_num << "\t" << p1->name << "\t" << p1->sex
									 << "\t" << p1->password
									 << "\t" << p1->money << "\t" << p1->open_time << "\t"
									 << p1->states << endl;

								Save2(bk, bank2);
								return;
							}
							else
							{
								cout << "余额不足！" << endl;
								return;
							}
						}
					}
					p2 = p2->next;
				}
				cout << "账户不存在" << endl;
			}
			p1 = p1->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "账户不存在！" << endl;
	}
}

double cal_year(int a, int b)
{
	int year1 = a / 10000;
	int year2 = b / 10000;
	int month1 = a / 100 % 100;
	int month2 = b / 100 % 100;
	int day1 = a % 100;
	int day2 = b % 100;
	double year = 0;
	if (year1 == year2)
		year = 0.5;
	else if (month2 == month1 && day1 == day2)
	{
		year = year2 - year1;
	}
	else if (month2 < month1 || month2 == month1 && day1 > day2)
	{
		year = year2 - year1 - 0.5;
	}
	else if (month2 > month1 || month2 == month1 && day1 < day2)
	{
		year = year2 - year1 + 0.5;
	}
	return year;
}

// 9、计算利息

/*
活期 每年 0.35% (0.0035)
不足一年按半年算，利率为活期年利率的一半，即 0.0035 * 0.5
定期 第一年 2.75%（0.0275）
第二年 3.35%（0.0335）
第三年 4%（0.04）
*/
void Calculation_interest(BankList &BL)
{
	string account_num; // 账户
	int password;		// 密码
	Banker *p = BL->next;
	cout << "请输入账户：";
	cin >> account_num;
	if (account_num_exist(BL, account_num))
	{
		char charPassword[20]; // 用于存储密码字符数组
		cout << "请输入密码：";
		int i = 0;
		char ch;
		while ((ch = _getch()) != '\r') // \r是回车键
		{
			if (ch == '\b' && i > 0) // 处理退格键
			{
				cout << "\b \b";
				--i;
			}
			else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
			{
				charPassword[i++] = ch;
				cout << '*';
			}
		}
		charPassword[i] = '\0';		   // 字符串结束标志
		password = atoi(charPassword); // 将字符数组转换为整数
		cout << endl;
		cout << "\n";
		cout << endl
			 << "你输入的密码是：" << password << endl;
		while (p != NULL)
		{
			if (p->account_num == account_num && p->password == password)
			{
				cout << "请输入存款到期时间：";
				int end_time;
				double rate = 0;
				cin >> end_time;
				double year = cal_year(p->open_time, end_time);
				int year2 = year; // 转化为整数年份
				if (p->states == "活期")
				{
					for (int i = 1; i <= year2; i++)
					{
						rate += (rate + p->money) * 0.0035;
					}
					if (year > year2 + 0.1)
						rate += (rate + p->money) * 0.0035 * 0.5;
				}
				else if (p->states == "定期")
				{
					for (int i = 1; i <= year; i++)
					{
						if (i == 1)
						{
							rate += (rate + p->money) * 0.0275;
						}
						else if (i == 2)
						{
							rate += (rate + p->money) * 0.0335;
						}
						else if (i >= 3)
						{
							rate += (rate + p->money) * 0.04;
						}
					}
					int year2 = year;
					if (fabs(year - 0.5) <= 1e-6)
						rate += (rate + p->money) * 0.0275 * 0.5;
					else if (fabs(year - 1.5) <= 1e-6)
						rate += (rate + p->money) * 0.0335 * 0.5;
					else if (fabs(year - 2.5) <= 1e-6)
						rate += (rate + p->money) * 0.04 * 0.5;
				}
				cout << "您获得的利息为：" << rate << endl;
				return;
			}
			p = p->next;
		}
		cout << "密码错误！" << endl;
	}
	else
	{
		cout << "账户不存在！" << endl;
	}
}

// 10、保存账户信息
void Save2(BankList &BL, string file)
{
	ofstream ofs;
	ofs.open(file.c_str());
	Banker *p = BL->next;
	while (p)
	{
		ofs << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
			<< "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
		p = p->next;
	}
	ofs.close();
	cout << "保存成功！" << endl;
}
// 11、显示所有账户信息
void Display2(BankList &BL)
{
	Banker *p = BL->next;
	cout << "账户\t\t姓名\t性别\t密码\t金额\t\t开户日期\t开户状态" << endl;
	while (p)
	{
		cout << p->account_num << "\t" << p->name << "\t" << p->sex << "\t" << p->password
			 << "\t" << p->money << "\t" << p->open_time << "\t" << p->states << endl;
		p = p->next;
	}
}

// 1、VIP放号
void releaseVIP()
{
	int x;
	cout << "请输入需要放号的个数：";
	cin >> x;
	for (int i = 0; i < x; i++)
		VIPQue.push(VIPNum++);
	cout << "VIP 放号成功，当前 VIP 号为：" << VIPQue.back() << endl;
}
// 2、Normal放号
void releaseNormal()
{
	int x;
	cout << "请输入需要放号的个数：";
	cin >> x;
	for (int i = 0; i < x; i++)
		normalQue.push(normalNum++);
	cout << "Normal 放号成功，当前 Normal 号为：" << normalQue.back() << endl;
}
// 打分
void Grade(Staff *&goal)
{
	int score;
	while (1)
	{
		cout << "请客户输入打分（1-5 分）：";
		cin >> score;
		if (score < 1 || score > 5)
		{
			cout << "请在1~5之间打分 " << endl;
			system("pause");
			system("cls");
			Menu3(); // 菜单栏
			DisplayVIP();
			DisplayNormal();
		}
		else
			break;
	}
	// 这里假设当前登录的员工 goal 可以接收打分
	// 并更新其平均分和总接待人数
	goal->grade = (goal->grade * goal->n + score) / (goal->n + 1);
	goal->n++;
	cout << "打分成功！" << endl;
}
// 3、结束VIP服务
void endVIPServe(Staff *&goal)
{
	if (!VIPQue.empty())
	{
		cout << "VIP 服务结束" << endl;
		Grade(goal);
		cout << "当前服务号为：" << VIPQue.front() << endl;
		VIPQue.pop();
		cnt++;
		if (!VIPQue.empty())
		{
			cout << "下一个 VIP 号为：" << VIPQue.front() << endl;
		}
		else
		{
			cout << "目前没有 VIP 在排队。" << endl;
		}
	}
	else
	{
		cout << "没有 VIP 在排队等待服务。" << endl;
	}
}
// 4、结束Normal服务
void endNormalServe(Staff *&goal)
{
	if (!normalQue.empty())
	{
		cout << "Normal 服务结束" << endl;
		Grade(goal);
		cout << "当前服务号为：" << normalQue.front() << endl;
		normalQue.pop();
		cnt++;
		if (!normalQue.empty())
		{
			cout << "下一个 Normal 号为：" << normalQue.front() << endl;
		}
		else
		{
			cout << "目前没有 Normal 在排队。" << endl;
		}
	}
	else
	{
		cout << "没有 Normal 在排队等待服务。" << endl;
	}
}

// 展示VIP
void DisplayVIP()
{
	if (VIPQue.empty())
	{
		cout << "VIP 队列当前为空 " << endl;
	}
	else
	{
		cout << "VIP窗口    ";
		queue<int> tempQue = VIPQue;
		while (!tempQue.empty())
		{
			cout << tempQue.front();
			tempQue.pop();
			if (!tempQue.empty())
			{
				cout << "<-";
			}
		}
		cout << endl;
	}
}
// 展示Normal
void DisplayNormal()
{
	if (normalQue.empty())
	{
		cout << "Normal 队列当前为空" << endl;
	}
	else
	{
		cout << "Normal窗口 ";
		queue<int> tempQue = normalQue;
		while (!tempQue.empty())
		{
			cout << tempQue.front();
			tempQue.pop();
			if (!tempQue.empty())
			{
				cout << "<-";
			}
		}
		cout << endl;
	}
}
// 5、展示职员得分
void DisplayScore(Staff *&goal)
{
	cout << "已服务 " << cnt << " 位客户" << endl;
	cout << "客户为您打分的平均分为" << goal->grade << endl;
}

// (1) 对图初始化
MGraph initgraph()
{
	int i = 0, j = 0;
	MGraph c;

	c.vexnum = 28;				   // 顶点个数
	c.arcnum = 39;				   // 边的个数
	for (i = 0; i < c.vexnum; i++) // 依次设置顶点编号
		c.vexs[i].position = i;

	// 依次输入顶点信息
	strcpy(c.vexs[0].name, "南门");
	strcpy(c.vexs[0].introduction, "出门有美食街");
	strcpy(c.vexs[1].name, "西门");
	strcpy(c.vexs[1].introduction, "出门有BRT公交站");
	strcpy(c.vexs[2].name, "1号教学楼");
	strcpy(c.vexs[2].introduction, "导员办公室在1461");
	strcpy(c.vexs[3].name, "2号教学楼");
	strcpy(c.vexs[3].introduction, "最常上课的楼 最后一排有插座");
	strcpy(c.vexs[4].name, "3号实验楼");
	strcpy(c.vexs[4].introduction, "上机课就在这 电梯特别挤");
	strcpy(c.vexs[5].name, "4号教学楼");
	strcpy(c.vexs[5].introduction, "办公楼旁边");
	strcpy(c.vexs[6].name, "图书馆");
	strcpy(c.vexs[6].introduction, "离南门最近");
	strcpy(c.vexs[7].name, "逸夫楼");
	strcpy(c.vexs[7].introduction, "离图书馆近");
	strcpy(c.vexs[8].name, "气膜馆");
	strcpy(c.vexs[8].introduction, "可预约打羽毛球");
	strcpy(c.vexs[9].name, "篮球场");
	strcpy(c.vexs[9].introduction, "可随时打篮球");
	strcpy(c.vexs[10].name, "排球场");
	strcpy(c.vexs[10].introduction, "打排球");
	strcpy(c.vexs[11].name, "网球场");
	strcpy(c.vexs[11].introduction, "打网球");
	strcpy(c.vexs[12].name, "纳博士地下超市");
	strcpy(c.vexs[12].introduction, "超市＋小吃");
	strcpy(c.vexs[13].name, "711");
	strcpy(c.vexs[13].introduction, "便利店");
	strcpy(c.vexs[14].name, "西苑餐厅");
	strcpy(c.vexs[14].introduction, "吃饭 上完课吃饭最近");
	strcpy(c.vexs[15].name, "东苑餐厅");
	strcpy(c.vexs[15].introduction, "吃饭 6，7号宿舍楼出来吃饭最近 旁边有民族餐厅");
	strcpy(c.vexs[16].name, "北苑餐厅");
	strcpy(c.vexs[16].introduction, "吃饭 近办公楼");
	strcpy(c.vexs[17].name, "校医院");
	strcpy(c.vexs[17].introduction, "看病");
	strcpy(c.vexs[18].name, "操场");
	strcpy(c.vexs[18].introduction, "体育课 跑步 晚上没灯");
	strcpy(c.vexs[19].name, "菜鸟驿站");
	strcpy(c.vexs[19].introduction, "取快递");
	strcpy(c.vexs[20].name, "海岱美术馆");
	strcpy(c.vexs[20].introduction, "美术馆 没去过");
	strcpy(c.vexs[21].name, "教工宿舍");
	strcpy(c.vexs[21].introduction, "教职工租住地");
	strcpy(c.vexs[22].name, "1，2号宿舍楼");
	strcpy(c.vexs[22].introduction, "离菜鸟驿站近");
	strcpy(c.vexs[23].name, "3，5号宿舍楼");
	strcpy(c.vexs[23].introduction, "离西苑 菜鸟驿站 2号楼都近");
	strcpy(c.vexs[24].name, "6，7，8宿舍楼");
	strcpy(c.vexs[24].introduction, "东苑餐厅附近");
	strcpy(c.vexs[25].name, "9，10，11宿舍楼");
	strcpy(c.vexs[25].introduction, "离东苑也近 在6，7号宿舍楼前面");
	strcpy(c.vexs[26].name, "学景大酒店");
	strcpy(c.vexs[26].introduction, "酒店 南门门口");
	strcpy(c.vexs[27].name, "办公楼");
	strcpy(c.vexs[27].introduction, "办公楼 离西门近 这个电梯不挤");

	// 依次输入边上的权值信息
	for (i = 0; i < c.vexnum; i++)
		for (j = 0; j < c.vexnum; j++)
			c.arcs[i][j].adj = Infinity; // 先初始化图的邻接矩阵

	// 部分弧长
	c.arcs[0][26].adj = 20;
	c.arcs[0][6].adj = 60;
	c.arcs[1][2].adj = 70;
	c.arcs[1][27].adj = 60;
	c.arcs[2][3].adj = 50;
	c.arcs[2][4].adj = 60;
	c.arcs[2][27].adj = 40;
	c.arcs[3][4].adj = 20;
	c.arcs[4][14].adj = 100;
	c.arcs[5][27].adj = 30;
	c.arcs[5][17].adj = 120;
	c.arcs[6][7].adj = 20;
	c.arcs[7][19].adj = 30;
	c.arcs[8][9].adj = 20;
	c.arcs[8][10].adj = 10;
	c.arcs[9][10].adj = 30;
	c.arcs[9][11].adj = 20;
	c.arcs[9][17].adj = 10;
	c.arcs[9][23].adj = 50;
	c.arcs[10][16].adj = 130;
	c.arcs[12][13].adj = 10;
	c.arcs[13][18].adj = 40;
	c.arcs[13][23].adj = 40;
	c.arcs[14][19].adj = 30;
	c.arcs[14][22].adj = 10;
	c.arcs[15][18].adj = 10;
	c.arcs[15][24].adj = 30;
	c.arcs[16][27].adj = 30;
	c.arcs[18][20].adj = 20;
	c.arcs[19][22].adj = 10;
	c.arcs[21][27].adj = 30;
	c.arcs[24][25].adj = 20;
	c.arcs[25][26].adj = 10;

	for (i = 0; i < c.vexnum; i++) // 邻接矩阵是对称矩阵，对称赋值
		for (j = 0; j < c.vexnum; j++)
			c.arcs[j][i].adj = c.arcs[i][j].adj;
	return c;
} // initgraph

// (2) 查找景点在图中的序号
int locatevex(MGraph c, int v)
{
	int i;
	for (i = 0; i < c.vexnum; i++)
		if (v == c.vexs[i].position)
			return i; // 找到，返回顶点序号i

	return -1; // 否则，返回-1
}

//(3) 、(4) 求两景点间的所有路径

// (3) 打印序号为m,n景点间的长度不超过12个景点的路径
void path(MGraph c, int m, int n, int k)
{
	int s, x = 0;
	int t = k + 1;			 // t 记载路径上下一个中间顶点在d[]数组中的下标
	if (d[k] == n && k < 12) // d[k]存储路径顶点。若d[k]是终点n且景点个数<=8，则输出该路径
	{						 // 递归出口，找到一条路径
		for (s = 0; s < k; s++)
			printf("%s--->", c.vexs[d[s]].name); // 输出该路径。s=0 时为起点m
		printf("%s", c.vexs[d[s]].name);		 // 输出最后一个景点名(即顶点n的名字，此时 s == k)
		printf("\n\n");
	}
	else
	{
		s = 0;
		while (s < c.vexnum) // 从第m个顶点，试探至所有顶点是否有路径
		{
			if ((c.arcs[d[k]][s].adj < Infinity) && (visited[s] == 0)) // 初态：顶点m到顶点s有边，且未被访问
			{
				visited[s] = 1;
				d[k + 1] = s;	  // 存储顶点编号s 至d[k + 1]中
				path(c, m, n, t); // 求从下标为t = k + 1的第d[t]个顶点开始的路径(递归调用)，同时打印出一条m至n的路径
				visited[s] = 0;	  // 将找到的路径上顶点的访问标志重新设置为0，以用于试探新的路径
			}
			s++; // 试探从下一个顶点 s 开始是否有到终点的路径
		} // endwhile
	} // endelse

} // endpath

//(4) 打印两景点间的景点个数不超过12的所有路径。调用(3)
int allpath(MGraph c)
{
	int k, i, j, m, n;
	printf("\n\n请输入你要查询的两个景点编号:\n\n");
	scanf("%d%d", &i, &j);
	printf("\n\n");
	m = locatevex(c, i); // 调用(2)，确定该顶点是否存在。若存在，返回该顶点编号
	n = locatevex(c, j);
	d[0] = m;					   // 存储路径起点m (int d[]数组是全局变量)
	for (k = 0; k < c.vexnum; k++) // 全部顶点访问标志初值设为0
		visited[k] = 0;
	visited[m] = 1;	  // 第m个顶点访问标志设置为1
	path(c, m, n, 0); // 调用(3)。k = 0，对应起点d[0] == m。k为d[]数组下标
	return 1;
}

// (5) 用迪杰斯特拉算法，求出一个景点到其他景点间的最短路径，并打印
void shortestpath_dij(MGraph c)
{
	// 迪杰斯特拉算法，求从顶点v0到其余顶点的最短路经及其带权长度d[v]
	// 若p[v][w]为1，则w是从v0到v的最短路经上的顶点
	// final[v]类型用于设置访问标志

	int v, w, i, min, t = 0, x, flag = 1, v0; // vo为起始景点的编号
	int final[35], d[35], p[35][35];
	printf("\n请输入一个起始景点的编号：");
	scanf("%d", &v0);
	printf("\n\n");
	while (v0 < 0 || v0 > c.vexnum)
	{
		printf("\n你所输入的景点编号不存在\n");
		printf("请重新输入：");
		scanf("%d", &v0);
	} // while
	for (v = 0; v < c.vexnum; v++)
	{
		final[v] = 0;				   // 初始化各顶点访问标志
		d[v] = c.arcs[v0][v].adj;	   // v0 到各顶点 v 的权值赋值给d[v]
		for (w = 0; w < c.vexnum; w++) // 初始化p[][]数组，各顶点间的程序段
			p[v][w] = 0;
		if (d[v] < Infinity) // v0 到v 有边相连，修改p[v][v0]的值为1
		{
			p[v][v0] = 1;
			p[v][v] = 1; // 各顶点自己到自己要连通
		}
	} // for
	d[v0] = 0;					   // 自己到自己的权值设为0
	final[v0] = 1;				   // v0的访问标志设为1，v 属于 s 集
	for (i = 1; i < c.vexnum; i++) // 对其余c.vexnum-1个顶点w，依次求 v 到 w 的最短路径
	{
		min = Infinity;
		for (w = 0; w < c.vexnum; w++) // 在未被访问的顶点中，查找与 v0 最近的顶点v
			if (!final[w])
				if (d[w] < min) // v0 到 w (有边)的权值<min
				{
					v = w;
					min = d[w];
				} // if
		final[v] = 1;										  // v 的访问标志设置为1，v 属于s集
		for (w = 0; w < c.vexnum; w++)						  // 修改v0 到其余各顶点w 的最短路径权值d[w]
			if (!final[w] && (min + c.arcs[v][w].adj < d[w])) // 若w 不属于s，且v 到w 有边相连
			{
				d[w] = min + c.arcs[v][w].adj; // 修改v0 到w 的权值d[w]
				for (x = 0; x < c.vexnum; x++) // 所有v0 到v 的最短路径上的顶点x，都是v0 到w 的最短路径上的顶点
					p[w][x] = p[v][x];
				p[w][w] = 1;
			} // if
	} // for
	for (v = 0; v < c.vexnum; v++) // 输出v0 到其它顶点v 的最短路径
	{
		if (v != v0)
			printf("%s", c.vexs[v0].name); // 输出景点v0 的景点名
		for (w = 0; w < c.vexnum; w++)	   // 对图中每个顶点w，试探w 是否是v0 到v 的最短路径上的顶点
		{
			if (p[v][w] && w != v0 && w != v) // 若w 是且w 不等于v0，则输出该景点
				printf("--->%s", c.vexs[w].name);
		}
		printf("---->%s", c.vexs[v].name);
		printf("\n总路线长为%d米\n\n", d[v]);
	} // for
} // shortestpath_dij

//(6)-(11)修改图的信息。包括建图、更新信息、删除、增加结点和边

//(6) 构造图的邻接矩阵
int creatgragh(MGraph *c) // 建图。以图的邻接矩阵存储图
{
	int i, j, m, n;
	int v0, v1;
	int distance;
	printf("请输入图的顶点数和边数: \n");
	scanf("%d %d", &c->vexnum, &c->arcnum);
	printf("下面请输入景点的信息：\n");
	for (i = 0; i < c->vexnum; i++) // 构造顶点向量(数组)
	{
		printf("请输入景点的编号：");
		scanf("%d", &c->vexs[i].position);
		printf("\n请输入景点的名称：");
		char name_[50];
		cin>>name_;
		strcpy(c->vexs[i].name,name_);
		printf("\n请输入景点的简介：");
		char introduction_[200];
		cin>>introduction_[200];
		strcpy(c->vexs[i].introduction,introduction_);
	}
	for (i = 0; i < c->arcnum; i++) // 初始化邻接矩阵
		for (j = 0; j < c->arcnum; j++)
			c->arcs[i][j].adj = Infinity;

	printf("下面请输入图的边的信息：\n");
	for (i = 1; i <= c->arcnum; i++) // 构造邻接矩阵
	{
		printf("\n第%d条边的起点 终点 长 度为：", i);
		scanf("%d %d %d", &v0, &v1, &distance);
		m = locatevex(*c, v0);
		n = locatevex(*c, v1);
		if (m >= 0 && n >= 0)
		{
			c->arcs[m][n].adj = distance;
			c->arcs[n][m].adj = c->arcs[m][n].adj;
		}
	}
	return 1;
} // creatgragh

// (7) 更新图的部分信息。返回值: 1
int newgraph(MGraph *c)
{
	int changenum; // 计数。用于记录要修改的对象的个数
	int i, m, n, t, distance, v0, v1;
	printf("\n下面请输入你要修改的景点的个数：\n");
	scanf("%d", &changenum);
	while (changenum < 0 || changenum > c->vexnum)
	{
		printf("\n输入错误！请重新输入");
		scanf("%d", &changenum);
	}

	for (i = 0; i < changenum; i++)
	{
		printf("\n请输入景点的编号：");
		scanf("%d", &m);
		t = locatevex(*c, m);
		printf("\n请输入景点的名称：");
		scanf("%s", &c->vexs[t].name);
		printf("\n请输入景点的简介：");
		scanf("%s", &c->vexs[t].introduction);
	}

	printf("\n下面请输入你要更新的边数");
	scanf("%d", &changenum);
	while (changenum < 0 || changenum > c->arcnum)
	{
		printf("\n输入错误！请重新输入");
		scanf("%d", &changenum);
	}

	printf("\n下面请输入更新边的信息：\n");
	for (i = 1; i <= changenum; i++)
	{
		printf("\n修改的第%d条边的起点 终点 长 度为：", i);
		scanf("%d", &v0, &v1, &distance);
		m = locatevex(*c, v0);
		n = locatevex(*c, v1);
		if (m >= 0 && n >= 0)
		{
			c->arcs[m][n].adj = distance;
			c->arcs[n][m].adj = c->arcs[m][n].adj;
		}
	}
	return 1;
} // newgraph

// (8) 增加一条边。返回值：1
int enarc(MGraph *c)
{
	int m, n, distance;
	printf("\n请输入边的起点和终点编号,权值：");
	scanf("%d %d %d", &m, &n, &distance);
	while (m < 0 || m > c->vexnum || n < 0 || n > c->vexnum)
	{
		printf("输入错误，请重新输入：");
		scanf("%d", &m, &n);
	}
	if (locatevex(*c, m) < 0)
	{
		printf("此结点%d已删除", m);
		return 1;
	}
	if (locatevex(*c, n) < 0)
	{
		printf("此结点%d已被删除：", n);
		return 1;
	}
	c->arcs[m][n].adj = distance;
	c->arcs[n][m].adj = c->arcs[m][n].adj; // 对称赋值
	return 1;
} // enarc

int envex(MGraph* c)
{
	int i;
	printf("请输入你要增加结点的信息：");
	printf("\n编号：");
	cin >> c->vexs[c->vexnum].position;
	printf(" 名称：");
	cin >> c->vexs[c->vexnum].name;
	printf("简介：");
	cin >> c->vexs[c->vexnum].introduction;
	c->vexnum++;
	for (i = 0; i < c->vexnum; i++)
	{
		c->arcs[c->vexnum - 1][i].adj = Infinity; 
		c->arcs[i][c->vexnum - 1].adj = Infinity; 
	}
	return 1;
} 

// (10) 删除图的一个顶点。返回值：1
int delvex(MGraph *c)
{
	int i = 0, j;
	int m;
	int v;
	if (c->vexnum <= 0)
	{
		printf("图中已无顶点");
		return 1;
	}
	printf("\n下面请输入你要删除的景点编号：");
	scanf("%d", &v);
	while (v < 0 || v > c->vexnum)
	{
		printf("\n输入错误！请重新输入");
		scanf("%d", &v);
	}
	m = locatevex(*c, v);
	if (m < 0)
	{
		printf("此顶点 %d 已删除", v);
		return 1;
	}
	for (i = m; i < c->vexnum - 1; i++) // 对顶点信息所在顺序表进行删除m 点的操作
	{
		strcpy(c->vexs[i].name, c->vexs[i + 1].name);
		strcpy(c->vexs[i].introduction, c->vexs[i + 1].introduction);
	}
	// 对原邻接矩阵，删除该顶点到其余顶点的邻接关系。分别删除相应的行和列
	for (i = m; i < c->vexnum - 1; i++)		   // 行
		for (j = 0; j < c->vexnum; j++)		   // 列
			c->arcs[i][j] = c->arcs[i + 1][j]; // 二维数组，从第m + 1行开始依次往前移一行。即删除第m 行
	for (i = m; i < c->vexnum - 1; i++)
		for (j = 0; j < c->vexnum; j++)
			c->arcs[j][i] = c->arcs[j][i + 1]; // 二维数组，从第m + 1列开始依次往前移一列。即删除第m 列
	c->vexnum--;
	return 1;
} // delvex

//(11) 删除图的一条边。返回值：1
int delarc(MGraph *c)
{
	int m, n;
	int v0, v1;
	if (c->arcnum <= 0)
	{
		printf("图中已无边，无法删除。");
		return 1;
	}
	printf("\n下面请输入你要删除的边的起点和终点编号：");
	scanf("%d %d", &v0, &v1);
	m = locatevex(*c, v0);
	if (m < 0)
	{
		printf("此 %d 顶点已删除", v0);
		return 1;
	}
	n = locatevex(*c, v1);
	if (n < 0)
		return 1;
	c->arcs[m][n].adj = Infinity; // 修改邻接矩阵对应的权值
	c->arcs[n][m].adj = Infinity;
	c->arcnum--;
	return 1;
} // delarc

// (12) 输出图的邻接矩阵的值
void printmatrix(MGraph c)
{
	int i, j, k = 0; // k 用于计数，控制换行

	for (i = 0; i < c.vexnum; i++)
		for (j = 0; j < c.vexnum; j++)
		{
			if (c.arcs[i][j].adj == Infinity)
				printf("----");
			else
				printf("%4d", c.arcs[i][j].adj);
			k++;
			if (k % c.vexnum == 0)
				printf("\n");
		}
} // printpath

//(13)图操作的主调函数。返回值：1
int changegraph(MGraph *c)
{

	while (1)
	{
		system("cls");
		int yourchoice;

		system("cls");
		system("color 0E");
		cout << "******************************" << endl;
		cout << "|    欢迎使用网点更改系统    |" << endl;
		cout << "******************************" << endl;
		cout << "|1、再次建图                 |" << endl;
		cout << "|2、删除结点                 |" << endl;
		cout << "|3、删除边                   |" << endl;
		cout << "|4、增加结点                 |" << endl;
		cout << "|5、增加边                   |" << endl;
		cout << "|6、修改网点信息             |" << endl;
		cout << "|7、返回                     |" << endl;
		cout << "******************************" << endl;
		cout << "请选择操作" << endl;
		scanf("%d", &yourchoice);
		printf("\n\n");
		while (!(yourchoice == 1 || yourchoice == 2 || yourchoice == 3 || yourchoice == 4 || yourchoice == 5 || yourchoice == 6 || yourchoice == 7))
		{
			printf("输入选择不明确，请重输\n");
			scanf("%d", &yourchoice);
		}
		switch (yourchoice)
		{
		case 1:
			creatgragh(c);
			break;
		case 2:
			delvex(c);
			break;
		case 3:
			delarc(c);
			break;
		case 4:
			envex(c);
			break;
		case 5:
			enarc(c);
			break;
		case 6:
			newgraph(c);
			break;
		case 7:
			return 1;
		}
		system("pause");
	} // endwhile(1)

	return 1;
} // changegraph

// (14) 查询两景点间的最短路径
void shortestpath_floyd(MGraph c)
{
	// 用floyd算法求各对顶点v和w间的最短路经及其带权长度的d[v][w]。
	// 若p[v][w][u]==1；则u是v到w的当前求得的最短路经上的顶点

	int i, j, k, d[35][35], p[35][35][35];
	int v, u, w;
	for (v = 0; v < c.vexnum; v++) // 初始化各对顶点 v，w 之间的起始距离 d[v][w] 及 路径 p[v][w][] 数组
	{
		for (w = 0; w < c.vexnum; w++)
		{
			d[v][w] = c.arcs[v][w].adj;	   // d[v][w] 中存放 v 至 w 间初始权值
			for (u = 0; u < c.vexnum; u++) // 初始化最短路径 p[v][w][] 数组，第 3 个分量全部清0
				p[v][w][u] = 0;
			if (d[v][w] < Infinity) // 如果 v 至 w 间有边相连
			{
				p[v][w][v] = 1; // v 是 v 至 w  最短路径上的顶点
				p[v][w][w] = 1; // w 是 v 至 w  最短路径上的顶点
			} // if
		} // for
	} // endfor

	for (u = 0; u < c.vexnum; u++) // 求v至w的最短路径及距离。对任意顶点u，试探其是否为v至w最短路径上的顶点
	{
		for (v = 0; v < c.vexnum; v++)
			for (w = 0; w < c.vexnum; w++)
				if (d[v][u] + d[u][w] < d[v][w]) // 从 v 经 u 到 w 的一条路径更短
				{
					d[v][w] = d[v][u] + d[u][w];			   // 修改 v 至 w 的最短路径长度
					for (i = 0; i < c.vexnum; i++)			   // 修改 v 至 w 的最短路径数组。 若i是v至u的最短路径上的顶点,
						p[v][w][i] = p[v][u][i] || p[u][w][i]; // 或i是u至w的最短路径上的顶点, 则i是v至w的最短路径上的顶点
				}
	} // endfor

	printf("\n请输入出发点和目的地编号：");
	scanf("%d%d", &k, &j);
	printf("\n\n");
	while (k < 0 || k > c.vexnum || j < 0 || j > c.vexnum)
	{
		printf("\n你所输入的景点编号不存在！");
		printf("\n请重新输入出发点和目的地编号：\n\n");
		scanf("%d%d", &k, &j);
		printf("\n\n");
	}
	printf("%s", c.vexs[k].name); // 输出出发景点名称
	for (u = 0; u < c.vexnum; u++)
		if (p[k][j][u] && k != u && j != u) // 输出最短路径上中间景点名称
			printf("--->%s", c.vexs[u].name);
	printf("--->%s", c.vexs[j].name); // 输出目的地景点名称

	printf("\n\n\n总长为%d米\n\n\n", d[k][j]);

} // shortestpath_floyd

// (15) 查询景点的信息
void seeabout(MGraph c)
{
	int k;
	printf("\n请输入要查询的景点编号：");
	scanf("%d", &k);
	while (k < 0 || k > c.vexnum)
	{
		printf("\n你所输入的景点编号不存在！");
		printf("\n请重新输入：");
		scanf("%d", &k);
	}
	printf("\n\n编号：%4d\n", c.vexs[k].position);
	printf("\n\n景点名称：%10s\n", c.vexs[k].name);
	printf("\n\n介绍：%80s\n\n", c.vexs[k].introduction);

} // seeabout

// (16) 显示所有景点信息
void browsecompus(MGraph c)
{
	int i;
	printf(" \n\n编号      景点名称                  简介\n");
	printf("___________________________________________________________________________________________________________\n");

	for (i = 0; i < c.vexnum; i++)
		printf("%-10d%-25s%-80s\n", c.vexs[i].position, c.vexs[i].name, c.vexs[i].introduction);
	printf("___________________________________________________________________________________________________________\n\n");

} // browsecompus

void Readfile1(StaffList &SL, string file)
{
	ifstream ifs;
	Staff *p = SL;
	ifs.open(file.c_str());
	if (!ifs)
		return;
	while (!ifs.eof())
	{
		Staff *pt = new Staff;
		pt->next = NULL;
		ifs >> pt->Number >> pt->name >> pt->sex >> pt->age >> pt->password;
		if (ifs.eof())
			break;
		p->next = pt;
		p = pt;
	}
	ifs.close();
}

void Readfile2(BankList &BL, string file)
{
	ifstream ifs;
	Banker *p = BL;
	ifs.open(file.c_str());
	if (!ifs)
		return;
	while (!ifs.eof())
	{
		Banker *pt = new Banker;
		pt->next = NULL;
		ifs >> pt->account_num >> pt->name >> pt->sex >> pt->password >> pt->money >> pt->open_time >> pt->states;
		if (ifs.eof())
			break;
		p->next = pt;
		p = pt;
	}
	ifs.close();
}

void Menu() // 待选择系统菜单界面
{
	system("cls");
	system("color B0");
	cout << "******************************" << endl;
	cout << "|    请选择您要进入的系统    |" << endl;
	cout << "******************************" << endl;
	cout << "|1、职员管理系统             |" << endl;
	cout << "|2、客户管理系统             |" << endl;
	cout << "|3、排队管理系统             |" << endl;
	cout << "|4、网点查询系统             |" << endl;
	cout << "|0、退出系统                 |" << endl;
	cout << "******************************" << endl;
}

void Menu1() // 职员系统菜单界面
{
	system("cls");
	system("color 0E");
	cout << "******************************" << endl;
	cout << "|      银行职员管理系统      |" << endl;
	cout << "******************************" << endl;
	cout << "|1、查询职员信息             |" << endl;
	cout << "|2、入职                     |" << endl;
	cout << "|3、离职                     |" << endl;
	cout << "|4、修改职员信息             |" << endl;
	cout << "|5、显示所有职员信息         |" << endl;
	cout << "|6、保存职员信息             |" << endl;
	cout << "|0、退出系统                 |" << endl;
	cout << "******************************" << endl;
}

void Menu2() // 客户系统菜单界面
{
	system("cls");
	system("color 0E");
	cout << "******************************" << endl;
	cout << "|      银行账户管理系统      |" << endl;
	cout << "******************************" << endl;
	cout << "|1、查询账户信息             |" << endl;
	cout << "|2、开户                     |" << endl;
	cout << "|3、销户                     |" << endl;
	cout << "|4、修改账户密码             |" << endl;
	cout << "|5、账户排序                 |" << endl;
	cout << "|6、存款                     |" << endl;
	cout << "|7、取款                     |" << endl;
	cout << "|8、转账                     |" << endl;
	cout << "|9、计算利息                 |" << endl;
	cout << "|10、保存账户信息            |" << endl;
	cout << "|11、显示所有账户信息        |" << endl;
	cout << "|0、退出系统                 |" << endl;
	cout << "******************************" << endl;
}

void Menu3() // 排队管理系统菜单界面
{
	system("cls");
	system("color 0E");
	cout << "******************************" << endl;
	cout << "|        排队管理系统        |" << endl;
	cout << "******************************" << endl;
	cout << "|1、VIP放号                  |" << endl;
	cout << "|2、Normal放号               |" << endl;
	cout << "|3、VIP队首完成服务          |" << endl;
	cout << "|4、Normal队首完成服务       |" << endl;
	cout << "|5、展示目前您的得分情况     |" << endl;
	cout << "|0、退出系统                 |" << endl;
	cout << "******************************" << endl;
	cout << "请选择你的操作" << endl;
}

void Menu3_2(Staff *&goal, string &realFileName) // 排队系统员工登录界面
{
	system("cls");
	system("color FD");
	string number;
	int password;	  // 输入的
	int realPassword; // 实际的
	cout << "欢迎使用排队管理系统!" << endl;
	cout << "请输入您的员工工号和密码进行登录" << endl;
	cout << "工号: ";
	cin >> number;
	StaffList SL;
	InitList1(SL);
	string fileNames[] = {"GroupA.txt", "GroupB.txt", "GroupC.txt"};
	bool found = false;
	for (const string &fileName : fileNames)
	{
		Readfile1(SL, fileName);
		Staff *p = SL->next;
		while (p)
		{
			if (p->Number == number)
			{
				found = true;
				realPassword = p->password;
				realFileName = fileName;
				goal = p;
				break;
			}
			p = p->next;
		}
		if (found)
			break;
	}
	if (!found)
	{
		cout << "工号不存在，请重新输入 " << endl;
		system("pause");
		Menu3_2(goal, realFileName);
		return;
	}

	cout << "密码: ";
	char charPassword[20]; // 用于存储密码字符数组
	int i = 0;
	char ch;
	while ((ch = _getch()) != '\r') // \r是回车键
	{
		if (ch == '\b' && i > 0) // 处理退格键
		{
			cout << "\b \b";
			--i;
		}
		else if (ch >= '0' && ch <= '9') // 只接受数字（0 - 9）
		{
			charPassword[i++] = ch;
			cout << '*';
		}
	}
	charPassword[i] = '\0';		   // 字符串结束标志
	password = atoi(charPassword); // 将字符数组转换为整数
	cout << endl;
	cout << "\n";
	if (password != realPassword)
	{
		cout << "密码不正确，请重新输入 " << endl;
		system("pause");
		Menu3_2(goal, realFileName);
		return;
	}
	cout << "登录成功!" << endl;
	cout << "正在进入 请稍后";
	Wait();
	cout << endl;
	system("cls");
}

void Menu4() // 银行网点查询系统菜单界面
{

	system("cls");
	system("color 0E");
	cout << "******************************" << endl;
	cout << "|   山财大银行网点查询系统   |" << endl;
	cout << "******************************" << endl;
	cout << "|1、学校网点介绍             |" << endl;
	cout << "|2、游览网点路线             |" << endl;
	cout << "|3、查询网点间最短路径       |" << endl;
	cout << "|4、查询网点信息             |" << endl;
	cout << "|5、更改图的信息             |" << endl;
	cout << "|6、查询景点间可行路径       |" << endl;
	cout << "|7、打印邻接矩阵             |" << endl;
	cout << "|0、退出系统                 |" << endl;
	cout << "******************************" << endl;
	cout << "请选择操作" << endl;
}

void Wait()
{
	cout << ".";
	Sleep(80);
	cout << ".";
	Sleep(80);
	cout << ".";
	Sleep(80);
	cout << " ";
	cout << ".";
	Sleep(80);
	cout << ".";
	Sleep(80);
	cout << ".";
	Sleep(80);
}
// 选择需要操作的文件
void Choice_a_file(BankList &BL, string &FileName)
{
	system("cls");
	int ch;
	system("color F0");
	cout << "1：VIP1.txt" << endl;
	cout << "2：VIP2.txt" << endl;
	cout << "3：Normal1.txt" << endl;
	cout << "4：Normal2.txt" << endl;
	cout << "5：Normal3.txt" << endl;
	cout << "0：退出" << endl;
	while (1)
	{
		cout << "请选择需要操作的文件:" << endl;
		cin >> ch;
		if (ch == 1)
		{
			FileName = "VIP1.txt";
			cout << "OK 正在进入 VIP1.txt";
			Wait();
			Readfile2(BL, "VIP1.txt");
			break;
		}
		else if (ch == 2)
		{
			FileName = "VIP2.txt";
			cout << "OK 正在进入 VIP2.txt";
			Wait();
			Readfile2(BL, "VIP2.txt");
			break;
		}
		else if (ch == 3)
		{
			FileName = "Normal1.txt";
			cout << "OK 正在进入 Normal1.txt";
			Wait();
			Readfile2(BL, "Normal1.txt");
			break;
		}
		else if (ch == 4)
		{
			FileName = "Normal2.txt";
			cout << "OK 正在进入 Normal2.txt";
			Wait();
			Readfile2(BL, "Normal2.txt");
			break;
		}
		else if (ch == 5)
		{
			FileName = "Normal3.txt";
			cout << "OK 正在进入 Normal3.txt";
			Wait();
			Readfile2(BL, "Normal3.txt");
			break;
		}
		else if (ch == 0)
		{
			cout << "好的 正在退出 请稍后";
			Wait();
			system("cls");
			cout << "已成功退出!" << endl;
			exit(0);
		}
		else
		{
			cout << "您的输入有误！\n请重新输入! " << endl;
			Sleep(1000);
			system("cls");
			system("color F0");
			cout << "1：VIP1.txt" << endl;
			cout << "2：VIP2.txt" << endl;
			cout << "3：Normal1.txt" << endl;
			cout << "4：Normal2.txt" << endl;
			cout << "5：Normal3.txt" << endl;
			cout << "0：退出" << endl;
		}
	}
}

// 选择需要操作的文件
void Choice_a_person(StaffList &SL, string &FileName)
{
	system("cls");
	int ch;
	system("color F0");
	cout << "1：GroupA.txt" << endl;
	cout << "2：GroupB.txt" << endl;
	cout << "3：GroupC.txt" << endl;
	cout << "0：退出" << endl;
	while (1)
	{
		cout << "请选择需要操作的文件:" << endl;
		cin >> ch;
		if (ch == 1)
		{
			FileName = "GroupA.txt";
			cout << "OK 正在进入 GroupA.txt";
			Wait();
			Readfile1(SL, "GroupA.txt");
			break;
		}
		else if (ch == 2)
		{
			FileName = "GroupB.txt";
			cout << "OK 正在进入 GroupB.txt";
			Wait();
			Readfile1(SL, "GroupB.txt");
			break;
		}
		else if (ch == 3)
		{
			FileName = "GroupC.txt";
			cout << "OK 正在进入 GroupC.txt";
			Wait();
			Readfile1(SL, "GroupC.txt");
			break;
		}

		else if (ch == 0)
		{
			cout << "好的 正在退出 请稍后";
			Wait();
			system("cls");
			cout << "已成功退出!" << endl;
			exit(0);
		}
		else
		{
			cout << "您的输入有误！\n请重新输入! " << endl;
			Sleep(1000);
			system("cls");
			system("color F0");
			cout << "1：GroupA.txt" << endl;
			cout << "2：GroupB.txt" << endl;
			cout << "3：GroupC.txt" << endl;
			cout << "0：退出" << endl;
		}
	}
}

void mainwork1()
{
	string FileName;
	StaffList SL;
	InitList1(SL);
	Choice_a_person(SL, FileName);
	int choice1; // 职员系统choice1
	while (1)
	{
		// 菜单调用
		Menu1();
		cout << "请选择您要进行的操作" << endl;
		cout << "（每一步操作后记得 6、保存职员信息）" << endl;
		cin >> choice1;
		switch (choice1)
		{
		case 1: // 1、查询职员信息
			SelectStaff(SL);
			break;
		case 2: // 2、入职
			AddStaff(SL);
			break;
		case 3: // 3、离职
			DeleteStaff(SL);
			break;
		case 4: // 4、修改职员信息
			Revise(SL);
			break;
		case 5: // 5、显示所有职员信息
			Display1(SL);
			break;
		case 6: // 6、保存职员信息
			Save1(SL, FileName);
			break;
		case 0: // 0、退出系统
			cout << "欢迎下次使用" << endl;
			system("pause");
			exit(0);
			break;
		default:
			break;
		}
		system("pause");
	}
}

void mainwork2()
{
	BankList BL;
	InitList2(BL);
	int choice2; // 客户系统choice2
	string FileName;
	Choice_a_file(BL, FileName); // 选择待操作文件
	while (1)
	{
		// 菜单调用
		Menu2();
		cout << "请选择您要进行的操作";
		cout << "（每一步操作后记得 10、保存账户信息）" << endl;

		cin >> choice2;
		switch (choice2)
		{
		case 1: // 1、查询账户信息
			SelectAccount(BL);
			break;
		case 2: // 2、开户
			Open_an_ccount(BL);
			break;
		case 3: // 3、销户
			Pin_households(BL);
			break;
		case 4: // 4、修改账户密码
			Modify_account_password(BL);
			break;
		case 5: // 5、账户排序
			Sort_accountInfo(BL);
			break;
		case 6: // 6、存款
			deposit(BL);
			break;
		case 7: // 7、取款
			withdrawals(BL);
			break;
		case 8: // 8、转账
			transfer(BL);
			break;
		case 9: // 9、计算利息
			Calculation_interest(BL);
			break;
		case 10: // 10、保存账户信息
			Save2(BL, FileName);
			break;
		case 11: // 11、显示所有账户信息
			Display2(BL);
			break;
		case 0: // 0、退出系统
			cout << "欢迎下次使用" << endl;
			system("pause");
			exit(0);
			break;
		default:
			break;
		}
		system("pause");
	}
}

void mainwork3()
{
	Staff *goal = new Staff; // 所登陆的账号
	string fileName;
	Menu3_2(goal, fileName); // 登录页面
	while (1)
	{
		Menu3(); // 菜单栏
		DisplayVIP();
		DisplayNormal();
		int choice3;
		cin >> choice3;
		switch (choice3)
		{
		case 1: // 1、VIP放号
			releaseVIP();
			break;
		case 2: // 2、Normal放号
			releaseNormal();
			break;
		case 3: // 3、VIP队首结束服务
			endVIPServe(goal);
			break;
		case 4: // 4、Normal队首结束服务
			endNormalServe(goal);
			break;
		case 5: // 5、展示当前分数
			DisplayScore(goal);
			break;
		case 0: // 0、退出
			cout << "欢迎下次使用" << endl;
			system("pause");
			exit(0);
		default:
			cout << "无效选择，请重新输入  " << endl;
		}
		system("pause");
	}
}

// 主要工作函数 操作区用户界面
void mainwork4()
{
	int yourchoice;
	MGraph campus = initgraph();
	while (1)
	{
		system("cls");
		Menu4();
		printf("请输入你的选择：");
		scanf("%d", &yourchoice);
		while (!(yourchoice == 1 || yourchoice == 2 || yourchoice == 3 || yourchoice == 4 || yourchoice == 5 || yourchoice == 6 || yourchoice == 7 || yourchoice == 0))
		{
			printf("输入选择不明确，请重输\n");
			scanf("%d", &yourchoice);
		}
		switch (yourchoice)
		{
		case 1:
			system("cls");
			browsecompus(campus);
			break;
		case 2:
			system("cls");
			shortestpath_dij(campus);
			break;
		case 3:
			system("cls");
			shortestpath_floyd(campus);
			break;
		case 4:
			system("cls");
			seeabout(campus);
			break;
		case 5:
			system("cls");
			changegraph(&campus);
			break;
		case 6:
			system("cls");
			allpath(campus);
			break;
		case 7:
			system("cls");
			printmatrix(campus);
			break;
		case 0:
			system("cls");
			exit(0);
			break;
		default:
			break;
		}
		system("pause");
	}
}

void MainWork()
{
	int choice_root; // 选择不同系统
	Menu();
	cin >> choice_root;
	cout << "正在进入 请稍后";
	Wait();
	switch (choice_root)
	{
	case 1: // 职员管理
		mainwork1();
		break;

	case 2: // 客户管理
		mainwork2();
		break;

	case 3: // 3、叫号系统
		mainwork3();
		break;

	case 4: // 4、网点查询系统
		mainwork4();
		break;

	case 0: // 0、退出系统
		cout << "欢迎下次使用" << endl;
		system("pause");
		break;
	default:
		break;
	}
}

int main()
{
	MainWork();
	return 0;
}
