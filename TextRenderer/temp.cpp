
#include <iostream>
#include "document.h"
#include <codecvt>
#include "istreamwrapper.h"
#include "ostreamwrapper.h"
#include "filereadstream.h"
#include "encodedstream.h"
#include <fstream>
#include <string>
#include <Windows.h>
#include <WinUser.h>
#include "encodings.h"
#include "writer.h"

using namespace rapidjson;
using namespace std;

typedef GenericDocument<UTF16<> > WDocument;
typedef GenericValue<UTF16<> > WValue;


/**
 * 1 . Sử dụng file UTF8 rồi chuyển đổi sang UTF16 hiển thị Unicode
 * 2.  Sử dụng file UTF16 sử dụng trực tiếp
 * 
 */


// Sử dụng convert UTF8 sang UTF16 để hiển thị 
wstring conveUTF8toUTF16(const string a)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(a);
    return wide;
}


string conveUTF16toUTF8(const wstring a)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow = converter.to_bytes(a);
    return narrow;
}




void ShowMSG(wstring msg)
{
    HWND  hWnd = GetConsoleWindow();
    MessageBoxW(hWnd,msg.c_str(), L"Test", MB_OK);
}


int main()
{
    ifstream ifs("inputu8.json");
    IStreamWrapper  isw(ifs);
    typedef EncodedInputStream<UTF8<>, IStreamWrapper> InputStream;
    InputStream  eis(isw);
    Document d;
    d.ParseStream<0, UTF8<>>(eis);

    string a = d["floor"].GetString();
    int ia = d["time"].GetDouble();

    //Muốn hiển thị UNICODE hàm Covert to UTF16
    //wstring wa = conveUTF8toUTF16(a);

    if (a == u8"Ngô văn Thường え ぉ お か が き ⾝ ⾜ ⿕⾝ ⾜ ⿕")
    {
        ShowMSG(conveUTF8toUTF16(a));
    }
    

    ofstream ofs("outputu8.json");
    OStreamWrapper osw(ofs);
    typedef EncodedOutputStream<UTF8<>, OStreamWrapper> OutputStream;
    OutputStream eos(osw, true);

    Value newText(kStringType);
    newText.SetString(u8"おはよう");

    d.AddMember("vungmoi" , newText , d.GetAllocator());

    Writer<OutputStream, UTF8<>, UTF8<>> writer(eos);
    d.Accept(writer);
}


//int main()
//{
//    ifstream ifs("input.json");
//    IStreamWrapper  isw(ifs);
//    typedef EncodedInputStream<UTF16LE<>, IStreamWrapper> InputStream;
//    InputStream  eis(isw);
//    WDocument d;
//    d.ParseStream<0, UTF16LE<>>(eis);
//
//    wstring wa = d[L"floor"].GetString();
//    int ia = d[L"time"].GetDouble();
//
//    if (wa == L"Ngô văn Thường え ぉ お か が き ⾝ ⾜ ⿕⾝ ⾜ ⿕")
//    {
//        ShowMSG(wa);
//    }
//
//    ofstream ofs("input.json");
//    OStreamWrapper osw(ofs);
//    typedef EncodedOutputStream<UTF16LE<>, OStreamWrapper> OutputStream;
//    OutputStream eos(osw, true);
//
//    Writer<OutputStream, UTF16LE<>, UTF16LE<>> writer(eos);
//    d.Accept(writer);
//
//}




===================================



#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <Windows.h>
#include <WinUser.h>
#include <clocale>
#include "json/json.h"

using namespace std;


void ShowMSG(wstring msg)
{
    HWND  hWnd = GetConsoleWindow();
    MessageBoxW(hWnd, msg.c_str(), L"Test", MB_OK);
}



// Sử dụng convert UTF8 sang UTF16 để hiển thị 
wstring conveUTF8toUTF16(const string a)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(a);
    return wide;
}

string conveUTF16toUTF8(const wstring a)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string narrow = converter.to_bytes(a);
    return narrow;
}

int main()
{
    Json::Value root;  
    Json::CharReaderBuilder builder;
    std::ifstream test("output.json", std::ifstream::binary);
    std::string errs;
    bool ok = Json::parseFromStream(builder, test, &root, &errs);
    if (!ok)
    {
        // report to the user the failure and their locations in the document.
        std::cout << errs << "\n";
    }

    wstring ws = conveUTF8toUTF16(root["floor"]["title"].asCString());
    ShowMSG(ws);

    Json::StreamWriterBuilder writebuilder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(writebuilder.newStreamWriter());
    std::locale loc(std::locale(), new std::codecvt_utf8<char16_t>);
    std::ofstream  outputFileStream("input.json");
    //std::basic_ofstream<char32_t> ofs("output.json");
    outputFileStream.imbue(loc);
    writer->write(root, &outputFileStream);
    getchar();
}

