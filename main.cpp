#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

using namespace std;
struct SpotData {
  float latitude;
  float longitude;
  float price_per_hour; // per hour
  string payment_type;
  string permit;
  int time_limit_minutes; // minutes
};

struct Filter {
  unordered_map<string, pair<double, double>> filters;
};

// Function to show information about Map objects (parking spaces)
void showMapData(const unordered_map<string, SpotData> &data, const Filter &filter) {
  int totalItems = 0;

  if (data.empty()) {
    cout << "No data to display" << '\n';
    return;
  }
  string latitude, longitude, spot_name, time_limit, permit, price, payment;
  bool display;
  for (const auto &it : data) {
    display = true; // reset display to true

    for (const auto &f : filter.filters) {
      const string &key = f.first;
      const auto &range = f.second;

      if (key == "price_per_hour") {
        if (it.second.price_per_hour < range.first ||
            it.second.price_per_hour > range.second) {
          display = false;
          break;
        }
      } else if (key == "time_limit_minutes") {
        if (it.second.time_limit_minutes < range.first ||
            it.second.time_limit_minutes > range.second) {
          display = false;
          break;
        }
      } else if (key == "payment_type") {
        // tikai pirmo izmanto payment_type
        if (it.second.payment_type != to_string(range.first)) {
          display = false;
          break;
        }
      } else if (key == "permit") {
        // tikai pirmo izmanto permit
        if (it.second.permit != to_string(range.first)) {
          display = false;
          break;
        }
      }
    }

    if (!display)
      continue;

    spot_name = "Name: " + it.first;
    latitude = "Latitude: " + to_string(it.second.latitude);
    longitude = "Longitude: " + to_string(it.second.longitude);

    if (it.second.permit == "") {
      permit = "Permit: Not necessary";
    } else {
      permit = "Permit: " + it.second.permit;
    }
    if (it.second.time_limit_minutes == 0) {
      time_limit = "Time limit: No time limit";
    } else {
      time_limit =
          "Time limit: " + to_string(it.second.time_limit_minutes) + " minutes";
    }
    if (it.second.price_per_hour == 0) {
      price = "Price: Free";
      cout << spot_name << '\n'
           << latitude << '\n'
           << longitude << '\n'
           << price << '\n'
           << permit << '\n'
           << time_limit << '\n'
           << '\n';
    } else {
      price = "Price: " + to_string(it.second.price_per_hour) + " Euros";
      payment = "Payment_type: " + it.second.payment_type;
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
}

// Test
int main() {
  // Example user data
  unordered_map<string, SpotData> spotData = {
      {"spot_1", {1.1412, 2.3123, 5.00, "Maksas veids 1", "", 0}},
      {"spot_2", {1.1413, 2.3124, 0, "", "", 120}},
      {"spot_3", {1.1415, 2.3125, 0, "", "Atļauja nr 1", 0}}};

  Filter filter{{{"price_per_hour", {3.0, 10000.0}}}};

  showMapData(spotData, filter);
  return 0;
}