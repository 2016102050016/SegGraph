#include "SegGraph.h"

void getPCDFile(char *txtFile, char *pcdFile);

int
main(int argc, char** argv)
{
	/*
	step1. ��txt��ʽ�ĵ�������ת����PCL�е�.pcd��ʽ
		   txt��ʽ��ȡ��KITTI���ݼ�.����KITTI�����ṩ��pythonn�⽫��ת����.txt��ʽ�ĵ�������
		   KITTI���ݼ���ַ��http://www.cvlibs.net/datasets/kitti/eval_odometry.php
	*/
	getPCDFile("scan2.txt", "scan2.pcd");
	getPCDFile("scan836.txt", "scan836.pcd");
	getPCDFile("scan414.txt", "scan414.pcd");
	/*
	step2. ������������������������ָ��㷨���зָ����ȡ�ָ��ÿһ�����ƴص�����������fea�ļ�
	*/
	segment("scan2.pcd", "fea2.txt");
	segment("scan836.pcd", "fea836.txt");
	segment("scan414.pcd", "fea414.txt");

	/*
	step3. �����ƶ�Ӧ��fea�����ļ������н�ͼ�����������ǵĽ��ƹ�����ͼ����㷨�������
	*/
	//loopDetection("fea2.txt","fea836.txt",10);
	loopDetection("fea2.txt", "fea414.txt", 10);
	return (0);
}
void getPCDFile(char *txtFile, char *pcdFile)
{
	pcl::PointCloud<pcl::PointXYZ> cloud;

	// Fill in the cloud data
	cloud.width = 999999;
	cloud.height = 1;
	cloud.is_dense = false;
	cloud.points.resize(cloud.width * cloud.height);

	char buffer[256];
	std::ifstream f(txtFile);
	if (!f.is_open())
	{
		std::cout << "Error opening file " << txtFile << " ! " << std::endl;
		exit(1);
	}
	int ncnt = 0;
	while (!f.eof())
	{
		f.getline(buffer, 100);
		//std::cout << "---------beg---------" << std::endl;
		//std::cout << buffer << std::endl;
		int len = strlen(buffer);
		for (int i = 0; i<len; i++)
			if (buffer[i] == ' ')
				buffer[i] = '#';
		char *res = NULL;
		res = strtok(buffer, "#");
		int cnt = 0;
		while (res != NULL)
		{
			double ans = atof(res);
			if (cnt == 0)
			{
				cloud.points[ncnt].x = ans;
			}

			else if (cnt == 1)
			{
				cloud.points[ncnt].y = ans;
			}

			else if (cnt == 2)
			{
				cloud.points[ncnt].z = ans;
			}

			res = strtok(NULL, "#");
			cnt++;
		}
 
		ncnt++;
	}
	cloud.width = ncnt - 1;
	cloud.points.resize(cloud.width * cloud.height);


	pcl::io::savePCDFileASCII(pcdFile, cloud);
	std::cerr << "Saved " << cloud.points.size() << " data points to " << pcdFile << "!" << std::endl;
}
