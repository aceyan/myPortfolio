#include"Matrix4x4.h"
#include<math.h>

Matrix4x4::Matrix4x4(float a1, float a2, float a3, float a4,
	float b1, float b2, float b3, float b4,
	float c1, float c2, float c3, float c4,
	float d1, float d2, float d3, float d4)
{
	m[0][0] = a1;  m[0][1] = a2;  m[0][2] = a3;  m[0][3] = a4;
	//
	m[1][0] = b1;  m[1][1] = b2;  m[1][2] = b3;  m[1][3] = b4;
	//
	m[2][0] = c1;  m[2][1] = c2;  m[2][2] = c3;  m[2][3] = c4;
	//
	m[3][0] = d1;  m[3][1] = d2;  m[3][2] = d3;  m[3][3] = d4;
}

Matrix4x4 operator *(Matrix4x4 lhs, Matrix4x4 rhs)
{
	Matrix4x4 nm;
	nm.SetZero();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				nm.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
			}
		}
	}
	return nm;
}


void Matrix4x4::Identity()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;  m[0][3] = 0;
	//
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;  m[1][3] = 0;
	//
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	//
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}
void Matrix4x4::SetZero()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = 0;
		}
	}
}
void Matrix4x4::Transpose()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = i; j < 4; j++)
		{

			float temp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = temp;
		}
	}
}
float Matrix4x4::Determinant()
{
	return Determinant(m, 4);
}

float Matrix4x4::Determinant(float mat[4][4], int n)
{
	if (n == 1)
	{
		return mat[0][0];
	}
	else
	{
		float result = 0;
	
		float tempM[4][4];//Using fixed size of storage space to improve performance and avoiding dynamic allocation of storage space

		for (int i = 0; i < n; i++)
		{
			//calculates the cofactors
			for (int j = 0; j < n - 1; j++)
			{
				for (int k = 0; k < n - 1; k++)//ап
				{
					int x = j + 1;
					int y = k >= i ? k + 1 : k;
					tempM[j][k] = mat[x][y];
				}
			}

			result += (float)pow((float)-1, 1 + (1 + i)) * mat[0][i] * Determinant(tempM, n - 1);
		}

		return result;
	}
}

Matrix4x4 Matrix4x4::GetAdjoint()
{
	int x, y;
	float tempM[4][4];
	Matrix4x4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				for (int t = 0; t < 3; ++t)
				{
					x = k >= i ? k + 1 : k;
					y = t >= j ? t + 1 : t;

					tempM[k][t] = m[x][ y];
				}
			}
			result.m[i][j] = (float)pow((float)-1, (1 + j) + (1 + i)) * Determinant(tempM, 3);
		}
	}
	result.Transpose();
	return result;
}
Matrix4x4 Matrix4x4::Inverse()
{
	Matrix4x4 adj;
	float a = Determinant();
	if (a == 0)
	{
		//the Matrix is irreversible
		return adj;
	}
	adj = GetAdjoint();//get Adjoint matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			adj.m[i][j] = adj.m[i][j] / a;
		}
	}
	return adj;
}

