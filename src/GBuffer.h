#ifndef GBUFFER_H
#define GBUFFER_H

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE
	{
		GBUFFER_TEXTURE_COLOR,
		GBUFFER_TEXTURE_NORMAL,
		GBUFFER_TEXTURE_POSITION,
		GBUFFER_TEXTURE_DETPH
	};

	GBuffer();
	GBuffer(unsigned windowWidth, unsigned windowHeight);

	void bind_framebuffer() const;
	void unbind_framebuffer() const;

	void bind_texture(GLenum texture_unit, GBUFFER_TEXTURE_TYPE texture) const;

	void resize(unsigned windowWidth, unsigned windowHeight) const;

	static unsigned prepare_texture(unsigned windowWidth, unsigned windowHeight, GLenum wrap, GLenum filter); 

private:
	unsigned m_FBO;
	unsigned m_Color_Attachment;
	unsigned m_Normal_Attachment;
	unsigned m_Position_Attachment;
	unsigned m_Depth_Attachment;
};


//GBUFFER_H
#endif 