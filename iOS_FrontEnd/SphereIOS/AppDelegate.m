//
//  AppDelegate.m
//  SphereIOS
//
//  Created by IGOR KOZNOV on 28.03.13.
//  Copyright (c) 2013 Biarum. All rights reserved.
//

#import "AppDelegate.h"
#import "SphereView.h"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    self.window = [[UIWindow alloc] initWithFrame:screenBounds];
    
    self.glView = [[SphereView alloc] initWithFrame:screenBounds];
    [self.window addSubview:_glView];
    [self.window makeKeyAndVisible];
    
//    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
//    // Override point for customization after application launch.
//    self.window.backgroundColor = [UIColor whiteColor];
//    
//    SphereView *sphereView = [[SphereView alloc] initWithFrame:self.window.bounds];
//    [self.window addSubview:sphereView];
//    
//    [self.window makeKeyAndVisible];
    
    return YES;
}

@end
