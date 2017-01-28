#ifndef HEADER_MATRIX
#define HEADER_MATRIX
//------------------------------------------------------------------------
#include<iostream>
#include<vector>

using namespace std;

template<typename T>
class Matrix {
private:
	unsigned int row, col;
	T *pData = nullptr;
public:
	//构造函数，输入行列数r,c;
	Matrix(int r = 0, int c = 0) :row(r), col(c) {
		this->pData = new T[r*c];
	}
	//拷贝构造函数
	Matrix(const Matrix& copy) {
		this->pData = new T[copy.row*copy.col];
		this->row = copy.row;
		this->col = copy.col;
		for (unsigned int i = 0; i < copy.row*copy.col; i++) {
			*(this->pData + i) = *(copy.pData + i);
		}
	}
	//析构函数
	~Matrix() {
		delete[] this->pData;
		this->row = 0;
		this->col = 0;
		this->pData = nullptr;
	}
	//重载运算符'='
	Matrix& operator=(Matrix& rightMat) {
		delete[] this->pData;
		this->pData = new T[rightMat.row*rightMat.col];
		this->row = rightMat.row;
		this->col = rightMat.col;
		for (unsigned int i = 0; i < rightMat.row*rightMat.col; i++) {
			*(this->pData + i) = *(rightMat.pData + i);
		}
		return *this;
	}
	//重载运算符'[]'
	T* const operator[](unsigned int index)const {
		return (this->pData + index*this->col);
	}
	//重载运算符'+'
	Matrix operator+(const Matrix<T>& rightMat)const {
		if (this->row != rightMat.row || this->col != rightMat.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<T> res(this->row, this->col);
			for (unsigned int i = 0; i < this->row*this->col; i++) {
				*(res.pData + i) = *(this->pData + i) + *(rightMat.pData + i);
			}
			return res;
		}
	}
	//重载运算符'+='
	Matrix& operator+=(const Matrix<T>& rightMat) {
		if (this->row != rightMat.row || this->col != rightMat.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			for (unsigned int i = 0; i < this->row*this->col; i++) {
				*(this->pData + i) = *(this->pData + i) + *(rightMat.pData + i);
			}
			return *this;
		}
	}
	//重载运算符'-'
	Matrix operator-(const Matrix& rightMat)const {
		if (this->row != rightMat.row || this->col != rightMat.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<T> res(*this);
			for (unsigned int i = 0; i < this->row*this->col; i++) {
				*(res.pData + i) = *(this->pData + i) - *(rightMat.pData + i);
			}
			return res;
		}
	}
	//重载运算符'-='
	Matrix& operator-=(const Matrix& rightMat) {
		if (this->row != rightMat.row || this->col != rightMat.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			for (unsigned int i = 0; i < this->row*this->col; i++) {
				*(this->pData + i) = *(this->pData + i) - *(rightMat.pData + i);
			}
			return *this;
		}
	}
	//重载运算符'*'
	Matrix operator*(const Matrix& rightMat)const {
		if (this->col != rightMat.row) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<T> res(this->row, rightMat.col);
			for (unsigned int j = 0; j < rightMat.col; j++) {
				for (unsigned int i = 0; i < this->row; i++) {
					T temp = 0;
					for (unsigned int k = 0; k < this->col; k++) {
						temp += (*this)[i][k] * rightMat[k][j];
					}
					res[i][j] = temp;
				}
			}
			return res;
		}
	}
	//重载运算符'*='
	Matrix operator*=(const Matrix& rightMat) {
		*this = (*this)*rightMat;
		return *this;
	}
	//重载运算符<
	Matrix operator<(Matrix& m) {
		if (this->row != m.row || this->col != m.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<int> res(this->row, this->col);
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					res[i][j] = ((*this)[i][j] < m[i][j]);
				}
			}
			return res;
		}
	}
	//重载运算符>
	Matrix operator>(Matrix& m) {
		if (this->row != m.row || this->col != m.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<int> res(this->row, this->col);
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					res[i][j] = ((*this)[i][j] > m[i][j]);
				}
			}
			return res;
		}
	}
	//重载运算符==
	Matrix operator==(Matrix& m) {
		if (this->row != m.row || this->col != m.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<int> res(this->row, this->col);
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					res[i][j] = ((*this)[i][j] == m[i][j]);
				}
			}
			return res;
		}
	}
	//重载运算符!=
	Matrix operator!=(Matrix& m) {
		if (this->row != m.row || this->col != m.col) {
			throw(invalid_argument("Input Error!The row and col of two Matrixs must be equal!"));
		}
		else {
			Matrix<int> res(this->row, this->col);
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					res[i][j] = ((*this)[i][j] != m[i][j]);
				}
			}
			return res;
		}
	}
	//矩阵的幂次
	Matrix& pow(unsigned int power = 1) {
		for (unsigned int i = 1; i < power; i++) {
			(*this) = (*this)*(*this);
		}
		return *this;
	}
	//矩阵形状转换
	void resize(unsigned int sizeRow, unsigned int sizeCol) {
		if (sizeRow*sizeCol != this->row*this->col) {
			throw(invalid_argument("Input Error!Params row*col must be equal!"));
		}
		this->row = sizeRow;
		this->col = sizeCol;
	}
	//行或列交换
	void swaprows(unsigned int row1, unsigned int row2) {
		for (unsigned int i = 0; i < this->col; i++) {
			T temp;
			temp = (*this)[row1][i];
			(*this)[row1][i] = (*this)[row2][i];
			(*this)[row2][i] = temp;
		}
	}
	void swapcols(unsigned int col1, unsigned int col2) {
		for (unsigned int i = 0; i < this->row; i++) {
			T temp;
			temp = (*this)[i][col1];
			(*this)[i][col1] = (*this)[i][col2];
			(*this)[i][col2] = temp;
		}
	}
	//获得矩阵的大小
	unsigned int* size() {
		unsigned int res[2] = { this->row,this->col };
		return res;
	}
	//矩阵的输出
	void print() {
		for (unsigned int i = 0; i < this->row; i++) {
			for (unsigned int j = 0; j < this->col; j++) {
				cout << (*this)[i][j] << " ";
			}
			cout << endl;
		}
	}
	//矩阵转置函数
	void transpose() {
		Matrix<T> temp(*this);
		this->row = temp.col;
		this->col = temp.row;
		for (unsigned int i = 0; i < temp.row; i++) {
			for (unsigned int j = 0; j < temp.col; j++) {
				(*this)[j][i] = temp[i][j];
			}
		}
	}
	//返回矩阵对角元素
	vector<T> diag() {
		if (this->row != this->col) {
			throw(invalid_argument("The Matrix Is Not Square!"));
		}
		else {
			vector<T> res;
			for (unsigned int i = 0; i < this->row; i++) {
				res.push_back((*this)[i][i]);
			}
			return res;
		}
	}
	//矩阵范数
	T norm() {
		T temp = 0;
		for (unsigned int i = 0; i < this->row; i++) {
			for (unsigned int j = 0; j < this->col; j++) {
				temp += (*this)[i][j] * (*this)[i][j];
			}
		}
		return (T)sqrt(temp);
	}
	//矩阵最大元素
	T max() {
		T temp = *(this->pData);
		for (unsigned int i = 0; i < this->row; i++) {
			for (unsigned int j = 0; j < this->col; j++) {
				if ((*this)[i][j] > temp)
					temp = (*this)[i][j];
			}
		}
		return temp;
	}
	//最大元素的坐标
	unsigned int* max_index() {
		T temp = *(this->pData);
		unsigned int r = 0, c = 0;
		for (unsigned int i = 0; i < this->row; i++) {
			for (unsigned int j = 0; j < this->col; j++) {
				if ((*this)[i][j] > temp) {
					temp = (*this)[i][j];
					r = i; c = j;
				}
			}
		}
		unsigned int res[2] = { r,c };
		return res;
	}
	//矩阵求逆,考虑使用Gauss_Jordan法
	Matrix inv() {
		if (this->row != this->col) {
			throw(invalid_argument("The Matrix Must Be the Square Matrix!"));
		}
		else if (this->row == 1) {
			Matrix revMat(1, 1);
			revMat[0][0] = T(0) / *(this->pData);
			return revMat;
		}
		else {
			Matrix<T> copy(this->row, this->col * 2);
			//初始化矩阵
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					copy[i][j] = (*this)[i][j];
					copy[i][j + this->col] = T(0);
				}
				copy[i][i + this->col] = T(1.0);
			}
			//转换为上三角矩阵
			for (unsigned int i = 0; i < copy.row - 1; i++) {
				if (copy[i][i] == T(0)) {
					unsigned int j;
					for (j = 0; j < copy.row; j++) {
						if (copy[j][i] != T(0))
							break;
					}
					if (j == this->row)
						continue;
					copy.swaprows(i, j);
				}
				for (unsigned int j = i + 1; j < copy.row; j++) {
					T temp = copy[j][i];
					for (unsigned int k = i; k < copy.col; k++) {
						copy[j][k] -= (copy[i][k] * temp) / copy[i][i];
					}
				}
			}
			//cout << "上三角矩阵："<<endl<<copy << endl;
			//转换为单位矩阵
			for (unsigned int i = 0; i < copy.row; i++) {
				if (copy[i][i] == T(0)) {
					throw(invalid_argument("Input Error!Only Matrix Whose Rank Is Full Can Have Inversed Matrix!"));
				}
				else {
					T temp = copy[i][i];
					for (unsigned int j = 0; j < copy.col; j++) {
						copy[i][j] /= temp;
					}
				}
			}
			for (unsigned int i = copy.row - 1; i >= 1; i--) {
				for (unsigned int j = i + 1; j < copy.col / 2; j++) {
					copy[i][j] = 0;
					for (unsigned int k = copy.col / 2; k < copy.col; k++) {
						copy[i][k] -= copy[j][k] * copy[i][j];
					}
				}
			}
			for (unsigned int j = 1; j < copy.col / 2; j++) {
				for (unsigned int k = copy.col / 2; k < copy.col; k++) {
					copy[0][k] -= copy[j][k] * copy[0][j];
				}
				copy[0][j] = 0;
			}
			//cout << "单位矩阵：" << endl << copy << endl;
			Matrix<T> ans(this->row, this->col);
			for (unsigned int i = 0; i < this->row; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					ans[i][j] = copy[i][j + this->col];
				}
			}
			return ans;
		}
	}
	//矩阵行列式，方法是转换行列式为上三角行列式，然后对对角线上元素求积
	T det() const {
		if (this->row != this->col) {
			throw(invalid_argument("The Matrix Must Be the Square Matrix!"));
		}
		else if (this->row == 1) {
			return *(this->pData);
		}
		else {
			Matrix<T> copy(*this);
			for (unsigned int i = 0; i < copy.row - 1; i++) {
				if (copy[i][i] == T(0)) {
					unsigned int j;
					for (j = 0; j < copy.row; j++) {
						if (copy[j][i] != T(0))
							break;
					}
					if (j == this->row)
						continue;
					copy.swaprows(i, j);
				}
				for (unsigned int j = i + 1; j < copy.row; j++) {
					T temp = copy[j][i];
					for (unsigned int k = i; k <copy.row; k++) {
						copy[j][k] -= (copy[i][k] * temp) / copy[i][i];
					}
				}
			}
			T res = T(1);
			cout << copy << endl;
			for (unsigned int i = 0; i < this->row; i++) {
				res *= copy[i][i];
			}
			return res;
		}
	}

	int getRow() {
		return this->row;
	}
	int getCol() {
		return this->col;
	}
	//重载运算符<<
	friend ostream& operator<<(ostream& os, const Matrix<T> &m) {
		for (unsigned int i = 0; i < m.row; i++) {
			for (unsigned int j = 0; j < m.col; j++) {
				os << m[i][j] << " ";
			}
			os << endl;
		}
		return os;
	}
	//重载运算符>>
	friend istream& operator >> (istream& is, Matrix<T> &m) {
		for (unsigned int i = 0; i < m.row; i++) {
			for (unsigned int j = 0; j < m.col; j++) {
				in >> m[i][j];
			}
		}
		return is;
	}
};


//------------------------------------------------------------------------
#endif