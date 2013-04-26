//
//  SphereView.m
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import "SphereView.h"
#include "mStr.h"
#include "icosahedron.h"
#include "tomatoes.h"

#pragma mark -
#pragma mark SphereView implementation
@implementation SphereView

#pragma mark -
#pragma mark Initialization
- (id)init { self = [super init]; [self customInit]; return self; }
- (id)initWithFrame:(CGRect)frame { self = [super initWithFrame:frame]; [self customInit]; return self; }
- (void)customInit {
    if (_didCustomInit) return;
    _didCustomInit = YES;
    
    _rootMovie = tomatoes("Titanic");
    
    [self setupLayer];
    [self setupContext];
    
    _width = self.frame.size.width*self.layer.contentsScale;
    _height = self.frame.size.height*self.layer.contentsScale;
    _resoucePath = strdup( [[[NSBundle mainBundle] bundlePath] UTF8String] );
    
    setupDepthBuffer();
    setupRenderBuffer();
    setupFrameBuffer();
    
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    
    compileShaders();
    setupVBOs();
    [self setupDisplayLink];
    [self setupGestures];
    
    loadPosters();
}

#pragma mark -
#pragma mark Gesture setup
- (void)setupGestures {
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panHandler:)];
    [self addGestureRecognizer:pan];
}
- (void)panHandler:(UIPanGestureRecognizer *)gesture {
    CGPoint translation = [gesture translationInView:self];
    _lon += translation.x * self.layer.contentsScale;
    _lat += translation.y * self.layer.contentsScale;
    [gesture setTranslation:CGPointZero inView:self];
}

#pragma mark -
#pragma mark Layer setup
+ (Class)layerClass { return [CAEAGLLayer class]; }
- (void)setupLayer { _eaglLayer = (CAEAGLLayer *)self.layer; _eaglLayer.opaque = YES; _eaglLayer.contentsScale = [[UIScreen mainScreen] scale]; }
- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

#pragma mark Context setup
- (void)setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) { NSLog(@"Failed to initialize OpenGLES 2.0 context"); exit(1); }
    if (![EAGLContext setCurrentContext:_context]) { NSLog(@"Failed to set current OpenGL context"); exit(1); }
}

- (void)render:(CADisplayLink*)displayLink
{
    render();
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
