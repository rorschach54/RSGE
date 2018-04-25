#ifndef __RSGE_GFX_ANIMATION_H_
#define __RSGE_GFX_ANIMATION_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/shape.h>
#include <rsge/gfx/surface.h>
#include <linmath.h>

typedef enum {
    RSGE_ANIMATION_OBJECT_TYPE_SURFACE,
    RSGE_ANIMATION_OBJECT_TYPE_SHAPE
} rsge_animation_object_type_t;

typedef struct {
    /**
     * \brief The transformation to use.
     */
    vec3 transform;
    
    /**
     * \brief The rotation to use.
     */
    vec3 rotation;
} rsge_animation_keyframe_t;

typedef struct {
    /**
     * \brief Object type to use.
     */
    rsge_animation_object_type_t objectType;
    
    union {
        /**
         * \brief Surface object to use.
         */
        rsge_surface_t* surface;
        
        /**
         * \brief Shape object to use.
         */
        rsge_shape_t* shape;
    } object;
    
    /**
     * \brief Keyframes.
     */
    rsge_animation_keyframe_t* keyframes;
    
    /**
     * \brief The keyframe count.
     */
    int keyframeCount;
    
    /**
     * \brief The keyframe number.
     */
    int keyframeCurrent;
} rsge_animation_t;

/**
 * \fn rsge_error_e rsge_animation_createSurface(rsge_animation_t* animation,rsge_surface_t* surface,rsge_animation_keyframe_t* keyframes,int keyframeCount)
 * \brief Creates an animation for a surface.
 * \param animation The pointer to store the animation.
 * \param surface The surface to use.
 * \param keyframes The keyframes.
 * \param keyframeCount The number of keyframes.
 * \return An error code.
 */
rsge_error_e rsge_animation_createSurface(rsge_animation_t* animation,rsge_surface_t* surface,rsge_animation_keyframe_t* keyframes,int keyframeCount);

/**
 * \fn rsge_error_e rsge_animation_createShape(rsge_animation_t* animation,rsge_shape_t* shape,rsge_animation_keyframe_t* keyframes,int keyframeCount)
 * \brief Creates an animation for a shape.
 * \param animation The pointer to store the animation.
 * \param shape The shape to use.
 * \param keyframes The keyframes.
 * \param keyframeCount The number of keyframes.
 * \return An error code.
 */
rsge_error_e rsge_animation_createShape(rsge_animation_t* animation,rsge_shape_t* shape,rsge_animation_keyframe_t* keyframes,int keyframeCount);

#endif