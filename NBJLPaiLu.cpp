#include "NBJLPaiLu.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;
using namespace ui;

namespace NBJL
{
	static const char* GAME_PAILU_NODE	= "NBJL/pailu.csb";

	static const char* RED_ROUND = "NBJL/pailu/quan.png";        //红色圈
	static const char* RED_DIAN = "NBJL/pailu/zhuangdui.png";    //红色点
	static const char* RED_XIAN = "NBJL/pailu/xiexian.png";      //红色线
	static const char* BLUE_ROUND = "NBJL/pailu/quan1.png";      //蓝色圈
	static const char* BLUE_DIAN = "NBJL/pailu/xiandui.png";     //蓝色点
	static const char* BLUE_XIAN = "NBJL/pailu/xiexian1.png";    //蓝色线


	NBJLPaiLu::NBJLPaiLu()
	{

	}

	NBJLPaiLu::~NBJLPaiLu()
	{

	}

	NBJLPaiLu* NBJLPaiLu::create(vector<LuziData> luZiDatas)
	{
		NBJLPaiLu* ptr = new NBJLPaiLu();
		if (nullptr != ptr && ptr->init(luZiDatas))
		{
			ptr->autorelease();
			return ptr;
		}
		CC_SAFE_DELETE(ptr);
		return nullptr;
	}

	bool NBJLPaiLu::init(vector<LuziData> luZiDatas)
	{
		if (!HNLayer::init()) return false;

		Size winSize = Director::getInstance()->getWinSize();
		this->setVisible(false);
		auto csNode = CSLoader::createNode(GAME_PAILU_NODE);
		csNode->setPosition(winSize / 2);
		this->addChild(csNode);
		//适配
		auto bg = dynamic_cast<ImageView*> (csNode->getChildByName("Image_Bg"));
		bg->setScale(_winSize.width / 1280, _winSize.height / 720);
		//关闭
		auto btn_close = dynamic_cast<Button*> (bg->getChildByName("Button_close"));
		btn_close->addClickEventListener([=](Ref* pSender){
			if (onCloseCallBack) onCloseCallBack();
		});

		//top层
		image_top = dynamic_cast<ImageView*> (bg->getChildByName("Image_top"));
		zhuang_win = dynamic_cast<Text*> (image_top->getChildByName("Text_zhuang"));
		xian_win = dynamic_cast<Text*> (image_top->getChildByName("Text_xian"));
		he = dynamic_cast<Text*> (image_top->getChildByName("Text_he"));
		gameCount = dynamic_cast<Text*> (image_top->getChildByName("Text_jushu"));

		//第六个牌路需要的显示相关的数据
		quanZhuang = dynamic_cast<ImageView*> (image_top->getChildByName("Image_quan_zhuang"));
		dianZhuang = dynamic_cast<ImageView*> (image_top->getChildByName("Image_dian_zhuang"));
		xianZhuang = dynamic_cast<ImageView*> (image_top->getChildByName("Image_xiexian_zhuang"));
		quanXian = dynamic_cast<ImageView*> (image_top->getChildByName("Image_quan_xian"));
		dianXian = dynamic_cast<ImageView*> (image_top->getChildByName("Image_dian_xian"));
		xianXian = dynamic_cast<ImageView*> (image_top->getChildByName("Image_xiexian_xian"));



		auto plane_down = dynamic_cast<Layout*> (bg->getChildByName("Panel_down"));
		//左边的显示的字层
		leftTableView = dynamic_cast<Layout*> (plane_down->getChildByName("Panel_left"));

		_DownLeftDelegate = new  PaiLuLeftDelegate();
		_DownLeftTable = TableView::create(_DownLeftDelegate, leftTableView->getContentSize());
		_DownLeftTable->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		_DownLeftTable->setDelegate(_DownLeftDelegate);
		_DownLeftTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		leftTableView->addChild(_DownLeftTable);

		//右边上半部分的大路层
		rightDaluTableView = dynamic_cast<Layout*> (plane_down->getChildByName("Panel_rightUp"));
		_DownRightDaluDelegate = new  PaiLuRightDaLuDelegate();
		_DownRightDaluTable = TableView::create(_DownRightDaluDelegate, rightDaluTableView->getContentSize());
		_DownRightDaluTable->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		_DownRightDaluTable->setDelegate(_DownRightDaluDelegate);
		_DownRightDaluTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		rightDaluTableView->addChild(_DownRightDaluTable);

		//右边下半部分的大眼仔层
		rightDaYanZaiView = dynamic_cast<Layout*> (plane_down->getChildByName("Panel_rightDownDaYanZai"));
		_DownRightDaYanZaiDelegate = new  PaiLuRighDaYanZaiDelegate();
		_DownRightDaYanZaiTable = TableView::create(_DownRightDaYanZaiDelegate, rightDaYanZaiView->getContentSize());
		_DownRightDaYanZaiTable->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		_DownRightDaYanZaiTable->setDelegate(_DownRightDaYanZaiDelegate);
		_DownRightDaYanZaiTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		rightDaYanZaiView->addChild(_DownRightDaYanZaiTable);


		//右边下半部分的小路层
		rightXiaoLuView = dynamic_cast<Layout*> (plane_down->getChildByName("Panel_rightDownXiaoLu"));
		_DownRightXiaoLuDelegate = new  PaiLuRighXiaoLuDelegate();
		_DownRightXiaoLuTable = TableView::create(_DownRightXiaoLuDelegate, rightXiaoLuView->getContentSize());
		_DownRightXiaoLuTable->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		_DownRightXiaoLuTable->setDelegate(_DownRightXiaoLuDelegate);
		_DownRightXiaoLuTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		rightXiaoLuView->addChild(_DownRightXiaoLuTable);

		//右边下半部分的蟑螂路层
		rightZhangLangView = dynamic_cast<Layout*> (plane_down->getChildByName("Panel_rightDownZhangLangLu"));
		_DownRightZhangLangDelegate = new  PaiLuRighZhangLangDelegate();
		_DownRightZhangLangTable = TableView::create(_DownRightZhangLangDelegate, rightZhangLangView->getContentSize());
		_DownRightZhangLangTable->setDirection(extension::ScrollView::Direction::HORIZONTAL);
		_DownRightZhangLangTable->setDelegate(_DownRightZhangLangDelegate);
		_DownRightZhangLangTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
		rightZhangLangView->addChild(_DownRightZhangLangTable);

		

		//刷新一次数据
		refreshLuZiData(luZiDatas);
		return true;
	}

	void NBJLPaiLu::isShow(bool isShow)
	{
		this->setVisible(isShow);
	}

	void NBJLPaiLu::refreshLuZiData(vector<LuziData> luZiDatas)
	{
		//把服务器上当前有记录的局数的结果数据拿下来， 一次性最多80个数据，超过了就换新的80个。 chriss
		vector<BYTE> RunSeqs;
		int xianCount = 0;
		int zhuangCount = 0;
		int heCount = 0;
		for (auto luZiData : luZiDatas)
		{
			RunSeqs.push_back(luZiData.byWinResult);
			if (luZiData.byWinResult == 1 || luZiData.byWinResult == 4 || luZiData.byWinResult == 5 || luZiData.byWinResult == 8)
			{
				xianCount++;
			}
			else if (luZiData.byWinResult == 2 || luZiData.byWinResult == 6 || luZiData.byWinResult == 7 || luZiData.byWinResult == 9)
			{
				zhuangCount++;
			}
			else if (luZiData.byWinResult == 3)
			{
				heCount++;
			}
			else
			{

			}
		}
		//top层的显示
		zhuang_win->setString(StringUtils::format(GBKToUtf8("庄 %d"), zhuangCount));
		xian_win->setString(StringUtils::format(GBKToUtf8("闲 %d"), xianCount));
		he->setString(StringUtils::format(GBKToUtf8("和 %d"), heCount));
		gameCount->setString(StringUtils::format(GBKToUtf8("局数 %d"), 80));

		//-------------------------字--------------------------//
		//保存新的字列表数据，然后刷新
		_DownLeftDelegate->setAllResultData(RunSeqs);
		_DownLeftTable->reloadData();

		//字界面往前移动，当当前这一行满了的话
		int NumColLeft = 0;
		for (int i = 0; i < RunSeqs.size(); i++)
		{
			if (RunSeqs[i] > 9 || RunSeqs[i] < 0)   //因为字的默认值是255
			{
				NumColLeft = (i + 1) / leftListitemCount;
				break;
			}
		}

		if (NumColLeft == 0)
		{
			NumColLeft = (RunSeqs.size() / leftListitemCount);
		}

		if (RunSeqs.size() % leftListitemCount)
		{
			NumColLeft++;
		}

		if (NumColLeft > leftListCount)
		{
			float offset = -leftSizeWidth * (NumColLeft - leftListCount);
			_DownLeftTable->setContentOffset(Vec2(offset, 0));
		}

		//-------------------------右上方大路圈--------------------------//
		_DownRightDaluDelegate->setAllResultData(RunSeqs);
		_DownRightDaluTable->reloadData();

		int NumCol_RightDalu = _DownRightDaluDelegate->getClassResultDataSize();
		if (NumCol_RightDalu >rightLongListCount)
		{
			float offset = -rightSizeWidth * (NumCol_RightDalu - rightLongListCount);
			_DownRightDaluTable->setContentOffset(Vec2(offset, 0));
		}

		//-------------------------右下方大眼仔圈--------------------------//
		_DownRightDaYanZaiDelegate->setAllResultData(RunSeqs);
		_DownRightDaYanZaiTable->reloadData();
		int NumCol_RightDaYanZai = _DownRightDaYanZaiDelegate->getClassResultDataSize();
		if (NumCol_RightDaYanZai > rightLongListCount)
		{
			float offset = -rightSizeWidth * (NumCol_RightDaYanZai - rightLongListCount);
			_DownRightDaYanZaiTable->setContentOffset(Vec2(offset, 0));
		}
		//下一轮开庄的话
		switch (_DownRightDaYanZaiDelegate->getNextZhuang())
		{
			case XiaSanLuType::NOne:
				quanZhuang->setVisible(false);
				break;
			case XiaSanLuType::red:
				quanZhuang->loadTexture(RED_ROUND);
				break;
			case XiaSanLuType::blue:
				quanZhuang->loadTexture(BLUE_ROUND);
				break;
			default:
				break;
		}
		//下一轮开闲的话
		switch (_DownRightDaYanZaiDelegate->getNextXian())
		{
		case XiaSanLuType::NOne:
			quanZhuang->setVisible(false);
			break;
		case XiaSanLuType::red:
			quanXian->loadTexture(RED_ROUND);
			break;
		case XiaSanLuType::blue:
			quanXian->loadTexture(BLUE_ROUND);
			break;
		default:
			break;
		}
		//-------------------------右下方小路圈--------------------------//
		_DownRightXiaoLuDelegate->setAllResultData(RunSeqs);
		_DownRightXiaoLuTable->reloadData();
		int NumCol_RightXiaoLu = _DownRightXiaoLuDelegate->getClassResultDataSize();
        if (NumCol_RightXiaoLu > rightShotListCount)
		{
			float offset = -rightSizeWidth * (NumCol_RightXiaoLu - rightShotListCount);
			_DownRightXiaoLuTable->setContentOffset(Vec2(offset, 0));
		}
		//下一轮开庄的话
		switch (_DownRightXiaoLuDelegate->getNextZhuang())
		{
		case XiaSanLuType::NOne:
			dianZhuang->setVisible(false);
			break;
		case XiaSanLuType::red:
			dianZhuang->loadTexture(RED_DIAN);
			break;
		case XiaSanLuType::blue:
			dianZhuang->loadTexture(BLUE_DIAN);
			break;
		default:
			break;
		}
		//下一轮开闲的话
		switch (_DownRightXiaoLuDelegate->getNextXian())
		{
		case XiaSanLuType::NOne:
			dianXian->setVisible(false);
			break;
		case XiaSanLuType::red:
			dianXian->loadTexture(RED_DIAN);
			break;
		case XiaSanLuType::blue:
			dianXian->loadTexture(BLUE_DIAN);
			break;
		default:
			break;
		}
		//-------------------------右下方蟑螂圈--------------------------//
		_DownRightZhangLangDelegate->setAllResultData(RunSeqs);
		_DownRightZhangLangTable->reloadData();
		int NumCol_RightZhangLang = _DownRightZhangLangDelegate->getClassResultDataSize();
		if (NumCol_RightZhangLang > rightShotListCount)
		{
			float offset = -rightSizeWidth * (NumCol_RightZhangLang - rightShotListCount);
			_DownRightZhangLangTable->setContentOffset(Vec2(offset, 0));
		}
		//下一轮开庄的话
		switch (_DownRightZhangLangDelegate->getNextZhuang())
		{
		case XiaSanLuType::NOne:
			xianZhuang->setVisible(false);
			break;
		case XiaSanLuType::red:
			xianZhuang->loadTexture(RED_XIAN);
			break;
		case XiaSanLuType::blue:
			xianZhuang->loadTexture(BLUE_XIAN);
			break;
		default:
			break;
		}
		//下一轮开闲的话
		switch (_DownRightZhangLangDelegate->getNextXian())
		{
		case XiaSanLuType::NOne:
			xianXian->setVisible(false);
			break;
		case XiaSanLuType::red:
			xianXian->loadTexture(RED_XIAN);
			break;
		case XiaSanLuType::blue:
			xianXian->loadTexture(BLUE_XIAN);
			break;
		default:
			break;
		}
	}
}
