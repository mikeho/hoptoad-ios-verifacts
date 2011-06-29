/*
 
 Copyright (C) 2011 GUI Cocoa, LLC.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#import <TargetConditionals.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#elif TARGET_OS_MAC
#import <Cocoa/Cocoa.h>
#else
#error [Hoptoad] unsupported platform
#endif
#import <SystemConfiguration/SystemConfiguration.h>

#import "HTNotifierDelegate.h"

// notifier version
extern NSString * const HTNotifierVersion;

// internal
extern NSString * const HTNotifierAlwaysSendKey;

/*
 
 use these variables in your alert title and alert
 body to have their values substituted at runtime.
 
 */
extern NSString * const HTNotifierBundleName;      // app name
extern NSString * const HTNotifierBundleVersion;   // bundle version

/*
 
 these standard environment names provide default values
 for you to pick from. the automatic environment will
 set development or release depending on the DEBUG flag.
 
 */
extern NSString * const HTNotifierDevelopmentEnvironment;
extern NSString * const HTNotifierAdHocEnvironment;
extern NSString * const HTNotifierAppStoreEnvironment;
extern NSString * const HTNotifierReleaseEnvironment;
extern NSString * const HTNotifierAutomaticEnvironment;

/*
 
 HTNotifier is the primary class of the notifer library
 
 start the notifier by calling
 startNotifierWithAPIKey:environmentName:
 
 access the shared instance by calling sharedNotifier
 
 */
#if TARGET_OS_IPHONE
@interface HTNotifier : NSObject <UIAlertViewDelegate> {
#else
@interface HTNotifier : NSObject {
#endif
@private
    NSMutableDictionary *_environmentInfo;
    NSString *_environmentName;
    NSString *_apiKey;
	NSObject<HTNotifierDelegate> *_delegate;
    BOOL _useSSL;
	SCNetworkReachabilityRef reachability;
}

// properties
@property (nonatomic, readonly) NSDictionary *environmentInfo;
@property (nonatomic, readonly) NSString *apiKey;
@property (nonatomic, readonly) NSString *environmentName;
@property (nonatomic, assign) NSObject<HTNotifierDelegate> *delegate;

/*
 
 control whether notices are posted using SSL. your account
 must support this feature
 
 default:NO
 
 */
@property (nonatomic, assign) BOOL useSSL;

/*
 
 this method is the entry point for the library. any code
 executed after this method call is monitored for crashes
 and signals
 
 the values for key and environment name must not be nil
 and must have a length greater than 0
 
 include any of the above constant strings in the
 enviromnent name to have the value replaced by the library
 
 */
+ (void)startNotifierWithAPIKey:(NSString *)key environmentName:(NSString *)name;

/*
 
 access the shared notifier object.
 
 if this is called before `startNotifierWithAPIKey:environmentName:`
 nil will be returned.
 
 */
+ (HTNotifier *)sharedNotifier;

/*
 
 writes a test notice if one does not exist already. it
 will be reported just as an actual crash.
 
 */
- (void)writeTestNotice;

/*
 
 set environment info key/value pair. passing nil as the
 value will remove the value for the given key.
 
 */
- (void)setEnvironmentValue:(NSString *)valueOrNil forKey:(NSString *)key;

/*
 
 get environment info value for a given key.
 
 */
- (NSString *)environmentValueForKey:(NSString *)key;

/*
 
 scan for notices and take action if hoptoad is reachable.
 if the user has chosen to always send notices they will
 be posted imediately, otherwise the user will be asked
 for their preference.
 
 */
- (BOOL)postNotices;

@end
