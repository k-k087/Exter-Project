#pragma once

static const int TEXT_DATA_LEN = 256;

static const int TUTORIAL_TEXT_START_NUM = 3;
static const char* TUTORIAL_TEXT_START[TUTORIAL_TEXT_START_NUM] =
{
	"ごきげんよう\n私はこの世界のガイド係だ",
	"この世界の主人公である君のミッションは\nすべての敵を倒すことだ",
	"まずは 左スティック を動かす、または WASDキー を押して\nプレイヤーを操作してみてくれたまえ",
};

static const int TUTORIAL_TEXT_AVOID_NUM = 1;
static const char* TUTORIAL_TEXT_AVOID[TUTORIAL_TEXT_AVOID_NUM] =
{
	"いいぞ 次は回避をしてみよう\nAボタン（左シフト、スペースキー）でできるぞ",
};


static const int TUTORIAL_TEXT_MOVE_NUM = 3;
static const char* TUTORIAL_TEXT_MOVE[TUTORIAL_TEXT_MOVE_NUM] =
{
	"おみごとだ。回避中はごくわずかに無敵時間ができる",
	"うまく回避すれば敵の攻撃に当たらずに済むかもしれない\n有効活用しよう",
	"ちなみに 右スティック移動（マウス移動）はカメラの向きを\n変えることができる。覚えておくといい",
};

static const int TUTORIAL_TEXT_ENEMY_NUM = 2;
static const char* TUTORIAL_TEXT_ENEMY[TUTORIAL_TEXT_ENEMY_NUM] =
{
	"おや？",
	"早速だが、モンスターが出現するぞ"
};

static const int TUTORIAL_TEXT_ATTACK_NUM = 3;
static const char* TUTORIAL_TEXT_ATTACK[TUTORIAL_TEXT_ATTACK_NUM] =
{
	"モンスターを攻撃し、倒してくれ",
	"モンスターは右上のミニマップに表示される\nこれで敵の位置を確認しよう",
	"通常攻撃は Xボタン（左クリック）で攻撃できる\n連打することでコンボ攻撃ができるぞ",
};

static const int TUTORIAL_TEXT_ATTACK2_NUM = 3;
static const char* TUTORIAL_TEXT_ATTACK2[TUTORIAL_TEXT_ATTACK2_NUM] =
{
	"いいぞ 次はもう一つの通常攻撃だ\n　Yボタン（右クリック）で攻撃できるぞ",
	"Yボタンの通常攻撃はダメージが少し高いうえに攻撃範囲が広い",
	"しかし、MP（青いバー）を少し消費するぞ\n実際に Yボタン（右クリック）で攻撃してみよう",
};

static const int TUTORIAL_TEXT_STRONG_ATTACK_NUM = 3;
static const char* TUTORIAL_TEXT_STRONG_ATTACK[TUTORIAL_TEXT_STRONG_ATTACK_NUM] =
{
	"その調子だ。また、Xボタン（右クリック）とYボタン\n（左クリック）を交互に押すことでコンボが長くつながるぞ",
	"よし、次は強攻撃をしてみよう",
	"　LBボタン（Qキー）または　RBボタン（Eキー）で\n強攻撃ができるぞ",
};

static const int TUTORIAL_TEXT_BOOST_GO_NUM = 4;
static const char* TUTORIAL_TEXT_BOOST_GO[TUTORIAL_TEXT_BOOST_GO_NUM] =
{
	"すごいな　強攻撃はMP（青いバー）を大きく消費する",
	"しかしその分、攻撃範囲が広いうえに威力も高い　強い敵でも\nひるませることもできるのでタイミングをみてうまく使おう",
	"おや、ブーストバーが溜まったみたいだな\nブーストバーは攻撃することで少しずつ溜まる",
	"Aボタン（Zキー）を押してブーストモードになってみよう",
};

static const int TUTORIAL_TEXT_BOOST_ATTACK_NUM = 2;
static const char* TUTORIAL_TEXT_BOOST_ATTACK[TUTORIAL_TEXT_BOOST_ATTACK_NUM] =
{
	"ブーストに成功したぞ\nブースト中は攻撃速度が向上する",
	"また、攻撃コンボも増加する　敵に向かって\nXボタン（左クリック）を連打してみよう",
};

static const int TUTORIAL_TEXT_END_NUM = 4;
static const char* TUTORIAL_TEXT_END[TUTORIAL_TEXT_END_NUM] =
{
	"素晴らしい 完全に使いこなせているな",
	"ブーストモード中はゲージが減り、\n完全になくなるとブーストモード終了だ",
	"つまり、ここぞというときに使用するべきだな！",
	"これでチュートリアルは終了だ\nまた会おう",
};

