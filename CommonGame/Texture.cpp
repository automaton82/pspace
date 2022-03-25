//David Millman


#include "Texture.h"

Texture::Texture() : 
	texData_(0), //textureId_(-1), 
	adjustWidth_(1), adjustHeight_(1)
{
}

Texture::~Texture()
{
}

int Texture::getTextureID() const	
{ 
	//return textureId_; 
	if(!texData_)
		return -1;
	return texData_->glTextureId;
}

Uint Texture::getWidth() const		
{ 
	if(!texData_)
		return 0;
	return texData_->width; 
}

Uint Texture::getHeight() const		
{ 
	if(!texData_)
		return 0;
	return texData_->height; 
}

const char* Texture::getData() const	
{ 
	/*if(!texData_)
		return 0;
	return texData_->texData_; */

	return 0;
}

void Texture::getVertices(VertexData& vd) const
{
	vd.blX = 0;					vd.blY = 0;
	vd.tlX = 0;					vd.tlY = texData_->height;
	vd.brX = texData_->width;	vd.brY = 0;
	vd.trX = texData_->width;	vd.trY = texData_->height;
}

void Texture::getTexCoords(TexCoordData& td) const
{
	td.blX = 0;					td.blY = 0;
	td.tlX = 0;					td.tlY = adjustHeight_;
	td.brX = adjustWidth_;		td.brY = 0;
	td.trX = adjustWidth_;		td.trY = adjustHeight_;
}

void Texture::setTextureData(TextureData* data)
{
	texData_ = data;
	//textureId_ = data->glTextureId;

	adjustWidth_ = (double)getWidth() / (double)texData_->memWidth;
	adjustHeight_ = (double)getHeight() / (double)texData_->memHeight;
}

const TextureData* Texture::getTextureData() const
{
	return texData_;
}

bool Texture::load(const string& filename, bool stretch)
{
	filename_ = filename;

	/*textureId_ = textureManager.loadTexture(filename_);
	if(textureId_ < 0)
		return false;*/

	int id = textureManager.loadTexture(filename_, stretch);
	if(id < 0)
		return false;

	//texData_ = textureManager.getItem(textureId_);
	texData_ = textureManager.getItem(id);

	adjustWidth_ = (double)getWidth() / (double)texData_->memWidth;
	adjustHeight_ = (double)getHeight() / (double)texData_->memHeight;

	initAfterLoad();

	return true;
}

bool Texture::unload()
{
	return textureManager.unloadTexture(filename_);
}

void Texture::initAfterLoad()
{
	//do nothing here
}

bool Texture::enable() const
{
	/*assert(textureId_ == texData_->glTextureId);

	if(textureId_ < 0)
		return false;

	glBindTexture(GL_TEXTURE_2D, textureId_);*/

	if(!texData_)
		return false;

	if(texData_->glTextureId < 0)
		return false;

	glBindTexture(GL_TEXTURE_2D, texData_->glTextureId);

	return true;
}

void Texture::display() const
{
	glEnable(GL_TEXTURE_2D);
	
	if(!enable())
		return;

	glBegin(GL_QUAD_STRIP);
		glTexCoord2d(0, 0);
		glVertex2d(0, 0);
		
		glTexCoord2d(0, adjustHeight_);	
		glVertex2d(0, texData_->height);

		glTexCoord2d(adjustWidth_, 0);	
		glVertex2d(texData_->width, 0);

		glTexCoord2d(adjustWidth_, adjustHeight_);	
		glVertex2d(texData_->width, texData_->height);
	glEnd();
}

void Texture::displayAll() const
{
	glEnable(GL_TEXTURE_2D);

	if(!this->enable())
		return;

	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex2d(0, 0);
		
		glTexCoord2f(0, 1);	
		glVertex2d(0, texData_->memHeight);

		glTexCoord2f(1, 0);	
		glVertex2d(texData_->memWidth, 0);

		glTexCoord2f(1, 1);	
		glVertex2d(texData_->memWidth, texData_->memHeight);
	glEnd();
}

void Texture::displayArea(double x, double y, double width, double height) const
{
	glEnable(GL_TEXTURE_2D);
	
	if(!enable())
		return;

	glBegin(GL_QUAD_STRIP);
		glTexCoord2d(x*adjustWidth_, y*adjustHeight_);
		glVertex2d(0, 0);
		
		glTexCoord2d(x*adjustWidth_, (y+height)*adjustHeight_);	
		glVertex2d(0, height * texData_->height);
		//glVertex2d(0, 1);

		glTexCoord2d((x+width)*adjustWidth_, y*adjustHeight_);	
		glVertex2d(width * texData_->width, 0);
		//glVertex2d(1, 0);

		glTexCoord2d((x+width)*adjustWidth_, (y+height)*adjustHeight_);	
		glVertex2d(width * texData_->width, height * texData_->height);
		//glVertex2d(1, 1);
	glEnd();
}
