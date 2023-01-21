/**
 * @file Analyzer
 * @author Kofi Boateng
 * @date 2023-01-11
 *
 */

#pragma once

#include <iostream>
#include <stdexcept>
#include <ostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <core/models/headers/pricehistory.h>
using namespace std;

enum class MovingAverageType
{
    SIMPLE,
    EXPONENTIAL
};

class Analyzer
{
private:
    /*
        The arbitrage threshold you want the analyzer to act on when
        calculating for arbitrage opportunities.
    */
    double arbitrage_threshold;
    /*
        The correlation threshold you want the analyzer to use
        when making correlation calculations.
    */
    double correlation_threshold;

    /*
        Helper function that is called when the user wants to generate
        an exponential moving average. Weights will be generated based
        on the window argument, and will range from 0 to 1 with earlier
        prices taking smaller weights and earlier prices having stronger
        weights.
    */
    vector<double> generate_ema(const PriceHistory &stock, const int &window) const;
    /*
        Helper function that is called when the user wants to generate
        a simple moving average. The weight of each average will be equal
        across all calculations.
    */
    vector<double> generate_sma(const PriceHistory &stock, const int &window) const;

public:
    Analyzer();
    Analyzer(const double &arbitrage_thresh);
    Analyzer(const double &arbitrage_thresh, const double &correlation_threshold);
    Analyzer(const Analyzer &obj);
    Analyzer &operator=(const Analyzer &obj);
    ~Analyzer();

    //================================ GETTERS & SETTERS ====================================//

    double get_arbitrage_threshold() const;

    void set_arbitrage_threshold(const double &arb_thresh);

    double get_correlation_threshold() const;

    void set_correlation_threshold(const double &corr_thresh);

    //=============================== OTHER METHODS =========================================//

    bool is_arbitrage_present(tuple<PriceHistory> &stock_tuple);

    /*
        Takes in a pair of stocks and returns a correlation value between the two stocks.
    */
    double generate_correlation_value(PriceHistory &stock1, PriceHistory &stock2);

    /*
        Takes in a vector of pairs and will return an unordered map of the pairs as the
        key and the mapped value will be the correlation between the pair of stocks
    */
    unordered_map<pair<PriceHistory, PriceHistory>, double> generate_correlation_map(const vector<pair<PriceHistory, PriceHistory>> &stock_pairs);

    /*
        Takes in a tuple of pairs and will return an unordered map with a stock as
        the key and the mapped value will mapping with a stock and its correlation
        to the the stock we are currently iterating over in the tuple.

        The map will return a step mapping. The first map will will have all the
        stocks and their correlation to the first stock. As we move down the vector,
        we will add in n-1 stocks. The mapping will resemble something similar to this

        mapping = [
            s1: {s2:double, s3:double, s4:double},
            s2: {s3:double, s4:double},
            s3: {s4:double}
        ]
    */
    unordered_map<PriceHistory, unordered_map<PriceHistory, double>> generate_correlation_map(const vector<PriceHistory> &stock_vec);

    /*
        Takes in a stock and a window, and will return the simple moving average of the
        stock based on the window designated.

        The Simple Moving Average, is an averaging of a stock price that smooths out the
        price changes and shows a average trend of the stock. Each price is weighted
        equally across the Time Series.
    */
    vector<double> generate_moving_average(const PriceHistory &stock, const int &window, const MovingAverageType type) const;

    /*
        Returns an unorder map with the key being a stock and the value
        being a ordered map with the key being a window and the values
        being a vector of either a simple moving average or exponential
        moving average.
    */
    unordered_map<PriceHistory, map<int, vector<double>>> generate_moving_averages_map(const PriceHistory &stock, const vector<int> &windows, const MovingAverageType type) const;

    unordered_map<PriceHistory, map<int, vector<double>>> generate_moving_averages_map(const vector<PriceHistory> &stock_vec, const int &window, const MovingAverageType type) const;

    unordered_map<PriceHistory, map<int, vector<double>>> generate_moving_averages_map(const vector<PriceHistory> &stock_vec, const vector<int> &windows, const MovingAverageType type) const;

    /*
        Takes in a stock and a window used to calculate a exponential moving average, and
        will return a string stating whether the trend is "up" or if the trend is "down".

        An EMA (exponential moving average) is an average, similar to a simple moving average,
        that will show the trend of a stock over a certain period of time. The EMA however will
        add more weight to recent prices in the stock as to reflect its current market situation.
        A small EMA window will be very sensitive to price changes, whereas a wide window will
        not be as sensitive but will still reflect trend sentiment.
    */
    string generate_stock_momentum(const PriceHistory &stock, const int &window) const;

    unordered_map<PriceHistory, string> generate_stocks_momentum_map(const vector<PriceHistory> &stock_vec, const int &windows) const;
};