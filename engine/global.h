#pragma once
#include "core/core.h"
#include "loader/loadertool.h"
#include "core/log.h"
#include "v8/v8engine.h"
#include  <memory>

namespace cg
{
    class global {
        public:
            static core core;
            static loadertool loadertool;
            static log log;
            static v8engine v8engine;
    };
}