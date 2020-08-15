
#include <iostream>
#include <cstdlib>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>


using namespace std;
using namespace Eigen;

int main(int argc, char **argv)
{
	int mag_data_counter = 1000;
	FILE * fp;
	double mag_x, mag_y, mag_z;

	MatrixXd mat_D(mag_data_counter, 9);
	MatrixXd mat_DT;
	fp = fopen("magdata", "r");

	for (int i = 0; i < mag_data_counter; i++)
	{

		fscanf(fp, "%lf %lf %lf\n", &mag_x, &mag_y, &mag_z);
		mat_D(i, 0) = mag_x * mag_x;
		mat_D(i, 1) = mag_y * mag_y;
		mat_D(i, 2) = mag_z * mag_z;
		mat_D(i, 3) = 2 * mag_x * mag_y;
		mat_D(i, 4) = 2 * mag_x * mag_z;
		mat_D(i, 5) = 2 * mag_y * mag_z;
		mat_D(i, 6) = 2 * mag_x;
		mat_D(i, 7) = 2 * mag_y;
		mat_D(i, 8) = 2 * mag_z;
	}
	fclose(fp);
	mat_DT = mat_D.transpose();

	MatrixXd mat_Ones = MatrixXd::Ones(mag_data_counter, 1);

	MatrixXd mat_Result =  (mat_DT * mat_D).inverse() * (mat_DT * mat_Ones);

	Matrix<double, 4, 4>  mat_A_4x4;

	mat_A_4x4(0, 0) = mat_Result(0, 0);
	mat_A_4x4(0, 1) = mat_Result(3, 0);
	mat_A_4x4(0, 2) = mat_Result(4, 0);
	mat_A_4x4(0, 3) = mat_Result(6, 0);

	mat_A_4x4(1, 0) = mat_Result(3, 0);
	mat_A_4x4(1, 1) = mat_Result(1, 0);
	mat_A_4x4(1, 2) = mat_Result(5, 0);
	mat_A_4x4(1, 3) = mat_Result(7, 0);

	mat_A_4x4(2, 0) = mat_Result(4, 0);
	mat_A_4x4(2, 1) = mat_Result(5, 0);
	mat_A_4x4(2, 2) = mat_Result(2, 0);
	mat_A_4x4(2, 3) = mat_Result(8, 0);

	mat_A_4x4(3, 0) = mat_Result(6, 0);
	mat_A_4x4(3, 1) = mat_Result(7, 0);
	mat_A_4x4(3, 2) = mat_Result(8, 0);
	mat_A_4x4(3, 3) = -1.0;


	MatrixXd mat_Center = -((mat_A_4x4.block(0, 0, 3, 3)).inverse() * mat_Result.block(6, 0, 3, 1));

	Matrix<double, 4, 4>  mat_T_4x4;
	mat_T_4x4.setIdentity();
	mat_T_4x4.block(3, 0, 1, 3) = mat_Center.transpose();

	MatrixXd mat_R = mat_T_4x4 * mat_A_4x4 * mat_T_4x4.transpose();

	EigenSolver<MatrixXd> eig(mat_R.block(0, 0, 3, 3) / -mat_R(3, 3));
	//mat_T_4x4(3,0)=mat_Center()
	MatrixXd mat_Eigval(3, 1) ;
	MatrixXd mat_Evecs(3, 3) ;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat_Evecs(i, j) = (eig.eigenvectors())(i, j).real();
		}
	}
	mat_Eigval(0, 0) = (eig.eigenvalues())(0, 0).real();
	mat_Eigval(1, 0) = (eig.eigenvalues())(1, 0).real();
	mat_Eigval(2, 0) = (eig.eigenvalues())(2, 0).real();
	MatrixXd mat_Radii = (1.0 / mat_Eigval.array()).cwiseSqrt();
	MatrixXd mat_Scale = MatrixXd::Identity(3, 3) ;
	mat_Scale(0, 0) = mat_Radii(0, 0);
	mat_Scale(1, 1) = mat_Radii(1, 0);
	mat_Scale(2, 2) = mat_Radii(2, 0);
	double min_Radii = mat_Radii.minCoeff();

	mat_Scale = mat_Scale.inverse().array() * min_Radii;
	MatrixXd mat_Correct = mat_Evecs * mat_Scale * mat_Evecs.transpose();


	cout << "The Ellipsoid center is:" << endl << mat_Center << endl;
	cout << "The Ellipsoid radii is:" << endl << mat_Radii << endl;
	cout << "The scale matrix  is:" << endl << mat_Scale << endl;
	cout << "The correct matrix  is:" << endl << mat_Correct << endl;

}