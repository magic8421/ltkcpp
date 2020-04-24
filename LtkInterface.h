#pragma once

/*! \mainpage LTK轻量界面库
 *
 * \section intro_sec 简介
 *
 * 点击查看文档: LtkInterface.h
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */

#ifdef LTK_EXPORTS
#define LTK_API
#else
#define LTK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LTK_DECLARE_TYPE(type) \
 typedef struct _##type { \
	int dummy; \
} type

typedef struct _LtkRect {
	float x;
	float y;
	float width;
	float height;
} LtkRect;

typedef struct _LtkPoint
{
    float x;
    float y;
} LtkPoint;

typedef struct _LtkSize
{
    float width;
    float height;
} LtkSize;

typedef struct _LtkMargin
{
    float left;
    float top;
    float right;
    float bottom;
} LtkMargin;

enum
{
    LTK_VAR_STRING,
    LTK_VAR_REAL,
    LTK_VAR_INTEGER,
    LTK_VAR_RECT,
    LTK_VAR_SIZE,
    LTK_VAR_MARGIN,
};

typedef struct _LtkVariant
{
    UINT type;
    union
    {
        LPCSTR psz;
        float real;
        int	integer;
        LtkRect rect;
        LtkPoint point;
        LtkSize size;
        LtkMargin margin;
    } u;
    char reserved[16];
} LtkVariant;


enum LtkStatus
{
    LtkOk = 0,
    LtkInvalidHandle,
    LtkTypeError
};


/**
 * @brief 初始化LTK
 *
 * 在调用任何LTK的API之前调用
 */
LTK_API UINT WINAPI LtkInitialize();

/**
 * @brief 退出LTK
 *
 * 在程序退出前调用
 */
LTK_API void WINAPI LtkUninitialize();

/**
 * @brief 进入消息循环
 *
 * 阻塞直到用户代码调用PostQuitMessage
 */
LTK_API void WINAPI LtkRunMessageLoop();

struct _HLTK {
	int dummy;
};

/**
 * @brief LTK通用句柄类型
 */
typedef _HLTK* HLTK;

/**
 * @brief 销毁HLTK句柄
 *
 * 可传入任何HLTK类型 此函数会调用相应的析构函数释放内存 并发送 LTK_DELETE_EVENT 消息
 */
LTK_API void WINAPI LtkDelete(HLTK obj);

/**
 * @brief 延时销毁HLTK句柄
 *
 * 此函数会立即返回 并且在下一次消息处理时销毁HLTK句柄 方便处理某些特殊情况
 * 比如在消息响应函数中无法销毁控件自身 可以使用此函数
 */
LTK_API void WINAPI LtkDeleteLater(HLTK obj);


LTK_API BOOL WINAPI LtkIsValid(HLTK obj);

/**
 * @brief LTK回调函数
 *
 * 通用回调函数类型 注意：每个消息码所对应的回调函数参数均不相同 请参考具体消息码的文档
 */
typedef int (CALLBACK *LtkCallback)(void *userdata);

/**
 * @brief 内部化字符串
 *
 * 返回一个经过内部化的字符串指针 内部化的字符串指针可以直接和其他内部化的字符串指针进行比较
 * 地址相等即字符串内容相等 而不必调用strcmp() 调用者不必释放返回的指针 系统会在退出时释放
 * 内容相同的字符串会共用一片内存区域
 * 建议此函数只用于字符串字面量 而不可用于程序外部输入数据 以免浪费过多内存
 * 建议在程序启动时做一次内部化 保存在静态变量即可 而不是每次使用时 因为每次调用会查询哈希表
 */
LTK_API LPCSTR WINAPI LtkInternString(LPCSTR str);

/**
 * @brief 设置对象的名称
 *
 * 设置对象的名称 主要用于回调函数中区分消息来源 在xml中由name属性设置
 */
LTK_API void WINAPI LtkSetName(HLTK o, LPCSTR name);

/**
 * @brief 取对象的名称
 *
 * 取对象的名称 在xml中由name属性设置 在代码中由LtkSetName()设置 返回的指针是内部化的字符串指针
 * 可以与任何经过 LtkInternString() 的指针直接比较地址相等即可
 */
LTK_API LPCSTR WINAPI LtkGetName(HLTK o);

/**
 * @brief 注册消息回调
 *
 * @param obj 发出消息的对象句柄
 * @param event_id 消息码
 * @param cb 回调函数 对于每一个消息码都有不同的回调参数 LtkCallback 只是占位符 并不是实际参数 请参考消息码文档
 * @param userdata 用户指针 一般传this
 */
LTK_API void WINAPI LtkRegisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);


LTK_API void WINAPI LtkUnregisterCallback(HLTK obj, UINT event_id, LtkCallback cb, void* userdata);
LTK_API void WINAPI LtkUnregisterCallbackByUserdata(HLTK obj, UINT event_id, void* userdata);

/**
 * @brief 获取发送消息的对象句柄
 *
 * 仅在消息回调函数中有效 请在第一时间取得这个句柄并保存在局部变量中 如果接下来调用任何LTK api都有可能覆盖这个值
 */
LTK_API HLTK WINAPI LtkGetEventSender();

LTK_API void* WINAPI LtkGetProp(LPCSTR name);
LTK_API void WINAPI LtkSetProp(LPCSTR name, void *data);

/**
 * @brief 通过xml构建对象树
 *
 * @param path xml文件的路径 建议使用相对路径
 */
LTK_API HLTK WINAPI LtkBuildFromXml(LPCSTR path);

LTK_API void WINAPI LtkEnalbeApiCheck(BOOL);
LTK_API void WINAPI LtkEnalbeBreakOnError(BOOL);
LTK_API UINT WINAPI LtkGetLastError();
LTK_API void WINAPI LtkSetLastError(UINT);

#define LTK_CREATE_EVENT	1
// BOOL(void *userdata)
#define LTK_DELETE_EVENT    2
#define LTK_PAINT_EVENT		3
#define LTK_SIZE_EVENT		4

#define LTK_MOUSE_FIRST			5
#define LTK_MOUSE_MOVE			6
#define LTK_MOUSE_LEAVE			7
#define LTK_MOUSE_WHEEL			8
#define LTK_LBUTTON_DOWN		9
#define LTK_LBUTTON_UP			10
#define LTK_LBUTTON_DBCLICK		11
#define LTK_RBUTTON_DOWN		12
#define LTK_RBUTTON_UP			13
#define LTK_MOUSE_LAST			14

#define LTK_KEY_DOWN			15
#define LTK_KEY_UP				16
#define LTK_CHAR_INPUT			17
#define LTK_IME_INPUT			18

#define LTK_SET_FOCUS			19
#define LTK_KILL_FOCUS			20

#define LTK_RECREATE_RESOURCE	21
#define LTK_THEME_CHANGED		22

typedef struct _LtkEvent
{
	UINT id;
	HLTK sender;
} LtkEvent;

typedef struct _LtkMouseEvent
{
	LtkEvent hdr;
	float x;
	float y;
	UINT flag;		// ctrl shift atl
	float delta;	// wheel
} LtkMouseEvent;

typedef struct _LtkDelegateMouseEvent
{
	LtkEvent hdr;
	LtkMouseEvent* data;
	BOOL bHandled;
} LtkDelegateMouseEvent;

typedef struct _LtkKeyEvent
{
	LtkEvent hdr;
	DWORD keyCode;
	DWORD flag;
} LtkKeyEvent;

struct ID2D1RenderTarget;

typedef struct _LtkPaintEvent
{
	LtkEvent hdr;
	ID2D1RenderTarget* target;
} LtkPaintEvent;

typedef struct _LtkSizeEvent
{
	LtkEvent hdr;
	float width;
	float height;
} LtkSizeEvent;

typedef struct _LtkImeEvent
{
	LtkEvent hdr;
	LPCWSTR text;
} LtkImeEvent;

typedef struct _LtkFocusEvent
{
	LtkEvent hdr;
	HLTK oldFocus;
} LtkFocusEvent;

typedef struct _LtkRecreateResource {
	LtkEvent hdr;
	ID2D1RenderTarget* target;
} LtkRecreateResource;


// LtkMenuBar 基类：LtkWidget
// LtkPopupMenu 基类：LtkWidget


// BOOL(void *userdata, BOOL *pProceed)
#define LTK_WINDOW_CLOSE		102

// BOOL(void *userdata)
#define LTK_WINDOW_DESTROY      103

/**
 * @brief 创建窗体对象
 *
 * 创建一个顶层窗口 此函数仅创建LTK对象 并不创建HWND 
 * 参见 LtkWindow_Create() LtkWindow_CreateCenter()
 */
#define LtkWindow_New() LtkWindow_New_(__FILE__, __LINE__)

 /**
  * @brief 创建窗体对象
  *
  * 参见 LtkWindow_New() 易语言调用参数请填0
  */
LTK_API HLTK WINAPI LtkWindow_New_(LPCSTR source, int line);

LTK_API BOOL WINAPI LtkWindow_CheckType(HLTK);

/**
 * @brief 创建窗口HWND 并显示
 *
 * @param parent 父窗口句柄
 * @param rc 窗口矩形
 */
LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc);

/**
 * @brief 创建窗口HWND 并居中显示
 *
 * @param parent 父窗口句柄
 * @param width 窗口宽度
 * @param height 窗口高度
 */
LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height);

/**
 * @brief 设置窗口标题
 *
 * 请在创建HWND之后再调用此函数 否则任务栏上不能显示标题文字
 */
LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCSTR text);

/**
 * @brief 设置窗口背景
 *
 * @param name 背景的资源名称 需要在背景xml中定义
 */
LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name);

/**
 * @brief 刷新所有主题及布局
 *
 * 刷新所有主题及布局 控件会去背景管理器中取得当前主题对应的资源
 */
LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self);

/**
 * @brief 设置客户区控件
 *
 * 设置客户区控件 同时窗口负责管理这个控件及其所有自控件的生命周期
 */
LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget);

/**
 * @brief 设置菜单条
 *
 * @param menu_bar 菜单条 类型 LtkMenuBar
 */
LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar);

/**
 * @brief 取HWND
 *
 * 获取这个窗口管理的HWND句柄
 */
LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self);


LTK_API void WINAPI LtkStyleManager_SwitchTheme(LPCSTR name);


// LtkBoxLayout 基类：LtkWidget

#define LTK_HORIZONTAL  1
#define LTK_VERTICAL    2

#define LtkBoxLayout_New(o) LtkBoxLayout_New_(o, __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkBoxLayout_New_(UINT orientation, LPCSTR source, int line);

LTK_API void WINAPI LtkBoxLayout_AddLayoutItem(HLTK self, HLTK widget, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_AddSpaceItem(HLTK self, float preferedSize, float growFactor);
LTK_API void WINAPI LtkBoxLayout_SetSpacing(HLTK self, float spacing);

// LtkButton 基类：LtkWidget

// void CALLBACK OnAction(void* userdata, LPCSTR name, BOOL *bHandled)
#define LTK_ACTION		201

#define LtkButton_New() LtkButton_New_(__FILE__, __LINE__)
LTK_API HLTK WINAPI LtkButton_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkButton_SetText(HLTK self, LPCSTR);

// LtkHeaderCtrl 基类：LtkWidget

#define LtkHeaderCtrl_New() LtkHeaderCtrl_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkHeaderCtrl_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkHeaderCtrl_AddColumn(HLTK self, LPCSTR text, float width);



// LtkListView 基类：LtkWidget

// void CALLBACK OnListViewSelectChange(void* userdata, int row, int oldRow)
#define LTK_LISTVIEW_SELECT_CHANGE		301

// void CALLBACK OnListViewDbClick(void* userdata, int row) // TODO NOT IMPLEMENTED
#define LTK_LISTVIEW_DBCLICK			302


#define LtkListView_New() LtkListView_New_( __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkListView_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkListView_AddColumn(HLTK self, LPCSTR text, float width);
LTK_API UINT WINAPI LtkListView_AddRow(HLTK self);
LTK_API void WINAPI LtkListView_SetCellText(HLTK self, UINT row, UINT col, LPCWSTR text);
LTK_API LPCWSTR WINAPI LtkListView_GetCellText(HLTK self, UINT row, UINT col);
LTK_API int WINAPI LtkListView_GetSelectedRow(HLTK self);
LTK_API HLTK WINAPI LtkListView_GetHeaderCtrl(HLTK self);
LTK_API void WINAPI LtkListView_UpdateColumnWidth(HLTK self);

// LtkSplitter 基类：LtkWidget

#define LtkSplitter_New(o) LtkSplitter_New_(o,  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkSplitter_New_(UINT orientation, LPCSTR source, int line);
LTK_API UINT WINAPI LtkSplitter_AddClient(HLTK self, HLTK widget);
LTK_API void WINAPI LtkSplitter_SetClientSize(HLTK self, UINT idx, float size);

// LtkTreeView 基类：LtkWidget

#define LtkTreeView_New() LtkTreeView_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTreeView_New_(LPCSTR source, int line);

// LtkTreeNode 基类：LtkObject

LTK_API HLTK WINAPI LtkTreeView_GetRootNode(HLTK self);

#define LTK_TREEVIEW_SELECT_CHANGE 501

#define LtkTreeNode_New() LtkTreeNode_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTreeNode_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkTreeNode_AddChild(HLTK self, HLTK node);
LTK_API void WINAPI LtkTreeNode_SetText(HLTK self, LPCSTR text);

LTK_DECLARE_TYPE(LtkTextEdit); // 基类：LtkWidget

#define LTK_TEXTEDIT(o) LtkIsTextEdit(o) ? (LtkTextEdit*)o : NULL
LTK_API BOOL WINAPI LtkIsTextEdit(HLTK o);

#define LtkTextEdit_New() LtkTextEdit_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTextEdit_New_(LPCSTR source, int line);

LTK_DECLARE_TYPE(LtkMenuItem);

#define LTK_MENU_CLICK 601

#define LtkMenuBar_New() LtkMenuBar_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkMenuBar_New_(LPCSTR source, int line);

LTK_API UINT WINAPI LtkMenuBar_AddItem(HLTK self, LPCSTR text);
LTK_API void WINAPI LtkMenuBar_SetPopupMenu(HLTK self, UINT idx, HLTK popup);

#define LtkPopupMenu_New() LtkPopupMenu_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkPopupMenu_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkPopupMenu_AddItem(HLTK self, LPCSTR text, LPCSTR name);
LTK_API void WINAPI LtkPopupMenu_AddSeparator(HLTK self);
LTK_API void WINAPI LtkPopupMenu_SetWidth(HLTK self, float width);
LTK_API void WINAPI LtkPopupMenu_SetSubMenu(HLTK self, UINT idx, HLTK popup);

#ifdef __cplusplus
} // extern "C"
#endif
