#ifndef __NBJL_TableCell_H__
#define __NBJL_TableCell_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "HNNetExport.h"
#include "NBJLMessageHead.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace ui;
using namespace extension;

namespace NBJL
{
	/// 字列表结构
	struct DownLeftStruct
	{
		bool	bRefresh;			//是否刷新
		BYTE	ZiVaule[6];			//字值  一列有6个
		DownLeftStruct()
		{
			memset(this, 0, sizeof(DownLeftStruct));
		}
	};
	//圈列表上半部分大路的趋势结构
	struct DownRightDaLuStruct
	{
		bool	bRefresh;			    //是否刷新
		bool	bLastHe;			    //最后一行是否为和（用于之前列剩余数据）
		vector<BYTE>	QuanVaule;		//大路圈值
		DownRightDaLuStruct()
		{
			memset(this, 0, sizeof(DownRightDaLuStruct));
		}
	};

	//字列表
	class DownLeftItemCell :public TableViewCell
	{
	public:
		static DownLeftItemCell* create();
	public:
		void bulidCell(DownLeftStruct DownLeftInfo);

	private:
		Layout* _layout;

		DrawNode* drawNode= nullptr;          //绘制工具
	private:
		void drawRedPoint(int idx);             //绘制一个红色点  在图片的左上角

		void drawBluePoint(int idx);            //绘制一个蓝色点  在图片的右下角

		void drawRedAndBluePoint(int idx);      //绘制红，蓝色，两个点
	};
	//大路圈列表
	class DownRightDaLuItemCell :public TableViewCell
	{
	public:
		static DownRightDaLuItemCell* create();
	public:
		void bulidCell(struct DownRightDaLuStruct  DownRightInfo);
	private:
		Layout* _layout;
		DrawNode* drawNode = nullptr;           //绘制工具
		Text*   text_he = nullptr;              //用来生成text 显示和
	private:
		void drawRedPoint(int idx);             //绘制一个红色点  在左上角

		void drawBluePoint(int idx);            //绘制一个蓝色点  在右下角

		void drawRedAndBluePoint(int idx);      //绘制红，蓝色，两个点

		void drawRedRound(int idx);             //绘制一个红色圈  

		void drawBlueRound(int idx);            //绘制一个蓝色圈  
	};

	//大眼仔圈列表
	class DownRightDaYanZaiItemCell :public TableViewCell
	{
	public:
		static DownRightDaYanZaiItemCell* create();
	public:
		void bulidCell(struct DownRightDaLuStruct  DownRightInfo);
	private:
		Layout* _layout;
		DrawNode* drawNode = nullptr;           //绘制工具
		Text*   text_he = nullptr;              //用来生成text 显示和
	private:
	    
		void drawRedRound(int x);            //绘制一个红色圈  

		void drawBlueRound(int x);            //绘制一个蓝色圈  
	};

	//小路圈列表
	class DownRightXiaoLuItemCell :public TableViewCell
	{
	public:
		static DownRightXiaoLuItemCell* create();
	public:
		void bulidCell(struct DownRightDaLuStruct  DownRightInfo);
	private:
		Layout* _layout;
		DrawNode* drawNode = nullptr;           //绘制工具
		Text*   text_he = nullptr;              //用来生成text 显示和
	private:

		void drawRedDot(int idx);            //绘制一个红色圈  

		void drawBlueDot(int idx);            //绘制一个蓝色圈  
	};

	//蟑螂路圈列表
	class DownRightZhangLangItemCell :public TableViewCell
	{
	public:
		static DownRightZhangLangItemCell* create();
	public:
		void bulidCell(struct DownRightDaLuStruct  DownRightInfo);
	private:
		Layout* _layout;
		DrawNode* drawNode = nullptr;           //绘制工具
		Text*   text_he = nullptr;              //用来生成text 显示和
	private:

		void drawRedLine(int idx);
		void drawBlueLine(int idx);            //绘制一个蓝色圈  
	};
}
#endif // __NBJL_TableCell_H__