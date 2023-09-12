#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <numeric>
#include <iomanip>

using namespace std;

map<string, vector<long double>> fillElementMap(ifstream & file, vector<string> & columnNames) {

    string line;
    map<string, vector<long double>> columnMap;
    
    //Fill column map with names and empty vectors
    for( unsigned int i = 0; i < columnNames.size(); i++ ){
        columnMap.insert(pair<string, vector<long double>>(columnNames[i], vector<long double>()));
    }

    //Fill empty vectors of column map with csv row entries
    //NOTE: Nested while loop is inefficient, leaving in for time sake but a recursive approach may be better
    while( getline( file, line ) ) {
        long double elementValue;
        string elementTemp;
        stringstream currentLine(line);
        for( unsigned int i = 0; i < columnNames.size(); i++ ){
            getline(currentLine, elementTemp, ',');
            elementValue = stold(elementTemp.c_str());
            columnMap[columnNames[i]].push_back(elementValue);

            // cout << columnNames[i] << " " << elementValue << "\n";
        }
    }
    
    return columnMap;
}

vector<string> getColumnNames(ifstream & file) {
    //Gets the column names of the csv
    vector<string> columnNames;
    string line = "";

    getline( file, line );
    stringstream inputString(line);
    string columnName;
    while( getline(inputString, columnName, ',') ) {
        columnNames.push_back(columnName);
    }

    return columnNames;
}

int main() { 
    //Open the file within the project directory
    string fileName = "../random_numbers.csv";

    ifstream file;
    file.open(fileName);

    if(!file.is_open()) {
        cout << "Error opening file.";
        return 1;
    }

    vector<string> columnNames = getColumnNames(file);
    map<string, vector<long double>> columnMap = fillElementMap(file, columnNames);
    for( unsigned int i=0; i<columnNames.size(); i++ ) {
        int count = columnMap[columnNames[i]].size();
        long double mean = accumulate(columnMap[columnNames[i]].begin(), columnMap[columnNames[i]].end(), 0.0) / count;
        long double standarddeviation = (mean / count - mean * mean);

        //NOTE: Would be ideal to break min max calculation outside of nested for loop for optimization
        long double minVal = .99;
        long double maxVal = 0.0;
        for ( unsigned int j=0; j<=count; j++ ) {
            if( columnMap[columnNames[i]][j] != 0.0 && columnMap[columnNames[i]][j] < minVal ) {
                minVal = columnMap[columnNames[i]][j];
            }
            if( columnMap[columnNames[i]][j] != 0.0 && columnMap[columnNames[i]][j] > maxVal ) {
                maxVal = columnMap[columnNames[i]][j];
            }
            
        }
        cout << setprecision(17);
        cout << fixed;
        cout << columnNames[i] << ": " << "[" << count << ", " << mean << ", " << standarddeviation << ", " << minVal << ", " << maxVal  << "]\n";
    }

    return 0;
}