
// wafer_final_mingView.h: CwaferfinalmingView 类的接口
//

#pragma once


class CwaferfinalmingView : public CView
{
protected: // 仅从序列化创建
	CwaferfinalmingView() noexcept;
	DECLARE_DYNCREATE(CwaferfinalmingView)

// 特性
public:
	CwaferfinalmingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CwaferfinalmingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // wafer_final_mingView.cpp 中的调试版本
inline CwaferfinalmingDoc* CwaferfinalmingView::GetDocument() const
   { return reinterpret_cast<CwaferfinalmingDoc*>(m_pDocument); }
#endif

