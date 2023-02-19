﻿#include "stdafx.h"
#include "SongDataManager.h"
#include "MusicPlayer2.h"

CSongDataManager CSongDataManager::m_instance;

CSongDataManager::CSongDataManager()
{
}

CSongDataManager::~CSongDataManager()
{
}

CSongDataManager& CSongDataManager::GetInstance()
{
    return m_instance;
}

void CSongDataManager::SaveSongData(std::wstring path)
{
    // 打开或者新建文件
    CFile file;
    BOOL bRet = file.Open(path.c_str(),
        CFile::modeCreate | CFile::modeWrite);
    if (!bRet)		//打开文件失败
    {
        return;
    }
    // 构造CArchive对象
    CArchive ar(&file, CArchive::store);
    // 写数据
    ar << CString(_T("2.751"));			//写入数据版本
    ar << static_cast<int>(m_song_data.size());		//写入映射容器的大小
    for (auto& song_data : m_song_data)
    {
        ar << CString(song_data.first.path.c_str())		//保存映射容器的键，即歌曲的绝对路径
            << song_data.second.start_pos.toInt()
            << song_data.second.end_pos.toInt()
            << song_data.second.bitrate
            << CString(song_data.second.title.c_str())
            << CString(song_data.second.artist.c_str())
            << CString(song_data.second.album.c_str())
            << CString(song_data.second.get_year().c_str())
            << CString(song_data.second.comment.c_str())
            << CString(song_data.second.genre.c_str())
            << song_data.second.genre_idx
            << song_data.second.track
            << song_data.second.tag_type
            << CString(song_data.second.GetSongId().c_str())
            << song_data.second.listen_time
            << song_data.second.info_acquired
            << song_data.second.is_cue
            //<< song_data.second.is_favourite

             //<< song_data.second.no_online_album_cover
             //<< song_data.second.no_online_lyric
            << song_data.second.flags
            << song_data.second.last_played_time
            << CString(song_data.second.lyric_file.c_str())
            << song_data.second.modified_time
            << song_data.second.rating
            << song_data.second.freq
            << song_data.second.bits
            << song_data.second.channels
            << CString(song_data.second.cue_file_path.c_str())
            << CString(song_data.second.album_artist.c_str())
            << song_data.second.total_tracks
            << song_data.second.disc_num
            << song_data.second.total_discs
            ;
    }
    // 关闭CArchive对象
    ar.Close();
    // 关闭文件
    file.Close();

    m_song_data_modified = false;
}

void CSongDataManager::LoadSongData(std::wstring path)
{
    // 打开文件
    CFile file;
    BOOL bRet = file.Open(path.c_str(), CFile::modeRead);
    if (!bRet) return;
    // 构造CArchive对象
    CArchive ar(&file, CArchive::load);
    // 读数据
    int size{};
    SongInfo song_info;
    CString temp;
    try
    {
        //读取版本
        ar >> m_data_version;
        //if (!CCommon::StringIsVersion(m_data_version))
        //    m_data_version = _T("0.00");
        if (m_data_version >= _T("2.664"))
        {
            ar >> size;		//读取映射容器的长度
        }
        else
        {
            size_t size_1;
            ar >> size_1;
            size = static_cast<int>(size_1);
        }
        for (int i{}; i < size; i++)
        {
            ar >> temp;
            song_info.file_path = temp;
            int song_start_pos{}, song_end_pos{};
            if (m_data_version >= _T("2.731"))
            {
                ar >> song_start_pos;
            }
            ar >> song_end_pos;
            song_info.start_pos.fromInt(song_start_pos);
            song_info.end_pos.fromInt(song_end_pos);
            if (m_data_version >= _T("2.691"))
            {
                ar >> song_info.bitrate;
            }
            else
            {
                int bitrate;
                ar >> bitrate;
                song_info.bitrate = bitrate;
            }
            ar >> temp;
            song_info.title = temp;
            ar >> temp;
            song_info.artist = temp;
            ar >> temp;
            song_info.album = temp;
            ar >> temp;
            song_info.SetYear(temp);
            ar >> temp;
            song_info.comment = temp;
            ar >> temp;
            song_info.genre = temp;
            ar >> song_info.genre_idx;
            if (m_data_version >= _T("2.66"))
            {
                ar >> song_info.track;
            }
            else
            {
                BYTE track;
                ar >> track;
                song_info.track = track;
            }

            if (m_data_version >= _T("2.691"))
            {
                ar >> song_info.tag_type;
            }
            else
            {
                int tag_type;
                ar >> tag_type;
                song_info.tag_type = tag_type;
            }
            ar >> temp;
            song_info.SetSongId(temp.GetString());

            if (m_data_version >= _T("2.64"))		//版本号大于等于2.64
            {
                ar >> song_info.listen_time;
                ar >> song_info.info_acquired;
            }

            if (m_data_version >= _T("2.731"))
            {
                ar >> song_info.is_cue;
            }

            if (m_data_version == _T("2.661"))
            {
                ar >> song_info.is_favourite;
            }

            if (m_data_version >= _T("2.663") && m_data_version < _T("2.690"))
            {
                bool no_online_album_cover{ song_info.NoOnlineAlbumCover() };
                bool no_online_lyric{ song_info.NoOnlineLyric() };
                ar >> no_online_album_cover;
                ar >> no_online_lyric;
            }

            if (m_data_version >= _T("2.690"))
            {
                ar >> song_info.flags;
            }

            if (m_data_version >= _T("2.680"))
            {
                ar >> song_info.last_played_time;
            }

            if (m_data_version >= _T("2.692"))
            {
                ar >> temp;
                song_info.lyric_file = temp;
            }

            if (m_data_version >= _T("2.700"))
            {
                ar >> song_info.modified_time;
            }

            if (m_data_version >= _T("2.720"))
            {
                ar >> song_info.rating;
            }

            if (m_data_version >= _T("2.730"))
            {
                ar >> song_info.freq;
                ar >> song_info.bits;
                ar >> song_info.channels;
            }
            if (m_data_version >= _T("2.75"))
            {
                ar >> temp;
                song_info.cue_file_path = temp;
            }
            if (m_data_version >= _T("2.751"))
            {
                ar >> temp;
                song_info.album_artist = temp;
                ar >> song_info.total_tracks;
                ar >> song_info.disc_num;
                ar >> song_info.total_discs;
            }
            CSingleLock sync(&m_critical, TRUE);
            m_song_data[song_info] = song_info;     // 将读取到的一首歌曲信息添加到映射容器中

            std::wstring file_name{ song_info.GetFileName() };
            m_song_file_name_map[file_name].push_back(song_info.file_path);
        }
    }
    catch (CArchiveException* exception)
    {
        CString info;
        info = CCommon::LoadTextFormat(IDS_SERIALIZE_ERROR, { path, exception->m_cause });
        theApp.WriteLog(wstring{ info });
    }
    // 关闭对象
    ar.Close();
    // 关闭文件
    file.Close();
}

void CSongDataManager::SetSongDataModified()
{
    m_song_data_modified = true;
}

bool CSongDataManager::IsSongDataModified() const
{
    return m_song_data_modified;
}

CString CSongDataManager::GetDataVersion() const
{
    return m_data_version;
}

void CSongDataManager::SaveSongInfo(const SongInfo& song_info)
{
    if (song_info.file_path.empty())
        return;
    CSingleLock sync(&m_critical, TRUE);
    SongInfo& song = m_song_data[song_info];
    song.file_path = song_info.file_path;
    song.cue_file_path = song_info.cue_file_path;
    song.CopyAudioTag(song_info);
    song.start_pos = song_info.start_pos;
    song.end_pos = song_info.end_pos;
    song.bitrate = song_info.bitrate;
    song.song_id = song_info.song_id;
    song.is_cue = song_info.is_cue;
    song.rating = song_info.rating;
    song.freq = song_info.freq;
    song.channels = song_info.channels;
    song.bits = song_info.bits;

    SetSongDataModified();
}

void CSongDataManager::LoadSongInfo(SongInfo& song_info)
{
    auto iter = m_song_data.find(song_info);
    if (iter != m_song_data.end())
    {
        const SongInfo& temp = iter->second;
        song_info.CopyAudioTag(temp);
        song_info.cue_file_path = temp.cue_file_path;
        song_info.start_pos = temp.start_pos;
        song_info.end_pos = temp.end_pos;
        song_info.bitrate = temp.bitrate;
        song_info.song_id = temp.song_id;
        song_info.info_acquired = temp.info_acquired;// 以后会更改为仅媒体库内使用，之后删掉这行
        song_info.modified_time = temp.modified_time;
        song_info.freq = temp.freq;
        song_info.channels = temp.channels;
        song_info.bits = temp.bits;
    }
}

SongInfo CSongDataManager::GetSongInfo(const SongDataMapKey& key) const
{
    SongInfo song;
    auto iter = m_song_data.find(key);
    if (iter != m_song_data.end())
        song = iter->second;
    if (key.cue_track != 0)
    {
        song.track = key.cue_track;
        song.is_cue = true;
    }
    return song;
}

SongInfo CSongDataManager::GetSongInfo3(const SongInfo& song) const
{
    if (song.IsEmpty())
        return song;
    ASSERT(!song.file_path.empty());
    SongInfo tmp;
    auto iter = m_song_data.find(song);
    if (iter != m_song_data.end())
        tmp = iter->second;
    else
        tmp = song;
    return tmp;
}

const CSongDataManager::SongDataMap& CSongDataManager::GetSongData()
{
    return m_song_data;
}

bool CSongDataManager::IsItemExist(const SongDataMapKey& key) const
{
    auto iter = m_song_data.find(key);
    return iter != m_song_data.end();
}

void CSongDataManager::AddItem(const SongInfo& song)
{
    CSingleLock sync(&m_critical, TRUE);
    ASSERT(!song.file_path.empty());
    m_song_data[song] = song;
    SetSongDataModified();
}

bool CSongDataManager::RemoveItem(const SongDataMapKey& key)
{
    CSingleLock sync(&m_critical, TRUE);
    auto iter = m_song_data.find(key);
    if (iter != m_song_data.end())
    {
        m_song_data.erase(iter);
        return true;
    }
    return false;
}

int CSongDataManager::RemoveItemIf(std::function<bool(const SongInfo&)> fun_condition)
{
    int clear_cnt{};		//统计删除的项目的数量
    //遍历映射容器，删除不必要的条目。
    for (auto iter{ m_song_data.begin() }; iter != m_song_data.end();)
    {
        if (fun_condition(iter->second))
        {
            CSingleLock sync(&m_critical, TRUE);
            iter = m_song_data.erase(iter);		//删除条目之后将迭代器指向被删除条目的前一个条目
            clear_cnt++;
        }
        else
        {
            iter++;
        }
    }
    return clear_cnt;
}

void CSongDataManager::ClearPlayTime()
{
    for (auto& data : m_song_data)
    {
        data.second.listen_time = 0;
    }
    SetSongDataModified();
}

void CSongDataManager::ClearLastPlayedTime()
{
    for (auto& item : m_song_data)
    {
        item.second.last_played_time = 0;
    }
    SetSongDataModified();
}

void CSongDataManager::ChangeFilePath(const wstring& file_path, const wstring& new_path)
{
    auto iter = m_song_data.find(file_path);
    if (iter != m_song_data.end())
    {
        CSingleLock sync(&m_critical, TRUE);
        SongInfo song = iter->second;
        if (!song.file_path.empty())
            song.file_path = new_path;
        m_song_data[new_path] = song;
    }
}

//计算两个字符串右侧匹配的字符数量
static int CalcualteStringRightMatchedCharNum(const std::wstring& str1, const std::wstring& str2)
{
    size_t index1{ str1.size() - 1 };
    size_t index2{ str2.size() - 1 };
    int char_matched{};
    for (; index1 >= 0 && index2 >= 0; index1--, index2--)
    {
        if (str1[index1] == str2[index2])
            char_matched++;
        else
            break;
    }
    return char_matched;
}

bool CSongDataManager::FixWrongFilePath(wstring& file_path)
{
    std::wstring file_name{ CFilePathHelper(file_path).GetFileName() };
    bool fixed{ false };
    auto iter = m_song_file_name_map.find(file_name);
    if (iter != m_song_file_name_map.end())
    {
        if (iter->second.size() == 1)      //媒体库中同名的文件只有一个时，直接修改为该文件的路径
        {
            file_path = iter->second.front();
            fixed = true;
        }
        else if (iter->second.size() > 1)   //媒体库中同名的文件有多个时，查找两个路径末尾相同字符数量最多的那项
        {
            size_t best_match_index{};
            int max_matched_char_mun{};
            for (size_t i{}; i < iter->second.size(); i++)
            {
                int cur_matched_char_num = CalcualteStringRightMatchedCharNum(file_path, iter->second[i]);
                if (cur_matched_char_num > max_matched_char_mun)
                {
                    max_matched_char_mun = cur_matched_char_num;
                    best_match_index = i;
                }
            }
            file_path = iter->second[best_match_index];
            fixed = true;
        }
    }
    return CCommon::FileExist(file_path);
}
