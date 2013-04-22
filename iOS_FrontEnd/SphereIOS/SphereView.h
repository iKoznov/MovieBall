//
//  SphereView.h
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface SphereView : UIView
{
    BOOL _didCustomInit;
    
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
}

@end

