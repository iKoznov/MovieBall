//
//  SphereView.h
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

@interface SphereView : UIView {
    BOOL _didCustomInit;
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    GLuint _colorRenderBuffer;
    GLuint _depthRenderBuffer;
    
    /* Shader Attribs */
    GLuint _positionSlot;
    GLuint _colorSlot;
    
    /* Shader Const */
    GLuint _projectionUniform;
    GLuint _modelViewUniform;
    
    /* Scene params */
    float _currentRotation;
}

@end
