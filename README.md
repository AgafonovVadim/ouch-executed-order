# OUCH Decoders

## New Protocol!
In the lecture example, we learned about the BOE protocol, mainly used in trading platforms owned by the CBOE exchange.  
In this task, you are introduced to another protocol – OUCH, which is more widely used (in various forms). However, we will focus on the version used on Nasdaq Nordic (INET) platforms, which encompass Scandinavian exchanges.

A significant difference: numbers are encoded in big-endian format.

## MMT Flags
There is a standard for so-called MMT flags – certain parameters of a completed trade used by regulators and other organizations for classifying trades. In the current version of the standard (3.04), there are 14 flags, each of which takes a single value from a finite set.  
For example, for a given trade, the MMT flags may look like this: `45------QP-2--`.

Each value is an ASCII character, typically a digit, letter, or hyphen. Most flags have a neutral value of `-`.

Flags are identified by numbers:
* 1 - Market Mechanism (default value: `1`)
* 2 - Trading Mode (default value: `2`)
* 3.1 - Transaction Category (default value: `-`)
* 3.2 - Negotiation Indicator (default value: `-`)
* 3.3 - Agency Cross Trade Indicator (default value: `-`)
* 3.4 - Modification Indicator (default value: `-`)
* 3.5 - Benchmark Indicator (default value: `-`)
* 3.6 - Special Dividend Indicator (default value: `-`)
* 3.7 - Off-Book Automated Indicator (default value: `-`)
* 3.8 - Ordinary/Standard Trades (default value: `P`)
* 3.9 - Algorithm Indicator (default value: `-`)
* 4.1 - Publication Mode (default value: `-`)
* 4.2 - Post-Trade Deferral (default value: `-`)
* 5 - Duplicative Indicator (default value: `-`)

In this task, we are interested in only a few flags:
* Trading Mode (2)
* Transaction Category (3.1)
* Algorithm Indicator (3.9)

All others should be set to their default values.

The MMT flags are to be encoded in a `char` array, where the last element should be a null value (`'\0'`).

## ExecutedOrder Message Decoding
### Idea
When an order is executed on an exchange, meaning a matching order is found and the exchange records the trade, the participant who sent the order receives a notification of the completed trade – an order execution.  
An order can be fully or partially executed, either at the specified price or a better one.

### Task
You need to implement a decoder for the ExecutedOrder message (see [protocol specifications](doc/OUCH_for_Nasdaq_Nordic_4.03.2.pdf)) with the following interface:
```cpp
enum class LiquidityIndicator
{
    Added,
    Removed
};

struct ExecutionDetails
{
    std::string cl_ord_id;
    unsigned match_number;
    unsigned filled_volume;
    double price;
    std::string counterpart;
    char mmt[15];
    LiquidityIndicator liquidity_indicator;
    bool internalized;
    bool self_trade;
};

ExecutionDetails decode_executed_order(const std::vector<unsigned char> & message)
{
    ExecutionDetails exec_details;
    // fill exec_details fields
    //   exec_details.cl_ord_id.assign(char_ptr, length);
    // or
    //   exec_details.cl_ord_id = std::string{char_ptr, length};
    // ...
    //   exec_details.filled_volume = x;
    // ...
    return exec_details;
}
```

To get a pointer to `char`, which is required to assign strings from a vector of `unsigned char`, you can use:
```cpp
const char * str = reinterpret_cast<const char *>(&message[n]);
```
where `n` is the index of the first character of the string within the message.

Required fields:
* Order Token -> cl_ord_id
* Executed Quantity -> filled_volume
* Execution Price -> price
* Match Number -> match_number
* Contra Firm -> counterpart
* Liquidity Attributes -> liquidity_indicator, internalized, self_trade
* Trading Mode, Transaction Category, Algorithm Indicator -> mmt

## BrokenTrade Message Decoding
### Idea
A completed trade can be canceled under certain circumstances (although this is discouraged). For example, both parties can inform the exchange that the trade was made in error. In that case, each party receives a BrokenTrade message.

### Task
You need to implement a decoder for the BrokenTrade message (see [protocol specifications](doc/OUCH_for_Nasdaq_Nordic_4.03.2.pdf)) with the following interface:
```cpp
enum class BreakReason
{
    Erroneous,
    Consent,
    Supervisory,
    External,
    Unknown
};

struct BrokenTradeDetails
{
    std::string cl_ord_id;
    unsigned match_number;
    char mmt[15];
    BreakReason reason;
};

BrokenTradeDetails decode_broken_trade(const std::vector<unsigned char> & message)
{
    // ...
}
```

To assign a value to an array element, you can do so as follows (element indices start at 0):
```cpp
break_details.mmt[1] = 'O';
```

Required fields:
* Order Token -> cl_ord_id
* Match Number -> match_number
* Reason -> reason
* Trading Mode, Transaction Category, Algorithm Indicator -> mmt
