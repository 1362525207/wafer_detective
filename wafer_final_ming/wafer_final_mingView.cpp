
// wafer_final_mingView.cpp: CwaferfinalmingView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "wafer_final_ming.h"
#endif

#include "wafer_final_mingDoc.h"
#include "wafer_final_mingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwaferfinalmingView

IMPLEMENT_DYNCREATE(CwaferfinalmingView, CView)

BEGIN_MESSAGE_MAP(CwaferfinalmingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CwaferfinalmingView 构造/析构

CwaferfinalmingView::CwaferfinalmingView() noexcept
{
	// TODO: 在此处添加构造代码

}

CwaferfinalmingView::~CwaferfinalmingView()
{
}

BOOL CwaferfinalmingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CwaferfinalmingView 绘图

void CwaferfinalmingView::OnDraw(CDC* /*pDC*/)
{
	CwaferfinalmingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CwaferfinalmingView 打印

BOOL CwaferfinalmingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CwaferfinalmingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CwaferfinalmingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CwaferfinalmingView 诊断

#ifdef _DEBUG
void CwaferfinalmingView::AssertValid() const
{
	CView::AssertValid();
}

void CwaferfinalmingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CwaferfinalmingDoc* CwaferfinalmingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CwaferfinalmingDoc)));
	return (CwaferfinalmingDoc*)m_pDocument;
}
#endif //_DEBUG


// CwaferfinalmingView 消息处理程序
