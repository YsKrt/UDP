

// ��M�֐�
int recieve() {
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
    addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANY�͂��ׂẴA�h���X����̃p�P�b�g����M����

    // �o�C���h
    // �A�h���X���̏���socket�ɓo�^����
    bind(sock, (struct sockaddr*) & addr, sizeof(addr));

    // �u���b�L���O�A�m���u���b�L���O�̐ݒ�
    //�i�C�ӁA�K�{�R�[�h�ł͂Ȃ��i�\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�Ȃ��߁j
    // val = 0 : �u���b�L���O���[�h �f�[�^����M�����܂őҋ@
    // val = 1 : �m���u���b�L���O �f�[�^����M����Ȃ��Ă����̏�����
    u_long val = 1;
    ioctlsocket(sock, FIONBIO, &val);

    // �o�b�t�@ �����Ɏ�M�����f�[�^������ �T�C�Y�͎��R�Ɍ��߂��邪�Achar�z��
    char buf[2048];
    memset(buf, 0, sizeof(buf)); // �������M����Ƃ��͑O��l���c��Ȃ��悤�ɂ��̂悤�� �o�b�t�@��0�ŃN���A����̂���ʓI �������[����֐�

    // ��M
    // recv(�\�P�b�g, ��M����f�[�^�̊i�[��, �f�[�^�̃o�C�g��, �t���O);
    // �o�C���h���Ă��Ȃ��ꍇ�� recvfrom(sock, buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr)); �ł�OK�H
    recv(sock, buf, sizeof(buf), 0);

    // �o��
    printf("%s\n", buf);

    // socket�̔j��
    closesocket(sock);

    // winsock�̏I��
    WSACleanup();

    return 0;
}

//���M�֐�
int send() {
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
    byte buf[2048];
    buf[0] = 77;
    // ���M
    // sendto(�\�P�b�g, ���M����f�[�^, �f�[�^�̃o�C�g��, �t���O, �A�h���X���, �A�h���X���̃T�C�Y);
    // ���M����f�[�^�ɒ��ڕ����� "HELLO" �������邱�Ƃ��ł���
    // �o�C���h���Ă���ꍇ�� send(sock, buf, 5, 0); �ł�OK�H
    sendto(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*) & addr, sizeof(addr));

    // socket�̔j��
    closesocket(sock);

    // winsock�̏I��
    WSACleanup();

    return 0;
}
/*
int main() {
    send();
    //recieve();

    return 0;
}
*/