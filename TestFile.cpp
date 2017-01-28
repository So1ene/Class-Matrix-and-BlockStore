#include"BlockStore.h"
#include<iostream>
using namespace std;

template<typename T>
void forEach(T& beg, T& end) {
	for (T p = beg; p != end; p++)
		cout << *p << " ";
}
template<typename InputIterator, typename ToDo>
int countIF(InputIterator First, InputIterator Last, ToDo DoSome) {
	int count = 0;
	for (; First != Last; ++First)
		if (DoSome(*First))++count;
	return (count);
}
template<typename T>
class doSome {
public:
	doSome(){}
	bool operator()(T val) {
		return val < (T)(500);
	}
};

int main() {
	BlockStore<int> blockA;
	cout << "测试push_back:移入0~999" << endl;
	for (int i = 0; i < 1000; i++)
		blockA.push_back(i);
	cout << "测试push_front:移入0~999" << endl;
	for (int i = 0; i < 1000; i++) 
		blockA.push_front(i);
	cout << "测试itr，遍历并输出,涉及到iterator构造函数、operator!=、operator++、operator*等操作" << endl;
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================华丽的分割线============================" << endl;
	cout << "测试pop_back():移出后500个元素" << endl;
	for (int i = 0; i < 500; i++)
		blockA.pop_back();
	cout << "测试pop_front():移除前500个元素" << endl;
	for (int i = 0; i < 500; i++)
		blockA.pop_front();
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================华丽的分割线============================" << endl;
	cout << "测试函数outPutBlock(),输出各Block已使用的大小" << endl;
	blockA.outPutBlock();
	cout << endl;
	cout << "测试operator[]，输出[0,500]中下标被50整除的元素" << endl;
	for (int i = 500; i >= 0; i -= 50)
		cout << blockA[i] << " ";
	cout << endl;
	cout << "测试delete(N):移除[0,500]中下标被50整除的元素" << endl;
	for (int i = 500; i >= 0; i -= 50)
		blockA.deleteN(i);
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================华丽的分割线============================" << endl;
	cout << "当前各个Block元素已经使用的大小：" << endl;
	blockA.outPutBlock();
	cout << endl << "The Size Of BlockA:" << blockA.getSize() << endl;
	cout << endl << "测试compress()函数，将各数据块压缩，处理后各个Block中已使用的大小：" << endl;
	blockA.compress();
	blockA.outPutBlock();
	cout << endl << "The Size Of BlockA:" << blockA.getSize() << endl;
	cout << endl << "==============================华丽的分割线============================" << endl;
	cout << "测试forEach(T beg,T end)函数：" << endl;
	forEach(blockA.begin(), blockA.end());
	cout << endl << "==============================华丽的分割线============================" << endl;
	cout << "测试countIF(T1 fir,T1 end,T2 dosth)函数,这里设置dosth用来判断值<500：" << endl;
	doSome<int> dosth;
	cout<<"COUNT="<<countIF(blockA.begin(),blockA.end(),dosth)<<endl;

	system("pause");

}
