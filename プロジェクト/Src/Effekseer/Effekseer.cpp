#include "DxLib.h"
#include "effekseer.h"
#include <locale.h>
#include <uchar.h>


//	new�Edelete�͂�����SAFE_RELEASE�ȂǂŊJ�������ق����悢
//	�ނ���X�}�[�g�|�C���^�Ȃǂ��g���Ă��悢
//	����Ă�����static�N���X����Ȃ��ق����悢

#ifdef _DEBUG
#ifdef _WIN64
#if _MSC_VER >= 1920
#pragma comment(lib,"Effekseer_vs2019_x64_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2019_x64_d.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2019_x64_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2017_x64_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2017_x64_d.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2017_x64_d.lib")
#endif
#else
#if _MSC_VER >= 1920
#pragma comment(lib,"Effekseer_vs2019_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2019_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2019_x86_d.lib")
#else
#pragma comment(lib,"Effekseer_vs2017_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2017_x86_d.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2017_x86_d.lib")
#endif
#endif
#else
#ifdef _WIN64
#if _MSC_VER >= 1920
#pragma comment(lib,"Effekseer_vs2019_x64.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2019_x64.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2019_x64.lib")
#else
#pragma comment(lib,"Effekseer_vs2017_x64.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2017_x64.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2017_x64.lib")
#endif
#else
#if _MSC_VER >= 1920
#pragma comment(lib,"Effekseer_vs2019_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2019_x86.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2019_x86.lib")
#else
#pragma comment(lib,"Effekseer_vs2017_x86.lib")
#pragma comment(lib,"EffekseerRendererDX9_vs2017_x86.lib")
#pragma comment(lib,"EffekseerRendererDX11_vs2017_x86.lib")
#endif
#endif
#endif

//---------------------------------
//		�R���X�g���N�^
//---------------------------------
CEffekseer::CEffekseer()
{
	ZeroMemory(&m_pos, sizeof(m_pos));
	m_hndl = -1;
	m_ID = -1;
	m_isLoop = false;

	m_scale = VGet(1.f, 1.f, 1.f);
	m_speed = 1.f;
	m_endFrame = -1;
}


//---------------------------------
//		�f�X�g���N�^
//---------------------------------
CEffekseer::~CEffekseer()
{
}


//---------------------------------
//		���Z�b�g
//---------------------------------
void	CEffekseer::Reset(void)
{
	m_hndl = -1;
	m_isLoop = false;
}








CEffekseer CEffekseerCtrl::m_eff[EFFECT_NUM];
std::vector<::Effekseer::Effect*>	CEffekseerCtrl::m_effect;
::EffekseerRendererDX9::Renderer* CEffekseerCtrl::m_renderer9 = NULL;
::EffekseerRendererDX11::Renderer* CEffekseerCtrl::m_renderer11 = NULL;
::Effekseer::Manager* CEffekseerCtrl::m_manager;
int			CEffekseerCtrl::m_allNum;
int			CEffekseerCtrl::m_useID;
int			CEffekseerCtrl::m_dxVersion;


#define DX_VERSION_9	(9)
#define DX_VERSION_11	(11)


//---------------------------------
//		�R���X�g���N�^
//---------------------------------
CEffekseerCtrl::CEffekseerCtrl()
{
	memset(m_eff, 0, sizeof(CEffekseer) * EFFECT_NUM);
	m_renderer9 = nullptr;
	m_renderer11 = nullptr;
	m_manager = nullptr;
	m_allNum = 0;
	m_dxVersion = -1;
	m_useID = 0;
}


//---------------------------------
//		�f�X�g���N�^
//---------------------------------
CEffekseerCtrl::~CEffekseerCtrl()
{
	Exit();
}


//---------------------------------
//		������
//---------------------------------
void	CEffekseerCtrl::Init(int effNum, int particleNum)
{
	Exit();

	// �`��p�C���X�^���X�̐���
	EffekseerRenderer::Renderer* renderer;
	ID3D11Device* device = (ID3D11Device*)GetUseDirect3D11Device();
	ID3D11DeviceContext* context = (ID3D11DeviceContext*)GetUseDirect3D11DeviceContext();
	if (device == nullptr || context == nullptr)
	{
		SetUseDirect3D9Ex(FALSE);	// LPDIRECT3DDEVICE9���擾�ł���悤�ɔO�̂��ߐݒ�
		LPDIRECT3DDEVICE9 device9 = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
		m_renderer9 = ::EffekseerRendererDX9::Renderer::Create(device9, particleNum);
		m_dxVersion = DX_VERSION_9;
		renderer = (EffekseerRenderer::Renderer*)m_renderer9;
	}
	else
	{
		m_renderer11 = ::EffekseerRendererDX11::Renderer::Create(device, context, particleNum);
		m_dxVersion = DX_VERSION_11;
		renderer = (EffekseerRenderer::Renderer*)m_renderer11;
	}


	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_manager = ::Effekseer::Manager::Create(particleNum);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(renderer->CreateRingRenderer());
	m_manager->SetModelRenderer(renderer->CreateModelRenderer());
	m_manager->SetTrackRenderer(renderer->CreateTrackRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(renderer->CreateTextureLoader());
	m_manager->SetModelLoader(renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(renderer->CreateMaterialLoader());

	// �f�o�C�X���X�g���̃R�[���o�b�N�ݒ�(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction(DeviceLostFunction, NULL);
	SetGraphicsDeviceRestoreCallbackFunction(DeviceRestoreFunction, NULL);

	// ���W�͍�����W�n��
	m_manager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);

	// ���e�s���ݒ�(����͉�)
	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(90.0f / 180.0f * 3.14f, 640.0f / 480.0f, 1.0f, 1000.0f));
	// �J�����s���ݒ�(�������)
	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(0.0f, 0.0f, -20.0f), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	// �ʃG�t�F�N�g�̃������m��
	/*m_eff = new CEffekseer[effNum];*/
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		m_eff[i].SetHndl(-1);
		m_eff[i].SetID(-1);
		m_eff[i].SetLoop(false);

		m_eff[i].SetScale(VGet(1.f, 1.f, 1.f));
		m_eff[i].SetSpeed(1.f);
		m_eff[i].SetEndFrame(-1);
		m_eff[i].SetTrans(VGet(0.f, 0.f, 0.f));
	}
	m_allNum = EFFECT_NUM;

	// ������
	m_effect.clear();
	m_useID = 0;
}


//---------------------------------
//		�I������
//---------------------------------
void	CEffekseerCtrl::Exit(void)
{
	// ������ƕs���Ȕj����ƒB
	if (m_eff != nullptr)
	{
		StopAll();

		//CEffekseer* eff = m_eff;
		//for (int index = 0; index < m_allNum; ++index)
		//{
		//	CEffekseer* del_eff = eff++;
		//	delete del_eff;
		//}

		/*delete[] m_eff;
		m_eff = nullptr;*/
		m_allNum = 0;
	}
	if (m_renderer9 != nullptr)
	{
		m_renderer9->Destroy();
		m_renderer9 = nullptr;
	}
	if (m_renderer11 != nullptr)
	{
		m_renderer11->Destroy();
		m_renderer11 = nullptr;
	}
	m_dxVersion = -1;
	if (m_manager != nullptr)
	{
		m_manager->Destroy();
		m_manager = nullptr;
	}

	std::vector<::Effekseer::Effect*>::iterator it = m_effect.begin();
	while (it != m_effect.end())
	{
		ES_SAFE_RELEASE(*it);
		it++;
	}
	m_effect.clear();
}


//---------------------------------
//		�G�t�F�N�g�f�[�^���[�h
//---------------------------------
int	CEffekseerCtrl::LoadData(const char* name)
{
	int	ret = -1;

	// �G�t�F�N�g�̓Ǎ�
	EFK_CHAR rename[256];
	for (int i = 0; i < 256; i++)
	{
		mbstate_t mbstate = { 0 };
		size_t s = mbrtoc16(&rename[i], &name[i], 1, &mbstate);
		if (s <= 0) break;
	}
	::Effekseer::Effect* temp = nullptr;
	temp = ::Effekseer::Effect::Create(m_manager, rename);
	if (temp != nullptr)
	{
		m_effect.push_back(temp);
		ret = static_cast<int>(m_effect.size()) - 1;
	}

	return ret;
}


//---------------------------------
//		���e�s��ݒ�
//---------------------------------
void	CEffekseerCtrl::SetProjectionMtx(float pers, float aspect, float nearLength, float farLength)
{
	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(pers, aspect, nearLength, farLength));
}


//---------------------------------
//		���e�s�񎩓��ݒ�
//---------------------------------
void	CEffekseerCtrl::SetAutoProjectionMtx()
{
	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	// �𑜓x�擾
	RECT rect;
	GetWindowCRect(&rect);
	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(GetCameraFov(), static_cast<float>(rect.right - rect.left) / static_cast<float>(rect.bottom - rect.top), GetCameraNear(), GetCameraFar()));
}


//---------------------------------
//		�J�����s��ݒ�
//---------------------------------
void	CEffekseerCtrl::SetCameraMtx(VECTOR eyePos, VECTOR focusPos, VECTOR up)
{
	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(eyePos.x, eyePos.y, eyePos.z),
			::Effekseer::Vector3D(focusPos.x, focusPos.y, focusPos.z),
			::Effekseer::Vector3D(up.x, up.y, up.z)));
}


//---------------------------------
//		�J�����s�񎩓��ݒ�
//---------------------------------
void	CEffekseerCtrl::SetAutoCameraMtx()
{
	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	VECTOR pos = GetCameraPosition();
	VECTOR target = GetCameraTarget();
	VECTOR up = GetCameraUpVector();

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(pos.x, pos.y, pos.z),
			::Effekseer::Vector3D(target.x, target.y, target.z),
			::Effekseer::Vector3D(up.x, up.y, up.z)));
}


//---------------------------------
//		�J�����s��ݒ�
//---------------------------------
void	CEffekseerCtrl::SetCameraRotMtx(VECTOR eyePos, VECTOR rot, VECTOR up)
{
	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	// ( 0, 0, 1 )�̃x�N�g����XYZ���̏��Ԃŉ�]��������@�Œ����_���o��
	VECTOR focusPos, temp;
	// X����]
	temp.x = 0.0f;
	temp.y = -1.0f * sinf(rot.x);
	temp.z = 1.0f * cosf(rot.x);
	memcpy(&focusPos, &temp, sizeof(VECTOR));
	// Y����]
	temp.x = focusPos.x * cosf(rot.y) + focusPos.z * sinf(rot.y);
	temp.y = focusPos.y;
	temp.z = focusPos.z * cosf(rot.y) - focusPos.x * sinf(rot.y);
	memcpy(&focusPos, &temp, sizeof(VECTOR));
	// Z����]
	temp.x = focusPos.x * cosf(rot.z) - focusPos.y * sinf(rot.z);
	temp.y = focusPos.x * sinf(rot.z) + focusPos.y * cosf(rot.z);
	temp.z = focusPos.z;
	memcpy(&focusPos, &temp, sizeof(VECTOR));
	// ���_�ɉ��Z
	focusPos = VAdd(eyePos, focusPos);

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(eyePos.x, eyePos.y, eyePos.z),
			::Effekseer::Vector3D(focusPos.x, focusPos.y, focusPos.z),
			::Effekseer::Vector3D(up.x, up.y, up.z)));
}


//---------------------------------
//		�X�V����
//---------------------------------
void	CEffekseerCtrl::Update(void)
{
	if (m_eff == nullptr) return;
	
	// �G�t�F�N�g�̍X�V�������s��
	m_manager->Update();
	

	for (int i = 0; i < m_allNum; i++)
	{
		int hndl = m_eff[i].GetHndl();
		if (hndl == -1 || m_manager->Exists(hndl))
		{
			continue;
		}

		// ���[�v�ݒ�̂��̂͏I����ēx����������
		if (m_eff[i].IsLoop())
		{
			VECTOR pos = m_eff[i].GetTrans();
			m_manager->StopEffect(m_eff[hndl].GetHndl());
			m_eff[i].SetHndl(m_manager->Play(m_effect[m_eff[i].GetID()], pos.x, pos.y, pos.z));


			SetSpeed(i, m_eff[i].GetSpeed());
			SetScale(i, m_eff[i].GetScale());
		}
		// ���[�v�ȊO�͍ė��p�̏���
		else
		{
			m_eff[i].SetHndl(-1);
			m_eff[i].SetEndFrame(-1);
			m_eff[i].Reset();


			m_eff[i].SetSpeed(1.f);
			m_eff[i].SetScale(VGet(1.f, 1.f, 1.f));
		}
	}
	
}


//---------------------------------
//		�X�V����
//---------------------------------
void	CEffekseerCtrl::UpdateAutoCamera(void)
{
	SetAutoProjectionMtx();
	SetAutoCameraMtx();
	Update();
}


//---------------------------------
//		�`�揈��
//---------------------------------
void	CEffekseerCtrl::Draw(void)
{

	

	EffekseerRenderer::Renderer* renderer;
	if (m_dxVersion == DX_VERSION_9)
		renderer = m_renderer9;
	else if (m_dxVersion == DX_VERSION_11)
		renderer = m_renderer11;
	else
		return;

	// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
	RenderVertex();

	// �G�t�F�N�g�̕`��J�n�������s���B
	renderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	m_manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	renderer->EndRendering();

	// DX���C�u�����̐ݒ��߂��B
	RefreshDxLibDirect3DSetting();
	
}


//---------------------------------
//		�G�t�F�N�g�Ăяo��
//---------------------------------
int	CEffekseerCtrl::Request(int ID, VECTOR pos, bool isLoop)
{
	if (m_eff == nullptr || ID < 0) return -1;
	
	for (int i = 0; i < m_allNum; i++)
	{
		int number = (m_useID + i) % m_allNum;
		if (m_eff[number].GetHndl() == -1)
		{
			m_eff[number].SetHndl(m_manager->Play(m_effect[ID], pos.x, pos.y, pos.z));
			m_eff[number].SetLoop(isLoop);
			m_eff[number].SetID(ID);
			m_eff[number].SetTrans(pos);
			m_useID = (number + 1) % m_allNum;
			return number;
		}
	}
	
	return -1;
}

int	CEffekseerCtrl::Request2(int ID, VECTOR pos, int start_frame, bool isLoop)
{
	if (m_eff == nullptr || ID < 0) return -1;

	for (int i = 0; i < m_allNum; i++)
	{
		int number = (m_useID + i) % m_allNum;
		if (m_eff[number].GetHndl() == -1)
		{
			m_eff[number].SetHndl(m_manager->Play(m_effect[ID], Effekseer::Vector3D(pos.x, pos.y, pos.z), start_frame));
			m_eff[number].SetLoop(isLoop);
			m_eff[number].SetID(ID);
			m_eff[number].SetTrans(pos);
			m_useID = (number + 1) % m_allNum;
			return number;
		}
	}

	return -1;
}

int CEffekseerCtrl::Request3(int ID, VECTOR pos, int start_frame, int end_frame, bool isLoop)
{
	if (m_eff == nullptr || ID < 0) return -1;

	for (int i = 0; i < m_allNum; i++)
	{
		int number = (m_useID + i) % m_allNum;
		if (m_eff[number].GetHndl() == -1)
		{
			m_eff[number].SetHndl(m_manager->Play(m_effect[ID], Effekseer::Vector3D(pos.x, pos.y, pos.z), start_frame));
			m_eff[number].SetLoop(isLoop);
			m_eff[number].SetID(ID);
			m_eff[number].SetTrans(pos);
			m_useID = (number + 1) % m_allNum;
			return number;
		}
	}

	return -1;
}


//---------------------------------
//		�w��G�t�F�N�g��~
//---------------------------------
void	CEffekseerCtrl::Stop(int hndl)
{
	if (hndl < 0)
	{
		return;
	}

	if (m_manager == nullptr) return;
	m_manager->StopEffect(m_eff[hndl].GetHndl());
	m_eff[hndl].Reset();
	m_eff[hndl].SetSpeed(1.f);
	m_eff[hndl].SetScale(VGet(1.f, 1.f, 1.f));
	m_eff[hndl].SetHndl(-1);
}


//---------------------------------
//		�S�G�t�F�N�g��~
//---------------------------------
void	CEffekseerCtrl::StopAll(void)
{
	if (m_manager == nullptr) return;

	m_manager->StopAllEffects();

	for (int i = 0; i < m_allNum; i++)
	{
		if (m_eff[i].GetHndl() != -1)
		{
			m_eff[i].Reset();

			m_eff[i].SetSpeed(1.f);
			m_eff[i].SetScale(VGet(1.f, 1.f, 1.f));
		}
	}
}


//---------------------------------
//		���W�ύX
//---------------------------------
void	CEffekseerCtrl::SetPosition(int hndl, VECTOR pos)
{
	if (m_manager != nullptr && m_eff != nullptr)
	{
		m_manager->SetLocation(m_eff[hndl].GetHndl(), pos.x, pos.y, pos.z);
	}
}


//---------------------------------
//		�g��k�����ύX
//---------------------------------
void	CEffekseerCtrl::SetScale(int hndl, VECTOR scale)
{
	if (m_manager != nullptr && m_eff != nullptr)
	{
		m_manager->SetScale(m_eff[hndl].GetHndl(), scale.x, scale.y, scale.z);

		m_eff[hndl].SetScale(scale);
	}
}

//	�Đ����x
void CEffekseerCtrl::SetSpeed(int handle, float speed)
{
	if (m_manager != nullptr && m_eff != nullptr)
	{
		m_manager->SetSpeed(m_eff[handle].GetHndl(), speed);

		m_eff[handle].SetSpeed(speed);
	}
}


//---------------------------------
//		��]�p�x�ύX
//---------------------------------
void	CEffekseerCtrl::SetRot(int hndl, VECTOR rot)
{
	if (m_manager != nullptr && m_eff != nullptr)
	{
		m_manager->SetRotation(m_eff[hndl].GetHndl(), rot.x, rot.y, rot.z);
	}
}


//---------------------------------
//		��]�p�x�ύX
//---------------------------------
bool	CEffekseerCtrl::IsActive(int hndl)
{
	int localHndl = m_eff[hndl].GetHndl();
	if (localHndl == -1)
	{
		return false;
	}
	return true;
}


//---------------------------------
//		�f�o�C�X���X�g���̃R�[���o�b�N�֐�
//---------------------------------
void	CEffekseerCtrl::DeviceLostFunction(void* data)
{
	// DirectX9�̂ݑΉ��Ƃ���
	if (m_dxVersion != DX_VERSION_9) return;

	// �f�o�C�X���X�g�������������ɌĂ�
	if (m_renderer9 != NULL) m_renderer9->OnLostDevice();

	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������
	if (m_eff != NULL)
	{
		for (int i = 0; i < m_allNum; i++)
		{
			m_effect[i]->UnloadResources();
		}
	}

	// DX���C�u�����͓����Ńf�o�C�X���̂��������Ă���̂�NULL��ݒ肷��
	if (m_renderer9 != NULL) m_renderer9->ChangeDevice(NULL);
}


//---------------------------------
//		�f�o�C�X���A���̃R�[���o�b�N�֐�
//---------------------------------
void	CEffekseerCtrl::DeviceRestoreFunction(void* data)
{
	// DirectX9�̂ݑΉ��Ƃ���
	if (m_dxVersion != DX_VERSION_9) return;

	// DX���C�u�����͉񕜎��ɓ����Ńf�o�C�X���Đ�������̂ŐV�����ݒ肷��
	if (m_renderer9 != NULL)
	{
		LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
		m_renderer9->ChangeDevice(device);
	}

	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���
	if (m_eff != NULL)
	{
		for (int i = 0; i < m_allNum; i++)
		{
			m_effect[i]->ReloadResources();
		}
	}

	// �f�o�C�X�����A����Ƃ��ɌĂ�
	if (m_renderer9 != NULL) m_renderer9->OnResetDevice();
}