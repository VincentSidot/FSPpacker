#pragma once
#include <stdexcept>
#include <initializer_list>

namespace Matrix
{
	template<int m_row, int m_column, class T>
	class Matrix
	{
	public:
		Matrix(T defval)
		{
			data = new T*[m_row];
			for (size_t i = 0; i < m_row; ++i)
			{
				data[i] = new T[m_column];
				for (int p = 0; p < m_column; ++p)
					data[i][p] = defval;
			}
		}
		Matrix(const Matrix< m_row, m_column, T> &other)
		{
			data = new T*[m_row];
			for (size_t i = 0; i < m_row; ++i)
			{
				data[i] = new T[m_column];
				for (int p = 0; p < m_column; ++p)
					data[i][p] = other.get(i + 1, p + 1);
			}
		}
		~Matrix()
		{
			for (size_t i = 0; i < m_row; ++i)
			{
				delete data[i];
			}
			delete data;
			data = NULL;
		}
		inline int column() { return m_column; }
		inline int row() { return m_row; }
		inline T get(const int &i, int const &j) const { if (i <= m_row && j <= m_column) return data[i - 1][j - 1]; }
		inline void set(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] = value; }
		inline void add(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] += value; }
		inline void sub(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] -= value; }
		inline void mult(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] *= value; }
		inline void divide(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] /= value; }
		inline void mod(const int &i, int const &j, T const &value) { if (i <= m_row && j <= m_column) data[i - 1][j - 1] %= value; }
		inline void fill(const T* value, int const &size) {
			for (int i = 0; i < m_row; i++)
			{
				for (int j = 0; j < m_column; ++j)
				{
					if ((i*m_column + j) < size)
					{
						data[i][j] = value[i*m_column + j];
					}
				}
			}
		}
		inline void fill(const std::initializer_list<T> &liste)
		{
			size_t i = 1,j = 1;
			for (std::initializer_list<T>::iterator val(liste.begin()); val != liste.end(); ++val) 
			{
				if (i <= m_row)
				{
					this->set(i, j, *val);
				}					
				if (j >= m_column)
				{
					j = 0;
					++i;
				}
				++j;
			}
		}
		inline void disp() const {
			for (int i = 0; i < m_row; ++i)
			{
				for (int j = 0; j < m_column; ++j)
				{
					std::cout << data[i][j] << " ";
				}
				std::cout << std::endl;
			}
		}
		inline void add(const Matrix<m_row, m_column, T> &other)
		{
			for (int i = 1; i <= m_row; ++i)
			{
				for (int j = 1; j <= m_column; ++j)
				{
					data[i - 1][j - 1] += other.get(i, j);
				}
			}
		}
		inline void sub(const Matrix<m_row, m_column, T> &other)
		{
			for (int i = 1; i <= m_row; ++i)
			{
				for (int j = 1; j <= m_column; ++j)
				{
					data[i - 1][j - 1] -= other.get(i, j);
				}
			}
		}

		inline Matrix<m_row, m_column, T>& operator=(const Matrix<m_row, m_column, T> &other)
		{
			for (int i = 1; i <= m_row; ++i)
			{
				for (int j = 1; j <= m_column; ++j)
				{
					this->set(i, j, other.get(i, j));
				}
			}
			return *this;
		}
		inline T& operator() (int i, int j) { if (i <= m_row && j <= m_column) return data[i - 1][j - 1]; }
		inline const T& operator() (int i, int j) const { if (i <= m_row && j <= m_column) return data[i - 1][j - 1]; }
		inline void operator+=(Matrix<m_row, m_column, T> &other)
		{
			for (int i = 1; i <= m_row; i++)
			{
				for (int j = 1; j <= m_column; j++)
				{
					this->add(i, j, other(i, j));
				}
			}
		}
		inline void operator-=(Matrix<m_row, m_column, T> &other)
		{
			for (int i = 1; i <= m_row; i++)
			{
				for (int j = 1; j <= m_column; j++)
				{
					this->sub(i, j, other(i, j));
				}
			}
		}
		inline void operator++()
		{
			for (int i = 1; i <= m_row; i++)
			{
				for (int j = 1; j <= m_column; j++)
				{
					this->add(i, j,T(1));
				}
			}
		}
		inline void operator--()
		{
			for (int i = 1; i <= m_row; i++)
			{
				for (int j = 1; j <= m_column; j++)
				{
					this->sub(i, j, T(1));
				}
			}
		}
		inline void reverse()
		{
			Matrix<m_row, m_column, T> temp(*this);
			for (int i = 1; i <= m_row; i++)
			{
				for (int j = 1; j <= m_column; j++)
				{
					this->set(j, i, temp(i,j));
				}
			}
		}
		inline T sum() const
		{
			T rep;
			for (int i = 1; i <= m_row; ++i)
			{
				for (int j = 0; j <= m_column; ++j)
				{
					T += get(i, j);
				}
			}
		}
		
	private:
		T **data;
	};

	template<int cst, int row, int col, class T>
	inline Matrix< row, col, T> mult(const Matrix < row, cst, T> &m1, const Matrix<cst, col, T> &m2)
	{
		Matrix<row, col, T> rep(T(0));
		for (int i = 1; i <= row; ++i)
		{
			for (int j = 1; j <= col; ++j)
			{
				for (int c = 1; c <= cst; ++c)
				{
					rep.add(i, j, m1.get(i, c) * m2.get(c, j));
				}
			}
		}
		return rep;
	}
	template<int row, int col, class T>
	inline Matrix<row, col, T> operator+(Matrix<row, col,T> const &m1, Matrix<row, col, T> const &m2)
	{
		Matrix<row, col, T> rep(m1);
		rep.add(m2);
		return rep;
	}
	template<int row, int col, class T>
	inline Matrix<row, col, T> operator-(Matrix<row, col, T> const &m1, Matrix<row, col, T> const &m2)
	{
		Matrix<row, col, T> rep(m1);
		rep.sub(m2);
		return rep;
	}
	template<int cst, int row, int col, class T>
	inline Matrix< row, col, T> operator*(const Matrix < row, cst, T> &m1, const Matrix<cst, col, T> &m2)
	{
		return mult(m1, m2);
	}
	template<int n,class T>
	inline Matrix<n,n,T> identity() 
	{
		Matrix<n, n, T> temp(T(0));
		for (int i = 1; i <= n; ++i)
		{
			temp(i, i) = T(1);
		}
		return temp;
	}
	template<int n,class T>
	inline Matrix<n, n, T> SqrMatrix(T const &defvalue)
	{
		return Matrix<n, n, T>(defvalue);
	}
	template<int row,int col,class T>
	inline Matrix<row, col, T> operator*(const T &lambda, const Matrix<row, col, T> &m)
	{
		Matrix<row, col, T> rep(m);
		for (int i = 1; i <= row; i++)
		{
			for (int j = 1; j <= col; ++j)
			{
				rep.mult(i, j, lambda); // mult
			}
		}
		return rep;
	}
	template<int row,int col,class T>
	inline Matrix<row, col, T> operator*(const Matrix<row, col, T> &m, const T &lambda)
	{
		return operator*(lambda, m);
	}
	template<int row, int col, class T>
	inline Matrix<row, col, T> reverse(const Matrix<row, col, T> &other)
	{
		Matrix<row, col, T> rep(other);
		rep.reverse();
		return rep;
	}
	template<int row,int col,class T>
	inline T sum(const Matrix<row, col, T> &othr)
	{
		return othr.sum()
	}
}
