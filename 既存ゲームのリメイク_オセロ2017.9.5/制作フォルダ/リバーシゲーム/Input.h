#pragma once
#include "DxLib.h"

/*キーの入力状態を更新,
  キー入力を有効にする場合、
  この関数を更新処理に呼び出す*/
void Updata_Key();

//引数のキーコードのキーの入力状態を返す
int Key(int keycode);
