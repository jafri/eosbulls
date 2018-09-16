#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/multi_index.hpp>

#include <string>

namespace eosio {
    using std::string;
    using boost::container::flat_map;
    using eosio::time_point;

    class eosbulls: public contract {
        public:
            eosbulls( account_name self ):contract(self){}

            void ontransfer( const name     from,
                             const name     to,
                             const asset&   quantity,
                             const string&  memo );

            void startround( const uint64_t& round_number,
                             const asset&    total_pot,      // Carried over from last round
                             const uint64_t& inflation_rate,
                             const uint64_t& last_price );   // Price to start at (usually $5)

            void   endround( const uint64_t round_number ); // End round
            void     payout( const uint64_t round_number ); // Pay out Winner + Pot (after round ends)
            void clearround( const uint64_t round_number ); // Free up RAM (after round ends)

    private:
            struct round {
                uint64_t       round_number;
                asset          total_pot;
                asset          total_referral_payout;

                flat_map<symbol_type, uint64_t>                  total_keys_used;
                flat_map<symbol_type, uint64_t>                  total_keys_bought;
                flat_map<symbol_type, flat_map<name, uint64_t>>  player_keys_used;

                account_name   last_key_owner;
                symbol_type    last_key_team;
                time_point     last_key_use_time;

                uint64_t       inflation_rate;
                uint64_t       last_price;

                uint64_t primary_key()const { return round_number; }
            };

            typedef eosio::multi_index<N(rounds), round> rounds;
    };
}

