//
//  AppDelegate.h
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SphereView.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    SphereView* _glView;
}

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic, retain) SphereView *glView;

@end
