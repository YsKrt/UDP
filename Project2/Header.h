#pragma once

#include "Source1.cpp"
#include"Source.cpp"



////////////////////////////////////////////������̂��
//TCP,UDP�ʐM�p���C�u����
#pragma comment(lib, "ws2_32.lib")

//TCP,UDP�ʐM�p�w�b�_
//#include <sys/sock.h> //linux
#include <WinSock2.h> //windows

#include <iostream>
// inet_addr()�֐��Ōx�����o��ꍇ�͈ȉ��Ōx���𖳌�������B
#pragma warning(disable:4996) 


//////////////////////////////////////////���[�ނ̂��
#include <DxLib.h>
#include <stdio.h>

#include<cstddef>
