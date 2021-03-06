// 1612272_BT02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

Mat DoConvolution(Mat &image, float kernel[], int size)
{
	// Lam cho anh trang den
	int half_size = size / 2; //Lay nua kich thuoc cua size
	
	// Lay dong va cot
	int row = image.rows;
	int col = image.cols;

	// Tao anh output
	Mat output(row, col, CV_8UC1);

	// Lay step cua anh output va anh image
	int image_Step = image.step[0];

	// Tao ma tran offset
	vector<int> offset;
	for (int i = -half_size; i <= half_size; i++)
	{
		for (int j = -half_size; j <= half_size; j++)
		{
			offset.push_back(image_Step*i + j);
		}
	}

	

	// Lay dia chi dong cua anh output va anh image
	uchar* pOutput = output.data;
	uchar* pImage = image.data;

	// Moi vong lap tang dia chi dong len output_Step va image_Step
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pImage++, pOutput++)
		{
			if (i < half_size || i >= row - half_size || j < half_size || j >= col - half_size)
			{
				pOutput[0] = 0;
				continue;
			}
			// Khoi tao bien tinh tong 
			float sum = 0;

			// Tinh dao ham
			for (int x = -half_size; x <= half_size; x++)
			{
				for (int y = -half_size; y <= half_size; y++)
				{
					int index = (x + half_size)*size + (y + half_size); // Vi tri trong mang kernel
					sum += pImage[offset[index]] * kernel[index];
				}
			}

			// Neu tong am thi ta gan bang 0
			if (sum < 0)
			{
				sum = 0;
			}
			pOutput[0] = (int)sum;
		}
				
	}
	return output;
}

int detectBySobel(Mat src, Mat dst)
{
	// Ma tran theo phuong x va phuong y
	float kernel_x[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float kernel_y[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	// Dao ham theo 2 huong
	Mat image_x = DoConvolution(src, kernel_x, 3);
	Mat image_y = DoConvolution(src, kernel_y, 3);
	
	// Xuat anh dao ham
	imshow("Dao ham theo x", image_x);
	imshow("Dao ham theo y", image_y);

	// Tinh sqrt ( image_x^2 + image_y^2)
	// Lay dong va cot
	int row = src.rows;
	int col = src.cols;

	// Lay dia chi anh dst, image_x, image_y
	uchar* pDst = dst.data;
	uchar* pImage_x = image_x.data;
	uchar* pImage_y = image_y.data;

	// Gan vao anh dst
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++,pDst++, pImage_x++, pImage_y++)
		{
			pDst[0] = (uchar)(sqrt(pImage_x[0] * pImage_x[0] + pImage_y[0] * pImage_y[0])/4);
		}
	}
	imshow("Destination", dst);
	waitKey(0);
	return 1;
}
int detectByPrewitt(Mat src, Mat dst)
{
	// Ma tran theo phuong x va phuong y
	float kernel_x[] = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };
	float kernel_y[] = { -1,-1,-1,0,0,0,1,1,1 };
	

	// Dao ham theo 2 huong
	Mat image_x = DoConvolution(src, kernel_x, 3);
	Mat image_y = DoConvolution(src, kernel_y, 3);

	// Xuat anh dao ham
	imshow("Dao ham theo x", image_x);
	imshow("Dao ham theo y", image_y);

	// Tinh sqrt ( image_x^2 + image_y^2)
	// Lay dong va cot
	int row = src.rows;
	int col = src.cols;

	// Lay dia chi anh dst, image_x, image_y
	uchar* pDst = dst.data;
	uchar* pImage_x = image_x.data;
	uchar* pImage_y = image_y.data;

	// Gan vao anh dst
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pDst++, pImage_x++, pImage_y++)
		{
			pDst[0] = (uchar)(sqrt(pImage_x[0] * pImage_x[0] + pImage_y[0] * pImage_y[0])/3);
		}
	}
	imshow("Destination", dst);
	waitKey(0);
	return 1;
}
int detectByLaplace(Mat src, Mat dst)
{
	// Tao kernel
	vector<float> kernel;
	int size = 3 / 2;
	
	kernel = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	// Lay step cua anh goc
	int step = src.step[0];

	// Tao offset
	vector<int> offset;
	for (int i = -size; i <= size; i++)
	{
		for (int j = -size; j <= size; j++)
		{
			offset.push_back(i*step + j);
		}
	}

	// Convolution
	int row = src.rows;
	int col = src.cols;

	uchar* pSrc = src.data;	
	uchar* pDst = dst.data;
	
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pSrc++, pDst++)
		{
			if (i < size || j < size || i >= row - size || j >= col - size)
			{
				pDst[0] = 0;
			}
			else
			{
				float sum = 0;
				for (int x = -size; x <= size; x++)
				{
					for (int y = -size; y <= size; y++)
					{
						int index = (x + size)*3 + (y + size);
						sum += pSrc[offset[index]] * kernel[index];
					}
				}
				if (sum > 0)
					pDst[0] = 255;
				else 
					pDst[0] = 0;				
			}
		}
	}
	imshow("Laplace", dst);
	waitKey(0);
	return 1;
}
int Hysterisis(uchar* pSource, vector<int> kernel, int &lowThreshold, int &highThreshold, int &row, int &col, int i, int j)
{
	if (i < 1 || j < 1 || i >= row - 1 || j >= col - 1)
		return 1;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			// Vi tri trong mang kernel
			int index = (x + 1) * 3 + (y + 1);
			if (pSource[kernel[index]] > lowThreshold && pSource[kernel[index]] < highThreshold)
			{
				pSource[kernel[index]] = 255;
				Hysterisis(pSource + kernel[index], kernel, lowThreshold, highThreshold, row, col, i + x, j + y);
			}
		}
	}
	return 1;
}
int detectByCanny(Mat sourceImage, Mat destinationImage,int lowThreshold,int highThreshold, bool blur = false,float sigma = 0,int ksize = 3)
{
	// Ma tran theo phuong x va phuong y
	float kernel_x[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	float kernel_y[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	// Lay dong va cot
	int row = sourceImage.rows;
	int col = sourceImage.cols;

	// Lay dia chi sourceImage
	uchar* pSource = sourceImage.data;

	// Lay step cua sourceImage
	int image_Step = sourceImage.step[0];

	// Tao ma tran offset va ma tran Gauss
	if (blur == true)
	{
		vector<int> offset;
		vector<float> Gauss;
		int size = ksize / 2;
		float sumGauss = 0;
		for (int i = -size; i <= size; i++)
		{
			for (int j = -size; j <= size; j++)
			{
				offset.push_back(image_Step*i + j);
				float coeff1 = 1 / (sigma*sqrtf(2 * 3.14));
				float coeff2 = -(i*i + j * j) / (2 * sigma*sigma);
				float result = coeff1 * powf(2.71, coeff2);
				Gauss.push_back(result);
				sumGauss += result;
			}
		}

		// Loc anh
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++, pSource++)
			{
				if (i < size || i >= row - size || j < size || j >= col - size)
				{
					continue;
				}
				// Khoi tao bien tinh tong
				float sum = 0;

				// Tinh dao ham
				for (int x = -size; x <= size; x++)
				{
					for (int y = -size; y <= size; y++)
					{
						int index = (x + size) * ksize + (y + size); // Vi tri trong mang Gauss
						sum += pSource[offset[index]] * Gauss[index];
					}
				}
				pSource[0] = (int)(sum / sumGauss);
			}
		}
		offset.clear();
		Gauss.clear();
	}
	
	
	// Tao ma tran offset khac
	vector<int> offset;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			offset.push_back(image_Step*i + j);
		}
	}

	// Anh dao ham
	Mat derivationImage(row, col, CV_8UC1);

	// Anh Non-maximum suppression
	Mat angleImage(row, col, CV_8UC1);

	// Lay dia chi cua derivationImage, angleImage, sourceImage
	uchar* pDerivationImage = derivationImage.data;
	uchar* pAngleImage = angleImage.data;
	pSource = sourceImage.data;

	// Tim anh derivation va anh nonmaxima
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pDerivationImage++, pAngleImage++, pSource++)
		{
			if (i < 1 || i >= row - 1 || j < 1 || j >= col - 1)
			{
				pDerivationImage[0] = 0;
				continue;
			}
			// Khoi tao bien tinh tong dao ham x va y
			float sum_x = 0;
			float sum_y = 0;

			// Tinh dao ham
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					int index = (x + 1)*3 + (y + 1); // Vi tri trong mang kernel
					sum_x += pSource[offset[index]] * kernel_x[index];
					sum_y += pSource[offset[index]] * kernel_y[index];
				}
			}

			// Gan vao anh dao ham
			float sum = sqrtf(powf(sum_x, 2) + powf(sum_y, 2)) / 4;
			pDerivationImage[0] = (int)sum;
			
			// Tim anh nomaxima
			float angle = 0;

			// Neu dao ham y bang 0 thi goc la 90
			if (sum_x == 0)
				angle = 90;
			else
			{
				// Tinh atan
				angle = atanf(sum_y / sum_x) * 180 / 3.14;

				// Chia truong hop
				if ((angle >= -22.5 && angle < 22.5) || (angle >= -157.5 && angle < 157.5))
					angle = 0;
				else if ((angle >= 22.5 && angle < 67.5) || (angle >= -157.5 && angle < -112.5))
					angle = 45;
				else if ((angle >= 67.5 && angle < 112.5) || (angle >= -112.5 && angle < -67.5))
					angle = 90;
				else
					angle = 135;
			}
			pAngleImage[0] = angle;
		}
	}

	// Show anh dao ham
	imshow("Derivation original", derivationImage);

	// Non-maxima Suppression
	pDerivationImage = derivationImage.data;
	pAngleImage = angleImage.data;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pDerivationImage++, pAngleImage++)
		{
			// Bo cac dong va cot khong can thiet
			if (i < 1 || i >= row - 1 || j < 1 || j >= col - 1)
			{
				pDerivationImage[0] = 0;
				continue;
			}
			// Tinh pixel lon nhat theo goc su dung lan can 8
			if (pAngleImage[0] == 0)
			{
				// Pixel ben trai va ben phai
				if ((pDerivationImage[0] < *(pDerivationImage - 1)) || (pDerivationImage[0] < *(pDerivationImage + 1)))
					pDerivationImage[0] = 0;					
			}
			else if (pAngleImage[0] == 45)
			{
				// Pixel goc phai tren va goc trai duoi
				if ((pDerivationImage[0] < *(pDerivationImage - image_Step + 1)) || (pDerivationImage[0] < *(pDerivationImage + image_Step - 1)))
					pDerivationImage[0] = 0;
			}
			else if (pAngleImage[0] == 90)
			{
				// Pixel ben tren va ben duoi
				if ((pDerivationImage[0] < *(pDerivationImage - image_Step)) || (pDerivationImage[0] < *(pDerivationImage + image_Step)))
					pDerivationImage[0] = 0;
			}
			else
			{
				// Pixel goc phai duoi va goc trai tren
				if ((pDerivationImage[0] < *(pDerivationImage + image_Step + 1)) || (pDerivationImage[0] < *(pDerivationImage - image_Step - 1)))
					pDerivationImage[0] = 0;
			}
		}
	}

	// Show anh dao ham
	imshow("Derivation suppression", derivationImage);

	// Hysterisis
	pDerivationImage = derivationImage.data;

	bool edgeExist = true; // Bien xac dinh trong anh con edge point hay khong
	bool firstHysterisis = true; // Duyet qua lan dau
	while (edgeExist)
	{
		edgeExist = false;
		pDerivationImage = derivationImage.data;
		for (int i = 1; i < row - 1; i++)
		{
			for (int j = 1; j < col - 1; j++, pDerivationImage++)
			{				
				if (firstHysterisis == true) 
				{
					// Thap hon nguong thap thi gan bang 0
					if (pDerivationImage[0] < lowThreshold)
					{
						pDerivationImage[0] = 0;
						
					}
					// Tren nguong cao thi gan bang 255
					else if (pDerivationImage[0] >= highThreshold)
					{
						pDerivationImage[0] = 255;
						
					}
					edgeExist = true;
				}
				else 
				{
					// Nam giua nguong thap va cao
					if(pDerivationImage[0] == 255)
					{					
						//// Xet lan can 8
						//for (int x = -1; x <= 1; x++)
						//{
						//	for (int y = -1; y <= 1; y++)
						//	{
						//		// Vi tri trong mang kernel
						//		int index = (x + 1) * 3 + (y + 1);
						//		if (pDerivationImage[offset[index]] > lowThreshold && pDerivationImage[offset[index]] < highThreshold)
						//		{
						//			pDerivationImage[offset[index]] = 255;
						//			edgeExist = true; // Chay hysterisis lan nua
						//		}
						//	}
						//}

						Hysterisis(pDerivationImage, offset, lowThreshold, highThreshold, row, col, i, j);
					}
				}
			}
		}
		firstHysterisis = false;
	}
	
	pDerivationImage = derivationImage.data;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pDerivationImage++)
		{
			if (pDerivationImage[0] != 0 && pDerivationImage[0] != 255)
				pDerivationImage[0] = 0;
		}
	}

	// Show anh final
	imshow("Final image", derivationImage);
	waitKey(0);
	return 1;
}

int main(int argc, char** argv)
{
	if (argc == 3) {
		Mat image;
		image = imread(argv[1], CV_8UC1);
		if (!image.data)
		{
			cout << "Khong the mo anh" << std::endl;
			return -1;
		}
		if (strcmp(argv[2], "Sobel") == 0) {
			Mat dst(image.rows, image.cols, CV_8UC1);
			detectBySobel(image, dst);
		}
		else if(strcmp(argv[2], "Prewitt") == 0) {
			Mat dst(image.rows, image.cols, CV_8UC1);
			detectByPrewitt(image, dst);
		}
		if (strcmp(argv[2], "Laplace") == 0)
		{
			Mat dst(image.rows, image.cols, CV_8UC1);
			detectByLaplace(image, dst);
		}
		else
		{
			cout << "Ma lenh sai" << endl;
			return -1;
		}
	}
	else if (argc == 8)
	{
		Mat image;
		image = imread(argv[1], CV_8UC1);
		if (!image.data)
		{
			cout << "Khong the mo anh" << std::endl;
			return -1;
		}
		
		if (strcmp(argv[2], "Canny") == 0) {
			Mat dst(image.rows, image.cols, CV_8UC1);
			int low = atoi(argv[3]);
			int high = atoi(argv[4]);
			bool blur = atoi(argv[5]);
			float sigma = atof(argv[6]);
			int ksize = atoi(argv[7]);
			cout << "Dang chay ... " << endl;
			detectByCanny(image, dst, low, high, blur, sigma, ksize);
		}
		else
		{
			cout << "Ma lenh sai" << endl;
			return -1;
		}
	}
	else {
		cout << "Thieu tham so" << endl;
		return -1;
	}
	return 0;
}