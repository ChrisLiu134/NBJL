#ifndef __NBJL_paiLuDelegate_H__
#define __NBJL_paiLuDelegate_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include <vector>
#include "HNUIExport.h"
#include "NBJLTableCell.h"
#include "NBJLMessageHead.h"
#include <string>
USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;
using namespace extension;

namespace NBJL
{
	struct HePos
	{
		int x = 0;
		int y = 0;
	};

	class PaiLuLeftDelegate :public TableViewDelegate   //左边字列表
		,public TableViewDataSource
	{
	public:
		PaiLuLeftDelegate();
		~PaiLuLeftDelegate();
		/* 必须实现的函数 */
		// 当滑动tableview时触发该方法 参数为当前的tableview对象
		virtual void scrollViewDidScroll(ui::ScrollView* view) {};
		// 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
		virtual void scrollViewDidZoom(ui::ScrollView* view) {}
		// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
		virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
		// 设置tableview的Cell大小
		virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
		// 获取编号为idx的cell
		virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
		// 设置tableview中cell的个数
		virtual ssize_t numberOfCellsInTableView(TableView *table);

		//保存结果数据
		void setAllResultData(vector<BYTE> AllResultData);
	private:
		
		vector<BYTE> _AllLeftResultData;//所有结果数据     字列表中的所有数据，其中有 //闲家赢 //庄家赢 //和 //闲赢闲对子 //闲赢庄对子//庄赢庄对子//庄赢闲对子 //闲对子庄对子闲赢//闲对子庄对子庄赢

	};
	class PaiLuRightDaLuDelegate :public TableViewDelegate   //圈列表 大路
		, public TableViewDataSource
	{
	public:
		PaiLuRightDaLuDelegate();
		~PaiLuRightDaLuDelegate();
		/* 必须实现的函数 */
		// 当滑动tableview时触发该方法 参数为当前的tableview对象
		virtual void scrollViewDidScroll(ui::ScrollView* view) {};
		// 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
		virtual void scrollViewDidZoom(ui::ScrollView* view) {}
		// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
		virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
		// 设置tableview的Cell大小
		virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
		// 获取编号为idx的cell
		virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
		// 设置tableview中cell的个数
		virtual ssize_t numberOfCellsInTableView(TableView *table);

		//保存结果数据
		void setAllResultData(vector<BYTE> AllResultData);
		//获取分类数据的大小
		int getClassResultDataSize();

	private:
		//刷新二维数组 用来保存数据 让数据拐弯
		void refreshArray();
		//判断类型，决定下一个数字是否要加入本队列
		int determineType(int num);
		//处理临时储存的数据，把这些数据加入到二维数组中。
		void disposeTempData(int idx ,vector<BYTE> nums);
		//导出最终数据。
		void exportData();

	private:
		vector<BYTE>			_AllRightDaLuResultData;	//所有结果数据
		vector<vector<BYTE>>	_ClassResultData;		    //已经分好类排好序保存的数据

		
		int dataArray[80][6] = {};              //二维数组 用来存放数据
		
		vector<HePos> heList;               //单独把和的位置存下来
	};

	class PaiLuRighDaYanZaiDelegate :public TableViewDelegate   //圈列表大眼仔路
		, public TableViewDataSource
	{
	public:
		PaiLuRighDaYanZaiDelegate();
		~PaiLuRighDaYanZaiDelegate();
		/* 必须实现的函数 */
		// 当滑动tableview时触发该方法 参数为当前的tableview对象
		virtual void scrollViewDidScroll(ui::ScrollView* view) {};
		// 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
		virtual void scrollViewDidZoom(ui::ScrollView* view) {}
		// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
		virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
		// 设置tableview的Cell大小
		virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
		// 获取编号为idx的cell
		virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
		// 设置tableview中cell的个数
		virtual ssize_t numberOfCellsInTableView(TableView *table);

		//保存结果数据
		void setAllResultData(vector<BYTE> AllResultData);
		//获取分类数据的大小
		int getClassResultDataSize();

	private:
		//刷新二维数组 用来保存数据 让数据拐弯
		void refreshArray();
		//判断类型，决定下一个数字是否要加入本队列
		int determineType(int num);
		//分析出大眼仔自己的红蓝队列，加入到一个 byte向量中
		vector<BYTE> analyzeDayYanZaiRedAndBlue(vector<vector<BYTE>> temps);
		//处理大眼仔的红蓝数据，把这些数据加入到二维数组中。
		void disposeTempData(int idx, vector<BYTE> nums);
		//导出最终数据。
		void exportData();

		
	private:
		vector<BYTE>			_AllRightDaLuResultData;	//从服务器上获取的所有结果数据
		vector<vector<BYTE>>	_tempDataList;		        //存放红蓝队列的数据集合
		vector<vector<BYTE>>	_ClassResultData;		    //已经分好类排好序保存的数据

		int dataArray[80][6] = {};              //二维数组 用来存放数据

	public:
		//第六种牌路算法。拿到最后需要比较的两个队列的信息，然后 添加一个庄，或者一个闲赢进去，看看颜色会是什么。
		int judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type);
		
		int nextZhuang = 0;   //用来保存下一轮开庄的数据
		int nextXian = 0;     //用来保存下一轮开和的数据

		int getNextZhuang();   //返回下一轮开庄的红蓝结果
		int getNextXian();     //返回下一轮开闲的红蓝结果
	};

	class PaiLuRighXiaoLuDelegate :public TableViewDelegate   //圈列表小路
		, public TableViewDataSource
	{
	public:
		PaiLuRighXiaoLuDelegate();
		~PaiLuRighXiaoLuDelegate();
		/* 必须实现的函数 */
		// 当滑动tableview时触发该方法 参数为当前的tableview对象
		virtual void scrollViewDidScroll(ui::ScrollView* view) {};
		// 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
		virtual void scrollViewDidZoom(ui::ScrollView* view) {}
		// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
		virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
		// 设置tableview的Cell大小
		virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
		// 获取编号为idx的cell
		virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
		// 设置tableview中cell的个数
		virtual ssize_t numberOfCellsInTableView(TableView *table);

		//保存结果数据
		void setAllResultData(vector<BYTE> AllResultData);
		//获取分类数据的大小
		int getClassResultDataSize();

	private:
		//刷新二维数组 用来保存数据 让数据拐弯
		void refreshArray();
		//判断类型，决定下一个数字是否要加入本队列
		int determineType(int num);
		//分析出小路自己的红蓝队列，加入到一个 byte向量中
		vector<BYTE> analyzeXiaoLuRedAndBlue(vector<vector<BYTE>> temps);
		//处理临时储存的数据，把这些数据加入到二维数组中。
		void disposeTempData(int idx, vector<BYTE> nums);
		//导出最终数据。
		void exportData();
	private:
		vector<BYTE>			_AllRightDaLuResultData;	//从服务器上获取的所有结果数据
		vector<vector<BYTE>>	_tempDataList;		        //存放红蓝队列的数据集合
		vector<vector<BYTE>>	_ClassResultData;		    //已经分好类排好序保存的数据

		int dataArray[80][6] = {};              //二维数组 用来存放数据

	public:
		//第六种牌路算法。拿到最后需要比较的两个队列的信息，然后 添加一个庄，或者一个闲赢进去，看看颜色会是什么。
		int judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type);

		int nextZhuang = 0;   //用来保存下一轮开庄的数据
		int nextXian = 0;     //用来保存下一轮开和的数据

		int getNextZhuang();   //返回下一轮开庄的红蓝结果
		int getNextXian();     //返回下一轮开闲的红蓝结果
	};

	class PaiLuRighZhangLangDelegate :public TableViewDelegate   //圈列表蟑螂路
		, public TableViewDataSource
	{
	public:
		PaiLuRighZhangLangDelegate();
		~PaiLuRighZhangLangDelegate();
		/* 必须实现的函数 */
		// 当滑动tableview时触发该方法 参数为当前的tableview对象
		virtual void scrollViewDidScroll(ui::ScrollView* view) {};
		// 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
		virtual void scrollViewDidZoom(ui::ScrollView* view) {}
		// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
		virtual void tableCellTouched(TableView* table, TableViewCell* cell){};
		// 设置tableview的Cell大小
		virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
		// 获取编号为idx的cell
		virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
		// 设置tableview中cell的个数
		virtual ssize_t numberOfCellsInTableView(TableView *table);

		//保存结果数据
		void setAllResultData(vector<BYTE> AllResultData);
		//获取分类数据的大小
		int getClassResultDataSize();

	private:
		//刷新二维数组 用来保存数据 让数据拐弯
		void refreshArray();
		//判断类型，决定下一个数字是否要加入本队列
		int determineType(int num);
		//分析出蟑螂路自己的红蓝队列，加入到一个 byte向量中
		vector<BYTE> analyzeXiaoLuRedAndBlue(vector<vector<BYTE>> temps);
		//处理临时储存的数据，把这些数据加入到二维数组中。
		void disposeTempData(int idx, vector<BYTE> nums);
		//导出最终数据。
		void exportData();
	private:
		vector<BYTE>			_AllRightDaLuResultData;	//从服务器上获取的所有结果数据
		vector<vector<BYTE>>	_tempDataList;		        //存放红蓝队列的数据集合
		vector<vector<BYTE>>	_ClassResultData;		    //已经分好类排好序保存的数据

		int dataArray[80][6] = {};              //二维数组 用来存放数据
	public:
		//第六种牌路算法。拿到最后需要比较的两个队列的信息，然后 添加一个庄，或者一个闲赢进去，看看颜色会是什么。
		int judgeRedOrBlue(vector<BYTE>frist, vector<BYTE>scend, int type);

		int nextZhuang = 0;   //用来保存下一轮开庄的数据
		int nextXian = 0;     //用来保存下一轮开和的数据

		int getNextZhuang();   //返回下一轮开庄的红蓝结果
		int getNextXian();     //返回下一轮开闲的红蓝结果
	};
}
#endif //__NBJL_paiLuDelegate_H__
