
#include "r0ketlcd.h"

/* -----------------------------------------------------------------------------
    Generate a thumbnail for file

   This function's job is to create thumbnail for designated file as fast as possible
   ----------------------------------------------------------------------------- */

OSStatus GenerateThumbnailForURL(void *thisInterface, QLThumbnailRequestRef thumbnail, CFURLRef url, CFStringRef contentTypeUTI, CFDictionaryRef options, CGSize maxSize)
{
    // load the actual file
    CFDataRef fileData = nil;
    CFDictionaryRef propertyDictionary = nil;
    SInt32 errorCode;
    if (CFURLCreateDataAndPropertiesFromResource(NULL, url, &fileData, &propertyDictionary, nil, &errorCode)) {
        
        CFIndex dataLength = CFDataGetLength(fileData);
        UInt8 *bytes = (UInt8 *)CFDataGetBytePtr(fileData);
        long pages = dataLength / ONE_LCD_FRAME_BYTESIZE;
        CGSize lcdSize = CGSizeMake(LCD_FRAME_PIXEL_WIDTH, LCD_FRAME_PIXEL_HEIGHT * pages);
        CGContextRef cgContext = QLThumbnailRequestCreateContext(thumbnail, lcdSize, true, NULL);
        if(cgContext) {
            
            CGColorRef lcdForegroundColor = CGColorCreateGenericRGB(0.467, 0.522, 0.047, 1.000);
            CGColorRef lcdBackgroundColor = CGColorCreateGenericRGB(0.227, 0.192, 0.000, 1.000);
            CGContextSetFillColorWithColor(cgContext, lcdBackgroundColor);
            CGContextFillRect(cgContext, CGRectMake(0,0,lcdSize.width,lcdSize.height));
            
            CGPoint origin = CGPointMake(0,0);
            while (pages-- > 0) {
                CGContextSetFillColorWithColor(cgContext, lcdForegroundColor);
                //                CGContextFillRect(cgContext, CGRectMake(origin.x,origin.y,10,10));
                for (int y=0;y<9;y++) {
                    for (int x=0; x < LCD_FRAME_PIXEL_WIDTH; x++) {
                        UInt8 byte = bytes[y*LCD_FRAME_PIXEL_WIDTH + (LCD_FRAME_PIXEL_WIDTH - x - 1)];
                        if (byte > 0) {
                            UInt8 byteMask = 1;
                            for (int littleY = 0; littleY < 8; littleY++) {
                                if ((byte & (byteMask << littleY))) {
                                    CGContextFillRect(cgContext, CGRectMake(origin.x + x,origin.y + y*8 + littleY,1,1));
                                }
                            }
                        }
                    }
                }
                origin.y += 68;
                bytes    += ONE_LCD_FRAME_BYTESIZE;
            }
            
            // When we are done with our drawing code QLPreviewRequestFlushContext() is called to flush the context
            QLThumbnailRequestFlushContext(thumbnail, cgContext);
        }
        CFRelease(cgContext);
        CFRelease(fileData);
        CFRelease(propertyDictionary);
    }
    return noErr;
}

void CancelThumbnailGeneration(void* thisInterface, QLThumbnailRequestRef thumbnail);
void CancelThumbnailGeneration(void* thisInterface, QLThumbnailRequestRef thumbnail)
{
//    // implement only if supported
}
