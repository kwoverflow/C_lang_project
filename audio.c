//��ü������ ���� ����
//����: bgm�� ������ �߰��� ȿ������ shuffle�� ������ ���� ����, 
//������ �� �� shuffle�� ��ȣ���� �̷������ ����.

//MCI_OPENPARMS openBgm;
//MCI_PLAY_PARMS playBgm;
//MCI_OPEN_PARMAS openShuffleSound;
//MCI_PLAY_PARMS playShuffleSound;
//
//#define bgm "���ϰ�� �Է�" //bgm �ҽ� ã�� 
//#define shuffle "���ϰ�� �Է�" // ȿ���� �ҽ� ã�� 
//int dwID;
//
//void play_bgm(){
// 	openBgm.lpstrElementName = BGM; //���� ����
//	openBgm.lpstrDeviceType = "mpegvideo" //mp3���� 
//	
//	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
//	dwID = openBgm.wDeviceID;
//	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&openBgm);
// }
// 
//void play_shuffle(){
// 	openShuffleSound.lpstrElementName = SHUFFLE;
// 	openShuffleSound.lpstrDeviceType = "mpegvideo";
// 	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openShuffleSound);
//	dwID = openShuffleSound.wDeviceID;
//	mciSendCommand(doID, MCI_PLAY, MCI_MODIFY, (DWORD)(LPVOID)&openShuffleSound);
//	Sleep(1800);
//	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
//}

