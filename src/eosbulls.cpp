#include <eosbulls/eosbulls.hpp>

namespace eosio {
    void eosbulls::ontransfer( const account_name from,
                           const account_name to,
                           const asset& quantity,
                           const std::string& memo )
    {
        eosio::print(from, " ", to, " ", quantity, " ", memo);
    }

    extern "C" {
        void apply(uint64_t receiver, uint64_t code, uint64_t action) {
            auto self = receiver;

            // If transfer from system contract eosio.token
            if (code == N(eosio.token) && action == N(transfer)) {
                eosbulls contract(self);
                eosio::execute_action(&contract, &eosbulls::ontransfer);
            }
        }
    } // ABI forwarder

} // eosio namespace