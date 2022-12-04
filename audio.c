//전체적으로 구현 실패
//원인: bgm이 나오고 중간에 효과음인 shuffle이 나오는 것은 성공, 
//하지만 그 뒤 shuffle의 재호출이 이루어지지 않음.

//MCI_OPENPARMS openBgm;
//MCI_PLAY_PARMS playBgm;
//MCI_OPEN_PARMAS openShuffleSound;
//MCI_PLAY_PARMS playShuffleSound;
//
//#define bgm "파일경로 입력" //bgm 소스 찾기 
//#define shuffle "파일경로 입력" // 효과음 소스 찾기 
//int dwID;
//
//void play_bgm(){
// 	openBgm.lpstrElementName = BGM; //파일 열기
//	openBgm.lpstrDeviceType = "mpegvideo" //mp3형식 
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

