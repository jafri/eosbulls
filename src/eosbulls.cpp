#include <eosbulls/eosbulls.hpp>

constexpr account_name bulls_token_contract = N(eosbullscoin);
constexpr account_name eos_token_contract = N(eosio.token);
constexpr account_name maria_contract = N(mariaeyeroll);

namespace eosio {
    static time_point current_time_point() {
        const static time_point ct{ microseconds{ static_cast<int64_t>( current_time() ) } };
        return ct;
    }

    void eosbulls::startround( const asset    total_pot,        // Carried over from last round
                               const uint64_t inflation_rate,
                               const uint64_t start_price,      // Price to start at (usually $5)
                               const uint64_t low_start,        // $1
                               const uint64_t low_end,          // $0
                               const uint64_t high_start,       // $9
                               const uint64_t high_end   )      // $10
    {
        require_auth(_self);

        // Check data
        eosio_assert(inflation_rate > 0, "Inflation rate must be positive");
        eosio_assert(start_price > 0, "Start price must be positive");
        eosio_assert(low_start > 0, "Low start must be positive");
        eosio_assert(low_end >= 0, "Low end must be 0 or positive");
        eosio_assert(high_start > 0, "High start must be positive");
        eosio_assert(high_end > 0, "High end must be positive");

        // Create new round
        rounds rounds_table(_self, _self);

        rounds_table.emplace(_self, [&]( auto &r ) {
            r.round_number = rounds_table.available_primary_key();
            r.total_pot = total_pot;

            r.inflation_rate = inflation_rate;
            r.current_price = start_price;

            r.low_start = low_start;
            r.low_end = low_end;
            r.high_start = high_start;
            r.high_end = high_end;
        });
    }

    void eosbulls::rmvallrounds()
    {
        require_auth(_self);

        rounds rounds_table(_self, _self);

        while (rounds_table.begin() != rounds_table.end()) {
            auto itr = rounds_table.begin();
            rounds_table.erase(itr);
        }
    }

    void eosbulls::ontransfer( const name         from,
                               const name         to,
                               const asset        quantity,
                               const std::string  memo )
    {
        action{
                permission_level{_self, N(active)},
                N(eosio.token),
                N(transfer),
                transfer {
                    .from     = _self,
                    .to       = from,
                    .quantity = quantity,
                    .memo     = memo
                }
        }.send();
    }

} // eosio namespace