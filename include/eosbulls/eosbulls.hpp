#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <string>

namespace eosio {
    using std::string;

    class eosbulls: public contract {
        public:
            eosbulls( account_name self ):contract(self){}

            void ontransfer( const account_name from,
                             const account_name to,
                             const asset& quantity,
                             const std::string& memo );

    };
}

