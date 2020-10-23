

#include "pch.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cmath>

using namespace std;


typedef enum _type //枚举mode类型
{
	SimpleDualPort,
	ROM,
	SinglePort,
	TrueDualPort,
}type;
typedef struct _item//五种数据种类
{
	int Circuit;
	int RamID;
	type Mode;
	int Depth;
	int Width;
}item;
int to_int(string str)//以int型式返回数据 
{
	int re;
	stringstream ss(str);
	ss >> re;
	return re;
}
void show(item one)//输出函数
{
	cout << "Circuit:";
	cout << one.Circuit << " ";
	cout << "RamID:";
	cout << one.RamID << " ";
	cout << "Mode:";
	cout << one.Mode << " ";
	switch (one.Mode)
	{
	case SimpleDualPort:
		cout << "SimpleDualPort";
		break;
	case ROM:
		cout << "ROM";
		break;
	case SinglePort:
		cout << "SinglePort";
		break;
	case TrueDualPort:
		cout << "TrueDualPort";
		break;
	}
	cout << " Depth:";
	cout << one.Depth << " ";
	cout << "Width:";
	cout << one.Width << " ";
	cout << endl;

}
//
int main()
{
	//logic_rams
	fstream file;
	file.open("logical_rams.txt");
	stringstream sstream;
	sstream << file.rdbuf();
	string str = sstream.str();
	/*数据预处理*/
	regex eA("SimpleDualPort");
	regex eB("ROM");
	regex eC("SinglePort");
	regex eD("TrueDualPort");
	string new_file = regex_replace(str, eA, "A");
	new_file = regex_replace(new_file, eB, "B");
	new_file = regex_replace(new_file, eC, "C");
	new_file = regex_replace(new_file, eD, "D");
	/*数据解析*/
	//cout << "logical_rams\r\n";
	vector<item> list;
	stringstream temp;
	item one;
	smatch m;
	regex e_item("(\\d+)\\s+(\\d+)\\s+(\\w+)\\s+(\\d+)\\s+(\\d+)");//正则表达式
	while (regex_search(new_file, m, e_item)) //写入数据
	{
		one.Circuit = to_int(m[1].str());
		one.RamID = to_int(m[2].str());
		if (m[3].str() == "A")
			one.Mode = SimpleDualPort;
		else if (m[3].str() == "B")
			one.Mode = ROM;
		else if (m[3].str() == "C")
			one.Mode = SinglePort;
		else if (m[3].str() == "D")
			one.Mode = TrueDualPort;
		one.Depth = to_int(m[4].str());
		one.Width = to_int(m[5].str());

		list.push_back(one);
		//cout << ">";
		//show(one);
		new_file = m.suffix().str();
	}
	//cout << "run end first";
	/*看看结果
	for (item a : list)
	{
		show(a);
	}
	for (unsigned int i = 0; i < list.size(); i++) {
		std::cout << "Circuit ID :" << list[i].Circuit << " " << list[i].RamID << " " << list[i].Mode<<  std::endl;

	}*/


	//logic_block_count
	//cout << "logic_block_count\r\n;";
	vector<int>logic_block_count_odd;//指针
	vector<int>logic_block_count_even;
	//cout << logic_block_count.size() << endl;
	ifstream infile;
	infile.open("logic_block_count.txt");
	int count1 = 0;
	int even_odd1 = 0;
	while (!infile.eof()) {
		int n;
		count1 = count1 + 1;
		if (count1 == 1) {
			string c;
			getline(infile, c);
		}
		else {
			if (even_odd1 == 0) {
				infile >> n;
				logic_block_count_even.push_back(n);
				even_odd1 = 1;
				//cout << even_odd << endl;
			}
			else {
				infile >> n;
				logic_block_count_odd.push_back(n);
				even_odd1 = 0;
				//cout << even_odd << endl;
			}
		}
	}
	/*cout << "N=10, K=6" << endl;
	cout << logic_block_count_even.size() << endl;
	cout << logic_block_count_odd.size() << endl;
	cout << "circuit" << endl;
	for (int i = 0; i < logic_block_count_even.size(); i++) {
		cout << logic_block_count_even[i];
		printf("\n");
	}
	cout << "logic blocks" << endl;
	for (int i = 0; i < logic_block_count_odd.size(); i++) {
		cout << logic_block_count_odd[i];
		printf("\n");
	}*/


	//MAP函数
	int small_widthnum[15249] = { 0 };//数量
	int small_depthnum[15249] = { 0 };
	int small_widthchoose[15249] = { 0 };//选择的size
	int small_depthchoose[15249] = { 0 };
	int RAM_type[15249] = { 0 };//选择哪种RAM
	int midwidth_num;//中间变量
	int midwidth_val;
	int middepth_num;
	int middepth_val;
	int num_LUTRAM[69] = { 0 };//算每个circuit的用的RAM值
	int num_8KRAM[69] = { 0 };
	int num_128KRAM[69] = { 0 };
	int i = 0;//识别CIRCUIT
	int RAM_choose[15249] = { 0 };
	int cw_num;
	int cd_num;
	for (int j = 0; j < list.size(); j++)
	{
		if (logic_block_count_even[i] == list[j].Circuit)
		{
			if (list[j].Mode == 3)
			{
				int depth[12] = { 131072,65536, 32768, 16384, 8192, 4096, 2048, 8192, 4096, 2048, 1024, 512 };//最大的SIZE是2048*64和512*16
				int width[12] = { 1, 2, 4, 8, 16, 32, 64,1, 2, 4, 8, 16 };
				int midwidth_val;
				int middepth_val;
				int w_smallest_area = 10000000;
				int w_area = 0;
				for (int x = 0; x < 12; x++)
				{
					midwidth_num = (ceil((double)list[j].Width / (double)width[x]));//向上取整
					midwidth_val = midwidth_num * width[x];
					middepth_num = ceil((double)list[j].Depth / (double)depth[x]);
					middepth_val = middepth_num * depth[x];
					w_area = middepth_val * midwidth_val - list[j].Depth * list[j].Width;
					if ((w_area < w_smallest_area) && (middepth_num < 16))
					{
						w_smallest_area = w_area;
						if (width[x] * depth[x] == 128 * 1024)
						{
							RAM_type[j] = 3;
						}
						else
						{
							RAM_type[j] = 2;
						}
						small_widthnum[j] = midwidth_num;
						small_depthnum[j] = middepth_num;
						small_widthchoose[j] = width[x];
						small_depthchoose[j] = depth[x];
					}
				}
				if (RAM_type[j] == 3)//计算单个Circuit用的RAM数量
				{
					num_128KRAM[i] = num_128KRAM[i] + small_widthnum[j] * small_depthnum[j];
				}
				else
				{
					num_8KRAM[i] = num_8KRAM[i] + small_widthnum[j] * small_depthnum[j];
				}
				//cout << "Circuit: " << list[j].Circuit << "  " << "RAM_ID: " << list[j].RamID << "  " << "Circuit_Width:" << list[j].Width << "  " << "Circuit_Depth: " << list[j].Depth << "  " << "Used_RAM_TYPE: " << RAM_type[j] << "  " << "Size_Width: " << small_widthchoose[j] << "  " << "Size_Depth: " << small_depthchoose[j] << "  " << "W_NUM: " << small_widthnum[j] << "  " << "D_NUM: " << small_depthnum[j];
				//printf("\n");
			}
			else
			{
				int depth[16] = { 131072,65536, 32768, 16384, 8192, 4096, 2048, 1024, 8192, 4096, 2048, 1024, 512, 256, 64,32 };
				int width[16] = { 1, 2, 4, 8, 16, 32, 64,128, 1, 2, 4, 8, 16, 32,10,20 };
				int w_smallest_area = 100000000;
				int w_area = 0;
				int midwidth_val;
				int middepth_val;
				for (int x = 0; x < 16; x++)
				{
					midwidth_num = ceil((double)list[j].Width / (double)width[x]);//向上取整
					midwidth_val = midwidth_num * width[x];
					middepth_num = ceil((double)list[j].Depth / (double)depth[x]);
					middepth_val = middepth_num * depth[x];
					w_area = middepth_val * midwidth_val - list[j].Depth * list[j].Width;
					if ((w_area < w_smallest_area) && (middepth_num < 16))
					{
						w_smallest_area = w_area;
						if (width[x] * depth[x] == 128 * 1024)
						{
							RAM_type[j] = 3;
						}
						else if (width[x] * depth[x] == 8 * 1024)
						{
							RAM_type[j] = 2;
						}
						else
						{
							RAM_type[j] = 1;
						}
						small_widthnum[j] = midwidth_num;
						small_depthnum[j] = middepth_num;
						small_widthchoose[j] = width[x];
						small_depthchoose[j] = depth[x];
					}
				}
				if (RAM_type[j] == 3)//计算单个Circuit用的RAM数量
				{
					num_128KRAM[i] = num_128KRAM[i] + small_widthnum[j] * small_depthnum[j];

				}
				else if (RAM_type[j] == 2)
				{
					num_8KRAM[i] = num_8KRAM[i] + small_widthnum[j] * small_depthnum[j];
				}
				else
				{
					num_LUTRAM[i] = num_LUTRAM[i] + small_widthnum[j] * small_depthnum[j];
				}
				//cout << "Circuit: " << list[j].Circuit << "  " << "RAM_ID: " << list[j].RamID << "  " << "Circuit_Width:" << list[j].Width << "  " << "Circuit_Depth: " << list[j].Depth << "  " << "Used_RAM_TYPE: " << RAM_type[j] << "  " << "Size_Width: " << small_widthchoose[j] << "  " << "Size_Depth: " << small_depthchoose[j] << "  " << "W_NUM: " << small_widthnum[j] << "  " << "D_NUM: " << small_depthnum[j];
				//printf("\n");
			}
		}
		else
		{
			//cout << "Next Circuit";
			//printf("\n");
			i = i + 1;
			j = j - 1;
		}
	}

	/*for (int j = 0; j < 69; j++)//算单个Circuit用的RAM有多少
	{
		cout << "Circuit: " << logic_block_count_even[j] << "  " << "LUT: " << num_LUTRAM[j] << "  " << "8K: " << num_8KRAM[j] << " " << "128K: " << num_128KRAM[j];
		printf("\n");
	}*/

	//算每个Circuit的MAX_width用来计算Area
	/*int max_width[69] = { 0 };
	int h = 0;
	for (int j = 0; j < list.size(); j++)
	{
		if (logic_block_count_even[h] == list[j].Circuit)
		{
			if (RAM_type[j] == 3 )
			{
				max_width[h] = 128;
			}
			else if(RAM_type[j] == 2 )
			{
				max_width[h] = 32;
			}
			else
			{
				max_width[h] = 20;
			}
		}
		else//如果不是一个Circuit，就连跳两次
		{
			//cout << "Circuit: " << list[j-1].Circuit<<" " <<"max_width:"<<max_width[h];
			//printf("\n");
			h = h + 1;
			j = j - 1;

		}
	}
	//cout << "Circuit: " << list[list.size()].Circuit << " " << "max_width:" << max_width[h];
	//printf("\n");
	*/

	//算Decoder_number
	int LUT_decoder_num[15249];

	for (int j = 0; j < list.size(); j++)
	{
		if (small_depthnum[j] == 1) { LUT_decoder_num[j] = 0; }
		else if (small_depthnum[j] == 2) { LUT_decoder_num[j] = 1; }
		else { LUT_decoder_num[j] = small_depthnum[j]; }
	}

	//算MUX
	int LUT_mux_num[15249];
	int mux_wnum[15249];
	int mux_dnum[15249];
	double mid_mux_wnum;

	for (int j = 0; j < list.size(); j++)
	{
		if (small_depthnum[j] == 1)
		{
			LUT_mux_num[j] = 0;
		}
		else if (small_depthnum[j] <= 4)
		{
			mux_wnum[j] = list[j].Width;
			mux_dnum[j] = 1;
			LUT_mux_num[j] = mux_dnum[j] * mux_wnum[j];
		}
		else
		{
			mux_wnum[j] = list[j].Width;
			mux_dnum[j] = small_depthnum[j] / 4 + 1;
			//mux_dnum[j] = ceil(log2((double)small_depthnum[j]))+ small_depthnum[j];
			LUT_mux_num[j] = mux_dnum[j] * mux_wnum[j];
		}
	}

	//算LUT用的逻辑块
	int Regular_LB[69] = { 0 };
	int Required_LB[69] = { 0 };
	int additional_logic[15249] = { 0 };
	int adlb[15249] = { 0 };
	//先算Regular_LB
	int mid_mapping;
	for (int i = 0; i < 69; i++)//对Regular_LB赋初值
	{
		Regular_LB[i] = logic_block_count_odd[i];
	}

	int k = 0;//把数据累加到Regular_LB中
	for (int j = 0; j < list.size(); j++)
	{
		if (logic_block_count_even[k] == list[j].Circuit)
		{
			int a = 10;
			mid_mapping = ceil((double)(LUT_mux_num[j] + LUT_decoder_num[j]) / (double)a);
			adlb[j] = LUT_mux_num[j] + LUT_decoder_num[j];
			additional_logic[j] = mid_mapping;//每一个RAM要用的additional_logic
			Regular_LB[k] = Regular_LB[k] + mid_mapping;
		}
		else
		{
			k = k + 1;
			j = j - 1;
		}
	}

	//算LUT,8K和128K用的LB
	int  Used_LUT[69] = { 0 };
	int  Used_8K[69] = { 0 };
	int  Used_128K[69] = { 0 };
	for (int j = 0; j < 69; j++)
	{
		Used_LUT[j] = Regular_LB[j] + num_LUTRAM[j];
		Used_8K[j] = num_8KRAM[j] * 10;
		Used_128K[j] = num_128KRAM[j] * 300;
	}

	//对三个RAM用的LB进行比较来得到Required
	for (int j = 0; j < 69; j++)
	{
		if ((Used_LUT[j] > Used_8K[j]) && (Used_LUT[j] > Used_128K[j]))
		{
			Required_LB[j] = Used_LUT[j];
		}
		else if ((Used_8K[j] > Used_LUT[j]) && (Used_8K[j] > Used_128K[j]))
		{
			Required_LB[j] = Used_8K[j];
		}
		else
		{
			Required_LB[j] = Used_128K[j];
		}
	}
	for (int j = 0; j < 69; j++)
	{
		cout << Regular_LB[j] << " " << Required_LB[j];
		printf("\n");
	}

	//算Area
	long double Total_area = 1;
	long double Circuit_area[69] = { 0 };
	double mid_area[69] = { 0 };
	double chu = 100000000;
	int num_lb;
	int num_8k;
	int num_128k;
	for (int j = 0; j < 69; j++)
	{
		int a = 10;
		int b = 300;
		num_lb = Required_LB[j];
		num_8k = floor((double)Required_LB[j] / (double)a);
		num_128k = floor((double)Required_LB[j] / (double)b);
		//标准计算
		Circuit_area[j] = num_lb * 37500 + num_8k * (9000 + 5 * 8192 + 90 * sqrt((double)8192) + 600 * 2 * 32)
			+ num_128k * (9000 + 5 * 131072 + 90 * sqrt((double)131072) + 600 * 2 * 128);
		//MTJ计算Area
		/* Circuit_area[j] = num_lut * 37500 + num_8k * (9000 + 1.25 * 8192 + 90 * sqrt(8192) + 600 * 2 * max_width[j])
			+ num_128k * (9000 + 1.25 * 131072 + 90 * sqrt(131072) + 600 * 2 * max_width[j]);*/
			//cout <<"CircuitID: "<< logic_block_count_even[j]<<" "<<"num_lut: "<< num_lut << " " << "num_8k: " << num_8k << " " << "num_128k: " << num_128k << " " << "Circuit_area: "<<Circuit_area[j];
			//printf("\n");
		mid_area[j] = Circuit_area[j] / chu;
		cout << mid_area[j];
		printf("\n");
		Total_area = Total_area * mid_area[j];
	}
	cout << pow(Total_area, (double)1 / 69.0) * 100000000;
	//输出TXT
	/*ofstream outputfile;
	outputfile.open("mapping_file.txt");
	for (int j = 0; j < list.size(); j++)
	{
		if (list[j].Mode == 0)
		{
			outputfile << list[j].Circuit << " " << list[j].RamID << " " << adlb[j] << " " << "LW" << " " << list[j].Width << " " << "LD" << " " << list[j].Depth << " "
				<< "ID" << " " << list[j].RamID << " " << "S" << " " << small_depthnum[j] << " " << "P" << " " << small_widthnum[j] << " "
				<< "Type" << " " << RAM_type[j] << " " << "Mode" << " " << "SimpleDualPort"
				<< " " << "W" << " " << small_widthchoose[j] << " " << "D" << " " << small_depthchoose[j]<< "\n";;
		}
		else if (list[j].Mode == 1)
		{
			outputfile << list[j].Circuit << " " << list[j].RamID << " " << adlb[j] << " " << "LW" << " " << list[j].Width << " " << "LD" << " " << list[j].Depth << " "
				<< "ID" << " " << list[j].RamID << " " << "S" << " " << small_depthnum[j] << " " << "P" << " " << small_widthnum[j] << " "
				<< "Type" << " " << RAM_type[j] << " " << "Mode" << " " << "ROM"
				<< " " << "W" << " " << small_widthchoose[j] << " " << "D" << " " << small_depthchoose[j] << "\n";
		}
		else if (list[j].Mode == 2)
		{
			outputfile << list[j].Circuit << " " << list[j].RamID << " " << adlb[j] << " " << "LW" << " " << list[j].Width << " " << "LD" << " " <<list[j].Depth << " "
				<< "ID" << " " << list[j].RamID << " " << "S" << " " << small_depthnum[j] << " " << "P" << " " << small_widthnum[j]<<" "
				<< "Type" << " " << RAM_type[j] << " " << "Mode" << " " << "SinglePort"
				<< " " << "W" << " " << small_widthchoose[j] << " " << "D" << " " << small_depthchoose[j] << "\n";
		}
		else
		{
			outputfile << list[j].Circuit << " " << list[j].RamID << " " << adlb[j]*2 << " " << "LW" << " " << list[j].Width << " " << "LD" << " " << list[j].Depth << " "
				<< "ID" << " " << list[j].RamID << " " << "S" << " " << small_depthnum[j] << " " << "P" << " " << small_widthnum[j] << " "
				<< "Type" << " " << RAM_type[j] << " " << "Mode" << " " << "TrueDualPort"
				<< " " << "W" << " " << small_widthchoose[j] << " " << "D" << " " << small_depthchoose[j] << "\n";
		}
		   //这里输出的分别是你之前帮我保存到vector的数据以及我自己载入的数组数据，然后我要输出我自己算的数据还有一些字符串，每一次cout都是一行。
	}
	outputfile.close();
	*/
	return 0;
}
