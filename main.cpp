#include <iostream>
#include <string>
#include <unordered_map>

// fix to cout is ambiguous
// https://stackoverflow.com/questions/1452721/why-is-stdcout-ambiguous-when-trying-to-print-a-string-in-c

using namespace std;

struct SpotData
{
  double latitude;
  double longitude;
  double price_per_hour; // per hour
  string payment_type;
  string permit;
  int time_limit_minutes; // minutes
};

struct Filter
{
  unordered_map<string, pair<double, double>> numericFilters;
  unordered_map<string, string> stringFilters;
};

// Function to show information about Map objects (parking spaces)
void showMapData(const unordered_map<string, SpotData> &data,
                 const Filter &filter)
{
  int totalItems = 0;

  if (data.empty())
  {
    std::cout << "No data to display" << '\n';
    return;
  }
  string latitude, longitude, spot_name, time_limit, permit, price, payment;
  bool display;
  for (const auto &parking_space : data)
  {
    display = true; // reset display to true

    for (const auto &f : filter.numericFilters)
    {
      const string &key = f.first;
      const auto &range = f.second;

      if (key == "price_per_hour" && (parking_space.second.price_per_hour < range.first || parking_space.second.price_per_hour > range.second) && parking_space.second.price_per_hour != 0)
      {
        display = false;
        break;
      }
      // ja filtra laiks ir lielaaks par minimalo prasito
      else if (key == "time_limit_minutes" && (parking_space.second.time_limit_minutes < range.first) && parking_space.second.time_limit_minutes != 0)
      {
        cout << "TIME LIMIT MINUTES " << parking_space.second.time_limit_minutes << " < " << range.first << '\n';
        display = false;
        break;
      }
    }

    for (const auto &f : filter.stringFilters)
    {
      const string &key = f.first;
      const string &value = f.second;

      if (key == "payment_type" && parking_space.second.payment_type != value)
      {
        display = false;
        break;
      }
      else if (key == "permit" && parking_space.second.permit != value)
      {
        display = false;
        break;
      }
    }

    if (!display)
      continue;

    spot_name = "Name: " + parking_space.first;
    latitude = "Latitude: " + to_string(parking_space.second.latitude);
    longitude = "Longitude: " + to_string(parking_space.second.longitude);

    if (parking_space.second.permit == "")
    {
      permit = "Permit: Not necessary";
    }
    else
    {
      permit = "Permit: " + parking_space.second.permit;
    }
    if (parking_space.second.time_limit_minutes == 0)
    {
      time_limit = "Time limit: No time limit";
    }
    else
    {
      time_limit =
          "Time limit: " + to_string(parking_space.second.time_limit_minutes) + " minutes";
    }
    if (parking_space.second.price_per_hour == 0)
    {
      price = "Price: Free";
    }
    else
    {
      price = "Price: " + to_string(parking_space.second.price_per_hour) + " Euros";
      payment = "Payment_type: " + parking_space.second.payment_type;
    }
    cout << spot_name << '\n'
         << latitude << '\n'
         << longitude << '\n'
         << price << '\n'
         << payment << '\n'
         << permit << '\n'
         << time_limit << '\n'
         << '\n';
  }
}

// Test
int main()
{
  // Example user data
  unordered_map<string, SpotData> spotData = {
      {"spot_1", {1.1412, 2.3123, 5.00, "Maksas veids 1", "", 0}},
      {"spot_2", {1.1413, 2.3124, 0, "", "", 120}},
      {"spot_3", {1.1415, 2.3125, 0, "", "Atļauja nr 1", 0}},
      {"spot_4", {1.1416, 2.3126, 3.50, "Maksas veids 2", "", 0}},
      {"spot_5", {1.1417, 2.3127, 0, "", "", 150}},
      {"spot_6", {1.1418, 2.3128, 0, "", "Atļauja nr 2", 0}},
      {"spot_7", {1.1419, 2.3129, 4.00, "Maksas veids 3", "", 0}},
      {"spot_8", {1.1420, 2.3130, 0, "", "", 180}},
      {"spot_9", {1.1421, 2.3131, 0, "", "Atļauja nr 3", 0}},
      {"spot_10", {1.1422, 2.3132, 4.50, "Maksas veids 4", "", 0}}};

  // static filter example
  // Filter filter{{{"price_per_hour", {3.0, 10000.0}}}};
  // showMapData(spotData, filter);

  // black box user input based filter
  while (true)
  {
    Filter dynamicFilter;
    int filterType;
    do {
      cout << "Ievadi filtra tipu (1-4, 5 beigtu filtru ievadi): ";
      cin >> filterType;
      if (filterType < 1 || filterType > 5) 
        cout << "Nederīgs filtra veids.\n";
      else if (filterType == 1 || filterType == 2)
      {
        if (filterType == 1)
        {
          cout << "Ievadi min un max vērtības cenai stundā: ";
          double min, max;
          cin >> min >> max;
          if (min > max || min < 0 || max < 0)
          {
            cout << "min <= max un nevar būt negatīvi.\n";
            continue;
          }
          dynamicFilter.numericFilters["price_per_hour"] = {min, max};
        }
        else if (filterType == 2)
        {
          double min, max;
          cout << "Ievadi minimalo stavēšanas laika vērtību minūtēs: ";
          cin >> min;
          if (min < 0)
          {
            cout << "Nevar būt negatīvs.\n";
            continue;
          }
          max = 10000;
          dynamicFilter.numericFilters["time_limit_minutes"] = {min, max};
        }
      }
      else if (filterType == 3)
      {
        cout << "Ievadi maksājuma tipa vērtību: ";
        string value;
        cin >> ws;
        getline(cin, value);
        dynamicFilter.stringFilters["payment_type"] = value;
      } 
      else if (filterType == 4)
      {
        cout << "Ievadi atļaujas vērtību: ";
        string value;
        cin >> ws;
        getline(cin, value);
        dynamicFilter.stringFilters["permit"] = value;
      }
    } while (filterType <= 4);
    showMapData(spotData, dynamicFilter);
  }
  return 0;
}