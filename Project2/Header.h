#pragma once

#include "Source1.cpp"
#include"Source.cpp"



////////////////////////////////////////////つうしんのやつ
//TCP,UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")

//TCP,UDP通信用ヘッダ
//#include <sys/sock.h> //linux
#include <WinSock2.h> //windows

#include <iostream>
// inet_addr()関数で警告が出る場合は以下で警告を無効化する。
#pragma warning(disable:4996) 


//////////////////////////////////////////げーむのやつ
#include <DxLib.h>
#include <stdio.h>

#include<cstddef>
