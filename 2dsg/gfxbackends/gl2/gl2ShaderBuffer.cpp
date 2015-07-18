/*
 * gl2ShaderBuffer.cpp
 *
 *  Created on: 5 juin 2015
 *      Author: Nicolas
 */

#include "gl2Shaders.h"
#include "string.h"

int ogl2ShaderBuffer::qualcommFix_=-1;

GLint ogl2ShaderBuffer::bindBuffer(GLint fbo)
{
	GLint oldFBO=0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
#ifdef OPENGL_DESKTOP
        if (GLEW_ARB_framebuffer_object)
#endif
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
#ifdef OPENGL_DESKTOP
		else
			glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);
#endif
        return oldFBO;
}

ogl2ShaderBuffer::ogl2ShaderBuffer(ShaderTexture *texture)
{

    if (qualcommFix_ == -1)
    {
        const char *extensions = (const char *)glGetString(GL_EXTENSIONS);
        qualcommFix_ = (extensions && strstr(extensions, "GL_QCOM"));
    }

    if (qualcommFix_)
        tempTexture_ = gtexture_TempTextureCreate(((ogl2ShaderTexture *)texture)->width, ((ogl2ShaderTexture *)texture)->height);
    else
        tempTexture_ = 0;


    textureId_=((ogl2ShaderTexture *)texture)->glid;

    GLint oldFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
#ifdef OPENGL_DESKTOP
    if (GLEW_ARB_framebuffer_object)
    {
#endif

        glGenFramebuffers(1, &glid);
        glBindFramebuffer(GL_FRAMEBUFFER, glid);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);

#ifdef OPENGL_DESKTOP
    }
	else
	{

    glGenFramebuffersEXT(1, &glid);
    glBindFramebufferEXT(GL_FRAMEBUFFER, glid);

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER, oldFBO);
	}
#endif
}

ogl2ShaderBuffer::~ogl2ShaderBuffer()
{
#ifdef OPENGL_DESKTOP
    if (GLEW_ARB_framebuffer_object)
#endif
    	glDeleteFramebuffers(1,&glid);
#ifdef OPENGL_DESKTOP
	else
		glDeleteFramebuffersEXT(1,&glid);
#endif
    if (tempTexture_)
        gtexture_TempTextureDelete(tempTexture_);
}

void ogl2ShaderBuffer::prepareDraw()
{
    if (qualcommFix_)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ((ogl2ShaderTexture *)gtexture_TempTextureGetName(tempTexture_))->glid, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId_, 0);
    }
}

void ogl2ShaderBuffer::readPixels(int x,int y,int width,int height,ShaderTexture::Format format,ShaderTexture::Packing packing,void *data)
{
    GLint oldFBO=bindBuffer(glid);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    GLuint glformat=GL_RGBA;
    switch (format)
    {
    	case ShaderTexture::FMT_ALPHA: glformat=GL_ALPHA; break;
    	case ShaderTexture::FMT_RGB: glformat=GL_RGB; break;
    	case ShaderTexture::FMT_RGBA: glformat=GL_RGBA; break;
    	case ShaderTexture::FMT_Y: glformat=GL_LUMINANCE; break;
    	case ShaderTexture::FMT_YA: glformat=GL_LUMINANCE_ALPHA; break;
    }
    GLuint gltype=GL_UNSIGNED_BYTE;
    switch (packing)
    {
    	case ShaderTexture::PK_UBYTE: gltype=GL_UNSIGNED_BYTE; break;
    	case ShaderTexture::PK_USHORT_565: gltype=GL_UNSIGNED_SHORT_5_6_5; break;
    	case ShaderTexture::PK_USHORT_4444: gltype=GL_UNSIGNED_SHORT_4_4_4_4; break;
    	case ShaderTexture::PK_USHORT_5551: gltype=GL_UNSIGNED_SHORT_5_5_5_1; break;
    }
    glReadPixels(x,y,width,height,glformat,gltype,data);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    bindBuffer(oldFBO);
}

