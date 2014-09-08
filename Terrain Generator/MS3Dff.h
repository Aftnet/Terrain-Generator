/*
Milkshape 3D v 1.8.2 Binary file format specification

Ripped straight off the Milkshape SDK

As every model will be rendered using a single texture and I have no plans to add animation support, some data is redundant.
*/

#ifndef __MS3DFF_H__
#define __MS3DFF_H__


//
// max values
//
#define MAX_VERTICES    65534
#define MAX_TRIANGLES   65534
#define MAX_GROUPS      255
#define MAX_MATERIALS   128
#define MAX_JOINTS      128



//
// flags
//
#define SELECTED        1
#define HIDDEN          2
#define SELECTED2       4
#define DIRTY           8



//
// types
//
#ifndef byte
typedef unsigned char byte;
#endif // byte

#ifndef word
typedef unsigned short word;
#endif // word

//
// First comes the header (sizeof(ms3d_header_t) == 14)
//
typedef struct
{
    char    id[10];                                     // always "MS3D000000"
    int     version;                                    // 4
} ms3d_header;

//
// Then comes the number of vertices
//

//word nNumVertices; // 2 bytes

typedef struct
{
    byte    flags;                                      // SELECTED | SELECTED2 | HIDDEN
    float   vertex[3];                                  //
    char    boneId;                                     // -1 = no bone
    byte    referenceCount;
} ms3d_vertex;

//word nNumTriangles; // 2 bytes

//
// Then come nNumTriangles times ms3d_triangle_t structs (sizeof(ms3d_triangle_t) == 70)
//
typedef struct
{
    word    flags;                                      // SELECTED | SELECTED2 | HIDDEN
    word    vertexIndices[3];                           //
    float   vertexNormals[3][3];                        //
    float   s[3];                                       //
    float   t[3];                                       //
    byte    smoothingGroup;                             // 1 - 32
    byte    groupIndex;                                 //
} ms3d_triangle;

//
// Then comes the number of groups
//
//word nNumGroups; // 2 bytes

//
// Then come nNumGroups times groups (the sizeof a group is dynamic, because of triangleIndices is numtriangles long)
//
typedef struct
{
    byte            flags;                              // SELECTED | HIDDEN
    char            name[32];                           //
    word            numtriangles;                       //
    word            *triangleIndices;					// pointer to triangle indices buffer, memory for it must be allocated at runtime
    char            materialIndex;                      // -1 = no material
} ms3d_group;

//
// number of materials
//
//word nNumMaterials; // 2 bytes

//
// Then come nNumMaterials times ms3d_material_t structs (sizeof(ms3d_material_t) == 361)
//
typedef struct
{
    char            name[32];                           //
    float           ambient[4];                         //
    float           diffuse[4];                         //
    float           specular[4];                        //
    float           emissive[4];                        //
    float           shininess;                          // 0.0f - 128.0f
    float           transparency;                       // 0.0f - 1.0f
    char            mode;                               // 0, 1, 2 is unused now
    char            texture[128];                        // texture.bmp
    char            alphamap[128];                       // alpha.bmp
} ms3d_material;

#endif