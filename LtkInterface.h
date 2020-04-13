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
 * 详细
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

//LTK_API void WINAPI LtkEnalbeApiCheck(BOOL);

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


#define LtkWindow_New() LtkWindow_New_(__FILE__, __LINE__)
LTK_API HLTK WINAPI LtkWindow_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkWindow_Create(HLTK self, HWND parent, LtkRect* rc);
LTK_API void WINAPI LtkWindow_CreateCenter(HLTK self, HWND parent, float width, float height);
LTK_API void WINAPI LtkWindow_SetCaption(HLTK self, LPCSTR text);
LTK_API void WINAPI LtkWindow_SetBackground(HLTK self, LPCSTR name);
LTK_API void WINAPI LtkWindow_UpdateTheme(HLTK self);
LTK_API void WINAPI LtkWindow_SetCentralWidget(HLTK self, HLTK widget);
LTK_API void WINAPI LtkWindow_SetMenu(HLTK self, HLTK menu_bar);
LTK_API HWND WINAPI LtkWindow_GetHWND(HLTK self);

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

LTK_API void WINAPI LtkButton_SetText(HLTK self, LPCWSTR);

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
LTK_API void WINAPI LtkTreeNode_SetText(HLTK self, LPCWSTR text);

LTK_DECLARE_TYPE(LtkTextEdit); // 基类：LtkWidget

#define LTK_TEXTEDIT(o) LtkIsTextEdit(o) ? (LtkTextEdit*)o : NULL
LTK_API BOOL WINAPI LtkIsTextEdit(HLTK o);

#define LtkTextEdit_New() LtkTextEdit_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkTextEdit_New_(LPCSTR source, int line);

LTK_DECLARE_TYPE(LtkMenuItem);

#define LTK_MENU_CLICK 601

#define LtkMenuBar_New() LtkMenuBar_New_(  __FILE__, __LINE__)
LTK_API HLTK WINAPI LtkMenuBar_New_(LPCSTR source, int line);

LTK_API void WINAPI LtkMenuBar_AddItem(HLTK self, LPCWSTR text);
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
