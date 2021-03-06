//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// SurfaceD3D.h: D3D implementation of an EGL surface

#ifndef LIBANGLE_RENDERER_D3D_SURFACED3D_H_
#define LIBANGLE_RENDERER_D3D_SURFACED3D_H_

#include "libANGLE/renderer/SurfaceImpl.h"
#include "libANGLE/renderer/d3d/NativeWindowD3D.h"

namespace egl
{
class Surface;
}

namespace rx
{
class SwapChainD3D;
class RendererD3D;

class SurfaceD3D : public SurfaceImpl
{
  public:
    ~SurfaceD3D() override;
    void releaseSwapChain();

    egl::Error initialize(const egl::Display *display) override;
    FramebufferImpl *createDefaultFramebuffer(const gl::FramebufferState &state) override;

    egl::Error swap(const gl::Context *context) override;
    egl::Error postSubBuffer(const gl::Context *context,
                             EGLint x,
                             EGLint y,
                             EGLint width,
                             EGLint height) override;
    egl::Error querySurfacePointerANGLE(EGLint attribute, void **value) override;
    egl::Error bindTexImage(gl::Texture *texture, EGLint buffer) override;
    egl::Error releaseTexImage(EGLint buffer) override;
    egl::Error getSyncValues(EGLuint64KHR *ust, EGLuint64KHR *msc, EGLuint64KHR *sbc) override;
    void setSwapInterval(EGLint interval) override;

    EGLint getWidth() const override;
    EGLint getHeight() const override;

    EGLint isPostSubBufferSupported() const override;
    EGLint getSwapBehavior() const override;

    // D3D implementations
    SwapChainD3D *getSwapChain() const;

    egl::Error resetSwapChain(const egl::Display *display);

    egl::Error checkForOutOfDateSwapChain(const gl::Context *context);

    gl::Error getAttachmentRenderTarget(const gl::Context *context,
                                        GLenum binding,
                                        const gl::ImageIndex &imageIndex,
                                        FramebufferAttachmentRenderTarget **rtOut) override;
    gl::Error initializeContents(const gl::Context *context,
                                 const gl::ImageIndex &imageIndex) override;

    const angle::Format *getD3DTextureColorFormat() const override;

  protected:
    SurfaceD3D(const egl::SurfaceState &state,
               RendererD3D *renderer,
               egl::Display *display,
               EGLNativeWindowType window,
               EGLenum buftype,
               EGLClientBuffer clientBuffer,
               const egl::AttributeMap &attribs);

    egl::Error swapRect(const gl::Context *context,
                        EGLint x,
                        EGLint y,
                        EGLint width,
                        EGLint height);
    egl::Error resetSwapChain(const gl::Context *context,
                              int backbufferWidth,
                              int backbufferHeight);
    egl::Error resizeSwapChain(const gl::Context *context,
                               int backbufferWidth,
                               int backbufferHeight);

    RendererD3D *mRenderer;
    egl::Display *mDisplay;

    bool mFixedSize;
    GLint mOrientation;

    GLenum mRenderTargetFormat;
    GLenum mDepthStencilFormat;
    const angle::Format *mColorFormat;

    SwapChainD3D *mSwapChain;
    bool mSwapIntervalDirty;

    NativeWindowD3D *mNativeWindow;  // Handler for the Window that the surface is created for.
    EGLint mWidth;
    EGLint mHeight;

    EGLint mSwapInterval;

    HANDLE mShareHandle;
    IUnknown *mD3DTexture;

    EGLenum mBuftype;
};

class WindowSurfaceD3D : public SurfaceD3D
{
  public:
    WindowSurfaceD3D(const egl::SurfaceState &state,
                     RendererD3D *renderer,
                     egl::Display *display,
                     EGLNativeWindowType window,
                     const egl::AttributeMap &attribs);
    ~WindowSurfaceD3D() override;
};

class PbufferSurfaceD3D : public SurfaceD3D
{
  public:
    PbufferSurfaceD3D(const egl::SurfaceState &state,
                      RendererD3D *renderer,
                      egl::Display *display,
                      EGLenum buftype,
                      EGLClientBuffer clientBuffer,
                      const egl::AttributeMap &attribs);
    ~PbufferSurfaceD3D() override;
};

}  // namespace rx

#endif // LIBANGLE_RENDERER_D3D_SURFACED3D_H_
