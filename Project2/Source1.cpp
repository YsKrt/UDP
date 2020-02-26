


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
    ChangeWindowMode(TRUE);//非全画面にセット
    SetGraphMode(640, 480, 32);//画面サイズ指定
    SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
    if (DxLib_Init() == 1) { return -1; }//初期化に失敗時にエラーを吐かせて終了

  
    pos player=pos(0,0,3,3,30,30,GetColor(100,100,200));
    pos enemy = pos(640-30, 480-30, 3, 3, 30, 30, GetColor(200, 100, 100));

    ///////////////////////////////////////////////////////////////
    // ソケット通信winsockの立ち上げ
    // wsaDataはエラー取得等に使用する
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと

    // socket作成
    // socketは通信の出入り口 ここを通してデータのやり取りをする
    // socket(アドレスファミリ, ソケットタイプ, プロトコル)
    SOCKET sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

    // アドレス等格納
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  //IPv4
    addr.sin_port = htons(12345);   //通信ポート番号設定
    addr.sin_addr.S_un.S_addr = inet_addr("10.111.167.204"); // 送信アドレスを127.0.0.1に設定

    // バッファ ここに送信するデータを入れる  サイズは自由に決められるが、char配列

    
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

    // ブロッキング、ノンブロッキングの設定
    //（任意、必須コードではない（ソケットの初期設定はブロッキングモードなため）
    // val = 0 : ブロッキングモード データが受信されるまで待機
    // val = 1 : ノンブロッキング データが受信されなくても次の処理へ
    u_long val = 1;
    ioctlsocket(sock, FIONBIO, &val);

    // バッファ ここに受信したデータが入る サイズは自由に決められるが、char配列

    memset(rbuf, 0, sizeof(rbuf)); // 複数回受信するときは前回値が残らないようにこのように バッファを0でクリアするのが一般的 メモリー操作関数

    int addrlen = sizeof(addr);

    // 受信
    // recv(ソケット, 受信するデータの格納先, データのバイト数, フラグ);
    // バインドしていない場合は recvfrom(sock, buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr)); でもOK？

    // 出力
    //std::cout << rbuf;

 
    //##############################################################################

    while (ProcessMessage() == 0)
    {

        //////////////////////////////////////////////////////////////////

    // 送信
    // sendto(ソケット, 送信するデータ, データのバイト数, フラグ, アドレス情報, アドレス情報のサイズ);
    // 送信するデータに直接文字列 "HELLO" 等を入れることもできる
    // バインドしている場合は send(sock, buf, 5, 0); でもOK？
        sendto(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*) & addr, sizeof(addr));
        ////////////////////////////////////////////////////////////////////

        //std::cout<<
            recvfrom(sock, (char*)rbuf, sizeof(rbuf), 0, (struct sockaddr*) & addr, &addrlen);
        //std::cout << rbuf[0]<<"\n";
       // std::cout << WSAGetLastError() << "\n";
        //**************:
  
        ClearDrawScreen();//裏画面消す
        SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に

        player.update();
        enemy.eupdate();

        ScreenFlip();//裏画面を表画面にコピー
    }

    ///////////////////////////////////////////////////////
       // socketの破棄
    closesocket(sock);

    // winsockの終了
    WSACleanup();
    ////////////////////////////////////////////////////////

    DxLib_End();
    //
    FreeConsole();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////



