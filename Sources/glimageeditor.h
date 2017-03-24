#ifndef GLIMAGEEDITOR_H
#define GLIMAGEEDITOR_H

/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <math.h>
#include <map>

#include <QWidget>
#include <QtOpenGL>

#include "glwidgetbase.h"
#include "formmaterialindicesmanager.h"

#include "CommonObjects.h"

#ifdef USE_OPENGL_330
    #include <QOpenGLFunctions_3_3_Core>
    #define OPENGL_FUNCTIONS QOpenGLFunctions_3_3_Core
#else
    #include <QOpenGLFunctions_4_0_Core>
    #define OPENGL_FUNCTIONS QOpenGLFunctions_4_0_Core
#endif

#define BasicProp activeImage->properties->Basic
#define RemoveShadingProp activeImage->properties->RemoveShading
#define ColorLevelsProp activeImage->properties->ColorLevels
#define SurfaceDetailsProp activeImage->properties->SurfaceDetails
#define AOProp activeImage->properties->AO
#define GrungeProp targetImageGrunge->properties->Grunge
#define GrungeOnImageProp activeImage->properties->GrungeOnImage
#define NormalMixerProp activeImage->properties->NormalsMixer
#define BaseMapToOthersProp activeImage->properties->BaseMapToOthers
#define RMFilterProp activeImage->properties->RMFilter

class GLImage : public GLWidgetBase , protected OPENGL_FUNCTIONS
{
    Q_OBJECT

public:
    GLImage(QWidget *parent = 0 );
    ~GLImage();
    void cleanup();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setActiveImage(FBOImageProporties* ptr);
    FBOImageProporties* getActiveImage(){return activeImage;}
    void enableShadowRender(bool enable);
    void setConversionType(ConversionType conversionType);
    ConversionType getConversionType();
    void updateCornersPosition(QVector2D dc1,QVector2D dc2,QVector2D dc3,QVector2D dc4);
    void render();


    FBOImageProporties* targetImageDiffuse;
    FBOImageProporties* targetImageNormal;
    FBOImageProporties* targetImageHeight;
    FBOImageProporties* targetImageSpecular;
    FBOImageProporties* targetImageOcclusion;
    FBOImageProporties* targetImageRoughness;
    FBOImageProporties* targetImageMetallic;
    FBOImageProporties* targetImageGrunge;

    FBOImageProporties* targetImageMaterial;
public slots:
    void resizeFBO(int width, int height);
    void imageChanged();
    void resetView();
    void selectPerspectiveTransformMethod(int method);
    void selectUVManipulationMethod(UVManipulationMethods method);
    void updateCornersWeights(float w1, float w2, float w3, float w4);
    void selectSeamlessMode(SeamlessMode mode);
    void toggleColorPicking(bool toggle);
    void pickImageColor(QtnPropertyABColor *property);

    void copyRenderToPaintFBO(); // when image is rendered copy it to paintFBO in order to display result
signals:
    void rendered();
    void readyGL();
    void colorPicked(QVector4D color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void relativeMouseMoveEvent(int dx, int dy, bool *wrapMouse, Qt::MouseButtons buttons);
    void wheelEvent(QWheelEvent *event);
    void updateMousePosition();
    void showEvent(QShowEvent* event);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
public:
    void applyNormalFilter(  QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyHeightToNormal(QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyPerspectiveTransformFilter(  QGLFramebufferObjectPtr inputFBO,
                                           QGLFramebufferObjectPtr outputFBO);

    void applySeamlessLinearFilter(QGLFramebufferObjectPtr inputFBO,
                                   QGLFramebufferObjectPtr outputFBO);

    void applySeamlessFilter(QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyOcclusionFilter(GLuint height_tex,
                              GLuint normal_tex,
                              QGLFramebufferObjectPtr outputFBO);

    void applyNormalToHeight(FBOImageProporties *image,
                             QGLFramebufferObjectPtr normalFBO,
                             QGLFramebufferObjectPtr heightFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyCPUNormalizationFilter(QGLFramebufferObjectPtr inputFBO,
                                  QGLFramebufferObjectPtr outputFBO);

    void applyAddNoiseFilter(QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyGaussFilter(QGLFramebufferObjectPtr sourceFBO, QGLFramebufferObjectPtr auxFBO,
                          QGLFramebufferObjectPtr outputFBO, int no_iter, float w =0);

    void applyGrayScaleFilter(QGLFramebufferObjectPtr inputFBO,
                              QGLFramebufferObjectPtr outputFBO);

    void applyInvertComponentsFilter(QGLFramebufferObjectPtr inputFBO,
                                    QGLFramebufferObjectPtr outputFBO);

    void applyColorHueFilter(  QGLFramebufferObjectPtr inputFBO,
                               QGLFramebufferObjectPtr outputFBO);

    void applyRoughnessFilter(QGLFramebufferObjectPtr inputFBO,
                              QGLFramebufferObjectPtr auxFBO,
                              QGLFramebufferObjectPtr outputFBO);

    void applyDGaussiansFilter(QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr auxFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyContrastFilter(QGLFramebufferObjectPtr inputFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyHeightProcessingFilter( QGLFramebufferObjectPtr inputFBO,
                                      QGLFramebufferObjectPtr outputFBO);

    void applyRemoveLowFreqFilter(QGLFramebufferObjectPtr inputFBO,
                                  QGLFramebufferObjectPtr auxFBO,
                                  QGLFramebufferObjectPtr outputFBO);

    void applyInverseColorFilter(QGLFramebufferObjectPtr inputFBO,
                                 QGLFramebufferObjectPtr outputFBO);

    void applyOverlayFilter( QGLFramebufferObjectPtr layerAFBO,
                             QGLFramebufferObjectPtr layerBFBO,
                             QGLFramebufferObjectPtr outputFBO);

    void applyRemoveShadingFilter(QGLFramebufferObjectPtr inputFBO,
                                   QGLFramebufferObjectPtr aoMaskFBO, QGLFramebufferObjectPtr refFBO,
                                   QGLFramebufferObjectPtr outputFBO);

    void applySmallDetailsFilter(QGLFramebufferObjectPtr inputFBO,
                                 QGLFramebufferObjectPtr auxFBO,
                                 QGLFramebufferObjectPtr outputFBO);
    void applyMediumDetailsFilter(QGLFramebufferObjectPtr inputFBO,
                                          QGLFramebufferObjectPtr auxFBO,
                                 QGLFramebufferObjectPtr outputFBO);

    void applySharpenBlurFilter(QGLFramebufferObjectPtr inputFBO,
                                QGLFramebufferObjectPtr auxFBO,
                                QGLFramebufferObjectPtr outputFBO);


    void applyNormalFilter(QGLFramebufferObjectPtr inputFBO);




    void applyNormalsStepFilter(QGLFramebufferObjectPtr inputFBO,
                                QGLFramebufferObjectPtr outputFBO);


    void applyNormalMixerFilter(QGLFramebufferObjectPtr inputFBO,
                                QGLFramebufferObjectPtr outputFBO);

    void applySobelToNormalFilter(QGLFramebufferObjectPtr inputFBO,
                                  QGLFramebufferObjectPtr outputFBO, BaseMapConvLevelProperties &convProp);

    void applyNormalAngleCorrectionFilter(QGLFramebufferObjectPtr inputFBO,
                                          QGLFramebufferObjectPtr outputFBO);

    void applyNormalExpansionFilter(QGLFramebufferObjectPtr inputFBO,
                                  QGLFramebufferObjectPtr outputFBO);

    void applyMixNormalLevels(GLuint level0,
                              GLuint level1,
                              GLuint level2,
                              GLuint level3,
                              QGLFramebufferObjectPtr outputFBO);


    void applyBaseMapConversion(QGLFramebufferObjectPtr baseMapFBO,
                                QGLFramebufferObjectPtr auxFBO,
                                QGLFramebufferObjectPtr outputFBO, BaseMapConvLevelProperties &convProp);
    void applyPreSmoothFilter(QGLFramebufferObjectPtr inputFBO,
                              QGLFramebufferObjectPtr auxFBO,
                             QGLFramebufferObjectPtr outputFBO, BaseMapConvLevelProperties &convProp);

    void applyCombineNormalHeightFilter(QGLFramebufferObjectPtr normalFBO,
                                        QGLFramebufferObjectPtr heightFBO,
                                        QGLFramebufferObjectPtr outputFBO);


    void applyRoughnessColorFilter(QGLFramebufferObjectPtr inputFBO,
                                   QGLFramebufferObjectPtr outputFBO);

    void copyFBO(QGLFramebufferObjectPtr src,QGLFramebufferObjectPtr dst);
    void copyTex2FBO(GLuint src_tex_id,QGLFramebufferObjectPtr dst);

    void applyAllUVsTransforms(QGLFramebufferObjectPtr inoutFBO);

    void applyGrungeImageFilter (QGLFramebufferObjectPtr inputFBO,
                                 QGLFramebufferObjectPtr outputFBO,
                                 QGLFramebufferObjectPtr grungeFBO);

    void applyGrungeRandomizationFilter(QGLFramebufferObjectPtr inputFBO,
                                        QGLFramebufferObjectPtr outputFBO);

    void applyGrungeWarpNormalFilter(QGLFramebufferObjectPtr inputFBO,
                                     QGLFramebufferObjectPtr outputFBO);

    void updateProgramUniforms(int step);

private:
    void makeScreenQuad();

    QOpenGLShaderProgram *program;
    FBOImageProporties* activeImage;
    QGLFramebufferObjectPtr averageColorFBO; // small FBO used for calculation of average color
    QGLFramebufferObjectPtr samplerFBO1; // FBO with size 1024x1024
    QGLFramebufferObjectPtr samplerFBO2; // FBO with size 1024x1024 used for different processing
    
    // FBOs used in image processing
    QGLFramebufferObjectPtr auxFBO1;
    QGLFramebufferObjectPtr auxFBO2;
    QGLFramebufferObjectPtr auxFBO3;
    QGLFramebufferObjectPtr auxFBO4;

    QGLFramebufferObjectPtr auxFBO1BMLevels[3]; // 2 times smaller. 4 and 8
    QGLFramebufferObjectPtr auxFBO2BMLevels[3]; //
    QGLFramebufferObjectPtr auxFBO0BMLevels[3]; //

    QGLFramebufferObjectPtr paintFBO;  // Used for painting texture
    QGLFramebufferObjectPtr renderFBO; // Used for rendering to it

    std::map<std::string,GLuint> subroutines;
    std::map<std::string,QOpenGLShaderProgram*> filter_programs; // all filters in one array

    GLuint vao, vbos[3];

    ConversionType conversionType;
    bool bShadowRender;
    bool bSkipProcessing;   // draw quad but skip all the processing step (using during mouse interaction)
    float windowRatio;      // window width-height ratio
    float fboRatio;         // active fbo width-height ratio

    // Image resize
    int resize_width;
    int resize_height;

    // Image translation and physical cursor position
    double xTranslation; // x position of the image in the window
    double yTranslation; // y position
    double cursorPhysicalXPosition; // physical cursor X position in window
    double cursorPhysicalYPosition;

    double zoom; // magnification of the image
    double orthographicProjHeight;
    double orthographicProjWidth;

    // perspective transformation
    QVector2D cornerPositions[4]; // position of four corner used to perform perspective transformation of quad
    QVector2D grungeCornerPositions[4];

    QVector4D cornerWeights;
    int draggingCorner; // contains Id of current corner dragged or -1
    QCursor cornerCursors[4];
    int gui_perspective_mode; // choose proper interpolation method
    int gui_seamless_mode; // if 0 standard blending, if 1 mirror mode

    bool bToggleColorPicking;
    QtnPropertyABColor* ptr_ABColor;

    // uv manipulations method
    UVManipulationMethods uvManilupationMethod;

    // openGL 330 variables
    TextureTypes openGL330ForceTexType;

    // rendering variables
    bool bRendering;//define if opengl is currently rendering some textures
};

#endif // GLIMAGEEDITOR_H

