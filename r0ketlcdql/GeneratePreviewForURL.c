#include "r0ketlcd.h"

/* -----------------------------------------------------------------------------
   Generate a preview for file

   This function's job is to create preview for designated file
   ----------------------------------------------------------------------------- */

#define LCD_GRID_WIDTH 0.05

OSStatus GeneratePreviewForURL(void *thisInterface, QLPreviewRequestRef preview, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options)
{
    // load the actual file
    CFDataRef fileData = nil;
    CFDictionaryRef propertyDictionary = nil;
    SInt32 errorCode;
    if (CFURLCreateDataAndPropertiesFromResource(NULL, url, &fileData, &propertyDictionary, nil, &errorCode)) {
        
        CFIndex dataLength = CFDataGetLength(fileData);
        UInt8 *bytes = (UInt8 *)CFDataGetBytePtr(fileData);
        long pages = dataLength / ONE_LCD_FRAME_BYTESIZE;
        CGSize lcdSize = CGSizeMake(LCD_FRAME_PIXEL_WIDTH, LCD_FRAME_PIXEL_HEIGHT);
        CGContextRef cgContext = QLPreviewRequestCreateContext(preview, lcdSize, false, NULL);
        CGRect pageRect = CGRectMake(0,0,LCD_FRAME_PIXEL_WIDTH,LCD_FRAME_PIXEL_HEIGHT);
        if (cgContext) {
            
            CGColorRef lcdForegroundColor = CGColorCreateGenericRGB(0.467, 0.522, 0.047, 1.000);
            CGColorRef lcdBackgroundColor = CGColorCreateGenericRGB(0.227, 0.192, 0.000, 1.000);
            int isFirst = true;
            CGPoint origin = CGPointMake(0. + LCD_GRID_WIDTH,0.0 + LCD_GRID_WIDTH);
            while (pages-- > 0) {
                if (!isFirst) {
                CGContextEndPage(cgContext);
                CGContextBeginPage(cgContext, &pageRect);
                }
                    CGContextSetFillColorWithColor(cgContext, lcdBackgroundColor);
                CGContextFillRect(cgContext, pageRect);
                
                
                CGContextSetFillColorWithColor(cgContext, lcdForegroundColor);
//                CGContextFillRect(cgContext, CGRectMake(origin.x,origin.y,10,10));
                for (int y=0;y<9;y++) {
                    for (int x=0; x < LCD_FRAME_PIXEL_WIDTH; x++) {
                        UInt8 byte = bytes[y*LCD_FRAME_PIXEL_WIDTH + (LCD_FRAME_PIXEL_WIDTH - x - 1)];
                        if (byte > 0) {
                            UInt8 byteMask = 1;
                            for (int littleY = 0; littleY < 8; littleY++) {
                                if ((byte & (byteMask << littleY))) {
                                    CGContextFillRect(cgContext, CGRectMake(origin.x + x,origin.y + y*8 + littleY,1.0-2*LCD_GRID_WIDTH,1.0-2*LCD_GRID_WIDTH));
                                }
                            }
                        }
                    }
                }
                //origin.y += 68;
                bytes    += ONE_LCD_FRAME_BYTESIZE;
                isFirst = false;
            }
            
            // When we are done with our drawing code QLPreviewRequestFlushContext() is called to flush the context
            QLPreviewRequestFlushContext(preview, cgContext);
        }
        CFRelease(cgContext);
        CFRelease(fileData);
        CFRelease(propertyDictionary);
    }
    return noErr;
}

void CancelPreviewGeneration(void* thisInterface, QLPreviewRequestRef preview);

void CancelPreviewGeneration(void* thisInterface, QLPreviewRequestRef preview)
{
//    // implement only if supported
}
