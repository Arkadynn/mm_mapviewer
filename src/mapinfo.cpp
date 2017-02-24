#include "stdafx.h"


namespace angel {

	Mapinfo* Mapinfo::m_instance = 0;

	Mapinfo* Mapinfo::getInstancePtr() { 
		return m_instance; 
	}

	Mapinfo& Mapinfo::getInstance() { 
		assert(m_instance); 
		return *m_instance; 
	}

	Mapinfo::Mapinfo() : BaseLayout("mapinfo.layout") {
		assert(!m_instance);
		m_instance = this;
		            
		assignWidget(mText, "Mapinfotext");
		mMainWidget->setVisible(true);
	}

	Mapinfo::~Mapinfo() {
		mMainWidget->setVisible(false);
		m_instance = 0;
	}

	wchar_t mb2wc(char ch) {
		wchar_t wch;
		MultiByteToWideChar(CP_ACP,0,&ch,1,&wch,sizeof(wch));
		return wch;
	}

	void Mapinfo::SetText(const std::string & str) {
		std::wstring wstr;
		std::transform(str.begin(),str.end(), std::back_insert_iterator<std::wstring>(wstr), mb2wc);
		mText->setCaption(Ogre::UTFString(wstr));
		const MyGUI::IntSize& size  = mText->getTextSize();
		mMainWidget->setSize(size+MyGUI::IntSize(10,4));
		mText->setSize(size);
	}

}
