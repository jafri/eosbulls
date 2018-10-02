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

    struct transfer
    {
        account_name from;
        account_name to;
        asset        quantity;
        string       memo;
    };

    class eosbulls: public contract {
        public:
            eosbulls( account_name self ):contract(self){}

            void ontransfer( const name     from,
                             const name     to,
                             const asset    quantity,
                             const string   memo );

            void startround( const asset    total_pot,        // Carried over from last round
                             const uint64_t inflation_rate,
                             const uint64_t start_price,      // Price to start at (usually $5)
                             const uint64_t low_start,        // $1
                             const uint64_t low_end,          // $0
                             const uint64_t high_start,       // $9
                             const uint64_t high_end   );     // $10

            void   endround( const uint64_t round_number ); // End round
            void     payout( const uint64_t round_number ); // Pay out Winner + Pot (after round ends)
            void clearround( const uint64_t round_number ); // Free up RAM (after round ends)

            void rmvallrounds(); // Free up all RAM

    private:
            struct round {
                uint64_t       round_number;
                asset          total_pot;
                asset          total_referral_payout = asset(0, S(4, EOS));

                // Key data about users (both BULLS and BEARS)
                flat_map<symbol_type, uint64_t>                  total_keys_used;
                flat_map<symbol_type, uint64_t>                  total_keys_bought;
                flat_map<symbol_type, flat_map<name, uint64_t>>  player_keys_used;

                // Data for deciding winner once game is over
                account_name   last_key_owner;
                symbol_type    last_key_team;
                time_point     last_key_use_time;

                // Price to start at and the inflation rate
                uint64_t       current_price;
                uint64_t       inflation_rate;

                // Price for danger zones
                uint64_t       low_start;
                uint64_t       low_end;
                uint64_t       high_start;
                uint64_t       high_end;

                bool           finished = false;

                uint64_t primary_key()const { return round_number; }
            };

            typedef eosio::multi_index<N(rounds), round> rounds;
    };
}


#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      if( action == N(transfer)){ \
         eosio_assert( code == N(eosio.token), "Must transfer EOS"); \
      } \
      if( code == self || action == N(onerror) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
} \

EOSIO_ABI_EX(eosio::eosbulls, (startround) (rmvallrounds) (ontransfer));