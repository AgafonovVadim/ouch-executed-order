#include "requests.h"

void decode_bit_mask(ExecutionDetails & exec_details, const std::uint8_t mask)
{
    exec_details.self_trade = (mask & (1u << 7)) != 0;
    exec_details.internalized = (mask & (1u << 5)) != 0;
    exec_details.liquidity_indicator = (mask & (1u << 4)) ? LiquidityIndicator::None : (mask & (1u << 3)) ? LiquidityIndicator::Removed
                                                                                                          : LiquidityIndicator::Added;
}

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    size_t current_index = 9;
    exec_details.cl_ord_id = string_parser(message, current_index, 14);
    exec_details.filled_volume = integer_parser(message, current_index);
    exec_details.price = static_cast<double>(integer_parser(message, current_index)) / 10000;
    exec_details.match_number = integer_parser(message, ++current_index);
    exec_details.counterpart = string_parser(message, current_index, 4);
    decode_mmt(message, exec_details.mmt, current_index);
    decode_bit_mask(exec_details, static_cast<std::uint8_t>(message[43]));
    return exec_details;
}

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message)
{
    BrokenTradeDetails break_details;
    size_t current_index = 9;
    break_details.cl_ord_id = string_parser(message, current_index, 14);
    break_details.match_number = integer_parser(message, current_index);
    switch (message[current_index++]) {
    case 'E':
        break_details.reason = BreakReason::Erroneous;
        break;
    case 'C':
        break_details.reason = BreakReason::Consent;
        break;
    case 'S':
        break_details.reason = BreakReason::Supervisory;
        break;
    case 'X':
        break_details.reason = BreakReason::External;
        break;
    default:
        break_details.reason = BreakReason::Unknown;
        break;
    }
    decode_mmt(message, break_details.mmt, current_index);
    return break_details;
}
