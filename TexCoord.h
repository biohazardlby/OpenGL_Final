///
//  TexCoord.h
//
//  Representation of 2D texture coordinates.
//
//  Author:  Warren R. Carithers
//  Date:    2019/04/13 16:10:27
///

#ifndef _TEXCOORD_H_
#define _TEXCOORD_H_

///
// Information pertaining to (U,V) texture coordinates
///

typedef
    struct st_tcoord {
        float u;
        float v;
    } TexCoord;

#endif
