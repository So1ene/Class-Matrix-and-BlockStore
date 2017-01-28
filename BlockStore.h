#ifndef _BlockStore_H
#define _BlockStore_H

#include<assert.h>
#include<iostream>
using namespace std;

template<typename T>
class BlockStore {
public:
	//constructor and destructor;
	//
	//constructor without parameters;
	BlockStore();
	//constructor with parameters;
	BlockStore(unsigned int bsize);
	//copy constructor;
	BlockStore(const BlockStore& rObj);
	//move constructor;
	BlockStore(BlockStore&& rObj);
	//destructor;
	~BlockStore();

	//class BlockTable used to record the statement of block;
	struct BlockTable;
	//the iterator of class BlockStore;
	class BlockIterator;
	//class Block to store elements;
	class Block;

	//set alias;
	//
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef BlockIterator iterator;
	typedef BlockTable table;
	typedef Block block;
	typedef unsigned int size_type;

	//Function
	//
	//add element from front;
	void push_front(const T& val);
	//add element from back;
	void push_back(const T& val);
	//set the original size of block whose default is 100;
	void setBlockOriSiz(unsigned int size = 100);
	//set the size block increases by each time whose default is 10percent;
	void setBlockIncSiz(double percent = 1.1);
	//remove one element from the back;
	void pop_back();
	//remove one element from the front;
	void pop_front();
	//remove the element with index N;
	void deleteN(unsigned int N);
	//compress the blocks to fulfill the blocks;
	void compress();
	//override operator[] to get the element with index N;
	unsigned int getSize();
	//output the size of each block;
	void outPutBlock();
	reference operator[](unsigned int N);
	//override operator=;
	BlockStore<T> operator=(const BlockStore<T>& rObj);
	//begin(): return the iterator pointing to the first element in the container;
	iterator begin();
	//end(): return the iterator pointing of the last element in the container;
	iterator end();

private:
	//param initBlockSize refers to the initial size of block;
	unsigned int initBlockSize;
	//param incrPercent refers to the percent of the size of block increased each time;
	double incrPercent;
	//param firBlock refers to the first node of BlockTable;
	BlockTable* firBlock;
	//param lstBlock refers to the last node of BlockTable;
	BlockTable* lstBlock;
	//param size refers to the number of element in BlockStore;
	unsigned int size;
};

template<typename T>
struct BlockStore<T>::BlockTable {
public:
	//member of struction;
	//
	//tolSize refers to the total size of the relevant block;
	unsigned int tolSize;
	//usdSize refers to the used size of the relevant block; 
	unsigned int usdSize;
	//location refers to the pointer to the relevant block;
	Block* location;
	//nextBlock refers to the next node of BlockTable;
	BlockTable* nextBlock;
	//lastBlock refers to the last node of BlockTable;
	BlockTable* lastBlock;

	//constructor and destructor
	//
	BlockTable(int tsize) {
		usdSize = 0;
		tolSize = tsize;
		location = new Block(this->tolSize);
		lastBlock = nullptr;
		nextBlock = nullptr;
	}
	BlockTable(const BlockTable& rObj) {
		this->tolSize = rObj.tolSize;
		this->usdSize = rObj.usdSize;
		this->location = new Block(*oObj->location);
	}
	~BlockTable() {
		this->tolSize = 0;
		this->usdSize = 0;
		delete location;
		location = nullptr;
	}
};

//The definition of class BlockIterator;
//
template<typename T>
class BlockStore<T>::BlockIterator {
public:
	//Iterator Specification;
	//set alias
	//
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef std::random_access_iterator_tag iterator_category;
	typedef int difference_type;

	//Constructor and destructor;
	//
	BlockIterator();
	BlockIterator(const BlockIterator& rObj);
	BlockIterator(BlockIterator&& rObj);
	~BlockIterator();

	//function member
	//
	//Operator Override;
	//operator* used as *itr;
	reference operator*();
	//operator[] equals function *(itr+N);
	reference operator[](unsigned int N);
	//operator-> used as itr->;
	pointer operator->();
	//operator:++itr|--itr|itr++|itr--|itr+N|itr-N;
	BlockIterator operator++();
	BlockIterator operator++(int back);
	BlockIterator operator--();
	BlockIterator operator--(int back);
	BlockIterator operator+(unsigned int N);
	BlockIterator operator-(unsigned int N);
	//operator ==|!=|<|<=|>|>=;
	bool operator==(const BlockIterator& rObj)const {
		return this->ptrNow == rObj.ptrNow;
	}
	bool operator!=(const BlockIterator& rObj)const {
		return !(*this == rObj);
	}
	bool operator<(const BlockIterator& rObj)const;
	bool operator<=(const BlockIterator& rObj)const;
	bool operator>(const BlockIterator& rObj) const {
		return !(*this) <= rObj;
	}
	bool operator>=(const BlockIterator& rObj)const {
		return !(*this)<rObj;
	}
	//return the distance bewteen two itrs;
	difference_type operator-(const BlockIterator& rObj);

	friend BlockStore;
private:
	pointer ptrNow;
	//Note: Parameter ptrFir&ptrLst was used to record the limitation of each Block,but when pushed or removed element the value of 
	//ptrFir or ptrLst may go wrong,so I change the strategy of the judgment.
	//The orginal version has been annotated for further discussion.
	//pointer ptrFir;
	//pointer ptrLst;
	table* node;
};
template<typename T>
BlockStore<T>::BlockIterator::BlockIterator() {
	this->ptrNow = nullptr;
	this->node = nullptr;
}
template<typename T>
BlockStore<T>::BlockIterator::BlockIterator(const BlockIterator& rObj) {
	this->ptrNow = rObj.ptrNow;
	this->node = rObj.node;
}
template<typename T>
BlockStore<T>::BlockIterator::BlockIterator(BlockIterator&& rObj) {
	this->ptrNow = rObj.ptrNow;
	this->node = rObj.node;
	rObj.ptrNow = nullptr;
	rObj.node = nullptr;
}
template<typename T>
BlockStore<T>::BlockIterator::~BlockIterator() {
	this->ptrNow = nullptr;
	this->node = nullptr;
}
template<typename T>
typename BlockStore<T>::reference BlockStore<T>::BlockIterator::operator*() {
	return *(this->ptrNow);
}
template<typename T>
typename BlockStore<T>::reference BlockStore<T>::BlockIterator::operator[](unsigned int N) {
	return *(this->ptrNow + N);
}
template<typename T>
typename BlockStore<T>::pointer BlockStore<T>::BlockIterator::operator->() {
	return this->ptrNow;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator++() {
	//use exception to avoid that the iterator may overflow;
	try {
		//if already reach the end of the block;
		if (this->ptrNow == this->node->location->last(this->node->lastBlock)) {
			//if no block any more;
			if (!this->node->nextBlock)
				throw(1);
			else {
				this->node = this->node->nextBlock;
				this->ptrNow = this->node->location->first(false);
			}
		}
		//if not reach the end of block;
		else {
			//if not the end of the elements;
			if (this->ptrNow != this->node->location->last(this->node->lastBlock))
				this->ptrNow++;
			//if not, which means the itr has already reached the end of the elements;
			else
				throw(1);
		}
		return *this;
	}
	catch (int) {
		assert("Out of the range of BlockStore!");
	}
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator--() {
	//serve the same function as operator++,just reverse the direction;
	try {
		if (this->ptrNow == this->node->location->first(this->node->lastBlock==nullptr)) {
			if (!this->node->lastBlock)
				throw(1);
			else {
				this->node = this->node->lastBlock;
				this->ptrNow = this->node->location->first(this->node->lastBlock==nullptr);
			}
		}
		else {
			if (this->ptrNow != this->node->location->first(true))
				this->ptrNow--;
			else
				throw(1);
		}
	}
	catch (int) {
		assert("Out of the range of BlockStore!");
	}
}
template<typename T>
bool BlockStore<T>::BlockIterator::operator<=(const BlockIterator& rObj)const {
	BlockIterator tmp = (*this);
	pointer flag;
	//get the last pointer to the last element;
	table lstnode = tmp->node;
	while (lstnode->nextBlock)
		lstnode = lstnode->nextBlock;
	flag = lstnode->location->last(lstnode->lastBlock);
	bool isTrue = false;
	//if rObj points to the end of the container,return true;
	if (rObj.ptrNow == flag)
		return true;
	//tmp continue to increase until reaching the end of the container or meeting the rObj.
	while (tmp->ptrNow != flag) {
		if (tmp == rObj) {
			isTrue = true;
			break;
		}
		tmp++;
	}
	return isTrue;
}
template<typename T>
bool BlockStore<T>::BlockIterator::operator<(const BlockIterator& rObj)const {
	//serve the similiar function as operator<=,but lack the process of judging equalty.
	BlockIterator tmp = (*this);
	pointer flag;
	table lstnode = tmp->node;
	while (lstnode->nextBlock)
		lstnode = lstnode->nextBlock;
	flag = lstnode->location->last(lstnode->lastBlock);
	bool isTrue = false;
	while (tmp->ptrNow != flag) {
		if (tmp == rObj) {
			isTrue = true;
			break;
		}
		tmp++;
	}
	return isTrue;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator++(int back) {
	BlockIterator tmp = *this;
	++(*this);
	return tmp;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator--(int back) {
	BlockIterator tmp = *this;
	--(*this);
	return tmp;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator+(unsigned int N) {
	for (unsigned int i = 0; i<N; i++)
		(*this)++;
	return *this;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::BlockIterator::operator-(unsigned int N) {
	for (unsigned int i = 0; i<N; i++)
		(*this)--;
	return *this;
}
template<typename T>
typename BlockStore<T>::BlockIterator::difference_type BlockStore<T>::BlockIterator::operator-(const BlockIterator& rObj) {
	//return the distance bewteen two itrs;
	//
	//because the addresses of different blocks are not continuous,I adopt the stratgy that 
	//increase one of the two itrs from both directions until meeting the other itr;
	BlockIterator selfTmp0 = *this;
	BlockIterator selfTmp1 = *this;
	BlockIterator tarTmp0 = rObj;
	BlockIterator tarTmp1 = rObj;
	difference_type count = 0;
	while (selfTmp0 != tarTmp0 = rObj&&selfTmp1 != tarTmp1 = rObj) {
		count++;
		selfTmp0--;
		selfTmp1++;
	}
	return count;
}
template<typename T>
class BlockStore<T>::Block {
public:
	Block(unsigned int inits);
	Block(const Block& rObj);
	~Block();

	inline bool isFull() {
		return this->tolSize == this->usdSize;
	}
	inline pointer first(bool isFront) const {
		if (!isFront)
			return this->valAry;
		return this->valAry + tolSize - usdSize;
	}
	inline pointer last(bool isFront) const {
		if (isFront)
			return this->valAry + this->tolSize - 1;
		return this->valAry + this->usdSize - 1;
	}
	inline pointer now(bool isFront) const {
		if (isFront) {

			return this->valAry + tolSize - usdSize;
		}
		else
			return this->valAry + usdSize;
	}
	void push_back(const T& val);
	bool pop_back();
	void push_front(const T& val);
	bool pop_front();
	void copy(pointer des, pointer src, unsigned int siz);
	void copy_rev(pointer des, pointer src, unsigned int siz);
	bool deleteN(size_type N, bool isFront);
	Block operator=(const Block& rObj);
	reference getElement(unsigned int N, bool isFront);
private:
	//total size;
	size_type tolSize;
	//used size;
	size_type usdSize;
	//the array used to store value;
	pointer valAry;
};
template<typename T>
BlockStore<T>::Block::Block(unsigned int inits) {
	tolSize = inits;
	usdSize = 0;
	valAry = new value_type[tolSize];
}
template<typename T>
BlockStore<T>::Block::Block(const Block& rObj) {
	this->tolSize = rObj.tolSize;
	this->usdSize = rObj.usdSize;
	this->valAry = new value_type[this->tolSize];
	copy(this->valAry, rObj.valAry, this->usdSize);
}
template<typename T>
BlockStore<T>::Block::~Block() {
	delete[] this->valAry;
	this->tolSize = 0;
	this->usdSize = 0;
	this->valAry = nullptr;
}
//The check of usdSize should be done before the call of function push_back
template<typename T>
void BlockStore<T>::Block::push_back(const T& val) {
	this->valAry[usdSize] = val;
	this->usdSize++;
}
//The check of usdSize should be done before the call of function push_front
template<typename T>
void BlockStore<T>::Block::push_front(const T& val) {
	this->valAry[tolSize - 1 - usdSize] = val;
	this->usdSize++;
}
//pop_back,return bool to affirm whether the block is empty;
template<typename T>
bool BlockStore<T>::Block::pop_back() {
	this->usdSize--;
	return this->usdSize == 0;
}
//pop_front,return bool to affirm whether the block is empty;
template<typename T>
bool BlockStore<T>::Block::pop_front() {
	this->usdSize--;
	return this->usdSize == 0;
}
//delete the index=N member,return bool to affirm whether the block is empty;
template<typename T>
bool BlockStore<T>::Block::deleteN(size_type N, bool isfront) {
	if (isfront) {
		if (N >= 1)
			copy_rev(this->first(isfront) + 1, this->first(isfront), N);
	}
	else {
		copy(this->valAry + N, this->valAry + N + 1, this->usdSize - N - 1);
	}
	this->usdSize--;
	return this->usdSize == 0;
}
//copy elements from the src to des whose number is siz;
template<typename T>
void BlockStore<T>::Block::copy(pointer des, pointer src, unsigned int siz) {
	for (int i = 0; i<siz; i++)
		*(des + i) = *(src + i);
}
//serve the same function as copy,but the sequence of the process of copy is reversed;
template<typename T>
void BlockStore<T>::Block::copy_rev(pointer des, pointer src, unsigned int siz) {
	for (int i = siz - 1; i >= 0; i--)
		*(des + i) = *(src + i);
}
template<typename T>
typename BlockStore<T>::block BlockStore<T>::Block::operator=(const Block& rObj) {
	this->tolSize = rObj.tolSize;
	this->usdSize = rObj.usdSize;
	delete[] this->valAry;
	this->valAry = new value_type[this->tolSize];
	copy(this->valAry, rObj.valAry, this->usdSize);
}
template<typename T>
typename BlockStore<T>::reference BlockStore<T>::Block::getElement(unsigned int N, bool isFront) {
	if (isFront) {
		return *(this->first(isFront) + N);
	}
	else {
		return *(this->valAry + N);
	}
}

template<typename T>
BlockStore<T>::BlockStore() {
	this->setBlockOriSiz(100);
	this->setBlockIncSiz(1.1);
	this->firBlock = new BlockTable(this->initBlockSize);
	this->lstBlock = firBlock;
	this->size = 0;
}
template<typename T>
BlockStore<T>::BlockStore(unsigned int bsize) {
	this->setBlockOriSiz(bsize);
	this->setBlockIncSiz(1.1);
	this->firBlock = new BlockTable(this->initBlockSize);
	this->lstBlock = firBlock;
	this->size = 0;
}
template<typename T>
BlockStore<T>::BlockStore(const BlockStore& rObj) {
	*this = rObj;
}
template<typename T>
BlockStore<T>::BlockStore(BlockStore&& rObj) {
	this->initBlockSize = rObj.initBlockSize;
	this->incrPercent = rObj.incrPercent
		this->size = rObj.size;
	this->firBlock = rObj.firBlock;
	this->lstBlock = rObj.lstBlock;
	rObj.firBlock = nullptr;
	rObj.lstBlock = nullptr;
	rObj.size = 0;
}
template<typename T>
BlockStore<T>::~BlockStore() {
	BlockTable* index = this->firBlock;
	this->firBlock = nullptr;
	this->lstBlock = nullptr;
	while (index->nextBlock) {
		index = index->nextBlock;
		delete index->lastBlock;
	}
	delete index;
}
template<typename T>
void BlockStore<T>::push_back(const T& val) {
	//if the block is full,add another block whose size is original size*percent increased;
	//
	if (this->lstBlock->usdSize < this->lstBlock->tolSize) {
		//cout << "BlockStore<T>::push_back:" << this->size << endl;
		this->lstBlock->location->push_back(val);
		this->size++;
		this->lstBlock->usdSize++;
	}
	else {
		table* newt = new table((int)(this->lstBlock->tolSize*this->incrPercent));
		newt->lastBlock = this->lstBlock;
		this->lstBlock->nextBlock = newt;
		this->lstBlock = newt;
		this->push_back(val);
	}
}
template<typename T>
inline void BlockStore<T>::setBlockOriSiz(unsigned int size)
{
	this->initBlockSize = size;
}
template<typename T>
inline void BlockStore<T>::setBlockIncSiz(double percent)
{
	this->incrPercent = percent;
}
template<typename T>
void BlockStore<T>::push_front(const T& val) {
	//serve the same function as push_back,just reverse the direction;
	if (this->firBlock->usdSize < this->firBlock->tolSize) {
		//cout << "BlockStore<T>::push_front:" << this->size << endl;
		this->size++;
		this->firBlock->usdSize++;
		this->firBlock->location->push_front(val);
	}
	else {
		table* newt = new table((int)(this->firBlock->tolSize*this->incrPercent));
		newt->nextBlock = this->firBlock;
		this->firBlock->lastBlock = newt;
		this->firBlock = newt;
		this->push_front(val);
	}
}
template<typename T>
void BlockStore<T>::pop_back() {
	this->size--;
	if (this->lstBlock->location->pop_back()) {
		this->lstBlock = this->lstBlock->lastBlock;
		delete this->lstBlock->nextBlock;
		this->lstBlock->nextBlock = nullptr;
	}
	else
		this->lstBlock->usdSize--;
	return;
}
template<typename T>
void BlockStore<T>::pop_front() {
	this->size--;
	if (this->firBlock->location->pop_front()) {
		this->firBlock = this->firBlock->nextBlock;
		delete this->firBlock->lastBlock;
		this->firBlock->lastBlock = nullptr;
	}
	else
		this->firBlock->usdSize--;
	return;
}
template<typename T>
void BlockStore<T>::deleteN(unsigned int N) {
	//approach:
	//count from the beginning to the end until getting the number N;
	if (N >= this->size)
		assert("The input must below the size of BlockStore");
	else {
		this->size--;
		if (this->firBlock->usdSize >= N+1) {
			//cout << "========================In FirBlock====================" << endl;
			this->firBlock->usdSize--;
			if (this->firBlock->location->deleteN(N,true)) {
				this->firBlock = this->firBlock->nextBlock;
				this->firBlock->lastBlock = nullptr;
			}
		}
		else {
			//cout << "========================Out FirBlock======================" << endl;
			size_type remaining = N;
			table* index = this->firBlock;
			while (remaining>=index->usdSize) {
				remaining = remaining - index->usdSize;
				index = index->nextBlock;
			}
			index->usdSize--;
			if (index->location->deleteN(remaining, false)) {
				if (index->nextBlock) {
					index->lastBlock->nextBlock = index->nextBlock;
					index->nextBlock->lastBlock = index->lastBlock;
				}
				else 
					index->lastBlock->nextBlock = nullptr;
				delete index;
			}
		}
	}
}
template<typename T>
void BlockStore<T>::compress() {
	//from the first block to the last,take the element from the latter one and push_back the former one;
	//until the end of the block;
	table* index = this->firBlock;
	//when index has nextBlock,continue compressing;
	while (index->nextBlock) {
		//during each loop,compress until the node index is full or the nextBlock is empty;
		index = index->nextBlock;
		//if no nextBlock anymore,end compressing;
		if (!index->nextBlock)
			break;
		//the number needed to be filled;
		int fillNum = index->tolSize - index->usdSize;
		while (fillNum--) {
			T movedVal = *(index->nextBlock->location->first(index->lastBlock));
			index->location->push_back(movedVal);
			index->usdSize++;
			index->nextBlock->usdSize--;
			//if the next node is empty,delete it;
			if (index->nextBlock->location->deleteN(0,false)) {
				if (index->nextBlock->nextBlock) {
					index->nextBlock->nextBlock->lastBlock = index;
					index->nextBlock = index->nextBlock->nextBlock;
				}
				else {
					this->lstBlock = index;
					return;
				}
			}
		}
	}
	this->lstBlock = index;
}
template<typename T>
inline unsigned int BlockStore<T>::getSize()
{
	return this->size;
}
template<typename T>
void BlockStore<T>::outPutBlock() {
	table* index = this->firBlock;
	int count = 0;
	while (index->nextBlock) {
		cout << "Index:" << count++ << " Size:" << index->usdSize << "\t";
		index = index->nextBlock;
	}
	cout << "Index:" << count << " Size:" << index->usdSize << "\t";
}
template<typename T>
typename BlockStore<T>::reference BlockStore<T>::operator[](unsigned int N) {
	if (this->firBlock->usdSize >= N + 1)
		return this->firBlock->location->getElement(N, true);
	else if (N>=size) {
		assert("The input must below the size of BlockStore");
	}
	else {
		size_type remaining = N;
		table* index = this->firBlock;
		while (remaining>=index->usdSize) {
			remaining = remaining - index->usdSize;
			index = index->nextBlock;
		}
		return index->location->getElement(remaining, false);
	}
}

template<typename T>
BlockStore<T> BlockStore<T>::operator=(const BlockStore<T>& rObj) {
	this->size = rObj.size;
	this->initBlockSize = rObj.initBlockSize;
	this->incrPercent = rObj.incrPercent;
	this->first = new table(*rObj.first);
	table* index = rObj.first;
	table* copt = this->first;
	while (index->nextBlock) {
		table* newt = new table(*index);
		newt->lastBlock = copt;
		copt->nextBlock = newt;
		copt = newt;
		index = index->nextBlock;
	}
	this->lstBlock = copt;
	return *this;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::begin() {
	iterator newItr;
	newItr.ptrNow = this->firBlock->location->now(true);
	newItr.node = this->firBlock;
	return newItr;
}
template<typename T>
typename BlockStore<T>::iterator BlockStore<T>::end() {
	iterator newItr;
	newItr.ptrNow = this->lstBlock->location->now(this->lstBlock->lastBlock==nullptr);
	newItr.node = this->lstBlock;
	return newItr;
}


#endif