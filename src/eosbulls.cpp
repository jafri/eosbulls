#include <eosbulls/eosbulls.hpp>

constexpr account_name bulls_token_contract = N(eosbullscoin);
constexpr account_name eos_token_contract = N(eosio.token);
constexpr account_name maria_contract = N(mariaeyeroll);

namespace eosio {
    static time_point current_time_point() {
        const static time_point ct{ microseconds{ static_cast<int64_t>( current_time() ) } };
        return ct;
    }

    void eosbulls::startround( const uint64_t& round_number,
                               const asset&    total_pot,      // Carried over from last round
                               const uint64_t& inflation_rate,
                               const uint64_t& last_price )    // Price to start at (usually $5)
    {
        rounds rounds_table(_self, _self);

        rounds_table.emplace(_self, [&]( auto r ) {
            r.round_number = round_number;
            r.total_pot = total_pot;

            r.total_referral_payout.amount = 0;
            r.total_referral_payout.symbol = S(4, EOS);

            r.total_keys_used = flat_map<symbol_type, uint64_t> ();
            r.total_keys_bought = flat_map<symbol_type, uint64_t> ();
            r.player_keys_used = flat_map<symbol_type, flat_map<name, uint64_t>> ();

            r.last_key_owner = maria_contract;
            r.last_key_team = S(0, BULLS);
            r.last_key_use_time = current_time_point();

            r.inflation_rate = inflation_rate;
            r.last_price = last_price;
        });
    }

    void eosbulls::ontransfer( const name from,
                               const name to,
                               const asset&       quantity,
                               const std::string& memo )
    {
        // eosio::print(from, " ", to, " ", quantity, " ", memo);
    }

    extern "C" {
        void apply(uint64_t receiver, uint64_t code, uint64_t action) {
            auto self = receiver;

            // If transfer from system contract eosio.token
            if (code == eos_token_contract && action == N(transfer)) {
                eosbulls contract(self);
                eosio::execute_action(&contract, &eosbulls::ontransfer);
            }

            // Start round
            if (code == maria_contract && action == N(startround)) {

                eosbulls contract(self);
                eosio::execute_action(&contract, &eosbulls::startround);
            }
        }
    } // ABI forwarder

} // eosio namespace