#include "StdAfx.h"
#include "MyImage_.h"

int MyImage_::nBit=0;

MyImage_::MyImage_(void)
{
	m_pBits=NULL;
	m_pBits2=NULL;
	isGray=false;
}


MyImage_::~MyImage_(void)//��MyImage_���е�ĳ�����m_pBits��Ա��m_pBits2��Ա��Ϊ�գ������������л�����ж���Ķ�Ӧ��Ա��������������
{

	if ( m_pBits!=NULL)
	{
		int h=m_CImage.GetHeight();

		for(int i=0;i<3;i++)//�ͷ�
		{
			for   (int   j=0;   j<h;   j++)    
			{
				delete []m_pBits[i][j];
				m_pBits[i][j]=NULL;
			}
			delete  []m_pBits[i];
			m_pBits[i]=NULL;
		}
		delete  []m_pBits;
		m_pBits=NULL;
	}
	if ( m_pBits2!=NULL)
	{
		int h=m_CImage.GetHeight();
		for   (int j=0; j<h; j++)    
		{
			delete []m_pBits2[j];
			m_pBits2[j]=NULL;
		}
		delete  []m_pBits2;
		m_pBits2=NULL;
	}
}

HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName) //
{
	m_CImage.Load(pszFileName);
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage�������ĵ�ַ//GetBits �� GetPitch ��ϵ��
																	//��GetPitch()<0ʱ��GetBits()��õ�ָ��ָ�����һ��
																	//��GetPitch()>0ʱ��GetBits()��õ�ָ��ָ���һ��
	nBit=m_CImage.GetBPP();//ʵ���grayͼƬ��ÿ�����ص�ֻռ8bit������ɫͼƬռ24bit
	/*������ά���鲢��m_CImage���ƽ���ά����*/
	if(nBit==24)
	{
		m_pBits= (BYTE***)new   BYTE**[3];   //һά����Ϊ3
		for(int i=0; i<3; i++)   
		{     
			m_pBits[i] = (BYTE**)new BYTE*[h];   
			for(int j=0; j<h; j++)         
			{   
				m_pBits[i][j]=new BYTE[w];   
			}
		}
		/*��m_CImage���ƽ���ά����*/
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				m_pBits[0][j][k]=psrc[j*nrow+k*3];//B
				m_pBits[1][j][k]=psrc[j*nrow+k*3+1];//G
				m_pBits[2][j][k]=psrc[j*nrow+k*3+2];//R
			}
		}
	}
	else if(nBit==8)
	{
		m_pBits2=(BYTE**)new BYTE*[h];
		for(int j=0; j<h; j++) 
		{
			m_pBits2[j]=new BYTE[w]; 
		}
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				m_pBits2[j][k]=psrc[j*nrow+k];
			}
		}
	}
	return( S_OK );//Ϊ����CImge���Load��������ֵ��ͬ
}
// HRESULT MyImage_::Load(_In_z_ LPCTSTR pszFileName) 
//{
//	m_CImage.Load(pszFileName);
//
//}

BOOL  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest,
	_In_ int nDestWidth,
	_In_ int nDestHeight) const 
{
	if(m_CImage.IsNull()) 
		return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	/*����ά���鸴�ƽ�m_CImage*/
	int nbit=m_CImage.GetBPP();
	if(nbit==24)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
				psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
				psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R
			}
		}
	}
	if(nbit==8)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k]=m_pBits2[j][k];
			}
		}
	}
	/*����ά���鸴�ƽ�m_CImage*/

	return m_CImage.Draw(hDestDC,xDest, yDest,nDestWidth, nDestHeight);//����CImage��Draw;
}
bool MyImage_::IsNull() const 
{
	return m_CImage.IsNull();
}

void MyImage_::Destroy() 
{
	m_CImage.Destroy();
}

int MyImage_::GetWidth() const 
{
	return m_CImage.GetWidth();
}
int MyImage_::GetHeight() const
{
	return m_CImage.GetHeight();
}


BOOL  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest) const
{
	if(m_CImage.IsNull()) 
	{
		return 0;
	}
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	/*����ά���鸴�ƽ�m_CImage*/
	int nbit=m_CImage.GetBPP();
	if(nbit==24)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
				psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
				psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R
			}
		}
	}
	if(nbit==8)
	{
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				psrc[j*nrow+k]=m_pBits2[j][k];
			}
		}
	}
	/*����ά���鸴�ƽ�m_CImage*/
	return m_CImage.Draw(hDestDC,xDest,yDest);
}

HRESULT MyImage_::Save(
	_In_z_ LPCTSTR pszFileName,
	_In_ REFGUID guidFileType) const
{
	if(m_CImage.IsNull()) return 0;
	int w=m_CImage.GetWidth();
	int h=m_CImage.GetHeight();
	int nrow=m_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc=(BYTE *)m_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	for (int j=0;j<h;j++)
	{
	
		for (int k=0;k<w;k++)
		{
			psrc[j*nrow+k*3]=m_pBits[0][j][k];//B
			psrc[j*nrow+k*3+1]=m_pBits[1][j][k];//G
			psrc[j*nrow+k*3+2]=m_pBits[2][j][k];//R

		}
	}
	return m_CImage.Save(pszFileName,guidFileType);

}