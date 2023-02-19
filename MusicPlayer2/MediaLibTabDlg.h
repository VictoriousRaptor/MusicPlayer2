﻿#pragma once
#include "TabDlg.h"
#include "SongInfo.h"
#include "ListCtrlEx.h"

class CMediaLibTabDlg : public CTabDlg
{
    DECLARE_DYNAMIC(CMediaLibTabDlg)

public:
    CMediaLibTabDlg(UINT nIDTemplate, CWnd *pParent = NULL);
    ~CMediaLibTabDlg();

public:
    //修改文件属性后刷新列表
    virtual void RefreshSongList() = 0;

protected:
    // 获取右侧选中歌曲列表（取出GetItemsSelected()指示的GetSongList()项目）
    virtual void GetSongsSelected(std::vector<SongInfo>& song_list) const;
    // 获取右侧歌曲列表
    virtual const vector<SongInfo>& GetSongList() const = 0;
    //获取右侧列表选中项
    virtual int GetItemSelected() const = 0;
    //获取右侧列表多个选中项
    virtual const vector<int>& GetItemsSelected() const = 0;

    virtual void AfterDeleteFromDisk(const std::vector<SongInfo>& files) = 0;   //执行从磁盘删除后的操作
    virtual wstring GetSelectedString() const = 0;          //获取选中单元格的文本（用于执行“复制到剪贴板”）

    virtual bool _OnAddToNewPlaylist(std::wstring& playlist_path);       //执行添加到新建播放列表命令，成功返回true，playlist_path用于接收新播放列表的路径

private:
    static UINT ViewOnlineThreadFunc(LPVOID lpParam);   //执行在线查看的线程函数

public:
    // 右侧列表没有选中项时无操作
    // 一个选中项则将右侧列表整体添加到临时播放列表并定位选中曲目
    // 多个选中项时将选中项添加到临时播放列表
    // 左侧列表选中的情况下不要调用此方法
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

    afx_msg void OnInitMenu(CMenu* pMenu);
    afx_msg void OnPlayItem();
    afx_msg void OnPlayItemInFolderMode();
    afx_msg void OnAddToNewPlaylist();
    afx_msg void OnAddToNewPalylistAndPlay();
    afx_msg void OnExploreOnline();
    afx_msg void OnExploreTrack();
    afx_msg void OnFormatConvert();
    afx_msg void OnDeleteFromDisk();
    afx_msg void OnItemProperty();
    afx_msg void OnCopyText();

};

