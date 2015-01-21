#include <GBuffer.h>

#include <vector>

GBuffer::GBuffer():
m_FBO(0),
m_Color_Attachment(0),
m_Normal_Attachment(0),
m_Depth_Attachment(0)
{}

GBuffer::GBuffer(unsigned windowWidth, unsigned windowHeight)
{
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);


	// ----------- Generate the Color Texture and bind it to the Framebuffer -------------
	m_Color_Attachment = prepare_texture(windowWidth, windowHeight, GL_CLAMP, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Color_Attachment, 0);

	// ----------- Generate the Normal Texture and bind it to the Framebuffer -------------
	m_Normal_Attachment = prepare_texture(windowWidth, windowHeight, GL_CLAMP, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_Normal_Attachment, 0);
	
	// ----------- Generate the Position Texture and bind it to the Framebuffer ----------------
	m_Position_Attachment = prepare_texture(windowWidth, windowHeight, GL_CLAMP, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_Position_Attachment, 0);

	// ----------- generate Depth Buffer ----------------
	glGenRenderbuffers(1, &m_Depth_Attachment);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Depth_Attachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);

	// bind the depth buffer to the Framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Depth_Attachment);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3,  attachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bind_framebuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void GBuffer::unbind_framebuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bind_texture(GLenum texture_unit, GBUFFER_TEXTURE_TYPE texture) const
{
	glActiveTexture(texture_unit);

	switch (texture)
	{
		case GBUFFER_TEXTURE_COLOR:
			glBindTexture(GL_TEXTURE_2D, m_Color_Attachment); break;
		case GBUFFER_TEXTURE_NORMAL:
			glBindTexture(GL_TEXTURE_2D, m_Normal_Attachment); break;
		case GBUFFER_TEXTURE_POSITION:
			glBindTexture(GL_TEXTURE_2D, m_Position_Attachment); break;
		case GBUFFER_TEXTURE_DETPH:
			glBindTexture(GL_TEXTURE_2D, m_Depth_Attachment); break;
		default:
			return; //TODO: ERROR
	}

}

void GBuffer::resize(unsigned windowWidth, unsigned windowHeight) const
{
	glBindTexture(GL_TEXTURE_2D, m_Color_Attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, m_Normal_Attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_Position_Attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, m_Depth_Attachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
}

unsigned GBuffer::prepare_texture( unsigned windowWidth, unsigned windowHeight, GLenum wrap, GLenum filter)
{
	unsigned texture(0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, windowWidth, windowHeight, 
				 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	return texture;
}