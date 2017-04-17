#include<opencv.hpp>
#include <iostream>
#include "facedetect-dll.h"
#include <stdio.h>
#include <opencv2\contrib\contrib.hpp> 
#include <opencv2\highgui\highgui.hpp>  
#include <string>
#include <io.h>
//#pragma comment(lib,"libfacedetect.lib")
#pragma comment(lib,"libfacedetect.lib")
//define the buffer size. Do not change the size!
#define DETECT_BUFFER_SIZE 0x20000

using namespace std;
using namespace cv;



string&   replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

void dir(string path , vector<string> a)
{
	string l;
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName, exdName;
	int i = 0;
	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		return;
	}
	do {
		if(i>1)
			l = fileInfo.name;
		//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
		i++;
	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);
	return;
}


int main(int argc, const char * argv[]) {

	string path = "C:\\Users\\ZBY\\Desktop\\ab";//遍历的总目录
	string path3, path1, path2;//三次遍历所需要的路径
	string a1, a2, a3;//文件名
	string path22, path33, path222;
	Mat image;
	Mat gray;
	Mat result_frontal_surveillance;
	Mat image_roi;
	int * pResults = NULL;
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if (!pBuffer)
	{
		fprintf(stderr, "Can not alloc buffer.\n");
		return -1;
	}

	int doLandmark = 1;
	char facefolder[100];
	long hFile1 = 0;
	struct _finddata_t fileInfo1;
	string pathName1, exdName1;
	int i1 = 0;
	if ((hFile1 = _findfirst(pathName1.assign(path).append("\\*").c_str(), &fileInfo1)) == -1) 
	{
		return 0;
	}
	do 
	{
		if (i1 > 1)
		{
			a1 = fileInfo1.name;
			path1 = path + "\\" + a1;
			cout << "path1:" << path1 <<endl;
			
			long hFile2 = 0;
			struct _finddata_t fileInfo2;
			string pathName2, exdName2;
			int i2 = 0;
			if ((hFile2 = _findfirst(pathName2.assign(path1).append("\\*").c_str(), &fileInfo2)) == -1)
			{
				break;
			}
			do
			{
				if (i2 > 1)
				{
					a2 = fileInfo2.name;
					path2 = path1 + "\\" + a2;
					cout << "path2:" << path2 << endl;
					path22 = "md " + path2 + "_done";
					path222= path2 + "_done";
					const char* ch = path22.c_str();
					cout << "ch:" << ch << endl;
					//system("pause");
					system(ch);//创建一个文件夹 
					long hFile3 = 0;
					struct _finddata_t fileInfo3;
					string pathName3, exdName3;
					int i3 = 0;
					if ((hFile3 = _findfirst(pathName3.assign(path2).append("\\*").c_str(), &fileInfo3)) == -1)
					{
						return 0;
					}
					do
					{
						if (i3 > 1)
						{
							a3 = fileInfo3.name;
							path3 = path2 + "\\" + a3;
							cout << "path3:" << path3 << endl;
							
							image = imread(path3, CV_LOAD_IMAGE_UNCHANGED);//导入图片
							cvtColor(image, gray, CV_BGR2GRAY);
							if (gray.empty())
							{
								fprintf(stderr, "Can not load the image file.\n");
								return -1;
							}

							///////////////////////////////////////////
							// frontal face detection designed for video surveillance / 68 landmark detection
							// it can detect faces with bad illumination.
							//////////////////////////////////////////
							//!!! The input image must be a gray one (single-channel)
							//!!! DO NOT RELEASE pResults !!!
							pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
								1.2f, 2, 48, 0, doLandmark);
							printf("%d faces detected.\n", (pResults ? *pResults : 0));
							result_frontal_surveillance = image.clone();;
							//print the detection results
							for (int q = 0; q < (pResults ? *pResults : 0); q++)
							{
								short * p = ((short*)(pResults + 1)) + 142 * q;
								int x = p[0];
								int y = p[1];
								int w = p[2];
								int h = p[3];
								int neighbors = p[4];
								int angle = p[5];

								printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
								rectangle(result_frontal_surveillance, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
								if (doLandmark)
								{
									for (int j = 0; j < 68; j++)
										circle(result_frontal_surveillance, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
								}
								w -= 3;
								h -= 3;
								Rect rect(x, y, w, h);
								image_roi = image(rect);
								//imshow("Results_frontal", image_roi);
								sprintf(facefolder, "%d.jpg", i3);
								path33 = facefolder;
								path33 = path222 + "\\" + path33;
								cout << "path33:" << path33 << endl;
								//replace_all_distinct(imgname, "/", "\\\\");
								imwrite(path33, image_roi);//riFrame为当前帧
								//system("pause");
							}
						}
						//cout << a1 << endl;
						//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
						i3++;
					} while (_findnext(hFile3, &fileInfo3) == 0);
					_findclose(hFile3);
				}
				//cout << a1 << endl;
				//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
				i2++;
			} while (_findnext(hFile2, &fileInfo2) == 0);
			_findclose(hFile2);
		}	
		//cout << a1 << endl;
		//cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;
		i1++;
	} while (_findnext(hFile1, &fileInfo1) == 0);
	_findclose(hFile1);


	system("pause");
	return 0;




	/*
	Directory dir;
	char filename[100];
	char facefolder[100];
	string fullpath;
	string path;
	string folderpath;
	string imgname;
	Mat image;
	Mat gray;
	int * pResults = NULL;
	//pBuffer is used in the detection functions.
	//If you call functions in multiple threads, please create one buffer for each thread!
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if (!pBuffer)
	{
		fprintf(stderr, "Can not alloc buffer.\n");
		return -1;
	}

	int doLandmark = 1;


	//第一次遍历文件夹
	string path1 = "C:/Users/ZBY/Desktop/2";
	string exten1 = "*";//"Image*";//"*"  
	bool addPath1 = true;//false  

	vector<string> foldernames1 = dir.GetListFolders(path1, exten1, addPath1);
	//cout << "1 tttt " << endl;
	//cout << foldernames1[0] << endl;
	for (int i1 = 0; i1 < foldernames1.size(); i1++)
	{
		//第二次遍历文件夹
		string path2 = foldernames1[i1];
		string exten2 = "*";//"Image*";//"*"  
		bool addPath2 = true;//false  

		vector<string> foldernames2 = dir.GetListFolders(path2, exten2, addPath2);
		//cout << "2 tttt " << endl;
		//cout << foldernames2[0] << endl;

		for (int i2 = 0; i2 < foldernames2.size(); i2++)
		{
			//第三次遍历文件
			string path3 = foldernames2[i2];
			string exten3 = "*.jpg";//"*"  
			bool addPath3 = false;//true;  

			vector<string> filenames3 = dir.GetListFiles(path3, exten3, addPath3);
			//cout << "3 tttt " << endl;
			//cout << filenames3[0] << endl;
			string a;
			a = foldernames2[i2];
			folderpath = a + "_done";
			replace_all_distinct(a, "/", "\\\\");
			path = "md " + a + "_done";

			cout << "path:" << path << endl;
			cout << "folderpath:" << folderpath << endl;
			const char* ch = path.c_str();
			cout << "ch:" << ch << endl;
			system(ch);//创建一个文件夹 

			for (int i3 = 0; i3 < filenames3.size(); i3++)
			{
				fullpath = foldernames2[i2] + '/' + filenames3[i3];
				//cout << fullpath << endl;
				replace_all_distinct(fullpath , "/", "\\\\");
				cout << fullpath << endl;
				//system("PAUSE");
				image = imread(fullpath, CV_LOAD_IMAGE_UNCHANGED);//导入图片
				cvtColor(image, gray, CV_BGR2GRAY);
				if (gray.empty())
				{
					fprintf(stderr, "Can not load the image file.\n");
					return -1;
				}

				///////////////////////////////////////////
				// frontal face detection designed for video surveillance / 68 landmark detection
				// it can detect faces with bad illumination.
				//////////////////////////////////////////
				//!!! The input image must be a gray one (single-channel)
				//!!! DO NOT RELEASE pResults !!!
				pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
					1.2f, 2, 48, 0, doLandmark);
				printf("%d faces detected.\n", (pResults ? *pResults : 0));
				Mat result_frontal_surveillance = image.clone();;
				//print the detection results
				for (int i = 0; i < (pResults ? *pResults : 0); i++)
				{
					short * p = ((short*)(pResults + 1)) + 142 * i;
					int x = p[0];
					int y = p[1];
					int w = p[2];
					int h = p[3];
					int neighbors = p[4];
					int angle = p[5];

					printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
					rectangle(result_frontal_surveillance, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
					if (doLandmark)
					{
						for (int j = 0; j < 68; j++)
							circle(result_frontal_surveillance, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
					}

					Rect rect(x, y, w, h);
					Mat image_roi = image(rect);
					//imshow("Results_frontal", image_roi);
					sprintf(facefolder, "%d_%d.jpg", i3, i);
					imgname = facefolder;
					imgname = folderpath + '/' + imgname;
					replace_all_distinct(imgname, "/", "\\\\");
					imwrite(imgname, image_roi);//riFrame为当前帧
					cout << "imgname:" << imgname << endl;
					//system("pause");
				}
			}
			//system("pause");
		}
	}
	return 0;
	*/
}