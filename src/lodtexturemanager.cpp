#include "stdafx.h"
#include "zlib.h"

template<> angel::LodTextureManager *Ogre::Singleton<angel::LodTextureManager>::ms_Singleton = 0;	
namespace angel {
    

	LodTextureManager *LodTextureManager::getSingletonPtr() {
		return ms_Singleton;
	}

	LodTextureManager &LodTextureManager::getSingleton() {  
		assert(ms_Singleton);  
		return(*ms_Singleton);
	}

	LodTextureManager::LodTextureManager() {}

	LodTextureManager::~LodTextureManager() {}

	Ogre::TexturePtr LodTextureManager::load(const Ogre::String &name, const Ogre::String &group) {
		Ogre::ResourceManager::ResourceCreateOrRetrieveResult res = Ogre::TextureManager::getSingleton().createOrRetrieve(name, group, true, this);
		Ogre::TexturePtr tex = res.first;
		tex->load();
		return tex;
	}

	void LodTextureManager::loadSprite(Ogre::Resource* res, pLodData hdr, pLodData ldata) {
		Texture* texture = static_cast<Texture*>(res);
		angel::Log <<"texture " << res->getName() << " error datasize "  << angel::aeLog::endl;
		return GetDefaultTexture(res);
	}

	void LodTextureManager::loadResource(Ogre::Resource* res) {
		int alpha = 0;
		angel::Log << angel::aeLog::debug <<"loading texture " << res->getName() << angel::aeLog::endl;
		
		angel::pLodData ldata = angel::LodManager.LoadFileData( res->getName() );
		
		if(!ldata)
			return GetDefaultTexture(res);
		angel::pLodData hdr = angel::LodManager.LoadFileHdr( res->getName() );

		BYTE*data= &((*ldata)[0]);
		BYTE*hdrdata= &((*hdr)[0]);
		int size = (int)ldata->size();
		int psize = *(int*)(hdrdata+0x4);
		unsigned int unpsize1 = *(int*)(hdrdata+0x0);
		unsigned long unpsize2 = *(int*)(hdrdata+0x18);

		if( unpsize2+0x300 != size ) {
			return loadSprite(res,hdr,ldata);
		}

		BYTE* pal = data + unpsize2;
		int width  = *(WORD*)(hdrdata+0x8);
		int height = *(WORD*)(hdrdata+0xa);
		int imgsize = width*height;
		BYTE *pSrc=data;
		
		int nummipmaps = 3;
		// Create the texture
		Texture* texture = static_cast<Texture*>(res);
		texture->setTextureType(TEX_TYPE_2D);
		texture->setWidth(width);
		texture->setHeight(height);
		texture->setNumMipmaps(nummipmaps);
		texture->setFormat(PF_BYTE_BGRA);
		texture->setUsage(TU_DEFAULT);
		texture->setDepth(1);
		texture->setHardwareGammaEnabled(false);
		texture->setFSAA(0);
		texture->createInternalResources();


		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		int w=width;
		int h=height;

		int n=0,off=0; 
		nummipmaps = (int)texture->getNumMipmaps();
		for ( n = 0,off= 0; off < (int)unpsize2 && n <nummipmaps + 1 ;  n++) {
			if( w < 1 || h <1 )
				break;
			// Get the pixel buffer
			HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer(0,n);

			// Lock the pixel buffer and get a pixel box
			pixelBuffer->lock(HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
			const PixelBox& pixelBox = pixelBuffer->getCurrentLock();

			uint8* pDest = static_cast<uint8*>(pixelBox.data);

			for (int j = 0; j < w; j++)
				for(int i = 0; i < h; i++) {
					int index=*pSrc++;
					int r = pal[index*3+0]; 
					int g = pal[index*3+1]; 
					int b = pal[index*3+2]; 
					int a = 0xff;
					if( index == 0 && ((r == 0 && g >250 && b > 250) || (r > 250 && g ==0 && b > 250))) {
						alpha=1;
						a= 0;
						r=g=b=0;
					}

					*pDest++ =   b; // G
					*pDest++ =   g; // R
					*pDest++ =   r;
					*pDest++ = a; // A
				}

				pixelBuffer->unlock();


				//off += w*h;
				w/=2;
				h/=2;
		}
		// Unlock the pixel buffer

	}

	void LodTextureManager::GetDefaultTexture(Ogre::Resource* res) {
		// Create the texture
		Texture* texture = static_cast<Texture*>(res);
		Ogre::Image img;
		img.load("pending.png",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		texture->loadImage(img);
	}
}
