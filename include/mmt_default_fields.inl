#if !defined(FIELD)
#  error You need to define FIELD
#else
FIELD(market_mechanism, '1', 0)
FIELD(trading_mode, '2', 1)
FIELD(transaction_category, '-', 2)
FIELD(negotiation_indicator, '-', 3)
FIELD(agency_cross_trade_indicator, '-', 4)
FIELD(modification_indicator, '-', 5)
FIELD(benchmark_indicator, '-', 6)
FIELD(special_dividend_indicator, '-', 7)
FIELD(off_book_automated_indicator, '-', 8)
FIELD(ordinary_or_standart_trades, 'P', 9)
FIELD(algorithm_indicator, '-', 10)
FIELD(publication_mode, '-', 11)
FIELD(post_trade_deferral, '-', 12)
FIELD(duplicative_indicator, '-', 13)
FIELD(empty, '\0', 14)

#undef FIELD

#endif
