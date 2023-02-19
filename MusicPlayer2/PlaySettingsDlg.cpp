﻿// CPlaySettingsDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlaySettingsDlg.h"
#include "afxdialogex.h"
#include "FfmpegCore.h"


// CPlaySettingsDlg 对话框

IMPLEMENT_DYNAMIC(CPlaySettingsDlg, CTabDlg)

CPlaySettingsDlg::CPlaySettingsDlg(CWnd* pParent /*=nullptr*/)
	: CTabDlg(IDD_PLAY_SETTING_DIALOG, pParent)
{

}

CPlaySettingsDlg::~CPlaySettingsDlg()
{
}

void CPlaySettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CTabDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STOP_WHEN_ERROR, m_stop_when_error_check);
    DDX_Control(pDX, IDC_SHOW_TASKBAR_PROGRESS, m_show_taskbar_progress_check);
    DDX_Control(pDX, IDC_OUTPUT_DEVICE_COMBO, m_output_device_combo);
    DDX_Control(pDX, IDC_OUTPUT_DEVICE_INFO_LIST, m_device_info_list);
    DDX_Control(pDX, IDC_AUTO_PLAY_WHEN_START_CHECK, m_auto_play_when_start_chk);
    DDX_Control(pDX, IDC_SHOW_PLAY_STATE_ICON_CHECK, m_show_play_state_icon_chk);
    DDX_Control(pDX, IDC_SOUND_FADE_CHECK, m_sound_fade_chk);
    DDX_Control(pDX, IDC_CONTINUE_WHEN_SWITCH_PLAYLIST_CHECK, m_continue_when_switch_playlist_check);
    DDX_Control(pDX, IDC_BASS_RADIO, m_bass_radio);
    DDX_Control(pDX, IDC_MCI_RADIO, m_mci_radio);
    DDX_Control(pDX, IDC_FFMPEG_RADIO, m_ffmpeg_radio);
    DDX_Control(pDX, IDC_FFMPEG_CACHE_LENGTH, m_ffmpeg_cache_length);
    DDX_Control(pDX, IDC_FFMPEG_MAX_RETRY_COUNT, m_ffmpeg_max_retry_count);
    DDX_Control(pDX, IDC_FFMPEG_URL_RETRY_INTERVAL, m_ffmpeg_url_retry_interval);
    DDX_Control(pDX, IDC_FFMPEG_ENABLE_WASAPI, m_ffmpeg_enable_wasapi);
    DDX_Control(pDX, IDC_FFMPEG_ENABLE_WASAPI_EXCLUSIVE, m_ffmpeg_enable_wasapi_exclusive);
    DDX_Control(pDX, IDC_FFMPEG_MAX_WAIT_TIME, m_ffmpeg_max_wait_time);
}

void CPlaySettingsDlg::ShowDeviceInfo()
{
    if (theApp.m_output_devices.empty())
        return;

    // 重新设置设备选择下拉框
    m_output_device_combo.ResetContent();
    for (const auto& device : theApp.m_output_devices)
    {
        m_output_device_combo.AddString(device.name.c_str());
    }
    m_output_device_combo.SetCurSel(m_data.device_selected);

    // 更新当前设备信息显示
	DeviceInfo& device{ theApp.m_output_devices[m_data.device_selected] };
	m_device_info_list.SetItemText(0, 1, device.name.c_str());
	m_device_info_list.SetItemText(1, 1, device.driver.c_str());
	m_data.output_device = device.name;
	DWORD device_type;
	device_type = device.flags & BASS_DEVICE_TYPE_MASK;
	CString type_info;
	switch (device_type)
	{
	case BASS_DEVICE_TYPE_DIGITAL:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_DIGITAL);
		break;
	case BASS_DEVICE_TYPE_DISPLAYPORT:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_DISPLAYPORT);
		break;
	case BASS_DEVICE_TYPE_HANDSET:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_HANDSET);
		break;
	case BASS_DEVICE_TYPE_HDMI:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_HDMI);
		break;
	case BASS_DEVICE_TYPE_HEADPHONES:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_HEADPHONES);
		break;
	case BASS_DEVICE_TYPE_HEADSET:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_HEADSET);
		break;
	case BASS_DEVICE_TYPE_LINE:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_LINE);
		break;
	case BASS_DEVICE_TYPE_MICROPHONE:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_MICROPHONE);
		break;
	case BASS_DEVICE_TYPE_NETWORK:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_NETWORK);
		break;
	case BASS_DEVICE_TYPE_SPDIF:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_SPDIF);
		break;
	case BASS_DEVICE_TYPE_SPEAKERS:
		type_info = CCommon::LoadText(IDS_DEVICE_TYPE_SPEAKERS);
		break;
	}
	m_device_info_list.SetItemText(2, 1, type_info);

}

void CPlaySettingsDlg::EnableControl()
{
    bool enable = !CPlayer::GetInstance().IsMciCore();
    bool ffmpeg_enable = CPlayer::GetInstance().IsFfmpegCore();
    m_sound_fade_chk.EnableWindow(CPlayer::GetInstance().IsBassCore());
    m_device_info_list.EnableWindow(enable);
    m_output_device_combo.EnableWindow(enable);
    m_ffmpeg_cache_length.EnableWindow(ffmpeg_enable);
    m_ffmpeg_max_retry_count.EnableWindow(ffmpeg_enable);
    m_ffmpeg_url_retry_interval.EnableWindow(ffmpeg_enable);
    if (ffmpeg_enable) {
        auto core = (CFfmpegCore*)CPlayer::GetInstance().GetPlayerCore();
        m_ffmpeg_enable_wasapi.EnableWindow(core->IsWASAPISupported());
        auto version = core->GetVersion();
        m_ffmpeg_max_wait_time.EnableWindow(version > FFMPEG_CORE_VERSION(1, 0, 0, 0));
    } else {
        m_ffmpeg_enable_wasapi.EnableWindow(FALSE);
        m_ffmpeg_max_wait_time.EnableWindow(FALSE);
    }
    if (m_ffmpeg_enable_wasapi.IsWindowEnabled() && m_data.ffmpeg_core_enable_WASAPI) {
        m_ffmpeg_enable_wasapi_exclusive.EnableWindow(TRUE);
    } else {
        m_ffmpeg_enable_wasapi_exclusive.EnableWindow(FALSE);
    }
}

void CPlaySettingsDlg::GetDataFromUi()
{
    m_data.ffmpeg_core_cache_length = m_ffmpeg_cache_length.GetValue();
    m_data.ffmpeg_core_max_retry_count = m_ffmpeg_max_retry_count.GetValue();
    m_data.ffmpeg_core_url_retry_interval = m_ffmpeg_url_retry_interval.GetValue();
    m_data.ffmpeg_core_max_wait_time = m_ffmpeg_max_wait_time.GetValue();
}

void CPlaySettingsDlg::ApplyDataToUi()
{
    ShowDeviceInfo();
    EnableControl();
}


BEGIN_MESSAGE_MAP(CPlaySettingsDlg, CTabDlg)
	ON_BN_CLICKED(IDC_STOP_WHEN_ERROR, &CPlaySettingsDlg::OnBnClickedStopWhenError)
	ON_BN_CLICKED(IDC_SHOW_TASKBAR_PROGRESS, &CPlaySettingsDlg::OnBnClickedShowTaskbarProgress)
	ON_CBN_SELCHANGE(IDC_OUTPUT_DEVICE_COMBO, &CPlaySettingsDlg::OnCbnSelchangeOutputDeviceCombo)
	ON_BN_CLICKED(IDC_AUTO_PLAY_WHEN_START_CHECK, &CPlaySettingsDlg::OnBnClickedAutoPlayWhenStartCheck)
	ON_BN_CLICKED(IDC_SHOW_PLAY_STATE_ICON_CHECK, &CPlaySettingsDlg::OnBnClickedShowPlayStateIconCheck)
    ON_BN_CLICKED(IDC_SOUND_FADE_CHECK, &CPlaySettingsDlg::OnBnClickedSoundFadeCheck)
    ON_BN_CLICKED(IDC_CONTINUE_WHEN_SWITCH_PLAYLIST_CHECK, &CPlaySettingsDlg::OnBnClickedContinueWhenSwitchPlaylistCheck)
    ON_BN_CLICKED(IDC_BASS_RADIO, &CPlaySettingsDlg::OnBnClickedBassRadio)
    ON_BN_CLICKED(IDC_MCI_RADIO, &CPlaySettingsDlg::OnBnClickedMciRadio)
    ON_BN_CLICKED(IDC_FFMPEG_RADIO, &CPlaySettingsDlg::OnBnClickedFfmpegRadio)
    ON_NOTIFY(NM_CLICK, IDC_FFMPEG_DOWN_SYSLINK, &CPlaySettingsDlg::OnNMClickFfmpegDownSyslink)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_FFMPEG_ENABLE_WASAPI, &CPlaySettingsDlg::OnBnClickedFfmpegEnableWasapi)
    ON_BN_CLICKED(IDC_FFMPEG_ENABLE_WASAPI_EXCLUSIVE, &CPlaySettingsDlg::OnBnClickedFfmpegEnableWasapiExclusive)
END_MESSAGE_MAP()


// CPlaySettingsDlg 消息处理程序


BOOL CPlaySettingsDlg::OnInitDialog()
{
	CTabDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化

    m_toolTip.Create(this);
    m_toolTip.SetMaxTipWidth(theApp.DPI(300));
    m_toolTip.AddTool(GetDlgItem(IDC_MCI_RADIO), CCommon::LoadText(IDS_MCI_KERNAL_TIP));
    m_toolTip.AddTool(GetDlgItem(IDC_FFMPEG_RADIO), CCommon::LoadText(IDS_FFMPEG_CORE));
    m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	//初始化各控件的状态
	m_stop_when_error_check.SetCheck(m_data.stop_when_error);
	m_auto_play_when_start_chk.SetCheck(m_data.auto_play_when_start);
    m_continue_when_switch_playlist_check.SetCheck(m_data.continue_when_switch_playlist);
	m_show_taskbar_progress_check.SetCheck(m_data.show_taskbar_progress);
	m_show_play_state_icon_chk.SetCheck(m_data.show_playstate_icon);
    m_sound_fade_chk.SetCheck(m_data.fade_effect);
	
	if (!CWinVersionHelper::IsWindows7OrLater() || theApp.GetITaskbarList3() == nullptr)
	{
		m_show_taskbar_progress_check.SetCheck(FALSE);
		m_show_play_state_icon_chk.SetCheck(FALSE);
		m_show_taskbar_progress_check.EnableWindow(FALSE);
		m_show_play_state_icon_chk.EnableWindow(FALSE);
	}

    bool enable_ffmpeg = false;
    if (CPlayer::GetInstance().IsFfmpegCore()) {
        enable_ffmpeg = true;
    } else {
        auto h = LoadLibraryW(L"ffmpeg_core.dll");
        if (h) {
            enable_ffmpeg = true;
            FreeLibrary(h);
        }
    }
    m_ffmpeg_radio.EnableWindow(enable_ffmpeg);
    ShowDlgCtrl(IDC_FFMPEG_DOWN_SYSLINK, !enable_ffmpeg);       //未检测到ffmpeg内核时，显示下载链接

    if (m_data.use_mci)
        m_mci_radio.SetCheck(TRUE);
    else if (m_data.use_ffmpeg)
        m_ffmpeg_radio.SetCheck(TRUE);
    else
        m_bass_radio.SetCheck(TRUE);

	//m_device_info_list.SetColor(theApp.m_app_setting_data.theme_color);
	CRect rect;
	m_device_info_list.GetClientRect(rect);
	int width0, width1;
	width0 = theApp.DPI(70);
	width1 = rect.Width() - width0 - theApp.DPI(20);

    m_device_info_list.SetExtendedStyle(m_device_info_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
	m_device_info_list.InsertColumn(0, CCommon::LoadText(IDS_ITEM), LVCFMT_LEFT, width0);		//插入第1列
	m_device_info_list.InsertColumn(1, CCommon::LoadText(IDS_VLAUE), LVCFMT_LEFT, width1);		//插入第2列
	m_device_info_list.InsertItem(0, CCommon::LoadText(IDS_NAME));
	m_device_info_list.InsertItem(1, CCommon::LoadText(IDS_DRIVER));
	m_device_info_list.InsertItem(2, CCommon::LoadText(IDS_DEVICE_TYPE));
	ShowDeviceInfo();

    EnableControl();

    m_ffmpeg_cache_length.SetRange(1, 60);
    m_ffmpeg_cache_length.SetValue(theApp.m_play_setting_data.ffmpeg_core_cache_length);
    m_ffmpeg_max_retry_count.SetRange(-1, SHORT_MAX);
    m_ffmpeg_max_retry_count.SetValue(theApp.m_play_setting_data.ffmpeg_core_max_retry_count);
    m_ffmpeg_url_retry_interval.SetRange(1, 120);
    m_ffmpeg_url_retry_interval.SetValue(theApp.m_play_setting_data.ffmpeg_core_url_retry_interval);
    m_ffmpeg_enable_wasapi.SetCheck(theApp.m_play_setting_data.ffmpeg_core_enable_WASAPI);
    m_ffmpeg_enable_wasapi_exclusive.SetCheck(theApp.m_play_setting_data.ffmpeg_core_enable_WASAPI_exclusive_mode);
    m_ffmpeg_max_wait_time.SetRange(100, 30000);
    m_ffmpeg_max_wait_time.SetValue(theApp.m_play_setting_data.ffmpeg_core_max_wait_time);

    //设置控件不响应鼠标滚轮消息
    m_output_device_combo.SetMouseWheelEnable(false);
    m_ffmpeg_cache_length.SetMouseWheelEnable(false);
    m_ffmpeg_max_retry_count.SetMouseWheelEnable(false);
    m_ffmpeg_url_retry_interval.SetMouseWheelEnable(false);
    m_ffmpeg_max_wait_time.SetMouseWheelEnable(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPlaySettingsDlg::OnBnClickedStopWhenError()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.stop_when_error = (m_stop_when_error_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedShowTaskbarProgress()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.show_taskbar_progress = (m_show_taskbar_progress_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnCbnSelchangeOutputDeviceCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.device_selected = m_output_device_combo.GetCurSel();
	ShowDeviceInfo();
}


void CPlaySettingsDlg::OnBnClickedAutoPlayWhenStartCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.auto_play_when_start = (m_auto_play_when_start_chk.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedShowPlayStateIconCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	m_data.show_playstate_icon = (m_show_play_state_icon_chk.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedSoundFadeCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    m_data.fade_effect = (m_sound_fade_chk.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedContinueWhenSwitchPlaylistCheck()
{
    // TODO: 在此添加控件通知处理程序代码
    m_data.continue_when_switch_playlist = (m_continue_when_switch_playlist_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedBassRadio()
{
    // TODO: 在此添加控件通知处理程序代码
    m_data.use_mci = false;
    m_data.use_ffmpeg = false;
}


void CPlaySettingsDlg::OnBnClickedMciRadio()
{
    // TODO: 在此添加控件通知处理程序代码
    m_data.use_mci = true;
    m_data.use_ffmpeg = false;
}


void CPlaySettingsDlg::OnBnClickedFfmpegRadio() {
    m_data.use_mci = false;
    m_data.use_ffmpeg = true;
}


BOOL CPlaySettingsDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_MOUSEMOVE)
        m_toolTip.RelayEvent(pMsg);

    return CTabDlg::PreTranslateMessage(pMsg);
}

void CPlaySettingsDlg::OnOK() {
}


void CPlaySettingsDlg::OnNMClickFfmpegDownSyslink(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    if (MessageBox(CCommon::LoadText(IDS_DOWNLOAD_FFMPEG_CORE_INFO), NULL, MB_ICONINFORMATION | MB_YESNO) == IDYES)
    {
        ShellExecute(NULL, _T("open"), _T("https://github.com/lifegpc/ffmpeg_core/releases"), NULL, NULL, SW_SHOW);
    }

    *pResult = 0;
}


HBRUSH CPlaySettingsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    UINT id{};
    if (pWnd != nullptr)
        id = pWnd->GetDlgCtrlID();
    if (id == IDC_FFMPEG_DOWN_SYSLINK)		//设置滑动条控件的背景色为白色
    {
        static HBRUSH brush{};
        if (brush == NULL)
            brush = CreateSolidBrush(m_background_color);
        pDC->SetBkColor(m_background_color);
        return brush;
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}


void CPlaySettingsDlg::OnBnClickedFfmpegEnableWasapi() {
    m_data.ffmpeg_core_enable_WASAPI = (m_ffmpeg_enable_wasapi.GetCheck() != 0);
    m_ffmpeg_enable_wasapi_exclusive.EnableWindow(m_ffmpeg_enable_wasapi.GetCheck());
}


void CPlaySettingsDlg::OnBnClickedFfmpegEnableWasapiExclusive() {
    m_data.ffmpeg_core_enable_WASAPI_exclusive_mode = (m_ffmpeg_enable_wasapi_exclusive.GetCheck() != 0);
}
