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
	cout << "����push_back:����0~999" << endl;
	for (int i = 0; i < 1000; i++)
		blockA.push_back(i);
	cout << "����push_front:����0~999" << endl;
	for (int i = 0; i < 1000; i++) 
		blockA.push_front(i);
	cout << "����itr�����������,�漰��iterator���캯����operator!=��operator++��operator*�Ȳ���" << endl;
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================�����ķָ���============================" << endl;
	cout << "����pop_back():�Ƴ���500��Ԫ��" << endl;
	for (int i = 0; i < 500; i++)
		blockA.pop_back();
	cout << "����pop_front():�Ƴ�ǰ500��Ԫ��" << endl;
	for (int i = 0; i < 500; i++)
		blockA.pop_front();
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================�����ķָ���============================" << endl;
	cout << "���Ժ���outPutBlock(),�����Block��ʹ�õĴ�С" << endl;
	blockA.outPutBlock();
	cout << endl;
	cout << "����operator[]�����[0,500]���±걻50������Ԫ��" << endl;
	for (int i = 500; i >= 0; i -= 50)
		cout << blockA[i] << " ";
	cout << endl;
	cout << "����delete(N):�Ƴ�[0,500]���±걻50������Ԫ��" << endl;
	for (int i = 500; i >= 0; i -= 50)
		blockA.deleteN(i);
	for (BlockStore<int>::iterator itr = blockA.begin(); itr != blockA.end(); itr++)
		cout << *itr << " ";
	cout << endl << "==============================�����ķָ���============================" << endl;
	cout << "��ǰ����BlockԪ���Ѿ�ʹ�õĴ�С��" << endl;
	blockA.outPutBlock();
	cout << endl << "The Size Of BlockA:" << blockA.getSize() << endl;
	cout << endl << "����compress()�������������ݿ�ѹ������������Block����ʹ�õĴ�С��" << endl;
	blockA.compress();
	blockA.outPutBlock();
	cout << endl << "The Size Of BlockA:" << blockA.getSize() << endl;
	cout << endl << "==============================�����ķָ���============================" << endl;
	cout << "����forEach(T beg,T end)������" << endl;
	forEach(blockA.begin(), blockA.end());
	cout << endl << "==============================�����ķָ���============================" << endl;
	cout << "����countIF(T1 fir,T1 end,T2 dosth)����,��������dosth�����ж�ֵ<500��" << endl;
	doSome<int> dosth;
	cout<<"COUNT="<<countIF(blockA.begin(),blockA.end(),dosth)<<endl;

	system("pause");

}
