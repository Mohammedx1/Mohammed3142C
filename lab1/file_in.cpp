// This file shows basic functions of reading in a CSV file using C++98
//Mohammed Alam

/* Homework Objective: fix this program to match the requirements listed:
  1. successfully reads all lines of the input file
  2. calculate the average price per brand, average price per category
  3. writes to an output file
  4. for each unique year, list the count of skus and also print out the skus as a list i.e. 2000 (3): 111, 211, 311 and make a new line per year.
All programs must be able to compile in C++98 standard (the default version on Linux Machine). That means you cannot use the new features of C++11! :(


*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

vector<vector<string> > read_data() { 
vector<vector<string> > data(5);
  string sku, brand, category, year, price;

  ifstream in_stream;
  in_stream.open("data.csv"); //opening the file.

  if (!in_stream.fail()) { //if the file is open

    string lines;
    getline(in_stream, lines); // skip the header line

    while (getline(in_stream, lines)) { //while the end of file is NOT reached

      // Instead of parsing the file directly as in the starter code, we parse each line separately:
      stringstream line (lines);

     
      getline(line, sku, ',');
      data[0].push_back(sku);

      getline(line, brand, ',');
      data[1].push_back(brand);

      getline(line, category, ',');
      data[2].push_back(category);

      getline(line, year, ',');
      data[3].push_back(year);

      getline(line, price, '\n');
      data[4].push_back(price);
    }

    in_stream.close(); //closing the file

   } else {
    cout << "Unable to open file";
   }

  return data;
}

map<string, float> compute_average(vector<vector<string> > data, int col) {
  map<string, int> count; // maps each unique criterion to the number of its occurences
  map<string, float> price_total; 
  map<string, float> price_average;

  for (int i = 0; i < data[col].size(); i++) {
    float t = atof(data[4][i].c_str());

    //increment 
    if (count.count(data[col][i])) {
      count[data[col][i]]++;
      price_total[data[col][i]] += t;
    } else { 
      count[data[col][i]] = 1;
      price_total[data[col][i]] = t;
    }
  }
 
  for (map<string, int>::iterator iter = count.begin(); iter != count.end(); ++iter) {
    string k = iter -> first;
    price_average[k] = price_total[k] / count[k];
  }

  return price_average;
}
map<string, vector<string> > skus_by_year(vector<vector<string> > data){
  map<string, vector<string> > by_year;
  for (int i = 0; i < data[0].size(); i++) {

    string year = data[3][i];
    string sku = data[0][i];
    if (by_year.count(year)) {
      by_year[year].push_back(sku);
    } else { // Otherwise, we initialize a new vector for the new year and push the current SKU into it before placing it in the map.
      vector<string> vect(1);
      vect.push_back(sku);
      by_year[year] = vect;
    }
  }
  return by_year;
}

void output_data(vector<vector<string> > data, ofstream& out_stream) {
 
  const int sku_width = 2;
  const int brand_width = 10;
  const int cat_width = 11;
  const int year_width = 12;
  const int price_width = 9;

  out_stream << "SKU" << setw(10) << "Brand" << setw(11) << "Category" << setw(12) << "Year" << setw(9) << "Price" << "\n";
  for (int i = 0; i < data[0].size(); i++) {
    out_stream << data[0][i] 
    << setw(brand_width) << data[1][i] 
    << setw(cat_width) << data[2][i] 
    << setw(year_width) << data[3][i] << right
    << setw(price_width) << data[4][i] << "\n";
  }
}

void output_averages(map<string, float> prices, ofstream& out_stream) {
  const int cat_width = prices.begin() -> first.length() == 1 ? 8 : 5;
  const int price_width = prices.begin() -> first.length() == 1 ? 20 : 23;

  for (map<string, float>::iterator iter = prices.begin(); iter != prices.end(); ++iter) {
    string k = iter -> first;
    float v = iter -> second;
    out_stream << setw(cat_width) << k << right << setw(price_width) << v << "\n";
  }
}

void output_year_skus(map<string, vector<string> > by_year, ofstream& out_stream) {
  // Iterates over the year:SKU vector mapping and outputs them in the specified format "Year (SKU count): SKU1, SKU2, ... , SKUn"

  out_stream << "\n" <<  "Year" << setw(10) << "SKUs" << "\n"; 
  for (map<string, vector<string> >::iterator iter = by_year.begin(); iter != by_year.end(); ++iter) {
    string k = iter -> first;
    vector<string> v = iter -> second;
    string years_skus = "";
    for (int i = 0; i < v.size(); i++) {
      years_skus += v[i];
      if (i > 0 && i != v.size()-1) { // no comma before first and after last SKU
        years_skus += ", ";
      }
    }
    out_stream << k << " (" << v.size()-1 << "): " << years_skus << "\n";
  }
}

int main() {
  ofstream out_stream;
  out_stream.open("data.txt");
  out_stream.precision(3);
  //output values
  vector<vector<string> > data = read_data();
  output_data(data, out_stream);

  map<string, float> brand_prices = compute_average(data, 1); // column 1 = brand
  out_stream << "\n" <<  "Brand" << setw(23) << "Average Price" << "\n"; 
  output_averages(brand_prices, out_stream);

  map<string, float> category_prices = compute_average(data, 2); // column 2 = category
  out_stream << "\n" <<  "Category" << setw(20) << "Average Price" << "\n"; 
  output_averages(category_prices, out_stream);

  map<string, vector<string> > by_year = skus_by_year(data);
  output_year_skus(by_year, out_stream);

  out_stream.close();

  return 0;
}
