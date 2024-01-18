//
// (c) 2022 Eduardo Doria.
//

#ifndef UNIQUETOKEN_H
#define UNIQUETOKEN_H

#include <string>

namespace Supernova {

    class UniqueToken {
    private:
        static int id;

        static std::string randString(const int len);

    public:
        static std::string get();

    };

}

#endif //UNIQUETOKEN_H
