#include "DxLib.h"

#include "TitleUI.h"

#include "../UIManager.h"

#include "../../Character/CharacterManager.h"
#include "../../Character/Player/PlayerManager.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Input/InputManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Camera/CameraManager.h"
#include "../../MyMath/MyMath.h"
#include "../../System/System.h"

#include "../../Collision/Collision.h"

#include "../../Input/Mouse/Mouse.h"

void CTitleUI::Init()
{

}
void CTitleUI::Load()
{

}
void CTitleUI::Step()
{

}
void CTitleUI::Update()
{

}
void CTitleUI::Draw()
{

}
void CTitleUI::Fin()
{

}

CTitleUI* CTitleUI::CreateTitleUI()
{
	return _NEW(CTitleUI);
}





bool CTitleUI::StepStartPhase() { return false; }
void CTitleUI::DrawStartPhase(){}

int CTitleUI::StepPause() { return 0; }
void CTitleUI::DrawPause(){}
void CTitleUI::SetBossHP(int hp){}