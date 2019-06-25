#include "NBJLpaiLuDelegate.h"

namespace NBJL
{
	/*******************字列表******************/
	PaiLuLeftDelegate::PaiLuLeftDelegate()
	{

	}
	PaiLuLeftDelegate::~PaiLuLeftDelegate()
	{

	}
	Size PaiLuLeftDelegate::tableCellSizeForIndex(TableView *table, ssize_t idx)
	{
		return CCSizeMake(50, 300);
	}

	TableViewCell* PaiLuLeftDelegate::tableCellAtIndex(TableView *table, ssize_t idx)
	{
		// 在重用队列中获取一个cell 然后判断cell是否为空 为空则创建一个新的
		DownLeftItemCell* cell = dynamic_cast<DownLeftItemCell*>(table->dequeueCell());
		bool isRefresh = cell;//是否更新
		if (!cell)
		{
			cell = DownLeftItemCell::create();
		}
		DownLeftStruct downleftInfo;
		downleftInfo.bRefresh = isRefresh;
		if (!_AllLeftResultData.empty())
		{
			for (int i = 0; i < leftListitemCount; i++)
			{
				if (idx * leftListitemCount + i > _AllLeftResultData.size() - 1)   break;
					
				if (_AllLeftResultData.at(idx * leftListitemCount + i) > 9 || _AllLeftResultData.at(idx * leftListitemCount + i) < 0)
				{
					downleftInfo.ZiVaule[i] = 0;
				}
				else
				{
					downleftInfo.ZiVaule[i] = _AllLeftResultData.at(idx * leftListitemCount + i);
				}
			}
		}
		cell->setIdx(idx);
		cell->bulidCell(downleftInfo);
		return cell;
	}

	ssize_t PaiLuLeftDelegate::numberOfCellsInTableView(TableView *table)
	{
		if (_AllLeftResultData.size() < leftListitemCount * leftListCount)         // 如果说数据没有满48个，就把剩下的用0填满，至少出现8个队列
		{
			for (int i = _AllLeftResultData.size(); i < leftListitemCount * leftListCount; i++)
			{
				_AllLeftResultData.push_back(0);
			}
		}
		if (_AllLeftResultData.size() % leftListitemCount == 0)
		{
			return _AllLeftResultData.size() / leftListitemCount;
		}
		return  _AllLeftResultData.size() / leftListitemCount + 1;

	}
	//保存结果数据
	void PaiLuLeftDelegate::setAllResultData(vector<BYTE> AllResultData)
	{
		_AllLeftResultData.clear();
		for (int i = 0; i < AllResultData.size(); i++)
		{
			_AllLeftResultData.push_back(AllResultData.at(i));
		}
	}

	/*******************圈列表大路******************/
	PaiLuRightDaLuDelegate::PaiLuRightDaLuDelegate()
	{

	}
	PaiLuRightDaLuDelegate::~PaiLuRightDaLuDelegate()
	{

	}
	Size PaiLuRightDaLuDelegate::tableCellSizeForIndex(TableView *table, ssize_t idx)
	{
		return CCSizeMake(26, 150);
	}

	TableViewCell* PaiLuRightDaLuDelegate::tableCellAtIndex(TableView *table, ssize_t idx)
	{
		// 在重用队列中获取一个cell 然后判断cell是否为空 为空则创建一个新的
		DownRightDaLuItemCell* cell = dynamic_cast<DownRightDaLuItemCell*>(table->dequeueCell());
		bool isRefresh = cell;//是否更新
		if (!cell)
		{
			cell = DownRightDaLuItemCell::create();
		}
		DownRightDaLuStruct downRightDaLu;
		downRightDaLu.bRefresh = isRefresh;
		if (!_ClassResultData.empty())
		{
			downRightDaLu.QuanVaule.assign(_ClassResultData[idx].begin(), _ClassResultData[idx].end());
		}
		cell->setIdx(idx);
		cell->bulidCell(downRightDaLu);
		return cell;
	}

	ssize_t PaiLuRightDaLuDelegate::numberOfCellsInTableView(TableView *table)
	{
		int idx = 0;  //当前列
		vector<BYTE> temp; //当前的结果，用来判断是不是和下一个结果相对，如果不相等就换列。 
		PailuType  type = None;
		_ClassResultData.clear(); //清空上一轮保存的数据
		heList.clear();  // 清空上一轮和的集合
		refreshArray();  // 重置二维数组

		if (_AllRightDaLuResultData.size() <= 0)  return 0;
		for (int i = 0; i < _AllRightDaLuResultData.size();i++)
		{
			//遍历中，走到一个元素的时候，先判断有没有上一个队列，然后比对一下，可不可以加入这个队列，如果不可以，那么就把上一个队列算到二维素组里面去，然后清除，然后换行 。 然后开始一个新的队列，并且计算新的队列的类型。
			if (temp.size() > 0)
			{
				if ((PailuType)determineType(_AllRightDaLuResultData[i]) == type || type == PailuType::he || (PailuType)determineType(_AllRightDaLuResultData[i]) == PailuType::he)   
					//如果这个队列的开头是和， 任意类型都可以加入，  所以下面要判断是否更新类型.  如果新的元素是和，那么也直接让他加入进来。
				{
					if (type == PailuType::he)
					{
						type = (PailuType)determineType(_AllRightDaLuResultData[i]);
					}
					
					temp.push_back(_AllRightDaLuResultData[i]);
				}
				else
				{
					disposeTempData(idx, temp);
					idx++;
					temp.clear();   
				}
			}
			//如果没有上一个队列，就开启新的队列
			if (temp.size() == 0)
			{
				temp.push_back(_AllRightDaLuResultData[i]);
				type = (PailuType)determineType(_AllRightDaLuResultData[i]);
			}
		}

		if (temp.size()>0)   //最后一个队列的时候 可能会漏掉
		{
			disposeTempData(idx, temp);
			temp.clear();  
		}

		exportData();
		
		return  _ClassResultData.size();
	}
	//保存结果数据
	void PaiLuRightDaLuDelegate::setAllResultData(vector<BYTE> AllResultData)
	{
		_AllRightDaLuResultData.clear();
		for (int i = 0; i < AllResultData.size(); i++)
		{
			_AllRightDaLuResultData.push_back(AllResultData.at(i));
		}
	}

	int PaiLuRightDaLuDelegate::getClassResultDataSize()
	{
		return _ClassResultData.size();
	}

	void PaiLuRightDaLuDelegate::refreshArray()
	{
		memset(dataArray, 0, sizeof(dataArray));
	}

	int PaiLuRightDaLuDelegate::determineType(int num)
	{
		if (num == 1 || num == 4 || num == 5 || num == 8)
		{
			return 1;
		}
		else if (num == 2 || num == 6 || num == 7 || num == 9)
		{
			return 2;
		}
		else if (num == 3)
		{
			return 3;
		}

		return 0;
	}

	void PaiLuRightDaLuDelegate::disposeTempData(int idx, vector<BYTE> nums)
	{

		int i = 0;          //当前列的位置   0-5   代表格子1-6
		int lastI = 0;      //这两个参数是给和记录的
		int lastIdx = idx;  //这两个参数是给和记录的

		for (auto num : nums)
		{
			//这个函数我的想法是， 首先 判断num是不是和， 如果是和，那么记住现在的位置  x ，y。  如果不是和，那么添加到二维数组里，如果这一列（1列有6个）走到底了，就往右边延伸。 后面来的行，如果走到之前的延伸了，那么就继续往自己的右边延伸
			if (num == 3)  // 如果是和的话
			{
				HePos  hePos;
				hePos.x = lastIdx;
				hePos.y = lastI;
				heList.push_back(hePos);
			}
			else
			{
				if (i < 6)   // i必须小于6
				{
					if (dataArray[idx][i] == 0)   //如果i没有到6，并且往下可以继续走
					{
						lastI = i;         //这里是先赋值 后++  所以在++前记录
						dataArray[idx][i] = num;
						i++;
					}
					else      //如果i没有到6，但是位置已经被前面的占了。那么直接右移
					{
						idx++;
						lastIdx = idx;       //这里是先++后赋值，所以在++后记录
						dataArray[idx][lastI] = num;   //这里用的是上一个i， 因为i++之后，进行判断是不能继续走，那么这里就得回到上一个i。
					}
				}
				else   //如果说i到6了，也就是这一列的6个铺满了，那么就往右边走。
				{
					idx++;          //因为这边是往右移， 所以需要记住移动后的位置
					lastIdx = idx;
					if (i == 6 ) 
					{
						i -= 1;
					}
					dataArray[idx][i] = num;
				}
			}
		}
	}

	void PaiLuRightDaLuDelegate::exportData()
	{
		int idx = 0;
		vector<BYTE> temp;
		for (int i = 0; i < 80; i++)
		{
			temp.clear();
			for (int j = 0; j < 6; j++)
			{
				
				if (idx < heList.size() && heList[idx].x == i && heList[idx].y == j)  //如果遇到了和要把和插入进去
				{
					temp.push_back(3);
					idx++;                  //因为都是从左往右保存， 所以这里直接把和list里面的向量一个个的向前进行判断，这样可以避免每次都要遍历和list
					j--;                    //如果发现这里是和，那么就继续拿这个位置算下一个和，直到不为
				}
				else
				{
					temp.push_back(dataArray[i][j]);
				}
			}
			_ClassResultData.push_back(temp);

			if (i - 4 > 0 && dataArray[i - 4][0] == 0)  //故意多生成4排空的。这关系到队列最后面会生成多少空队列
			{
				break;
			}
		}
	}


	/*******************圈列表大眼仔路******************/
	PaiLuRighDaYanZaiDelegate::PaiLuRighDaYanZaiDelegate()
	{

	}
	PaiLuRighDaYanZaiDelegate::~PaiLuRighDaYanZaiDelegate()
	{

	}
	Size PaiLuRighDaYanZaiDelegate::tableCellSizeForIndex(TableView *table, ssize_t idx)
	{
		return CCSizeMake(26, 75);
	}

	TableViewCell* PaiLuRighDaYanZaiDelegate::tableCellAtIndex(TableView *table, ssize_t idx)
	{
		// 在重用队列中获取一个cell 然后判断cell是否为空 为空则创建一个新的
		DownRightDaYanZaiItemCell* cell = dynamic_cast<DownRightDaYanZaiItemCell*>(table->dequeueCell());
		bool isRefresh = cell;//是否更新
		if (!cell)
		{
			cell = DownRightDaYanZaiItemCell::create();
		}
		DownRightDaLuStruct downRightDaLu;
		downRightDaLu.bRefresh = isRefresh;
		if (!_ClassResultData.empty() && idx<_ClassResultData.size())
		{
			downRightDaLu.QuanVaule.assign(_ClassResultData[idx].begin(), _ClassResultData[idx].end());
		}

		cell->setIdx(idx);
		cell->bulidCell(downRightDaLu);
		return cell;
	}
	ssize_t PaiLuRighDaYanZaiDelegate::numberOfCellsInTableView(TableView *table)
	{
		refreshArray();
		_ClassResultData.clear();
		_tempDataList.clear();
		if (_AllRightDaLuResultData.size() <= 0) return 0;
		
		vector<BYTE> temp;
		PailuType  type = None;
		for (int i = 0; i < _AllRightDaLuResultData.size(); i++)  //这个for循环整理出大路的红蓝情况
		{
			if (_AllRightDaLuResultData[i] == 3)  continue;  //遇到和直接干掉
				
			if (temp.size() > 0)
			{
				if ((PailuType)determineType(_AllRightDaLuResultData[i]) == type)
				{
					temp.push_back(_AllRightDaLuResultData[i]);
				}
				else
				{
					_tempDataList.push_back(temp);
					temp.clear();
				}
			}

			//如果没有上一个队列，就开启新的队列
			if (temp.size() == 0)
			{
				temp.push_back(_AllRightDaLuResultData[i]);
				type  = (PailuType)determineType(_AllRightDaLuResultData[i]);
			}
		}
		if (temp.size() > 0)
		{
			_tempDataList.push_back(temp);
			temp.clear();
		}
		
		//首先分析出大眼仔路自己的红蓝数据
		if (_tempDataList.size() <= 0) return 0;   
		vector<BYTE> tempDatas;
		tempDatas = analyzeDayYanZaiRedAndBlue(_tempDataList);   // 这个tempDatas里面只有 1 和 2

		//通过存放队列的集合，来判断最后一个开庄或者开合可能出现的
		if (_tempDataList.size() > 2)
		{
			nextZhuang = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 2], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::red);
			nextXian = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 2], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::blue);
		}
	
		//然后把这些红蓝数据用二维数组排列好
		if (tempDatas.size() <= 0)  return 0;  
		vector<BYTE> temp_1;
		int idx = 0;
		PailuType type_1 = None;
		for (size_t i = 0; i < tempDatas.size(); i++)
		{
			if (temp_1.size() > 0)
			{
				if ((PailuType)determineType(tempDatas[i]) == type_1)
				{
					temp_1.push_back(tempDatas[i]);
				}
				else
				{
					disposeTempData(idx, temp_1);
					temp_1.clear();
					idx++;
				}
			}
			if (temp_1.size() == 0)
			{
				temp_1.push_back(tempDatas[i]);
				type_1 = (PailuType)determineType(tempDatas[i]);
			}
		}

		if (temp_1.size()>0)   //最后一个队列的时候 可能会漏掉
		{
			disposeTempData(idx, temp_1);
			temp_1.clear();
		}

		//得出最终数据 并且返回数据size
		exportData();

		if (_ClassResultData.size() < rightLongListCount)
		{
			return 22; 
		}
		else
		{
			return  _ClassResultData.size();
		}
		
	}
	//保存结果数据
	void PaiLuRighDaYanZaiDelegate::setAllResultData(vector<BYTE> AllResultData)
	{
		_AllRightDaLuResultData.clear();
		for (int i = 0; i < AllResultData.size(); i++)
		{
			_AllRightDaLuResultData.push_back(AllResultData.at(i));
		}
	}

	int PaiLuRighDaYanZaiDelegate::getClassResultDataSize()
	{
		return _ClassResultData.size();
	}

	void PaiLuRighDaYanZaiDelegate::refreshArray()
	{
		memset(dataArray, 0, sizeof(dataArray));
	}

	int PaiLuRighDaYanZaiDelegate::determineType(int num)
	{
		if (num == 1 || num == 4 || num == 5 || num == 8)  //开闲的
		{
			return 1;
		}
		else if (num == 2 || num == 6 || num == 7 || num == 9)   //开庄
		{
			return 2;
		}
		else
		{
			return 0;    // 大眼仔路 和不处理
		}

	}

	vector<BYTE> PaiLuRighDaYanZaiDelegate::analyzeDayYanZaiRedAndBlue(vector<vector<BYTE>> temps)
	{
		vector<BYTE> temp;
		if (temps.size() <= 1)  return temp;   //只有第一列的时候， 直接return
		if (temps.size() == 2 && temps[1].size() <= 1) return temp;  //如果只有两列，并且第二列还只有1个的时候，return  因为从第二列第二口 开始
		int beforeFrist = -1;//比对时第一列的前一列
		int frist ;  //比对时第一列
		int scend = 1;  //比对时的第二列
		for ( frist = 0; scend < temps.size() ; frist++, scend++, beforeFrist++)
		{
			//拆分， 只比对当前列，和前一列，     或者说是这一列和后一列
			int f = temps[frist].size();
			int s = temps[scend].size();

			if (scend > 1)  //如果需要比对的第二个队列不是整个集合中的第二列
			{
				//如果本列只有一个元素或者某一列的第一个元素，那么就这么比对: 把前一列 + 1 和前前列进行比对。  如果有对，或者是连庄，本来应该写红，反而写蓝 。 如果不是对和连庄，那么反而写红		
				//如果本列不是只有一个元素，那么本列的第一个元素需要先判断前前列和前列的元素数量相等还是不相等算出结果，
				if (temps[beforeFrist].size() == f)  //相等：包含了齐脚跳，必然无对，无长连  所以写红
				{
					temp.push_back(XiaSanLuType::red);
				}
				else       //不想等: 包含了突脚跳， 本列的第一个拿去和前一列对比，发现有对， 或者有长庄，  所以写蓝
				{
					temp.push_back(XiaSanLuType::blue);
				}

				if (s == 1)  continue; //如果比对的第二列只有1个元素，那么上面已经生成了对应的红蓝，可以直接continue了。

				if (f >= s)  //比对的第一列比第二列长，或者相等的情况
				{
					for (int i = 0; i < s - 1; i++)     //去掉第一口已经比对过，剩下的和左边都能对上，所以全部加红
					{
						temp.push_back(XiaSanLuType::red);
					}
				}
				else  //比对的第二列比第一列长
				{
					for (int i = 0; i < f - 1; i++)   //先判断能对上的元素 写红
					{
						temp.push_back(XiaSanLuType::red);
					}

					temp.push_back(XiaSanLuType::blue);  //对子走完之后中间会有一个无对加蓝  

					for (int i = 0; i < s - f - 1; i++)   //然后判断形成了长连的部分
					{
						temp.push_back(XiaSanLuType::red);
					}
				}

			}
			else//如果需要比对的第二个队列刚好是整个集合的第二列
			{
				if (s == 1)  //如果这个第二列只有一个元素
				{
					continue; //直接continue 不判断
				}
				else
				{
					if (f >= s)  //比对的第一列比第二列长，或者相等的情况
					{
						for (int i = 0; i < s - 1; i++)     //去掉第一口已经比对过，剩下的和左边都能对上，所以全部加红
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
					else  //比对的第二列比第一列长
					{
						for (int i = 0; i < f - 1; i++)   //先判断能对上的元素 写红
						{
							temp.push_back(XiaSanLuType::red);
						}

						temp.push_back(XiaSanLuType::blue);  //对子走完之后中间会有一个无对加蓝  

						for (int i = 0; i < s - f - 1; i++)   //然后判断形成了长连的部分
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
				}
			}
		}
		return temp;
	}

	void PaiLuRighDaYanZaiDelegate::disposeTempData(int idx, vector<BYTE> nums)
	{
		int i = 0;          //当前列的位置   0-5   代表格子1-6
		int lastI = 0;      

		for (auto num : nums)
		{
			//这个函数我的想法是， 首先 判断num是不是和， 如果是和，那么记住现在的位置  x ，y。  如果不是和，那么添加到二维数组里，如果这一列（1列有6个）走到底了，就往右边延伸。 后面来的行，如果走到之前的延伸了，那么就继续往自己的右边延伸
			if (i < 6)   // i必须小于6
			{
				if (dataArray[idx][i] == 0)   //如果i没有到6，并且往下可以继续走
				{
					lastI = i;         //这里是先赋值 后++  所以在++前记录
					dataArray[idx][i] = num;
					i++;
				}
				else      //如果i没有到6，但是位置已经被前面的占了。那么直接右移
				{
					idx++;
					dataArray[idx][lastI] = num;   
				}
			}
			else   //如果说i到6了，也就是这一列的6个铺满了，那么就往右边走。
			{
				idx++;          //因为这边是往右移， 所以需要记住移动后的位置
				if (i == 6)
				{
					i -= 1;
				}
				dataArray[idx][i] = num;
			}
		}
	}

	void PaiLuRighDaYanZaiDelegate::exportData()
	{
		int idx = 0;
		vector<BYTE> temp;
		for (int i = 0; i < 80; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				temp.push_back(dataArray[i][j]);
			}

			if (i % 2 != 0)
			{
				_ClassResultData.push_back(temp);
				temp.clear();
			}
			

			if (i - 4 > 0 && dataArray[i - 4][0] == 0)  //故意多生成4排空的。这关系到队列最后面会生成多少空队列
			{
				break;
			}
		}
	}

	int PaiLuRighDaYanZaiDelegate::judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type)
	{
		
		if (determineType(scend[0]) == type)  //如果这个时候类型相同，
		{
			if (frist.size() == scend.size())//两列 数量相对的时候， 加入新的元素到第二组那么开蓝
			{
				return 1;    
			}
			else  //否则开红   第一列的size大，或者第二列的size大  第二列新增元素都可是返回红，  因为要么有对，要么长连
			{
				return 2;      //返回红
			}
		}
		else   //如果类型不同 那么就是开新的一组了   这里是突脚条这类的情况
		{
			if (frist.size() == scend.size())     //如果 第二列和第一列长度相等， 新的元素等于是齐脚条，所以是红， 其它任何情况，都是蓝
			{
				return 2;    //返回红
			}
			else
			{
				return 1;
			}
		}
	}

	int PaiLuRighDaYanZaiDelegate::getNextZhuang()
	{
		return nextZhuang > 2 || nextZhuang < 0 ? 0 : nextZhuang;
	}

	int PaiLuRighDaYanZaiDelegate::getNextXian()
	{
		return nextXian > 2 || nextXian < 0 ? 0 : nextXian;
	}


	/*******************圈列表小路******************/
	PaiLuRighXiaoLuDelegate::PaiLuRighXiaoLuDelegate()
	{

	}
	PaiLuRighXiaoLuDelegate::~PaiLuRighXiaoLuDelegate()
	{

	}
	Size PaiLuRighXiaoLuDelegate::tableCellSizeForIndex(TableView *table, ssize_t idx)
	{
		return CCSizeMake(26, 75);
	}

	TableViewCell* PaiLuRighXiaoLuDelegate::tableCellAtIndex(TableView *table, ssize_t idx)
	{
		// 在重用队列中获取一个cell 然后判断cell是否为空 为空则创建一个新的
		DownRightXiaoLuItemCell* cell = dynamic_cast<DownRightXiaoLuItemCell*>(table->dequeueCell());
		bool isRefresh = cell;//是否更新
		if (!cell)
		{
			cell = DownRightXiaoLuItemCell::create();
		}
		DownRightDaLuStruct downRightDaLu;
		downRightDaLu.bRefresh = isRefresh;
		if (!_ClassResultData.empty() && idx < _ClassResultData.size())
		{
			downRightDaLu.QuanVaule.assign(_ClassResultData[idx].begin(), _ClassResultData[idx].end());
		}

		cell->setIdx(idx);
		cell->bulidCell(downRightDaLu);
		return cell;
	}
	ssize_t PaiLuRighXiaoLuDelegate::numberOfCellsInTableView(TableView *table)
	{
		refreshArray();
		_ClassResultData.clear();
		_tempDataList.clear();
		if (_AllRightDaLuResultData.size() <= 0) return 0;

		vector<BYTE> temp;
		PailuType  type = None;
		for (int i = 0; i < _AllRightDaLuResultData.size(); i++)  //这个for循环整理出大路的红蓝情况
		{
			if (_AllRightDaLuResultData[i] == 3)  continue;  //遇到和直接干掉

			if (temp.size() > 0)
			{
				if ((PailuType)determineType(_AllRightDaLuResultData[i]) == type)
				{
					temp.push_back(_AllRightDaLuResultData[i]);
				}
				else
				{
					_tempDataList.push_back(temp);
					temp.clear();
				}
			}

			//如果没有上一个队列，就开启新的队列
			if (temp.size() == 0)
			{
				temp.push_back(_AllRightDaLuResultData[i]);
				type = (PailuType)determineType(_AllRightDaLuResultData[i]);
			}
		}
		if (temp.size() > 0)
		{
			_tempDataList.push_back(temp);
			temp.clear();
		}

		//首先分析出大眼仔路自己的红蓝数据
		if (_tempDataList.size() <= 0) return 0;
		vector<BYTE> tempDatas;
		tempDatas = analyzeXiaoLuRedAndBlue(_tempDataList);   // 这个temp里面只有 1 和 2

		//通过存放队列的集合，来判断最后一个开庄或者开合可能出现的
		if (_tempDataList.size() > 3)
		{
			nextZhuang = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 3], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::red);
			nextXian = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 3], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::blue);
		}

		//然后把这些红蓝数据用二维数组排列好
		if (tempDatas.size() <= 0)  return 0;
		vector<BYTE> temp_1;
		int idx = 0;
		PailuType type_1 = None;
		for (size_t i = 0; i < tempDatas.size(); i++)
		{
			if (temp_1.size() > 0)
			{
				if ((PailuType)determineType(tempDatas[i]) == type_1)
				{
					temp_1.push_back(tempDatas[i]);
				}
				else
				{
					disposeTempData(idx, temp_1);
					temp_1.clear();
					idx++;
				}
			}
			if (temp_1.size() == 0)
			{
				temp_1.push_back(tempDatas[i]);
				type_1 = (PailuType)determineType(tempDatas[i]);
			}
		}

		if (temp_1.size() > 0)   //最后一个队列的时候 可能会漏掉
		{
			disposeTempData(idx, temp_1);
			temp_1.clear();
		}

		//得出最终数据 并且返回数据size
		exportData();

		if (_ClassResultData.size() < rightShotListCount)
		{
			return rightShotListCount;
		}
		else
		{
			return  _ClassResultData.size();
		}
	}
	//保存结果数据
	void PaiLuRighXiaoLuDelegate::setAllResultData(vector<BYTE> AllResultData)
	{
		_AllRightDaLuResultData.clear();
		for (int i = 0; i < AllResultData.size(); i++)
		{
			_AllRightDaLuResultData.push_back(AllResultData.at(i));
		}
	}

	int PaiLuRighXiaoLuDelegate::getClassResultDataSize()
	{
		return _ClassResultData.size();
	}

	void PaiLuRighXiaoLuDelegate::refreshArray()
	{
		memset(dataArray, 0, sizeof(dataArray));
	}

	int PaiLuRighXiaoLuDelegate::determineType(int num)
	{
		if (num == 1 || num == 4 || num == 5 || num == 8)  //开闲的
		{
			return 1;
		}
		else if (num == 2 || num == 6 || num == 7 || num == 9)   //开庄
		{
			return 2;
		}
		else
		{
			return 0;    // 和不处理
		}

	}
	//拿第三列对比第一列，
	vector<BYTE> PaiLuRighXiaoLuDelegate::analyzeXiaoLuRedAndBlue(vector<vector<BYTE>> temps)
	{
		vector<BYTE> temp;
		if (temps.size() < 3)  return temp;   //只有前两列的时候， 直接return
		if (temps.size() == 3 && temps[2].size() <= 1) return temp;  //如果只有三列，并且第三列还只有1个的时候，return  因为从第三列第二口 开始
		int beforeFrist = -1;//比对时第三列的前一列  
		int frist ;  //比对时第一列
		int beforeScend = 1;//比对时的第三列的前一列
		int scend = 2;  //比对时的第三列
		for (frist = 0; scend < temps.size(); frist++, scend++, beforeFrist++,beforeScend++)
		{
			int f = temps[frist].size();
			int s = temps[scend].size();

			if (scend > 2) 
			{
				if (temps[beforeFrist].size() == temps[beforeScend].size())   //这里第一个判断的时候和大眼仔不一样，判断的是当前第三列的前一列， 比对当前第一列的前一列
				{
					temp.push_back(XiaSanLuType::red);
				}
				else      
				{
					temp.push_back(XiaSanLuType::blue);
				}

				if (s == 1)  continue; 

				if (f >= s)  
				{
					for (int i = 0; i < s - 1; i++)   
					{
						temp.push_back(XiaSanLuType::red);
					}
				}
				else 
				{
					for (int i = 0; i < f - 1; i++) 
					{
						temp.push_back(XiaSanLuType::red);
					}

					temp.push_back(XiaSanLuType::blue); 

					for (int i = 0; i < s - f - 1; i++)  
					{
						temp.push_back(XiaSanLuType::red);
					}
				}

			}
			else
			{
				if (s == 1) 
				{
					continue;
				}
				else
				{
					if (f >= s)  
					{
						for (int i = 0; i < s - 1; i++)     
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
					else  
					{
						for (int i = 0; i < f - 1; i++)   
						{
							temp.push_back(XiaSanLuType::red);
						}

						temp.push_back(XiaSanLuType::blue); 

						for (int i = 0; i < s - f - 1; i++)  
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
				}
			}
		}
		return temp;
	}

	void PaiLuRighXiaoLuDelegate::disposeTempData(int idx, vector<BYTE> nums)
	{
		int i = 0;          //当前列的位置   0-5   代表格子1-6
		int lastI = 0;

		for (auto num : nums)
		{
			//这个函数我的想法是， 首先 判断num是不是和， 如果是和，那么记住现在的位置  x ，y。  如果不是和，那么添加到二维数组里，如果这一列（1列有6个）走到底了，就往右边延伸。 后面来的行，如果走到之前的延伸了，那么就继续往自己的右边延伸
			if (i < 6)   // i必须小于6
			{
				if (dataArray[idx][i] == 0)   //如果i没有到6，并且往下可以继续走
				{
					lastI = i;         //这里是先赋值 后++  所以在++前记录
					dataArray[idx][i] = num;
					i++;
				}
				else      //如果i没有到6，但是位置已经被前面的占了。那么直接右移
				{
					idx++;
					dataArray[idx][lastI] = num;
				}
			}
			else   //如果说i到6了，也就是这一列的6个铺满了，那么就往右边走。
			{
				idx++;          //因为这边是往右移， 所以需要记住移动后的位置
				if (i == 6)
				{
					i -= 1;
				}
				dataArray[idx][i] = num;
			}
		}
	}

	void PaiLuRighXiaoLuDelegate::exportData()
	{
		int idx = 0;
		vector<BYTE> temp;
		for (int i = 0; i < 80; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				temp.push_back(dataArray[i][j]);
			}

			if (i % 2 != 0)
			{
				_ClassResultData.push_back(temp);
				temp.clear();
			}


			if (i - 4 > 0 && dataArray[i - 4][0] == 0)  //故意多生成4排空的。这关系到队列最后面会生成多少空队列
			{
				break;
			}
		}
	}

	int PaiLuRighXiaoLuDelegate::judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type)
	{

		if (determineType(scend[0]) == type)  //如果这个时候类型相同，
		{
			if (frist.size() == scend.size())//两列 数量相对的时候， 加入新的元素到第二组那么开蓝
			{
				return 1;
			}
			else  //否则开红   第一列的size大，或者第二列的size大  第二列新增元素都可是返回红，  因为要么有对，要么长连
			{
				return 2;      //返回红
			}
		}
		else   //如果类型不同 那么就是开新的一组了   这里是突脚条这类的情况
		{
			if (frist.size() == scend.size())     //如果 第二列和第一列长度相等， 新的元素等于是齐脚条，所以是红， 其它任何情况，都是蓝
			{
				return 2;    //返回红
			}
			else
			{
				return 1;
			}
		}
	}

	int PaiLuRighXiaoLuDelegate::getNextZhuang()
	{
		return nextZhuang > 2 || nextZhuang < 0 ? 0 : nextZhuang;
	}

	int PaiLuRighXiaoLuDelegate::getNextXian()
	{
		return nextXian > 2 || nextXian < 0 ? 0 : nextXian;
	}

	/*******************圈列表蟑螂路******************/
	PaiLuRighZhangLangDelegate::PaiLuRighZhangLangDelegate()
	{

	}
	PaiLuRighZhangLangDelegate::~PaiLuRighZhangLangDelegate()
	{

	}
	Size PaiLuRighZhangLangDelegate::tableCellSizeForIndex(TableView *table, ssize_t idx)
	{
		return CCSizeMake(26, 75);
	}

	TableViewCell* PaiLuRighZhangLangDelegate::tableCellAtIndex(TableView *table, ssize_t idx)
	{
		// 在重用队列中获取一个cell 然后判断cell是否为空 为空则创建一个新的
		DownRightZhangLangItemCell* cell = dynamic_cast<DownRightZhangLangItemCell*>(table->dequeueCell());
		bool isRefresh = cell;//是否更新
		if (!cell)
		{
			cell = DownRightZhangLangItemCell::create();
		}
		DownRightDaLuStruct downRightDaLu;
		downRightDaLu.bRefresh = isRefresh;
		if (!_ClassResultData.empty() && idx < _ClassResultData.size())
		{
			downRightDaLu.QuanVaule.assign(_ClassResultData[idx].begin(), _ClassResultData[idx].end());
		}

		cell->setIdx(idx);
		cell->bulidCell(downRightDaLu);
		return cell;
	}
	ssize_t PaiLuRighZhangLangDelegate::numberOfCellsInTableView(TableView *table)
	{
		refreshArray();
		_ClassResultData.clear();
		_tempDataList.clear();
		if (_AllRightDaLuResultData.size() <= 0) return 0;

		vector<BYTE> temp;
		PailuType  type = None;
		for (int i = 0; i < _AllRightDaLuResultData.size(); i++)  //这个for循环整理出大路的红蓝情况
		{
			if (_AllRightDaLuResultData[i] == 3)  continue;  //遇到和直接干掉

			if (temp.size() > 0)
			{
				if ((PailuType)determineType(_AllRightDaLuResultData[i]) == type)
				{
					temp.push_back(_AllRightDaLuResultData[i]);
				}
				else
				{
					_tempDataList.push_back(temp);
					temp.clear();
				}
			}

			//如果没有上一个队列，就开启新的队列
			if (temp.size() == 0)
			{
				temp.push_back(_AllRightDaLuResultData[i]);
				type = (PailuType)determineType(_AllRightDaLuResultData[i]);
			}
		}
		if (temp.size() > 0)
		{
			_tempDataList.push_back(temp);
			temp.clear();
		}

		//首先分析出大眼仔路自己的红蓝数据
		if (_tempDataList.size() <= 0) return 0;
		vector<BYTE> tempDatas;
		tempDatas = analyzeXiaoLuRedAndBlue(_tempDataList);   // 这个temp里面只有 1 和 2

		//通过存放队列的集合，来判断最后一个开庄或者开合可能出现的
		if (_tempDataList.size() > 4)
		{
			nextZhuang = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 4], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::red);
			nextXian = judgeRedOrBlue(_tempDataList[_tempDataList.size() - 4], _tempDataList[_tempDataList.size() - 1], XiaSanLuType::blue);
		}

		//然后把这些红蓝数据用二维数组排列好
		if (tempDatas.size() <= 0)  return 0;
		vector<BYTE> temp_1;
		int idx = 0;
		PailuType type_1 = None;
		for (size_t i = 0; i < tempDatas.size(); i++)
		{
			if (temp_1.size() > 0)
			{
				if ((PailuType)determineType(tempDatas[i]) == type_1)
				{
					temp_1.push_back(tempDatas[i]);
				}
				else
				{
					disposeTempData(idx, temp_1);
					temp_1.clear();
					idx++;
				}
			}
			if (temp_1.size() == 0)
			{
				temp_1.push_back(tempDatas[i]);
				type_1 = (PailuType)determineType(tempDatas[i]);
			}
		}

		if (temp_1.size() > 0)   //最后一个队列的时候 可能会漏掉
		{
			disposeTempData(idx, temp_1);
			temp_1.clear();
		}

		//得出最终数据 并且返回数据size
		exportData();

		if (_ClassResultData.size() < rightShotListCount)
		{
			return rightShotListCount;
		}
		else
		{
			return  _ClassResultData.size();
		}
	}
	//保存结果数据
	void PaiLuRighZhangLangDelegate::setAllResultData(vector<BYTE> AllResultData)
	{
		_AllRightDaLuResultData.clear();
		for (int i = 0; i < AllResultData.size(); i++)
		{
			_AllRightDaLuResultData.push_back(AllResultData.at(i));
		}
	}

	int PaiLuRighZhangLangDelegate::getClassResultDataSize()
	{
		return _ClassResultData.size();
	}

	void PaiLuRighZhangLangDelegate::refreshArray()
	{
		memset(dataArray, 0, sizeof(dataArray));
	}

	int PaiLuRighZhangLangDelegate::determineType(int num)
	{
		if (num == 1 || num == 4 || num == 5 || num == 8)  //开闲的
		{
			return 1;
		}
		else if (num == 2 || num == 6 || num == 7 || num == 9)   //开庄
		{
			return 2;
		}
		else
		{
			return 0;    // 和不处理
		}

	}

	//拿第三列对比第一列，
	vector<BYTE> PaiLuRighZhangLangDelegate::analyzeXiaoLuRedAndBlue(vector<vector<BYTE>> temps)
	{
		vector<BYTE> temp;
		if (temps.size() < 4)  return temp;   //只有前3列的时候， 直接return
		if (temps.size() == 4 && temps[3].size() <= 1) return temp;  //如果只有四列，并且第四列还只有1个的时候，return  因为从第四列第二口 开始
		int beforeFrist = -1;//比对时第三列的前一列  
		int frist;  //比对时第一列
		int beforeScend = 2;//比对时的第三列的前一列
		int scend = 3;  //比对时的第三列
		for (frist = 0; frist < temps.size() && scend < temps.size(); frist++, scend++, beforeFrist++, beforeScend++)
		{
			int f = temps[frist].size();
			int s = temps[scend].size();

			if (scend > 3)
			{
				if (temps[beforeFrist].size() == temps[beforeScend].size())   //这里第一个判断的时候和大眼仔不一样，判断的是当前第三列的前一列， 比对当前第一列的前一列
				{
					temp.push_back(XiaSanLuType::red);
				}
				else
				{
					temp.push_back(XiaSanLuType::blue);
				}

				if (s == 1)  continue;

				if (f >= s)
				{
					for (int i = 0; i < s - 1; i++)
					{
						temp.push_back(XiaSanLuType::red);
					}
				}
				else
				{
					for (int i = 0; i < f - 1; i++)
					{
						temp.push_back(XiaSanLuType::red);
					}

					temp.push_back(XiaSanLuType::blue);

					for (int i = 0; i < s - f - 1; i++)
					{
						temp.push_back(XiaSanLuType::red);
					}
				}

			}
			else
			{
				if (s == 1)
				{
					continue;
				}
				else
				{
					if (f >= s)
					{
						for (int i = 0; i < s - 1; i++)
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
					else
					{
						for (int i = 0; i < f - 1; i++)
						{
							temp.push_back(XiaSanLuType::red);
						}

						temp.push_back(XiaSanLuType::blue);

						for (int i = 0; i < s - f - 1; i++)
						{
							temp.push_back(XiaSanLuType::red);
						}
					}
				}
			}
		}
		return temp;
	}

	void PaiLuRighZhangLangDelegate::disposeTempData(int idx, vector<BYTE> nums)
	{
		int i = 0;          //当前列的位置   0-5   代表格子1-6
		int lastI = 0;

		for (auto num : nums)
		{
			//这个函数我的想法是， 首先 判断num是不是和， 如果是和，那么记住现在的位置  x ，y。  如果不是和，那么添加到二维数组里，如果这一列（1列有6个）走到底了，就往右边延伸。 后面来的行，如果走到之前的延伸了，那么就继续往自己的右边延伸
			if (i < 6)   // i必须小于6
			{
				if (dataArray[idx][i] == 0)   //如果i没有到6，并且往下可以继续走
				{
					lastI = i;         //这里是先赋值 后++  所以在++前记录
					dataArray[idx][i] = num;
					i++;
				}
				else      //如果i没有到6，但是位置已经被前面的占了。那么直接右移
				{
					idx++;
					dataArray[idx][lastI] = num;
				}
			}
			else   //如果说i到6了，也就是这一列的6个铺满了，那么就往右边走。
			{
				idx++;          //因为这边是往右移， 所以需要记住移动后的位置
				if (i == 6)
				{
					i -= 1;
				}
				dataArray[idx][i] = num;
			}
		}
	}

	void PaiLuRighZhangLangDelegate::exportData()
	{
		int idx = 0;
		vector<BYTE> temp;
		for (int i = 0; i < 80; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				temp.push_back(dataArray[i][j]);
			}

			if (i % 2 != 0)
			{
				_ClassResultData.push_back(temp);
				temp.clear();
			}

			if (i - 4 > 0 && dataArray[i - 4][0] == 0)  //故意多生成四排空的。这关系到队列最后面会生成多少空队列
			{
				break;
			}
		}
	}

	int PaiLuRighZhangLangDelegate::judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type)
	{

		if (determineType(scend[0]) == type)  //如果这个时候类型相同，
		{
			if (frist.size() == scend.size())//两列 数量相对的时候， 加入新的元素到第二组那么开蓝
			{
				return 1;
			}
			else  //否则开红   第一列的size大，或者第二列的size大  第二列新增元素都可是返回红，  因为要么有对，要么长连
			{
				return 2;      //返回红
			}
		}
		else   //如果类型不同 那么就是开新的一组了   这里是突脚条这类的情况
		{
			if (frist.size() == scend.size())     //如果 第二列和第一列长度相等， 新的元素等于是齐脚条，所以是红， 其它任何情况，都是蓝
			{
				return 2;    //返回红
			}
			else
			{
				return 1;
			}
		}
	}

	int PaiLuRighZhangLangDelegate::getNextZhuang()
	{
		return nextZhuang > 2 || nextZhuang < 0 ? 0 : nextZhuang;
	}

	int PaiLuRighZhangLangDelegate::getNextXian()
	{
		return nextXian > 2 || nextXian < 0 ? 0 : nextXian;
	}
}