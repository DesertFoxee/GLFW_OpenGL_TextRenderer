#include <iostream>
#include "document.h"
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


void ShowMSG(wstring msg)
{
    HWND  hWnd = GetConsoleWindow();
    MessageBoxW(hWnd,msg.c_str(), L"Test", MB_OK);
}


int main()
{
    ifstream ifs("input.json");
    IStreamWrapper  isw(ifs);
    typedef EncodedInputStream<UTF16LE<>, IStreamWrapper> InputStream;
    InputStream  eis(isw);
    WDocument d;
    d.ParseStream<0, UTF16LE<>>(eis);

    wstring a = d[L"floor"].GetString();
    int ia = d[L"time"].GetDouble();

    ShowMSG(a);

    ofstream ofs("input.json");
    OStreamWrapper osw(ofs);
    typedef EncodedOutputStream<UTF16LE<>, OStreamWrapper> OutputStream;
    OutputStream eos(osw, true);

    Writer<OutputStream, UTF16LE<>, UTF16LE<>> writer(eos);
    d.Accept(writer);
    
}