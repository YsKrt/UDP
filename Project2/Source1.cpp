


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




char rbuf[2048] = {};
byte buf[4] = {};

class pos
{
    int x;
    int y;
    int width;
    int height;
    int veloX;
    int veloY;
    int color;

public:
    pos(int x, int y, int veloX, int veloY, int width, int height,int color)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->veloX = veloX;
        this->veloY = veloY;
        this->color = color;
    }
    void draw()
    {
        DrawBox(this->x, this->y, this->x + this->width, this->y + this->height, this->color, TRUE);
    }
    void control()
    {
        if (CheckHitKey(KEY_INPUT_RIGHT) == 1) {
            this->x += this->veloX;
            buf[1] |=1;
        }
        else {
            buf[1] &= 254;
        }
        if (CheckHitKey(KEY_INPUT_LEFT) == 1) {
            this->x += -this->veloX;
            buf[1] |=2 ;
        }
        else {
            buf[1] &= 0b11111101;
        }
        if (CheckHitKey(KEY_INPUT_UP) == 1) {
            this->y += -this->veloY;
            buf[1] |=4;
        }
        else {
            buf[1] &= 0b11111011;
        }
        if (CheckHitKey(KEY_INPUT_DOWN) == 1) {
            this->y += this->veloY;
            buf[1] |= 8;
        }
        else {
            buf[1] &= 0b11110111;
        }
        std::cout << (int)rbuf[4]<<"\n";

    }
    void update()
    {
        this->draw();
        this->control();

    }
    void eupdate()
    {
        this->x = getx();
        this->y = gety();
        this->draw();
    }

    int getx()
    {
        int a1 = (int)rbuf[4];
        int a2 = (int)(rbuf[3] << 8);
        int a3 = (int)(rbuf[2] << 16);
        int a4 = (int)(rbuf[1] << 24);
        return a1 + a2 + a3 + a4;
    }

    int gety()
    {
        int wa = 0;
        for (int i = 5; i < 9; i++)
            wa += (int)(rbuf[i] << (i - 5) * 8);
        return wa;
    }

    void reflect()
    {

    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    //
    ChangeWindowMode(TRUE);//��S��ʂɃZ�b�g
    SetGraphMode(640, 480, 32);//��ʃT�C�Y�w��
    SetOutApplicationLogValidFlag(FALSE);//Log.txt�𐶐����Ȃ��悤�ɐݒ�
    if (DxLib_Init() == 1) { return -1; }//�������Ɏ��s���ɃG���[��f�����ďI��

  
    pos player=pos(0,0,3,3,30,30,GetColor(100,100,200));
    pos enemy = pos(640-30, 480-30, 3, 3, 30, 30, GetColor(200, 100, 100));

    ///////////////////////////////////////////////////////////////
    // �\�P�b�g�ʐMwinsock�̗����グ
    // wsaData�̓G���[�擾���Ɏg�p����
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)��winsock�̃o�[�W����2.0���Ă���

    // socket�쐬
    // socket�͒ʐM�̏o����� ������ʂ��ăf�[�^�̂���������
    // socket(�A�h���X�t�@�~��, �\�P�b�g�^�C�v, �v���g�R��)
    SOCKET sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H

    // �A�h���X���i�[
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  //IPv4
    addr.sin_port = htons(12345);   //�ʐM�|�[�g�ԍ��ݒ�
    addr.sin_addr.S_un.S_addr = inet_addr("10.111.167.204"); // ���M�A�h���X��127.0.0.1�ɐݒ�

    // �o�b�t�@ �����ɑ��M����f�[�^������  �T�C�Y�͎��R�Ɍ��߂��邪�Achar�z��

    
    ///////////////////////////////////////////////////////////////
    //###########################################################################

    struct sockaddr_in raddr;
   
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        exit(0);
    }
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(12345);
    raddr.sin_addr.S_un.S_addr = INADDR_ANY;
    //sock = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sock, (struct sockaddr*) & raddr, sizeof(raddr));

    // �u���b�L���O�A�m���u���b�L���O�̐ݒ�
    //�i�C�ӁA�K�{�R�[�h�ł͂Ȃ��i�\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�Ȃ��߁j
    // val = 0 : �u���b�L���O���[�h �f�[�^����M�����܂őҋ@
    // val = 1 : �m���u���b�L���O �f�[�^����M����Ȃ��Ă����̏�����
    u_long val = 1;
    ioctlsocket(sock, FIONBIO, &val);

    // �o�b�t�@ �����Ɏ�M�����f�[�^������ �T�C�Y�͎��R�Ɍ��߂��邪�Achar�z��

    memset(rbuf, 0, sizeof(rbuf)); // �������M����Ƃ��͑O��l���c��Ȃ��悤�ɂ��̂悤�� �o�b�t�@��0�ŃN���A����̂���ʓI �������[����֐�

    int addrlen = sizeof(addr);

    // ��M
    // recv(�\�P�b�g, ��M����f�[�^�̊i�[��, �f�[�^�̃o�C�g��, �t���O);
    // �o�C���h���Ă��Ȃ��ꍇ�� recvfrom(sock, buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr)); �ł�OK�H

    // �o��
    //std::cout << rbuf;

 
    //##############################################################################

    while (ProcessMessage() == 0)
    {

        //////////////////////////////////////////////////////////////////

    // ���M
    // sendto(�\�P�b�g, ���M����f�[�^, �f�[�^�̃o�C�g��, �t���O, �A�h���X���, �A�h���X���̃T�C�Y);
    // ���M����f�[�^�ɒ��ڕ����� "HELLO" �������邱�Ƃ��ł���
    // �o�C���h���Ă���ꍇ�� send(sock, buf, 5, 0); �ł�OK�H
        sendto(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*) & addr, sizeof(addr));
        ////////////////////////////////////////////////////////////////////

        //std::cout<<
            recvfrom(sock, (char*)rbuf, sizeof(rbuf), 0, (struct sockaddr*) & addr, &addrlen);
        //std::cout << rbuf[0]<<"\n";
       // std::cout << WSAGetLastError() << "\n";
        //**************:
  
        ClearDrawScreen();//����ʏ���
        SetDrawScreen(DX_SCREEN_BACK);//�`���𗠉�ʂ�

        player.update();
        enemy.eupdate();

        ScreenFlip();//����ʂ�\��ʂɃR�s�[
    }

    ///////////////////////////////////////////////////////
       // socket�̔j��
    closesocket(sock);

    // winsock�̏I��
    WSACleanup();
    ////////////////////////////////////////////////////////

    DxLib_End();
    //
    FreeConsole();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////



