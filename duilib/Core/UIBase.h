#ifndef __UIBASE_H__
#define __UIBASE_H__

#pragma once

namespace DuiLib
{
/////////////////////////////////////////////////////////////////////////////////////
//

#define UI_WNDSTYLE_CONTAINER (0)
#define UI_WNDSTYLE_FRAME (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER (0)
#define UI_CLASSSTYLE_FRAME (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)

/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT
#define ASSERT(expr) _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef DUITRACE
#define DUITRACE DUI__Trace
#endif
#define DUITRACEMSG DUI__TraceMsg
#else
#ifndef DUITRACE
#define DUITRACE
#endif
#define DUITRACEMSG _T("")
#endif

void DUILIB_API DUI__Trace(LPCTSTR pstrFormat, ...);
LPCTSTR DUILIB_API DUI__TraceMsg(UINT uMsg);

/////////////////////////////////////////////////////////////////////////////////////
//

/**
 * @brief 
 * 
 */
class DUILIB_API CNotifyPump
{
public:
    /**
     * @brief 
     * 
     * @param strName 
     * @param pObject 
     * @return true 
     * @return false 
     */
    bool AddVirtualWnd(CDuiString strName, CNotifyPump *pObject);
    /**
     * @brief 
     * 
     * @param strName 
     * @return true 
     * @return false 
     */
    bool RemoveVirtualWnd(CDuiString strName);
    /**
     * @brief 
     * 
     * @param msg 
     */
    void NotifyPump(TNotifyUI &msg);
    /**
     * @brief 
     * 
     * @param msg 
     * @return true 
     * @return false 
     */
    bool LoopDispatch(TNotifyUI &msg);
    /**
     * @brief Construct a new dui declare message map object
     * 
     */
    DUI_DECLARE_MESSAGE_MAP()
private:
    CDuiStringPtrMap m_VirtualWndMap;
};

/**
 * @brief 窗口基类
 * 
 */
class DUILIB_API CWindowWnd
{
public:
    CWindowWnd();
    /**
     * @brief 获得当前窗口句柄
     * 
     * @return HWND 当前窗口句柄 
     */
    HWND GetHWND() const;
    operator HWND() const;

    /**
     * @brief  窗口类注册
     * 
     * @return true  注册成功
     * @return false  注册失败
     */
    bool RegisterWindowClass();
    /**
     * @brief 父类窗口注册
     * 
     * @return true 注册成功
     * @return false 注册失败
     */
    bool RegisterSuperclass();

    /**
     * @brief 创建一个窗口
     * 
     * @param hwndParent 父窗口的句柄
     * @param pstrName 指向窗口名称的指针
     * @param dwStyle 窗口风格
     * @param dwExStyle 窗口的扩展风格
     * @param rc 窗口尺寸大小
     * @param hMenu 菜单的句柄或是子窗口的标识符
     * @return HWND 创建的窗口句柄
     */
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    /**
     * @brief 创建一个窗口
     * 
     * @param hwndParent 父窗口的句柄
     * @param pstrName 指向窗口名称的指针
     * @param dwStyle 窗口风格
     * @param dwExStyle 窗口的扩展风格
     * @param x 窗口的水平位置
     * @param y 窗口的垂直位置
     * @param cx 窗口的宽度
     * @param cy 窗口的高度
     * @param hMenu 菜单的句柄或是子窗口的标识符
     * @return HWND 创建的窗口句柄
     */
    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
    /**
     * @brief 创建一个DUI窗口
     * 
     * @param hwndParent 父窗口的句柄
     * @param pstrWindowName 窗口类名
     * @param dwStyle 窗口风格
     * @param dwExStyle 窗口的扩展风格
     * @return HWND 创建的窗口句柄
     */
    HWND CreateDuiWindow(HWND hwndParent, LPCTSTR pstrWindowName, DWORD dwStyle = 0, DWORD dwExStyle = 0);
    /**
     * @brief 子类化
     * 
     * 子类化指改变一个已创建窗口的行为。
     * 
     * @param hWnd 需要子类化的窗口句柄
     * @return HWND 
     */
    HWND Subclass(HWND hWnd);
    /**
     * @brief 取消子类化
     * 
     */
    void Unsubclass();
    /**
     * @brief 显示当前窗口
     * 
     * @param bShow 是否显示当前窗口
     * @param bTakeFocus 是否激活当前窗口
     */
    void ShowWindow(bool bShow = true, bool bTakeFocus = true);
    /**
     * @brief  显示模态框
     * 
     * @return UINT 窗口的激活状态？
     * 
     *  WA_ACTIVE       被非鼠标点击激活的状态
     *  WA_CLICKACTIVE  被鼠标点击激活的状态
     *  WA_INACTIVE     非激活状态
     */
    UINT ShowModal();
    /**
     * @brief 关闭当前窗口
     * 
     * @param nRet 关闭的状态码
     */
    void Close(UINT nRet = IDOK);
    /**
     * @brief 居中当前窗口，支持扩展屏幕
     * 
     */
    void CenterWindow();
    /**
     * @brief 设置窗口图标
     * 
     * @param nRes 图标资源
     */
    void SetIcon(UINT nRes);

    /**
     * @brief 发送指定消息到当前窗口
     * 
     * SendMessage函数将指定的消息发到窗口。它调用特定窗口的窗口处理函数，并且不会立即返回，直到窗口处理函数处理了这个消息
     * 
     * @param uMsg 需要发送的消息
     * @param wParam 额外的消息相关信息
     * @param lParam 额外的消息相关信息
     * @return LRESULT 消息处理结果，视具体消息而定
     */
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    /**
     * @brief 发送指定消息到当前窗口
     * 
     * PostMessage将一个消息放入与创建这个窗口的消息队列相关的线程中，并立刻返回不等待线程处理消息
     * 
     * @param uMsg 需要发送的消息
     * @param wParam 额外的消息相关信息
     * @param lParam 外的消息相关信息
     * @return LRESULT 消息处理结果，视具体消息而定
     */
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    /**
     * @brief 重置当前窗口大小
     * 
     * @param cx 以像素指定窗口的新的宽度
     * @param cy 以像素指定窗口的新的高度
     */
    void ResizeClient(int cx = -1, int cy = -1);

protected:
    virtual LPCTSTR GetWindowClassName() const = 0;
    virtual LPCTSTR GetSuperClassName() const;
    virtual UINT GetClassStyle() const;

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnFinalMessage(HWND hWnd);

    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;
    WNDPROC m_OldWndProc;
    bool m_bSubclassed;
};

} // namespace DuiLib

#endif // __UIBASE_H__
