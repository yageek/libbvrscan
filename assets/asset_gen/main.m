//
//  main.m
//  asset_gen
//
//  Created by Yannick Heinrich on 11.10.18.
//  Copyright © 2018 yageek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <CoreText/CoreText.h>
#import <CoreFoundation/CoreFoundation.h>

int main(int argc, const char * argv[]) {

    if( argc < 2 ){
        NSLog(@"Name output at minimum");
        exit(-1);
    }

    @autoreleasepool {

        // We load the font normally copied at the same level
        NSURL *fontURL = [NSURL fileURLWithPath:@"OCRB.ttf"];
        if(!fontURL) {
            NSLog(@"Impossible to locate the font");
            exit(-1);
        }       
        
        NSString *chars = @"0123456789<+>";
        NSString * output = [[NSString stringWithCString:argv[1] encoding:NSUTF8StringEncoding] stringByAppendingPathComponent:@"generated"];
        BOOL isDir = false;
        if(![[NSFileManager defaultManager] fileExistsAtPath:output isDirectory:&isDir]) {
            [[NSFileManager defaultManager] createDirectoryAtPath:output withIntermediateDirectories:NO attributes:nil error:NULL];
        }

        NSDictionary<NSAttributedStringKey, id>* attrs = @{
                                                     NSForegroundColorAttributeName: [NSColor blackColor],
                                                     NSFontAttributeName: [NSFont fontWithName:@"OCRB" size:30.0]
                                                     };

        for(NSUInteger i = 0; i < chars.length; i++) {

            NSString * s = [chars substringWithRange:NSMakeRange(i, 1)];
            NSLog(@"Converting %@...", s);

            NSAttributedString *attrString = [[NSAttributedString alloc] initWithString:s attributes:attrs];
            CTLineRef line = CTLineCreateWithAttributedString((CFAttributedStringRef)attrString);


           NSImage *image = [NSImage imageWithSize:NSMakeSize(32.0, 32.0) flipped:NO drawingHandler:^BOOL(NSRect dstRect) {

               [s drawInRect:NSMakeRect(0.0, 0.0, 32.0, 32.0) withAttributes:attrs];

               return YES;
           }];

           NSBitmapImageRep *bitmap = [NSBitmapImageRep imageRepWithData:image.TIFFRepresentation];
           NSData *data = [bitmap representationUsingType:NSBitmapImageFileTypePNG properties:@{}];


           NSString *outputName = [[output stringByAppendingPathComponent: s] stringByAppendingString:@".png"];
           [data writeToFile:outputName atomically:YES];

            // CGSize size = CGSizeMake(32.0, 32.0);
            // size_t bytesPerPixel = 1;
            // size_t bytesPerRow = size.width*bytesPerPixel;
            // uint8 *src = calloc(size.width*bytesPerPixel*size.height, 1);


            // CGColorSpaceRef inputColorSpace = CGColorSpaceCreateWithName(kCGColorSpaceLinearGray);
            // CGContextRef ctx = CGBitmapContextCreate(&src[0], size.width, size.height, 8, bytesPerRow, inputColorSpace, 0);
            // CGContextSetFillColorWithColor(ctx, [NSColor whiteColor].CGColor);
            // CGContextFillRect(ctx, CGRectMake(0, 0, size.width, size.height));
            // CTLineDraw(line, ctx);

                        // for (unsigned int i = 0; i < size.width; i++) {
                        //     for (unsigned int j = 0; j < size.height; j++) {
                        //         int index = i + j*size.width;
                        //         if(src[index] == 0xFF) {
                        //             src[index] = 0;
                        //         } else {
                        //             src[index] = 1;
                        //         }
                        //     }
                        // }

            // NSData * data = [NSData dataWithBytes:src length:size.width*bytesPerPixel*size.height];

            // NSString *outputName = [[output stringByAppendingPathComponent: s] stringByAppendingString:@".ocrb"];
            // [data writeToFile:outputName atomically:YES];


            // CGContextRelease(ctx);
            // CGColorSpaceRelease(inputColorSpace);
        }
    }
    return 0;
}
