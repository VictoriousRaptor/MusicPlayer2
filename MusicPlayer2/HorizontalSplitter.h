#pragma once
//���� https://blog.csdn.net/mary288267/article/details/123483656

// CHorizontalSpliter

//������Ӧ���ֱ仯�Ļص�����������RegAdjustLayoutCallBack��ע��˻ص�����
//rect �ָ����ľ�������
typedef void(*pfAdjustLayout)(CRect rect);

class CHorizontalSplitter : public CStatic
{
	DECLARE_DYNAMIC(CHorizontalSplitter)

public:
	CHorizontalSplitter();
	virtual ~CHorizontalSplitter();

    //�����ƶ��ָ��������жԻ����������С����
    void SetMinWidth(int left, int right);
    //�ָ������Ŀؼ�ID
    BOOL AttachCtrlAsLeftPane(DWORD idCtrl);
    //�ָ����Ҳ�Ŀؼ�ID
    BOOL AttachCtrlAsRightPane(DWORD idCtrl);
    //����ָ�����������Ŀؼ�
    BOOL DetachAllPanes();
    //���ݷָ���λ�ã������Ի��������пؼ�λ��
    void AdjustLayout();

    //ע����Ӧ���ֱ仯�Ļص���������������˴˺�������ָ���λ�øı�ʱ���ָ�������Ŀؼ���С�������Զ�������
    //���ǵ���ע��Ļص��������ɻص�������������ؼ��ĵ�����
    //ͨ���˺���ע����Ӧ���ֱ仯�Ļص������󣬽�������Ҫ����AttachCtrlAsLeftPane��AttachCtrlAsRightPane������
    void RegAdjustLayoutCallBack(pfAdjustLayout pFunc);

protected:
    //�ָ��������ƶ��ķ�Χ
    BOOL GetMouseClipRect(LPRECT rcClip, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    CRect m_rcOrgRect;
    CRect m_rcOldRect;
    CWnd* m_pParent;
    CPoint m_pPointStart;

    int m_iLeftMin, m_iRightMin;
    CDWordArray m_idLeft, m_idRight;
    pfAdjustLayout m_pAdjLayoutFunc{};

public:
    afx_msg void OnPaint();
    virtual void PreSubclassWindow();
};


