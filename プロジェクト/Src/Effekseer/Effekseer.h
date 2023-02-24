#pragma once
#pragma once

//			Effekseer�g�p�N���X
// �g�p���̓v���p�e�B�́uC/C++�v���u�S�ʁv���u�ǉ��̃C���N���[�h�f�B���N�g���v��
// �u�����J�[�v���u�S�ʁv���u�ǉ��̃��C�u�����f�B���N�g���v��
// �uEffekseer�ɕK�v�Ȃ��́v��ǉ����Ă��������B
// CEffekseer�N���X��static�Ȃ̂Ŏg�p���鎞�����v���W�F�N�g�ɒǉ����Ă��������B
// �܂��A�f�o�b�O�ł͌x�����o��̂Łu�����J�[�v���u�R�}���h���C���v���u�ǉ��̃I�v�V�����v��
// �u/ignore:4099 �v�ƋL�q���Ă��������B

//			�g�p���̒��ӎ���
// CEffekseerCtrl::Draw()�O�ɉ�������DxLib��2D�`��֐�(����������)���Ă΂�ĂȂ���
// �`�悳��Ȃ��\��������܂��B
// �܂��A3D�̓���or�������̕��̂̌��ɃG�t�F�N�g���o���Ă��܂��Ƃ������Ȍ����ڂɂȂ邱�Ƃ�����܂��B

// ���݂�VisualStudio2017��2019�ɂ̂ݑΉ����Ă��܂��B

#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerRendererDX11.h>
#include <vector>


class CEffekseer;


//		�G�t�F�N�g�Ǘ��N���X
//		�������g����
class CEffekseerCtrl {
public:
	static const int EFFECT_NUM = 120;
protected:
	static std::vector<::Effekseer::Effect*>	m_effect;		// �G�t�F�N�g�N���X
	static CEffekseer m_eff[EFFECT_NUM];			// �G�t�F�N�g�P�i�Ǘ��N���X
	static ::Effekseer::Manager* m_manager;		// �G�t�F�N�g�}�l�[�W��
	static ::EffekseerRendererDX9::Renderer* m_renderer9;	// �����_���[(DirectX9�p)
	static ::EffekseerRendererDX11::Renderer* m_renderer11;	// �����_���[(DirectX11�p)
	static int									m_allNum;		// �g�p�G�t�F�N�g����
	static int									m_useID;		// ���Ɏg�p�\���ID
	static int									m_dxVersion;	// DirectX�̃o�[�W����
public:
	//	�R���X�g���N�^�E�f�X�g���N�^
	CEffekseerCtrl();
	~CEffekseerCtrl();

	static std::vector<::Effekseer::Effect*> GetEffect() { return m_effect; }

	//	������
	//	effNum		:	�G�t�F�N�g����
	//	particleNum	:	�p�[�e�B�N������
	static void Init(int effNum, int particleNum);
	//	�I������
	static void Exit(void);

	//	�G�t�F�N�g�f�[�^���[�h
	//	name	:	�G�t�F�N�g��(�f�[�^���E�p�X�܂ߓ��{��A�S�p�����͖��Ή�)
	//	@return	:	����=�G�t�F�N�gID,���s=-1
	static int	LoadData(const char* name);
	//	���e�s��ݒ�
	//	pers		:	�J�����̃p�[�X
	//	aspect		:	�A�X�y�N�g��
	//	nearLength	:	�J�����̃j�A�[
	//	farLength	:	�J�����̃t�@�[
	//	@memo		:	�ݒ肪�ς�邽�тɌĂ�ł�������
	static void SetProjectionMtx(float pers, float aspect, float nearLength, float farLength);
	//	���e�s�񎩓��ݒ�
	//	@memo	:	DxLib�̊֐��Ŏ����ݒ�(�Ȃ�ׂ��g��Ȃ�)
	//				��ʕ����Ȃǂɂ͑Ή��s��
	static void SetAutoProjectionMtx(void);
	//	�J�����s��ݒ�
	//	eyePos		:	�J�����̎��_	
	//	focusPos	:	�J�����̒����_
	//	up			:	�J�����̏����
	//	@memo		:	�ݒ肪�ς�邽�тɌĂ�ł�������
	static void SetCameraMtx(VECTOR eyePos, VECTOR focusPos, VECTOR up);
	//	�J�����s�񎩓��ݒ�
	//	@memo	:	DxLib�̊֐��Ŏ����ݒ�(�Ȃ�ׂ��g��Ȃ�)
	static void SetAutoCameraMtx();
	//	�J�����s��ݒ�
	//	eyePos		:	�J�����̎��_	
	//	rot			:	�J�����̊p�x
	//	up			:	�J�����̏����
	//	@memo		:	�ݒ肪�ς�邽�тɌĂ�ł�������
	static void SetCameraRotMtx(VECTOR eyePos, VECTOR rot, VECTOR up);
	//	�X�V����
	static void Update(void);
	//	�X�V����
	//	@memo	: �J�����ݒ������(�Ȃ�ׂ��g��Ȃ�)
	//			  ProjectionMtx,CameraMtx�̐ݒ���s�v
	static void UpdateAutoCamera(void);
	//	�`�揈��
	static void Draw(void);


	//	�G�t�F�N�g�Ăяo��
	//	ID		:	�G�t�F�N�gID
	//	pos		:	�����ʒu
	//	isLoop	:	�G�t�F�N�g�����[�v���邩
	static int Request(int ID, VECTOR pos, bool isLoop);
	static int Request2(int ID, VECTOR pos, int start_frame, bool isLoop);
	static int Request3(int ID, VECTOR pos, int start_frame, int end_frame, bool isLoop);
	//	�w��G�t�F�N�g��~
	static void Stop(int hndl);
	//	�S�G�t�F�N�g��~
	static void StopAll(void);

	//	���W�ύX
	static void SetPosition(int hndl, VECTOR pos);
	//	�g��k�����ύX
	static void SetScale(int hndl, VECTOR scale);
	//	��]�p�x�ύX
	static void SetRot(int hndl, VECTOR rot);
	//	�G�t�F�N�g�A�N�e�B�u����
	static bool IsActive(int hndl);
	//	�Đ����x
	static void SetSpeed(int handle, float speed);

private:
	//	�f�o�C�X���X�g���̃R�[���o�b�N�֐�
	static void DeviceLostFunction(void* data);
	//	�f�o�C�X���A���̃R�[���o�b�N�֐�
	static void DeviceRestoreFunction(void* data);
};



//		�G�t�F�N�g�P�i�Ǘ��N���X
//		������͊�{�g��Ȃ�
class CEffekseer {
protected:
	VECTOR				m_pos;			// �G�t�F�N�g�̍Đ��ʒu
	int					m_hndl;			// �G�t�F�N�g�n���h��
	int					m_ID;			// �Đ����̃G�t�F�N�gID
	int					m_endFrame;		// �ŏI�t���[��
	bool				m_isLoop;		// ���[�v�t���O

	VECTOR				m_scale;
	float				m_speed;

public:
	//	�R���X�g���N�^�E�f�X�g���N�^
	CEffekseer();
	~CEffekseer();

	void Reset(void);

	//	���W�ݒ�
	inline void	SetTrans(VECTOR pos) { m_pos = pos; }
	//	���W�擾
	inline VECTOR	GetTrans(void) { return m_pos; }
	//	�G�t�F�N�g�n���h���ݒ�
	inline void	SetHndl(int hndl) { m_hndl = hndl; }
	//	�G�t�F�N�g�n���h���擾
	inline int	GetHndl(void) { return m_hndl; }
	//	�G�t�F�N�gID�ݒ�
	inline void	SetID(int ID) { m_ID = ID; }
	//	�G�t�F�N�gID�擾
	inline int	GetID(void) { return m_ID; }
	//	���[�v���ݒ�
	inline void	SetLoop(bool isLoop) { m_isLoop = isLoop; }
	//	���[�v���擾
	inline bool	IsLoop(void) { return m_isLoop; }
	//	�G���h�t���[��
	inline void SetEndFrame(int frame) { m_endFrame = frame; }


	inline VECTOR	GetScale(void) { return m_scale; }
	inline float	GetSpeed() { return m_speed; }
	void SetScale(VECTOR scale) { m_scale = scale; }
	void SetSpeed(float speed) { m_speed = speed; }
};