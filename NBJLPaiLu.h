#ifndef __NBJLPaiLu_h__
#define __NBJLPaiLu_h__

#include "NBJLMessageHead.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include <vector>
#include "NBJLpaiLuDelegate.h"
USING_NS_CC;

namespace NBJL
{
	class NBJLPaiLu : public HNLayer
	{
	public:
		static NBJLPaiLu* create(vector<LuziData> luZiDatas);
		bool init(vector<LuziData> luZiDatas);

		NBJLPaiLu();
		virtual ~NBJLPaiLu();
   
		typedef std::function<void()> CloseCallBack;
		CloseCallBack	onCloseCallBack = nullptr;
	private: //中间的数据层
		Layout*       leftTableView               = nullptr;         //左边的字列表窗口
		Layout*       rightDaluTableView          = nullptr;         //右边上半部分的大路 窗口

		//牌路视窗
		TableView*    _DownLeftTable              = nullptr;         //左边的字窗口
		TableView*    _DownRightDaluTable             = nullptr;         //右边的上半部分窗口

		//牌路数据
		PaiLuLeftDelegate*       _DownLeftDelegate            = nullptr;     //左边的字窗口里的数据
		PaiLuRightDaLuDelegate*  _DownRightDaluDelegate       = nullptr;     //右边的上半部分窗口里的大路数据

	private:  //大眼仔层
		Layout*       rightDaYanZaiView = nullptr;                        //右边的大眼仔层
		//牌路视窗
		TableView*    _DownRightDaYanZaiTable = nullptr;                  //右边的大眼仔窗口
		//牌路数据
		PaiLuRighDaYanZaiDelegate*       _DownRightDaYanZaiDelegate = nullptr;     //左边的字窗口里的数据

	private:  //小路层
		Layout*       rightXiaoLuView = nullptr;                        //右边的大眼仔层
		//牌路视窗
		TableView*    _DownRightXiaoLuTable = nullptr;                  //右边的大眼仔窗口
		//牌路数据
		PaiLuRighXiaoLuDelegate*       _DownRightXiaoLuDelegate = nullptr;     //左边的字窗口里的数据

	private:  //蟑螂路层
		Layout*       rightZhangLangView = nullptr;                        //右边的大眼仔层
		//牌路视窗
		TableView*    _DownRightZhangLangTable = nullptr;                  //右边的大眼仔窗口
		//牌路数据
		PaiLuRighZhangLangDelegate*       _DownRightZhangLangDelegate = nullptr;     //左边的字窗口里的数据
	private:   // top 层
		ImageView*   image_top                   = nullptr;         //走势top层
		Text*        zhuang_win                  = nullptr;         //庄赢的局数
		Text*        xian_win                    = nullptr;         //庄赢的局数
		Text*        he                          = nullptr;         //庄赢的局数
		Text*        gameCount                   = nullptr;         //当前游戏局数   
		ImageView*   quanZhuang                  = nullptr;         //大眼仔 下一轮开庄的显示
		ImageView*   dianZhuang                  = nullptr;         //小路 下一轮开庄的显示
		ImageView*   xianZhuang                  = nullptr;         //蟑螂路 下一轮开庄的显示
		
		ImageView*   quanXian                    = nullptr;         //大眼仔 下一轮开庄的显示
		ImageView*   dianXian                    = nullptr;         //小路 下一轮开庄的显示
		ImageView*   xianXian                    = nullptr;         //蟑螂路 下一轮开庄的显示
	public:
		void isShow(bool isShow);   //界面是否显示

		void refreshLuZiData(vector<LuziData> luZiDatas);
	};
}

#endif // __NBJLPaiLu_h__
