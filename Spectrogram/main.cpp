//
//  main.cpp
//  Spectrogram
//
//  Created by Даниил Волошин on 8/22/19.
//  Copyright © 2019 Даниил Волошин. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned char BYTE;


//Функция, которая считывает данные из файла и записывает их в вектор
vector<BYTE> readFile(const char* filename)
{
    // open the file:
    streampos fileSize;
    ifstream file(filename, ios::binary);
    
    // get its size:
    file.seekg(0, ios::end);
    fileSize = file.tellg();
    file.seekg(0, ios::beg);
    
    // read the data:
    vector<BYTE> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}



//Функция, которая переводит 8-битное число из little-endian в big-endian
unsigned char littleEndToBigEnd(unsigned char a){
    
    unsigned char b = 0;
    
    for (int i = 0; i<8; i++) {
        if (bool((1 << i)  &  a)) { //если a[i]=1
            b |= (1 << (7-i));      //то b[7-i]=1
        }
        else{                       //если a[i]=0
            b &= ~(1 << (7-i));     //то b[7-i]=0
        }
    }
    
    return b;
}


//Функция, которая переводит 3-байтное число формата little-endian в знаковый int (с учетом доп кода)
int threeCharsToInt(unsigned char a, unsigned char b, unsigned char c){
    
    int ans = 0;
    
    if (bool((1 << 0) & c) == 1) { //если число отрицательное (в доп коде)
        a = littleEndToBigEnd(a);
        b = littleEndToBigEnd(b);
        c = littleEndToBigEnd(c);
        a = ~a;
        b = ~b;
        c = ~c;
        ans = 1 + int(a) + int(b)*pow(2, 8)+int(c)*pow(2, 16);
        ans = -ans;
        
    }
    if (bool((1 << 0) & c) == 0) { //если число положительное (в обычном коде)
        a = littleEndToBigEnd(a);
        b = littleEndToBigEnd(b);
        c = littleEndToBigEnd(c);
        ans = int(a) + int(b)*pow(2, 8)+int(c)*pow(2, 16);
    }
    
    return ans;
}



int main() {
    
    ofstream pureDataObj;
    pureDataObj.open("pureData.csv"); //файл, куда будут экспортированы преобразованные данные
    
    vector<BYTE> fileData = readFile("30_06_2019 07_21_59 IRsv_21_01.irs"); //массив с raw data
    vector<float> data; //массив с преобразованными данными
    
    int numOfPacks = floor(fileData.size()/1024); //число пакетов в файле (в пакете 1024 байта)
    int samplingRate = int(fileData[12]); //частота дискретизации - 5 микросекунд
    float scale = 0.0000257; //цена одного битового отсчета (перевод в Вольты)
    
    cout << "Число байт в файле: " << fileData.size() << " байт" << endl;
    cout << "Число пакетов в файле: " << numOfPacks << endl;
    cout << "Частота дискретизации: " << samplingRate << " мкс" << endl;
    cout << endl;
    
    
    //Цикл, заполняющий массив с преобразованными данными
    for (int j = 0; j < numOfPacks; j++) {
        //Цикл, вычленяющий 100 отсчетов из i-го пакета (в файле 179 пакетов)
        for (int i = 0; i < 100; i++) {
            unsigned char a = fileData[1024*j+40+9*i];
            unsigned char b = fileData[1024*j+41+9*i];
            unsigned char c = fileData[1024*j+42+9*i];
            data.push_back(scale*float(threeCharsToInt(a, b, c)));
        }
    }
    
    cout << "Число измерений: " << data.size() << endl;
    cout << endl;
    
    
    //Цикл, который пишет в CSV-файл преобразованные данные с таймстэмпами
    for (int i = 0; i < data.size(); i++) {
        pureDataObj << samplingRate*i << "," << data[i] << endl;
    }
    
    
    
    
    return 0;
}
