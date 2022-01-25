// Декомпозиция и отладка кода "Автобусные остановки"

/* Задание
Перед вами шаблон с заготовками классов и функций. Не меняя его функцию main, 
реализуйте эти классы и функции так, чтобы получившаяся программа решала 
задачу «‎Автобусные остановки».*/

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

//=======================================================================
// Оператор чтения структуры Query
istream& operator>>(istream& input, Query& q) 
{
    
    // Считываем статус запроса
    string type;
    input >> type;

    if(type == "NEW_BUS"s)
    {
        q.type = QueryType::NewBus;
        input >> q.bus;
        
        int stop_count;
        input >> stop_count;

        q.stops.resize(stop_count);

        for (auto& stop : q.stops)
            input >> stop;
    }

    else if (type == "BUSES_FOR_STOP"s)
    {
        q.type = QueryType::BusesForStop;
        input >> q.stop;
    }

    else if (type == "STOPS_FOR_BUS"s)
    {
        q.type = QueryType::StopsForBus;
        input >> q.bus;
    }

    else
    {
        q.type = QueryType::AllBuses;
    }

    return input;
}

struct BusesForStopResponse 
{
    vector<string> stops_to_buses;
};

ostream& operator<<(ostream& output, const BusesForStopResponse& r) 
{
    for (const auto& stops_to_buses : r.stops_to_buses)
    {
        output << stops_to_buses;
    }
    
    return output;
}

struct StopsForBusResponse 
{
    vector<string> stops_to_buses;
};

ostream& operator<<(ostream& output, const StopsForBusResponse& r) 
{
    size_t cnt = 0;
    for (const auto& stops_to_buses : r.stops_to_buses)
    {
        cnt++;

        if(cnt == r.stops_to_buses.size())
            output << stops_to_buses;
        else
            output << stops_to_buses << endl;
    }

    return output;
}

struct AllBusesResponse 
{
    // Наполните полями эту структуру
    vector<string> all_buses;
};

ostream& operator<<(ostream& output, const AllBusesResponse& r) 
{
    size_t cnt = 0;
    for (const auto& buses : r.all_buses)
    {
        cnt++;

        if(cnt == r.all_buses.size())
            output << buses;
        else
            output << buses << endl;
    }

    return output;
}

class BusManager {
public:
    // NEW_BUS добавить маршрут автобуса
    void AddBus(const string& bus, const vector<string>& stops) 
    {
        buses_to_stops[bus] = stops;
        
        for (string stop : stops) 
        {
            stops_to_buses[stop].push_back(bus);
        }
    }

  
    // BUSES_FOR_STOP вывести названия всех маршрутов автобуса, проходящих через остановку stop
    BusesForStopResponse GetBusesForStop(const string& stop) const 
    {
        BusesForStopResponse buses_for_stop_response;

        if (stops_to_buses.count(stop) == 0) 
        {
            buses_for_stop_response.stops_to_buses.push_back("No stop"s);
        }
        else 
        {
            string buses = ""s;

            for (const string bus : stops_to_buses.at(stop)) 
            {
                buses += bus + " "s;
            }

            buses_for_stop_response.stops_to_buses.push_back(buses);
        }

        return buses_for_stop_response;
    }
    // STOPS_FOR_BUS вывести названия всех остановок маршрута bus со списком автобусов, куда можно пересесть на каждой из остановок.
    StopsForBusResponse GetStopsForBus(const string& bus) const 
    {
        StopsForBusResponse stops_for_bus_response;

        if (buses_to_stops.count(bus) == 0) 
        {
            stops_for_bus_response.stops_to_buses.push_back("No bus"s);
        }
        else 
        {
            for (const string& stop : buses_to_stops.at(bus)) 
            {
                if (stops_to_buses.at(stop).size() == 1) 
                {
                    stops_for_bus_response.stops_to_buses.push_back("Stop "s + stop + ": "s + "no interchange"s );
                }
                else 
                {
                    string buses = ""s;

                    for (const string& other_bus : stops_to_buses.at(stop)) 
                    {
                        if (bus != other_bus) 
                        {
                            buses += other_bus + " "s;
                        }
                    }

                    stops_for_bus_response.stops_to_buses.push_back("Stop "s + stop + ": "s + buses);
                }
            }
        }

        return stops_for_bus_response;
    }

    // ALL_BUSES — вывести список всех маршрутов с остановками.
    AllBusesResponse GetAllBuses() const 
    {
        AllBusesResponse all_buses_response;

        if (buses_to_stops.empty()) 
        {
            all_buses_response.all_buses.push_back("No buses"s);
        }
        else 
        {
            for (const auto& bus_item : buses_to_stops) 
            {
                string stops = ""s;
                
                for (const string& stop : bus_item.second) 
                {
                    stops += stop + " "s;
                }
                
                all_buses_response.all_buses.push_back("Bus "s + bus_item.first + ": "s + stops);
            }
        }

        return all_buses_response;
    }
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};
//=======================================================================

/*
NEW_BUS 32 1 Tolstopaltsevo 
NEW_BUS 32K 2 Tolstopaltsevo Skolkovo

STOPS_FOR_BUS 32K
Stop Tolstopaltsevo : 32
Stop Skolkovo : no interchange
*/

// UNIT - тесты
void TestPointInput()
{
    istringstream input;
    Query q1, q2, q3, q4;

    input.str("NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo"s);
    input >> q1;
    assert(q1.type == QueryType::NewBus);
    assert(q1.bus == "32"s);
    assert(q1.stops.size() == 3);
    assert(q1.stop == ""s);
    input.clear();

    input.str("BUSES_FOR_STOP Marushkino"s);
    input >> q2;
    assert(q2.type == QueryType::BusesForStop);
    assert(q2.bus == ""s);
    assert(q2.stops.empty());
    assert(q2.stop == "Marushkino");
    input.clear();

    input.str("STOPS_FOR_BUS 32K"s);
    input >> q3;
    assert(q3.type == QueryType::StopsForBus);
    assert(q3.bus == "32K"s);
    assert(q3.stops.empty());
    assert(q3.stop == ""s);
    input.clear();

    input.str("ALL_BUSES"s);
    input >> q4;
    assert(q4.type == QueryType::AllBuses);
    assert(q4.bus == ""s);
    assert(q4.stops.empty());
    assert(q4.stop == "");
    input.clear();
}

void RunUnitTest()
{
    TestPointInput();
}
//=======================================================================

// Не меняя тела функции main, реализуйте функции и классы выше

int lesson_6() {

    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }

    return 0;
}