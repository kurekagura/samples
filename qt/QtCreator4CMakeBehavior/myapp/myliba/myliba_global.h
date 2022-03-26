#ifndef MYLIBA_GLOBAL_H
#define MYLIBA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MYLIBA_LIBRARY)
#  define MYLIBA_EXPORT Q_DECL_EXPORT
#else
#  define MYLIBA_EXPORT Q_DECL_IMPORT
#endif

#endif // MYLIBA_GLOBAL_H
