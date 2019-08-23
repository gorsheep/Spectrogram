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

    
    /*
    unsigned char q = 0xff;
    unsigned char w = 0xf6;
    unsigned char e = 0x29;
    
    cout << threeCharsToInt(q, w, e) << endl;
    */
    
    
    
    
    /*
    int i = 0; //итератор
    
    ifstream stream;
    //stream.open("data4.irs", ios_base::binary);
    stream.open("data4.irs", ifstream::binary);
    if (!stream.bad()) {
        cout << hex;
        cout.width(2);
        
        while (!stream.eof()) {
            i++;
            unsigned char c;
            stream >> c;
            //cout << static_cast<unsigned>(c) << " , i = " << i << endl;
            cout << static_cast<unsigned>(c) << endl;
        }
    }
    
    cout << dec;
    cout << "i = " << i << endl;
    */
    
    
    
    
    
    /*
    FILE *f;
    char c;
    f=fopen("data3.irs","r");
    cout << hex;
    cout.width(2);
    while((c=fgetc(f))!=EOF)
    {
        cout << static_cast<unsigned>(c) << endl;
    }
    fclose(f);
    */
    
    
    
    
    
    /*
    ifstream is ("data4.irs", ifstream::binary);
    
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    
    unsigned char * buffer = new unsigned char [length];
    
    cout << "Reading " << length << " characters... ";
    // read data as a block:
    //is.read (buffer,length);
    is.read (reinterpret_cast<char*>(buffer),length);
    
    if (is)
        cout << "all characters read successfully." << endl;
    else
        cout << "error: only " << is.gcount() << " could be read" << endl;
    is.close();
    
    // ...buffer contains the entire file...
    
    for (int i=0; i<length; i++) {
        cout << buffer[i] << endl;
    }
    
    //delete[] buffer;
    
    
    cout << length << endl;
    cout << hex;
    cout << buffer[0] << endl;
    */
    
    
    vector<BYTE> fileData = readFile("data3.irs");
    cout << hex;
    cout.width(2);
    cout << static_cast<unsigned>(fileData[0]) << endl;
    cout << dec;
    cout << fileData.size() << endl;

    
    
    return 0;
}
