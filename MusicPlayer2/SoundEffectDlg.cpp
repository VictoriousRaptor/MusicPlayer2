﻿// SoundEffectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "SoundEffectDlg.h"
#include "afxdialogex.h"


// CSoundEffectDlg 对话框

IMPLEMENT_DYNAMIC(CSoundEffectDlg, CDialog)

CSoundEffectDlg::CSoundEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SOUND_EFFECT_DIALOG, pParent)
{

}

CSoundEffectDlg::~CSoundEffectDlg()
{
}

void CSoundEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CSoundEffectDlg, CDialog)
END_MESSAGE_MAP()


// CSoundEffectDlg 消息处理程序


BOOL CSoundEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CenterWindow();

    //创建子对话框
    m_equ_dlg.Create(IDD_EQUALIZER_DIALOG, &m_tab);
    m_reverb_dlg.Create(IDD_REVERB_DIALOG, &m_tab);

    //添加对话框
    m_tab.AddWindow(&m_equ_dlg, CCommon::LoadText(IDS_EQUALIZER));
    m_tab.AddWindow(&m_reverb_dlg, CCommon::LoadText(IDS_REVERB));

    //为每个标签添加图标
    CImageList ImageList;
    ImageList.Create(theApp.DPI(16), theApp.DPI(16), ILC_COLOR32 | ILC_MASK, 2, 2);
    ImageList.Add(theApp.m_icon_set.eq.GetIcon(true));
    ImageList.Add(theApp.m_icon_set.reverb);
    m_tab.SetImageList(&ImageList);
    ImageList.Detach();

    m_tab.SetItemSize(CSize(theApp.DPI(60), theApp.DPI(24)));
    m_tab.AdjustTabWindowSize();

    //设置默认选中的标签
    if (m_tab_selected < 0 || m_tab_selected >= m_tab.GetItemCount())
        m_tab_selected = 0;
    m_tab.SetCurTab(m_tab_selected);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSoundEffectDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();

	//CDialog::OnCancel();
}
