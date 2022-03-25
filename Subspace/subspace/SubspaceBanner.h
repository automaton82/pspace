#ifndef _SUBSPACEBANNER_H_
#define _SUBSPACEBANNER_H_

#include "Texture.h"

class SubspaceBanner : 
	public Texture
{
public:
	SubspaceBanner();
	SubspaceBanner(const SubspaceBanner& copy);
	~SubspaceBanner();

	SubspaceBanner& operator=(const SubspaceBanner& rhs);

	bool loadBanner(const char* indexes, Uint numIndexes = 96);

private:

	TextureData bannerData_;
};

#endif