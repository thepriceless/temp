#define CURL_STATICLIB
#include<json.hpp>
#include<json_fwd.hpp>
#include<iostream>
#include<string>
#include<curl\curl.h>
#include<ctime>
#include<Windows.h>
#include<conio.h>
#include<map>
#include<set>


class Taker {
private:
    CURL* curl;
    CURLcode curl_result;
    std::string string_json_result;

    static size_t Convert_Into_String(void* content, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)content, size * nmemb);
        return size * nmemb;
    }

public:
    Taker () {
        curl = curl_easy_init();
    }

    void Set_URL() {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.cbr-xml-daily.ru/latest.js");
    }

    void Write_Received_Data() {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Convert_Into_String);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string_json_result);
        curl_result = curl_easy_perform(curl);
    }

    void Clean() {
        curl_easy_cleanup(curl);
    }

    friend std::string Get_String(const Taker& object) {
        return object.string_json_result;
    }
};


class JSON_Processor {
private:
    nlohmann::json web_data;

    std::map<std::string, std::vector<float>> currency_rates;

    bool Check_Currency(std::string currency) {
        return web_data["rates"].count(currency);
    }
    
    float Search(const std::vector<float>& data, int k) const {       
        if (data.size() == 1) { return data[0]; }
        float pivot = data[data.size() / 2];
        std::vector<float> below;
        std::vector<float> pivots;
        std::vector<float> above;
        for (int i = 0; i < data.size(); i++) {
            if (abs(data[i] - pivot) < 0.001) { pivots.push_back(data[i]); }
            else if (data[i] < pivot) { below.push_back(data[i]); }
            else { above.push_back(data[i]); }
        }

        if (k < below.size()) { return Search(below, k); }
        else if (k < below.size() + pivots.size()) { return pivots[0]; }
        else { return Search(above, k - below.size() - pivots.size()); }
    }

public:
    JSON_Processor() {
        currency_rates["USD"] = {};
        currency_rates["EUR"] = {};
    }

    float Median_Value(const std::vector<float>& data) {
        if (data.size() % 2 != 0) { return Search(data, data.size() / 2); }
        else { return (Search(data, data.size() / 2) + Search(data, data.size() / 2 - 1)) * 0.5; }
    }

    float Arithmetic_Mean(const std::vector<float>& data) const {
        float result = 0;
        for (int i = 0; i < data.size(); i++) {
            result += data[i];
        }
        return result / (data.size());
    }

    void Parse(Taker& object, bool first = false) {
        std::string temp = Get_String(object);
        web_data = nlohmann::json::parse(temp);
        if (!first) {
            for (std::map<std::string, std::vector<float>> ::iterator it = currency_rates.begin(); it != currency_rates.end(); it++) {
                currency_rates[it->first].push_back((float)1 / (float)web_data["rates"][it->first]);
            }
        }
    }

    bool Parse_List_Of_Currencies() {
        std::string temp_currency;
        std::map<std::string, std::vector<float>> ::iterator it;
        std::cin >> temp_currency;
        if (temp_currency != "/") {
            it = currency_rates.find(temp_currency);
            if (Check_Currency(temp_currency)) {
                if (it == currency_rates.end()) {
                    currency_rates[temp_currency] = {};
                    std::cout << "The currency is successfully added! Now you can add another one.\n";
                    return true;
                }
                else {
                    std::cout << "This currency is already added.\n";
                    return true;
                }
            }
            else {
                std::cout << "There is no such currency. You can still enter another currency abbreviation.\n";
                return true;
            }
        }
        else { return false; }
    }

    void Print_All_Currencies() {
        std::cout << "\nList of the added currencies:";
        for (std::map<std::string, std::vector<float>> ::iterator it = currency_rates.begin(); it != currency_rates.end(); it++) { std::cout << "\n " << it->first; }
        std::cout << "\n\n";
    }

    void Print_Simple_Rates() {
        std::cout << "\n\nAccording to the Russian Center Bank:";
        for (std::map<std::string, std::vector<float>> ::iterator it = currency_rates.begin(); it != currency_rates.end(); it++) {
            if (it->second.size() > 0) {
                std::cout << "\n1 " << it->first << " costs " << round(100 * it->second[it->second.size() - 1]) / 100 << " RUB.";
            }
        }
    }

    void Print_Medians_Means() {
        std::cout << "\n\n  Medians and arithmetically middle values:\n";
        for (std::map<std::string, std::vector<float>> ::iterator it = currency_rates.begin(); it != currency_rates.end(); it++) {
            if (it->second.size() > 0) {
                std::cout << "\nMedian value for the 1 " << it->first << " is " << round(100 * Median_Value(it->second)) / 100 << " RUB.";
                std::cout << "\nArithmetically middle value for 1 " << it->first << " is " << round(100 * Arithmetic_Mean(it->second)) / 100 << " RUB.\n";
            }
        }
    }
};

int main() {

    JSON_Processor currency_json;
    char key;
    
    std::cout << "The program (by default) will give you info about USD_RUB and EUR_RUB exchange rates.\nIf you need any other currencies, enter their 3-lettered abbreviation (separate each currency by pressing <ENTER> button).\nWhen you finish, enter '/' in the empty string and then press <ENTER> button.\n";

    Taker curl_object;
    curl_object.Set_URL();
    curl_object.Write_Received_Data();
    currency_json.Parse(curl_object, true);

    while (currency_json.Parse_List_Of_Currencies()) {}
    currency_json.Print_All_Currencies();
    curl_object.Clean();

    int gap_time;
    std::cout << "Enter the gap (in seconds) between two different requests to the Bank Server.\n";
    std::cin >> gap_time;
    gap_time *= 1000;
    std::cout << "\n";
    double current_time, gap_start_time = GetTickCount() - gap_time;
    
    while (true) {

        current_time = GetTickCount() - gap_start_time;

        if (current_time >= gap_time) {
            Taker curl_object;
            curl_object.Set_URL();
            curl_object.Write_Received_Data();
            currency_json.Parse(curl_object);
            currency_json.Print_Simple_Rates();
            curl_object.Clean();

            gap_start_time = GetTickCount();
        }

        key = ' ';
        if (_kbhit()) { key = _getch(); }            
        if (key == 13) { break; }
        else if (key != ' ') { std::cout << "You pressed: " << key << '\n'; }
    }
    currency_json.Print_Medians_Means();
}
