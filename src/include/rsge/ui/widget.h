#ifndef __RSGE_UI_WIDGET_H_
#define __RSGE_UI_WIDGET_H_ 1

/**
 * \file src/include/rsge/ui/widget.h
 * \brief UI widget
 * \author Spaceboy Ross
 */

#include <rsge/error.h>

struct rsge_widget;

/**
 * \brief The prototype for destroying the widget.
 */
typedef void (*rsge_widget_impl_destroy)(struct rsge_widget* widget);

/**
 * \struct rsge_widget_t src/include/rsge/ui/widget.h rsge/ui/widget.h
 * \brief UI widget
 */
typedef struct rsge_widget {
    /**
     * \brief The name of the widget.
     */
    char* name;
    /**
     * \brief Implementation data.
     */
    void* impl;
    
    /**
     * \brief Widget destroy callback.
     */
    rsge_widget_impl_destroy destroy;
} rsge_widget_t;

#endif