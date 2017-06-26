#ifndef CLIENTVERSION_H
#define CLIENTVERSION_H

#if defined(HAVE_CONFIG_H)
#include "config/ion-config.h"
#endif

//
// client versioning
//

#if !defined(CLIENT_VERSION_MAJOR) || !defined(CLIENT_VERSION_MINOR) || !defined(CLIENT_VERSION_REVISION) || !defined(CLIENT_VERSION_BUILD) || !defined(CLIENT_VERSION_IS_RELEASE) || !defined(COPYRIGHT_YEAR)
// These need to be macros, as version.cpp's and ion-qt.rc's voodoo requires it
#define CLIENT_VERSION_MAJOR       2
#define CLIENT_VERSION_MINOR       1
#define CLIENT_VERSION_REVISION    6
#define CLIENT_VERSION_BUILD       0
#endif

#if !defined(CLIENT_VERSION_IS_RELEASE)
// Set to true for release, false for prerelease or test build
#define CLIENT_VERSION_IS_RELEASE  true
#endif

// Converts the parameter X to a string after macro replacement on X has been performed.
// Don't merge these into one macro!
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

#endif // CLIENTVERSION_H
