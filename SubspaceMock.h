#ifndef _SUBSPACE_MOCK_H_
#define _SUBSPACE_MOCK_H_

#include "DataTypes.h"

// Mock SubspaceGlobal for basic testing
class SubspaceGlobal 
{
private:
    Uint windowWidth_;
    Uint windowHeight_;

public:
    SubspaceGlobal() : windowWidth_(800), windowHeight_(600) {}
    
    void setWindowWidth(Uint width) { windowWidth_ = width; }
    void setWindowHeight(Uint height) { windowHeight_ = height; }
    Uint getWindowWidth() const { return windowWidth_; }
    Uint getWindowHeight() const { return windowHeight_; }
};

// Global instance
extern SubspaceGlobal subspaceGlobal;

#endif
