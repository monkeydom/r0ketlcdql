//
//  r0ketlcd.h
//  r0ketlcdql
//
//  Created by Dominik Wagner on 11.08.11.
//  Copyright 2011 TheCodingMonkeys. All rights reserved.
//

#ifndef r0ketlcdql_r0ketlcd_h
#define r0ketlcdql_r0ketlcd_h

#define ONE_LCD_FRAME_BYTESIZE 864
#define LCD_FRAME_PIXEL_WIDTH 96
#define LCD_FRAME_PIXEL_HEIGHT 68

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <QuickLook/QuickLook.h>

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options);
OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize);



#endif
