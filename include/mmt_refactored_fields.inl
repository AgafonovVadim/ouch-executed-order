#if !defined(FIELD)
#  error You need to define FIELD
#else
FIELD(trading_mode, 1)
FIELD(transaction_category, 2)
FIELD(algorithm_indicator, 10)

#undef FIELD

#endif
